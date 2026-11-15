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
;  drvstrat.inc
;  device driver interrupt and strategy routines 
;  (forward references to DATA and AUDIO specific IOCTL handlers)
;
;*******************************************************************************
; bits and values used in determining media status via AC_EVENTSTATUSNOTIFY
STATUS_MEDIA_UNKNOWN 	EQU 0		; word value
STATUS_MEDIA_NOCHANGE	EQU 1		; LSB byte
STATUS_MEDIA_CHANGED 	EQU -1	; LSB byte
STATUS_NEA 			EQU 80h	; mask for No Event Available flag
STATUS_CLASS 		EQU 07h	; mask for notification class
STATUS_MEDIA 		EQU 04h	; the class value for media status


PKT_GETEVENTSTATUSNOTIFY db AC_GETEVENTSTATUSNOTIFY, 1,0,0,10h,0,0,0,8,0
; 0 = get device event/status notification
; 1 = no lun, reserved, imm=1 (poll mode, maximize supported devices)
; 2 = reserved
; 3 = reserved
; 4 = set notification class to media
; 5 = reserved
; 6 = reserved
; 7 = MSB, max notification bytes to get
; 8 = LSB, recommended at >= 8 (so event cleared on all drives)
; 9 = vendor reserved

; Command to determine status (drive open/closed, media found, ...)
; only input is the [unitReq] refers to valid drive
; if any error or this packet command unsupported returns with
; carry set and AX==STATUS_MEDIA_UNKNOWN==-1
; on success returns with carry clear
; AH = media status byte, bit 0=tray open(1)/closed(0), bit 1=media present yes(1)/no(0), bits 2-7 reserved
; AL is either STATUS_MEDIA_NOCHANGE (same media as last call) or STATUS_MEDIA_CHANGED
; this call as currently writen will only work on newer CD drives that support event notification.
; TODO check error message for reason and possible add STATUS_CALL_NOT_SUPPORTED error value
; each packet file has a data block e.g.
getMediaStatus proc near
      DEBUG_PrintChar DBGCDB,'M'
	push DS
	push BX
      lds  BX, [CS:unitReqDevData]			; load DS:BX with DSD address

IFDEF OPT_8020ONLY
	jmp  @@assumeNotSupported
ELSE
	; initialize ATAPI command packet
	allocateBuffer 5					; allocate 10 bytes for cdb
	mov  AX, SS
	mov  word ptr [CS:packetcdbseg], AX		; setup cdb seg:off
	mov  word ptr [CS:packetcdboff], BP
	callInitPacket PKT_GETEVENTSTATUSNOTIFY, 10

	; initialize buffer for data transfer
      allocateBuffer 4                          ; reserve 4 words for buffer & set SS:BP to it
	call_clearBuffer 4				; set our working buffer to zero [only portion used, in words]

      DEBUG_PrintChar DBGCDB,'S'

	mov  byte ptr [CS:datadir], APC_INPUT	; indicate pkt cmd inputs data
	mov  AX, SS
	mov  word ptr [CS:packetbufseg], AX		; set buffer to our working buffer
	mov  word ptr [CS:packetbufoff], BP

	; handles sending packet to device and reading returned data into buffer
	call PerformPacketCmd	
	jc   @@assumeNotSupported

	testb ptr [SS:,BP+2], STATUS_NEA		; see if No Event Available indicates if media status supported
	jnz  @@assumeNotSupported

	mov2 AH, [SS:,BP]					; see if we got all the data we wanted, value stored BIG Endian
	mov2 AL, [SS:,BP+1]				; note the value is number of bytes following this field, so
	cmp  AX, 6						; subtract 2 from expected value including header
	jl   @@assumeNotSupported

	mov2 AL, [SS:,BP+2]				; we should only get media status event, but check to be sure
	and  AL, STATUS_CLASS				; mask the notification class event
	cmp  AL, STATUS_MEDIA				; see if was media status event
	jne  @@assumeNotSupported
								; CS:[buffer+3] indicates supported event classes
	; here we assume we actually got media status information
	; so return the information in expected format
	mov2 AH, [SS:,BP+5]				; set AH to media status byte, bits 2-7 reserved, 1=media, 0=tray
	mov2 AL, [SS:,BP+4]				; lower 4 bits indicate media event
	and  AL, 0Fh					; if AL&0x0F == 0 then no change
	jz   @@notChanged
ENDIF ;OPT_8020ONLY

	@@changed:
	and  [BX+DSD_NDX_FLAGS], NOT DSD_MEDIACHANGED
	mov  AL, STATUS_MEDIA_CHANGED			; assume changed for remaining values (eject request,
	clc							; new media, media removal, media change, & reserved)
	jmp  @@done


	@@notChanged:
	test AL, DSD_MEDIACHANGED			; check if should force media changed
	jnz   @@changed
	mov  AL, STATUS_MEDIA_NOCHANGE		; same media still there!
	clc
	jmp  @@done

	@@assumeNotSupported:
	; see if drive locked, if so return no change
	test byte ptr [BX+DSD_NDX_FLAGHIGH], DSD_LOCKED
	jnz  @@notChanged
	test AL, DSD_MEDIACHANGED			; check if should force media changed
	jnz   @@changed
	; indicate we failed/status unknown
	mov  AX, STATUS_MEDIA_UNKNOWN
	stc

	@@done:
      freeBuffer						; free allocated data transfer buffer
	freeBuffer						; free allocated cdb
	pop  BX
	pop  DS
	retn
endp  ;getMediaStatus


;*******************************************************************************
