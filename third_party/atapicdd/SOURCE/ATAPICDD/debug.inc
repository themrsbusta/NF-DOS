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
;  debug.inc
;  debug print statements, macros, and any other debug support routines
;
;*******************************************************************************

;Debuging options:
;  We use the request device (IOCTL_Out) command to enable or disable debugging
;  output on the fly.  Also, at startup either one can reassemble with a
;  different default setting (released debug builds default to off) or use
;  appropriate command line option to change it.
;  DebugChannel
;    bit 0: disable/enable all debugging output
;    bit 1: data specific device driver calls, show high level IOCTL &
;           subcommand calls relating to data transfer
;    bit 2: audio specific device driver calls, show "" relating to playing
;           audio
;    bit 3: perform SCSI command phase, trace into sending CDB to device and
;           getting results
;    bit 4: reset phase
;    bit 5: init phase
;    bit 6: ASPI specific chunks  ( most be set to see any SCSI/ASPI specific
;           debug output)
;    bit 7: ATA specific chunks   ( most be set to see any ATA specific debug
;           output, esp during init)

; <channel> values, OR together to enable/select multiple channels
DBGENABLE   EQU 0FFh       ; in DebugOptions switch on/off output, in Print
                           ; calls always print (if global on)
DBGDATA     EQU 02h
DBGAUDIO    EQU 04h
DBGCDB      EQU 08h
DBGRST      EQU 10h
DBGINIT     EQU 20h
DBGASPI     EQU 40h
DBGATA      EQU 80h
DBGALL      EQU 0FFh

; prints value (db) as two hex digits, e.g.  0F, FD, ...
;DEBUG_PrintNumber <channel> <value> 
; prints ASCII character (db), e.g. A, a, ...
;DEBUG_PrintChar <channel> <character>    
; prints ASCII$ string (msg db N dup (?),'$') until but not including ending '$'
;DEBUG_PrintMsg <channel> <message>

; activates (enables output for) given channel(s)
;DEBUG_EnableChannel <channel>         
; disable (stops output for) given channel(s) 
;DEBUG_DisableChannel <channel>        


IFNDEF OPT_DEBUG     ; empty macros for when debugging code is not assembled in

IFDEF __NASM_MAJOR__
%macro DEBUG_PrintNumber 2
%endmacro
%macro DEBUG_PrintChar 2
%endmacro
%macro DEBUG_PrintMsg 2
%endmacro
%macro DEBUG_EnableChannel 1
%endmacro
%macro DEBUG_DisableChannel 1
%endmacro
ELSE ; Tasm/Masm
DEBUG_PrintNumber MACRO channel, value
ENDM
DEBUG_PrintChar MACRO channel, ch
ENDM
DEBUG_PrintMsg MACRO channel, msg
ENDM
ENDIF
DEBUG_EnableChannel MACRO channel
ENDM
DEBUG_DisableChannel MACRO channel
ENDM

ELSE

; set according to 'Debugging options' described above, default settings
DebugOptions DB DBGENABLE      

; Strings (that may be) displayed during a debug build
DbgMsg_Newline			DB 0Dh, 0Ah, "$"
DbgMsg_EnterIntProc		DB "DBG: Entering Interrupt Procedure!", 0Dh, 0Ah, "$"
DbgMsg_LeaveIntProc		DB "DBG: Leaving Interrupt Procedure!", 0Dh, 0Ah, "$"
DbgMsg_UnknownIOCTLCmd		DB "DBG: Unknown IOCTL Cmd", 0Dh, 0Ah, "$"
DbgMsg_CallingIOCTLCmd		DB "DBG: Invoking IOCTL Cmd", 0Dh, 0Ah, "$"
DbgMsg_Cmd_NA			DB "DBG: Invalid or Unimplemented [IOCTL] Cmd", 0Dh, 0Ah, "$"
DbgMsg_Cmd_BADUNIT		DB "DBG: Invalid or unknown unit specified", 0Dh, 0Ah, "$"
DbgMsg_DevReqUnit			DB "DBG: Called with IOCTL Device Unit of 0x$"
DbgMsg_DevReqCmd			DB "DBG: Called with IOCTL Device Request Cmd of 0x$"
DbgMsg_IOCTLCmd_Init		DB "DBG: IOCTL Initialization", 0Dh, 0Ah, "$"
DbgMsg_IOCTLCmd_Input		DB "DBG: IOCTL Input", 0Dh, 0Ah, "$"
DbgMsg_IOCTLCmd_Output		DB "DBG: IOCTL Output", 0Dh, 0Ah, "$"
DbgMsg_IOCTLCmd_InputFlush	DB "DBG: IOCTL Input Flush", 0Dh, 0Ah, "$"
DbgMsg_IOCTLCmd_DevOpen		DB "DBG: IOCTL Device Open", 0Dh, 0Ah, "$"
DbgMsg_IOCTLCmd_DevClose	DB "DBG: IOCTL Device Close", 0Dh, 0Ah, "$"
DbgMsg_IOCTLCmd_ReadLong	DB "DBG: Cmd Read Long", 0Dh, 0Ah, "$"
DbgMsg_CmdI_Reserved		DB "DBG: Cmd Input Reserved", 0Dh, 0Ah, "$"
DbgMsg_CmdI_GetDevHdr		DB "DBG: CmdI Get Device Header", 0Dh, 0Ah, "$"
DbgMsg_CmdI_HeadLoc		DB "DBG: CmdI Head Location", 0Dh, 0Ah, "$"
DbgMsg_CmdI_ErrStats		DB "DBG: CmdI Error Statistics", 0Dh, 0Ah, "$"
DbgMsg_CmdI_AudioChannel	DB "DBG: CmdI Audio Channel", 0Dh, 0Ah, "$"
DbgMsg_CmdI_ReadDrvB		DB "DBG: CmdI Read Drive Bytes", 0Dh, 0Ah, "$"
DbgMsg_CmdI_DevStatus		DB "DBG: CmdI Device Status", 0Dh, 0Ah, "$"
DbgMsg_CmdI_SectorSize		DB "DBG: CmdI Sector Size", 0Dh, 0Ah, "$"
DbgMsg_CmdI_VolumeSize		DB "DBG: CmdI Volume Size", 0Dh, 0Ah, "$"
DbgMsg_CmdI_MediaChanged	DB "DBG: CmdI Media Changed", 0Dh, 0Ah, "$"
DbgMsg_CmdI_AudioDisk		DB "DBG: CmdI Audio Disk", 0Dh, 0Ah, "$"
DbgMsg_CmdI_AudioTrack		DB "DBG: CmdI Audio Track", 0Dh, 0Ah, "$"
DbgMsg_CmdI_AudioQCh		DB "DBG: CmdI Audio Q Channel", 0Dh, 0Ah, "$"
DbgMsg_CmdI_AudioSubCh		DB "DBG: CmdI Audio SubChannel", 0Dh, 0Ah, "$"
DbgMsg_CmdI_UPC			DB "DBG: CmdI UPC", 0Dh, 0Ah, "$"
DbgMsg_CmdI_AudioStatus		DB "DBG: CmdI Audio Status", 0Dh, 0Ah, "$"
DbgMsg_CmdO_Reserved		DB "DBG: Cmd Output Reserved", 0Dh, 0Ah, "$"
DbgMsg_CmdO_ResetDrive		DB "DBG: CmdO Reset Drive", 0Dh, 0Ah, "$"


; NOTE: as debug statements may occur anywhere we can not assume
; anything about any register (including DS)

; sets carry if debugging is not active or channel (in AH) is not active
DBG_Active proc near
      ; see if global debugging output bit is set (show debug prints)
      test byte ptr [CS:DebugOptions], DBGENABLE
      jz   @@nodebug

      ; yes, see if any of the channels this print message belongs to are active
      @@debug:
      test  [CS:DebugOptions], AH
      jz   @@nodebug

      ; yes, so set carry so caller will print
      stc
      retn

      ; no, clear carry so caller skips prints
      @@nodebug:
      clc
      retn
endp


IFDEF __NASM_MAJOR__
%macro DEBUG_EnableChannel 1
      or   [CS:DebugOptions], %1
%endmacro
%macro DEBUG_DisableChannel 1
      and   [CS:DebugOptions], ! %1
%endmacro
ELSE ; Tasm/Masm
DEBUG_EnableChannel MACRO channel
      or   CS:[DebugOptions], channel
ENDM
DEBUG_DisableChannel MACRO channel
      and  CS:[DebugOptions], NOT channel
ENDM
ENDIF


; Prints a character using BIOS int 10h video teletype function
; All registers are preserved except AX and flags
; Expects character to print in AL, channel in AH
DBG_PrintChar proc near
      call DBG_Active   ; see if debug output is enabled/wanted for channel AH
      jc   @@dbg        ; nope, so return as quickly as possible
      retn

      @@dbg:            ; ok, do actual printout

      push BP           ; for BIOSes that screw it up if text scrolls window
      push BX

	; get current active page
      push AX
	mov  AH, 0Fh	; get current video mode
	int  10h		; perform Video - BIOS request
				; on return AH=screen width (# of columns),
				; AL=display mode, and *** BH=active page # ***
	mov  BL, 07h	; ensure sane color in case in graphics mode
	pop  AX
	mov  AH, 0Eh	; Video - Teletype output
	int  10h		; invoke request

	pop BX
	pop BP
	retn
endp


; Uses PrintChar to display the number in AL
; Expects hex number (00-FF) to print in AL, channel in AH
DBG_PrintNumber proc near
      call DBG_Active   ; see if debug output is enabled/wanted for channel AH
      jc   @@dbg        ; nope, so return as quickly as possible
      retn

      @@dbg:            ; ok, do actual printout

	push AX		; store value so we can process a nibble at a time

	; print upper nibble
	shr  AL, 4		; move upper nibble into lower nibble
	cmp  AL, 09h	; if greater than 9, then don't base on '0', base on 'A'
	jbe @@printme
	add  AL, 7		; convert to character A-F
	@@printme:
	add  AL, '0'	; convert to ASCII character 0-9, A-F
	call DBG_PrintChar

	pop  AX		; restore for other nibble

	; print lower nibble
	and  AL, 0Fh	; ignore upper nibble
	cmp  AL, 09h	; if greater than 9, then don't base on '0', base on 'A'
	jbe @@printme2
	add  AL, 7		; convert to character A-F
	@@printme2:
	add  AL, '0'	; convert to ASCII character 0-9, A-F
	call DBG_PrintChar

	retn
endp


; display string using PrintChar
; we don't use DOS print string as it trashes our request header
; expects CS:DX to point to $ terminated string and AH to channel
; all registers preserved except AX and flags
DBG_PrintMsg proc near
      call DBG_Active   ; see if debug output is enabled/wanted for channel AH
      jc   @@dbg        ; nope, so return as quickly as possible
      retn

      @@dbg:            ; ok, do actual printout

      push BX		; save registers modified
      mov  BX, DX       ; copy offset into bx register for use as index value

      @@next:
      mov2 AL, [CS:,BX] ; copy current character into AL
                        ; AH already has channel value
      inc  BX
      cmp  AL, '$'      ; if reached end of string marker then end loop
      je   @@done
      call DBG_PrintChar
      jmp  @@next

      @@done:
      pop  BX           ; restore registers we changed
      retn
endp  ;PrintMsg


; Below are macros to use for calling, they save AX and flags (& CX:DX for Msg),
; set the value and channel registers, call the corresponding print function,
; and finally restore the register(s) and flags all in an easy to read line
IFDEF __NASM_MAJOR__

; Prints specified character
%macro DEBUG_PrintNumber 2
      pushf
      push AX
      mov  AL, %2
      mov  AH, %1
      call DBG_PrintNumber
      pop  AX
      popf
%endmacro
%macro DEBUG_PrintChar 2
      pushf
      push AX
      mov  AL, %2
      mov  AH, %1
      call DBG_PrintChar
      pop  AX
      popf
%endmacro
%macro DEBUG_PrintMsg 2
      pushf
      push AX
      push DX
      mov  AH, %1
      mov  DX, OFFSET %2
      call DBG_PrintMsg
      pop  DX
      pop  AX
      popf
%endmacro

ELSE ; Tasm/Masm

DEBUG_PrintNumber MACRO channel, value
      pushf
      push AX
      mov  AL, value
      mov  AH, channel
      call DBG_PrintNumber
      pop  AX
      popf
ENDM
DEBUG_PrintChar MACRO channel, ch
      pushf
      push AX
      mov  AL, ch
      mov  AH, channel
      call DBG_PrintChar
      pop  AX
      popf
ENDM
DEBUG_PrintMsg MACRO channel, msg
      pushf
      push AX
      push DX
      mov  AH, channel
      mov  DX, OFFSET msg
      call DBG_PrintMsg
      pop  DX
      pop  AX
      popf
ENDM

ENDIF


ENDIF 	; OPT_DEBUG


;*******************************************************************************
