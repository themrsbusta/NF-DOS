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
;  dirctrl.inc
;  direct control
;  allow external programs to have direct (pass-thru) control
;  of the CD drive(s) and limited access to driver internal
;  commands/status
;
;*******************************************************************************
IFNDEF OPT_DIRECTCTRL
CmdO_DirectCtrl proc near
	jmp Cmd_NA
endp  ;CmdO_DirectCtrl
ELSE

; IOCTL Output subcommand 4  = allows programs to have direct control
; on Entry ES:DI points to subcommand 4, followed by N dup (?) bytes defined by driver & device
; (we don't use IOCTL Input read drive bytes counterpart, and expand
; this call to allow access to driver as well as drive access)
; For this driver only! subject to change if a known convention is found
; Direct Control Header (immediately follows subcommand 4)
; 0 marker	DB 'JH'			; if this is not there, assume app expects different format
; 2 command	DW ?				; specifies action to take, see below 
; 4 status	DW ?				; set to error code before returning, 0==OK, -1==general error
; 6 reserved DW ?				; may contain future flags like disable DMA/Interrupt support
; 8 varies	DB ? DUP (?)		; remaining field varies in size depending on command
;
; If marker field does not match or an unsupported command is specified then
; device driver call returns same as command not available (Cmd_NA)
; Direct Control Request:  command values
;  0: external interface to PerformPacketCmd
;  1: external interface to PerformResetCmd
;  2: external interface to requestSense
;  3: get address of device specific data (should be treated as read only or changed with extreme care)
;  FF: get driver information (should include what options driver assembled with)
;  FE: enable debug output, sets corresponding channels
;  FD: disable debug output, clears corresponding channels ( a FreeDOS switch :-)
;
; FF get driver information
;   ? format yet to be determined ?
;
; FE Enable debug output for given channels (AL) without clearing previously set ones
;   channel       DB ?        ; indicate channels to enable debugging
;                             ; (DBGENABLE must be set before any output will be shown)
; FD Disable debug options for given channels (AL) without enabling effecting others
;   channel       DB ?        ; indicate channels to enable debugging
;                             ; (DBGENABLE must be set before any output will be shown)
; Note: to set a particular set of channels and only those, one should
;   first issue a Disable request with DBGALL (0xFF) then Enable request with desired ones
;
; 0 Send ATAPI/SCSI Packet Cmd
;   packetsize	DB ?		; size of packet to send, 6, 10, 12 or 16 bytes (should be true pkt size)
;   datadir       DB ?		; one of APC_INPUT, APC_OUTPUT, APC_NONE indicating if data input/output/not
;   packetcdboff  DW ?		; segment and offset of command description block
;   packetcdbseg  DW ?		; contents and length of data pointing to depend entirely on PACKET CMD
;   packetbufoff  DW ?		; segment and offset of buffer used for data transer (may be 0:0 for APC_NONE)
;   packetbufseg	DW ?		; assumed to be large enough and valid for any data input/output
;
;   The contents of above are copied to local fields and PerformPacketCmd called,
;   then the above status field is set with its return code (AX).
; Note: Send Packet Cmd allows one to send any packet command to the requested drive
;   thus as long as we detect the drive, you could ignore all other aspects of this
;   driver and use this as a simple means to control the drive.  It is a direct pass
;   through for external programs, so may also be used to test how the drive handles
;   various requests.  It is recommended that one use driver calls for those actions
;   supported (such as locking/unlocking drive) or manually updating driver through
;   this IOCTL call to avoid the driver's view of drive and its true status from differing.
;
;
; on return AX, CX, and DI are undefined
; TODO finish me
CmdO_DirectCtrl proc near
	inc  DI							; point to start of Direct Control Header

	movw ptr [ES:,DI+4], 0				      ; clear status

	mov2 AX, [ES:,DI]						; verify if application and us speaking same language
 	cmp  AX, 'HJ'
	jne  @@unsupported

	mov2 AX, [ES:,DI+2]					; get command (for now we only support cmd 0)

	cmp  AX, 0
	je   @@performPktCmd

IFDEF OPT_RESET
	cmp  AX, 1
	je   @@performRstCmd
ENDIF

	cmp  AX, 2
	je   @@getSenseData

	cmp  AX, 3
	je   @@getDSDaddr

	;... TODO finish em

	cmp  AX, 0FFh
	je   @@getDriverInfo

IFDEF OPT_DEBUG
	cmp  AX, 0FEh
	je   @@enableDBG

	cmp  AX, 0FDh
	je   @@disableDBG
ENDIF

	; if we reach here then unknown/invalid command
	jmp  @@unsupported


	; directly pass PACKET CMD to device
	@@performPktCmd:	
	mov2 AL, [ES:,DI+6+0]					; get packetsize
	mov  [CS:cdbLen], AL

	mov2 AL, [ES:,DI+6+1]					; get datadir
	mov  [CS:datadir], AL					; indicate if packet cmd inputs/outputs data or not

	mov2 AX, [ES:,DI+6+2]					; get seg:off of user supplied cdb and
	mov  [CS:packetcdboff], AX			      ; store for call to issue packet
	mov2 AX, [ES:,DI+6+4]					;
	mov  [CS:packetcdbseg], AX

	mov2 AX, [ES:,DI+6+6]					; get seg:off of user supplied cdb and
	mov  [CS:packetbufoff], AX			      ; store for call to issue packet
	mov2 AX, [ES:,DI+6+8]					;
	mov  [CS:packetbufseg], AX

	call PerformPacketCmd	; handles sending packet to device and reading returned data into buffer
	jnc  @@done
	movw ptr [ES:,DI+4], AX					; store error code for callee
	jmp  @@done


IFDEF OPT_RESET
	; attempt soft reset on this device
	@@performRstCmd:
	call PerformResetCmd
	jmp  @@done
ENDIF


	; return cached REQUEST SENSE data
	@@getSenseData:
	; TODO
	jmp  @@done


	; return address seg:off of device specific data for this unit (and its size)
	@@getDSDaddr:
	; TODO
	jmp  @@done


	; return driver information
	@@getDriverInfo:
	; TODO
	jmp  @@done


IFDEF OPT_DEBUG
	; enable specified debugging channels (without clearing currently active ones)
	@@enableDBG:
	mov2 AL, [ES:,DI+6+0]					; get channel(s) to activate
	or   [CS:DebugOptions], AL
	jmp  @@done

	
	; disable only specified debugging channels
	@@disableDBG:
      not  AL                        ; invert to mask of ones to keep enabled
      and  [CS:DebugOptions], AL     ; disable all ones not masked as keep enabled
	jmp  @@done
ENDIF ;OPT_DEBUG


	; (sub)command is not supported, request isinvalid, or caller mismatch,
	; return without modifying any data (caller may expect different layout)
	@@unsupported:
	jmp Cmd_NA

	@@done:
	retn
endp  ;CmdO_DirectCtrl


ENDIF ;OPT_DIRECTCTRL
;*******************************************************************************
