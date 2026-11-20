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
;  init.inc
;  generic initialization routines (and data)
;  all code and data that need not be resident and not specific to a
;  particular controller (ATA nor SCSI) should be here
;
;*******************************************************************************


; performs one time initialization
; detects number of drives using standard controller port addresses
; also will process command line options
; We do not support arbitrary ATA/ATAPI controller I/O addresses,
; however one can override a default value as long as the secondary
; controller (base2, status register, ...) is offset +0x200 from base I/O
; 
Init proc near
	mov  DX, OFFSET AtapiCDDMsg
	call PrintMsg

	push DS					; save these registers, we use them later
	push BX

	; peform command line processing
	lds  BX, [BX+DEVREQ_DATA+DEVREQI_CMDLINE]	; set DS:BX to cmd line
	call processCmdLine

	; initially no units detected
	mov  byte ptr [CS:units], 0

IFDEF OPT_ATAPI
	call search_for_atapi_drives
ENDIF ;OPT_ATAPI
IFDEF OPT_SCSI
	call search_for_scsi_drives
ENDIF ;OP_SCSI

	pop  BX
	pop  DS

	DEBUG_PrintChar DBGINIT, 'U'
	DEBUG_PrintNumber DBGINIT, [CS:units]
	cmp  byte ptr [CS:units], 0		; if (!units) return success
	jz   @@failure				; else return success


	; give device a chance to spin up (if there is a CD-ROM in there)
	mov  CX, 3
	mov  byte ptr [CS:unitReq], 0		; set to a drive we found (assumes units > 0)
      call ldsregDSDprep                  ; precalculate seg:off into dev data for unitReq
	@@testSpinUp:				; assumes if one drive spins up, others had 
	call testUnitReady			; time as well, though could actually check
	jnc  @@endSpinUpDelay			; all drives found (for i=0;i<units;i++) if desired
	loop @@testSpinUp
	@@endSpinUpDelay:
	; loop through all found devices and issue start (& close tray) command
	mov  CL, 0
	@@startSpinUp:
	mov  [CS:unitReq], CL
	mov  AX, 01h				; LoEj=0, Start=1; (don't close tray) start & read TOC
	call startStopCmd
	call getMediaStatus			; issue ATAPI media event status request so next call has correct value
	inc  CL
	cmp  CL, [CS:units]
	jl   @@startSpinUp


@@success:
	; set Config flag that we succeeded
	mov word ptr [BX+DEVREQ_DATA+DEVREQI_CFGFLG], 0
	; tell DOS we support 0 units (since we are character device)
	mov word ptr [BX+DEVREQ_DATA+DEVREQI_UNITS], 0
	; specify where end of resident section is
      xor AH, AH                         ; get # of units found into AX
      mov AL, [CS:units]
      mov DX, DEVDATASIZE                ; data in bytes taken by each unit
      mul DX                             ; result in DX:AX (we ignore DX)
      add AX, OFFSET endOfResidentSectionMarker             ; get true end offset
	mov word ptr [BX+DEVREQ_DATA+DEVREQI_ENDADDR], AX     ; and store it
	mov word ptr [BX+DEVREQ_DATA+DEVREQI_ENDADDR+2], CS

	mov  AL, [CS:units]
	call PrintDecNumber
	mov  DX, OFFSET DrivesFoundMsg
	call PrintMsg

	jmp @@done

@@failure:
	; indicate general failure error of in device request header
      mov  AL,  ERROR_GENERALFAILURE      ; set error to general failure
      call  Cmd_Error
	; set Config.sys flag that we failed, since no drives were found
	mov word ptr [BX+DEVREQ_DATA+DEVREQI_CFGFLG], 1
	; tell DOS we support 0 units (since we are character device)
	mov word ptr [BX+DEVREQ_DATA+DEVREQI_UNITS], 0
	; specify to keep none of the program resident
	mov word ptr [BX+DEVREQ_DATA+DEVREQI_ENDADDR], 0
	mov word ptr [BX+DEVREQ_DATA+DEVREQI_ENDADDR+2], CS

	mov DX, OFFSET NoDrivesFoundMsg
	call PrintMsg

@@done:
	retn
endp  ;Init


;*******************************************************************************
