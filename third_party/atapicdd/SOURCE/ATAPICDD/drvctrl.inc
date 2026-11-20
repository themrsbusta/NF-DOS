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
;  drvctrl.inc
;  routines for general CD drive control
;
;*******************************************************************************

; IOCTL Output subcommand 0  = eject the disk (opens the tray)
; will fail if disk is currently locked!
CmdO_EjectDisk proc near
IFNDEF OPT_DRVCTRL
	jmp Cmd_NA
ELSE
	; TODO, test our lock status, if locked immediately return with error (instead of waiting for drive to do so)
	; alternately, update our lock status depending on success or failure of command.
	; or decide based on device and us...

	mov  AX, 02h				; LoEj=1, Start=0; open tray
	call startStopCmd
	jc   @@error				; handle errors
	retn

	@@error:
      mov  AL,  ERROR_DEVICENOTREADY      ; set error to unknown/invalid unit
      jmp  Cmd_Error
ENDIF
endp  ;CmdO_EjectDisk


; IOCTL Output subcommand 5  = closes the tray (opposite of eject disk)
; will fail if disk is currently locked!
CmdO_CloseTray proc near
IFNDEF OPT_DRVCTRL
	jmp Cmd_NA
ELSE
	; TODO, test our lock status, if locked immediately return with error (instead of waiting for drive to do so)
	; alternately, update our lock status depending on success or failure of command.
	; or decide based on device and us...

	mov  AX, 03h				; LoEj=1, Start=1; close tray (?and start, read TOC?)
	call startStopCmd
	jc   @@error				; handle errors
	retn
 
	@@error:
      mov  AL,  ERROR_DEVICENOTREADY      ; set error to unknown/invalid unit
      jmp  Cmd_Error
ENDIF
endp  ;CmdO_CloseTray


;***************************************

; 1  = lock or unlock the door
CmdO_DoorLock proc near
IFNDEF OPT_DRVCTRL
	jmp Cmd_NA
ELSE
	; may optionally use ATA locking mechanism,
	; but we use packet one for SCSI compatibility

     ; load DS:BX with address of device specific data for current unit
     lds BX, [CS:unitReqDevData]

	inc DI				; point to lock byte, 0=unlock, 1=lock
	mov2  AL, [ES:,DI]			; get lock byte
	cmp  AL, 0				; if 0 then unlock request
	je   @@unlock			; a 1 means lock and anything else is invalid
						; but assume nonzero means lock drive
	@@lock:
	mov  AX, 03h			; lock drive (persistent & prevent)
	call preventMediumRemovalCmd
	jnc   @@notlocked			; failure is ignored (but we clear lock flag)
						; (should we return ERROR_GENERALFAILURE ?)
	; mark drive as locked
	or  byte ptr [BX+DSD_NDX_FLAGHIGH], DSD_LOCKED
	jmp @@done

	@@unlock:
	mov  AX, 00h			; lock drive (persistent & prevent)
	call preventMediumRemovalCmd
						; failure is ignored (but we clear lock flag)
						; (should we return ERROR_GENERALFAILURE ?)
	@@notlocked:
	; mark drive as unlocked
	and  byte ptr [BX+DSD_NDX_FLAGHIGH], NOT DSD_LOCKED

	@@done:
	retn
ENDIF
endp  ;CmdO_DoorLock


;***************************************

; 2  = resets the drive and this driver
CmdO_ResetDrive proc near
IFNDEF OPT_RESET
      jmp Cmd_NA
ELSE
	call PerformResetCmd           ; issue device specific reset
	retn
ENDIF
endp  ;CmdO_ResetDrive


;*******************************************************************************
