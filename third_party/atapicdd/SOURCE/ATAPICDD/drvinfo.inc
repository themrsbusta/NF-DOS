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
;  drvinfo.inc
;  general routines for obtaining information about requested CD drive
;
;*******************************************************************************

; IOCTL Input subcommand 1  = location of head
; TODO: implement me
CmdI_HeadLoc proc near
IFNDEF OPT_DRVINFO
      jmp  Cmd_NA
ELSE
	; point (ES:DI) to adress mode field
	INC DI	; ADD  DI, CBLK_ADDRMODE

	; point (ES:DI) to field to store address (location) of head
	INC DI	; DI == CBLK_LOCHEAD

	; store device header addr at DD pointed to by ES:DI
	xor  AX, AX			; offset should be 0, i.e. mov  word ptr ES:[DI], OFFSET devHdr
	stosw				; store into ES:DI and increment DI
	mov  AX, CS			; segment should be same as CS, i.e. mov  word ptr ES:[DI+2], SEG devHdr
	stosw				; store into ES:DI

	retn
ENDIF
endp  ;CmdI_HeadLoc



;***************************************

; 5  = read drive bytes
; Not used/implemented, see IOCTL Output subcommand 4 direct control
CmdI_ReadDrvB proc near
      jmp  Cmd_NA
endp  ;CmdI_ReadDrvB


;***************************************

; 6  = device status
; TODO: implement me
CmdI_DevStatus proc near
IFNDEF OPT_DRVINFO
      jmp  Cmd_NA
ELSE
	DEBUG_PrintMsg DBGDATA, DbgMsg_CmdI_DevStatus

      call Cmd_NA
	retn
ENDIF
endp  ;CmdI_DevStatus


;***************************************

; 7  = return size of sectors
; TODO: implement me
CmdI_SectorSize proc near
IFNDEF OPT_DRVINFO
      jmp  Cmd_NA
ELSE
	DEBUG_PrintMsg DBGDATA, DbgMsg_CmdI_SectorSize

      call Cmd_NA
	retn
ENDIF
endp  ;CmdI_SectorSize


;***************************************

; 9  = media changed
; TODO: if drive supports locking or other method to ensure no change, then return no change
;       for now we always report don't know, but adding real support will enhance performance
; Set media byte to one of:  0 is safe, 1 should only be returned if sure, 
; and -1 will causes CD-ROM info to be reloaded by CDEX
;   1         Media not changed
;   0         Don't know if changed
;  -1 (0FFh)  Media changed

CmdI_MediaChanged proc near
	; point (ES:DI) to media byte of control block
	INC DI	; ADD  DI, CBLK_MEDIABYTE

	; attempt to query device for status
	; check flags (force/lock) and when available use event status to
	; query device and determine if disk change has occurred, if so report this and exit
	call getMediaStatus			; issue ATAPI media event status request
	jc   @@nextcheck				; if error of any sort then ignore this check
	cmp  AL, STATUS_MEDIA_UNKNOWN		; media has not changed since last time we checked (made getMediaStatus call)
	je   @@nextcheck				; if check returned unsure, try another check
	movb ptr [ES:,DI], AL			; return media status as returned by above call
	add  AL, ','				; '+,-' with ,==44
	DEBUG_PrintChar DBGENABLE, AL
	jmp  @@done

	@@nextcheck:
	; check if drive is ready, any error we assume media changed (really assume drive not ready/no disc)
	call testUnitReady
	jnc  @@dontknow
	movb ptr [ES:,DI], -1
	DEBUG_PrintChar DBGENABLE, '>'
	jmp  @@done

	movb ptr [ES:,DI], 1 			; guess its unchanged
	DEBUG_PrintChar DBGENABLE, '='
	jmp  @@done

	@@dontknow:
	movb ptr [ES:,DI], 0			; indicate we don't know
	DEBUG_PrintChar DBGENABLE, '?'

	@@done:
	retn
endp  ;CmdI_MediaChanged


;*******************************************************************************
