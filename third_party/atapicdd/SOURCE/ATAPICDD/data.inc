;*******************************************************************************
;
;  ATAPICDD - ATAPI CD Driver for DOS
;  Written by Kenneth J. Davis <jeremyd@computer.org>, 2001-2003
;  Released to public domain  [ U.S. Definition ]
;
;  Use at own risk, author assumes no liability nor responsibility
;  for use, misuse, lack of use, or anything else as a result
;  of this program.  No warranty or other implied nor given.
;
;  Please send bug reports to me so that they may hopefully be fixed.
;  If possible please include your contact information (email address)
;  so I may ask you further details or to verify it as fixed.
;  Fixes will be supplied as my time permits.
;
;
;  data.inc
;  data specific routines (or data related)
;
;*******************************************************************************

; Command to flush input - should free all buffers & clear any pending requests
; Currently we have nothing to free/clear so simply return
; TODO: add once we support prefetching or other buffering
;       perhaps we should also flush CD-ROM's cache?
Cmd_InputFlush proc near
	DEBUG_PrintMsg DBGDATA, DbgMsg_IOCTLCmd_InputFlush
	retn
endp  ;Cmd_InputFlush


;*******************************************************************************

; Command to read data
; AX and DX modified, other registers preserved
; We do not support interleave, so interleave size & skip factor are ignored
;	DEVREQR_ILSIZE	EQU 0Ch	; interleave size
;	DEVREQR_ILSKIP	EQU 0Dh	; interleave skip factor
; For basic data read use READ10 as it is listed as mandatory in SCSI-2 specs
; TODO add support for MSF redbook mode
Cmd_ReadLong proc near
	DEBUG_PrintChar DBGDATA, 'R'
	DEBUG_PrintChar DBGDATA, 'L'
	mov  AX, word ptr [BX+DEVREQ_DATA+DEVREQR_SECTCNT]		; number (count) of sectors to read in (word)
	DEBUG_PrintNumber DBGDATA, AH
	DEBUG_PrintNumber DBGDATA, AL
	DEBUG_PrintChar DBGDATA, '-'
	mov  AX, word ptr [BX+DEVREQ_DATA+DEVREQR_START+2]		; high word starting sector
	DEBUG_PrintNumber DBGDATA, AH
	DEBUG_PrintNumber DBGDATA, AL
	DEBUG_PrintChar DBGDATA, ':'
	mov  AX, word ptr [BX+DEVREQ_DATA+DEVREQR_START]		; low word starting sector
	DEBUG_PrintNumber DBGDATA, AH
	DEBUG_PrintNumber DBGDATA, AL

	cmp  byte ptr [BX+DEVREQ_DATA+DEVREQR_ADDRMODE], AM_HSG	; see if LBA mode (High Sierra Group)
	jne  @@checkMSF
	DEBUG_PrintChar DBGDATA, 'L'
	DEBUG_PrintChar DBGDATA, 'B'
	DEBUG_PrintChar DBGDATA, 'A'

	cmp  byte ptr [BX+DEVREQ_DATA+DEVREQR_MODE], DM_COOKED	; let device handle error correction, 2048
	jne  @@checkRAW

	; cooked mode LBA read request
	DEBUG_PrintMsg DBGDATA, @@cooked

	; ************ send cooked LBA read request ***************
      allocateBuffer 5					; allocate 10 bytes for cdb
	mov  BX, SS
	mov  word ptr [CS:packetcdbseg], BX		; setup cdb seg:off
	mov  word ptr [CS:packetcdboff], BP
	callInitPacket PKT_READ10, 10
	mov  AX, word ptr [BX+DEVREQ_DATA+DEVREQR_START+2]	; set LBA start address (big endian MSB 1st)
	xchg AH, AL
	movw ptr [SS:,BP+2], AX
	mov  AX, word ptr [BX+DEVREQ_DATA+DEVREQR_START]	; low word of LBA start address (MSB 1st)
	xchg AH, AL
	movw ptr [SS:,BP+4], AX
	mov  AX, word ptr [BX+DEVREQ_DATA+DEVREQR_SECTCNT]	; number (count) of sectors to read in (word)
      xchg AH, AL								; MSB (big endian) of length for AC_READ10
	movw ptr [SS:,BP+7], AX

	mov  byte ptr [CS:datadir], APC_INPUT			; indicate pkt cmd inputs data
	mov  AX, word ptr [BX+DEVREQ_DATA+DEVREQR_TRANSADDR+2]; buffer ptr segment (we assume large enough)
	mov  [CS:packetbufseg], AX
	mov  AX, word ptr [BX+DEVREQ_DATA+DEVREQR_TRANSADDR]	; buffer ptr offset
	mov  [CS:packetbufoff], AX
	call PerformPacketCmd						; do request
	freeBuffer								; free allocated data
	jc   @@devError							; handle errors

	retn
	@@cooked DB 'cooked', 0Dh, 0Ah, '$'


	@@checkRAW:
	cmp  byte ptr [BX+DEVREQ_DATA+DEVREQR_MODE], DM_RAW		; raw 2352 sectors
	jne  @@unsupportedOption

	DEBUG_PrintMsg DBGDATA, @@raw


	; ************ implement me ***************
	jmp  @@devError			; for now just say device not ready
	@@raw DB 'raw', 0Dh, 0Ah, '$'


	@@checkMSF:
	cmp  byte ptr [BX+DEVREQ_DATA+DEVREQR_ADDRMODE], AM_RED	; see if MSF mode (Redbook)
	jne  @@unsupportedOption
	; presently we don't support minute/second/frame mode
	DEBUG_PrintChar DBGDATA, 'R'
	DEBUG_PrintChar DBGDATA, 'E'
	DEBUG_PrintChar DBGDATA, 'D'

      ; ************ implement me ***************

      @@unsupportedOption:		; addrmode, mode, or other option not supported
      call Cmd_NA
      retn

      @@devError:				; errors such as no media or whatever
      ; load DS:BX with address of device specific data for current unit
      lds BX, [CS:unitReqDevData]

      ; we always return not ready as the most common case will be no
      ; CD in drive or unable to read CD, either way SHSUCDX only checks
      ; if driver returns all ok (0x100) or not (anything else).
      or  byte ptr [BX+DSD_NDX_FLAGS], DSD_MEDIACHANGED	; force media change check to indicate changed
      mov  AL,  ERROR_DEVICENOTREADY      ; set error to unknown/invalid unit
      jmp  Cmd_Error				; or DiskChanged? GeneralError? ???
      retn
endp  ;Cmd_ReadLong


;*******************************************************************************

; Command to indicate if possible drive should prefetch or at least seek
; to indicated sector.  
; Note: It is optional for this command to do anything, but doing so will
; generally lead to better performance.
; presently we just initiate a seek request
; TODO: add support to buffer requests if not busy
Cmd_ReadLongPrefetch proc near
	call Cmd_Seek

	retn
endp  ;Cmd_ReadLongPrefetch


;*******************************************************************************

; Command used to tell drive to seek to particular location
; Given address mode & starting sector number, initiate moving head
; to that location and return immediately (ie don't wait for seek to
; complete, note: next command requiring disk activity will have to wait)
; Ignore the transfer address & sectors to read fields in request (should be 0)
Cmd_Seek proc near
	push AX
	push DX

	mov  DX, word ptr [BX+DEVREQ_DATA+DEVREQS_START+2]		; set LBA start address (big endian MSB 1st)
	mov  AX, word ptr [BX+DEVREQ_DATA+DEVREQS_START]		; low word of LBA start address (MSB 1st)
	call seekCmd
	; ignore errors for now

	pop  DX
	pop  AX
	retn
endp  ;Cmd_Seek


;*******************************************************************************
