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

; device driver request header
DEVREQ_LENGTH     EQU 00h     ; byte
DEVREQ_UNIT       EQU 01h     ; byte
DEVREQ_CMD        EQU 02h     ; byte
DEVREQ_STATUS     EQU 03h     ; word
DEVREQ_RESERVED   EQU 05h     ; 8 bytes
DEVREQ_DATA       EQU 0Dh     ; variable size

; flags for setting status bits in request header (assume status dw 0 on entry)
STATUS_ERROR      EQU 8000h   ; Should be set if error, with low 8bits == error code
STATUS_BUSY       EQU 0200h   ; Should always be set if in audio play mode
STATUS_DONE       EQU 0100h   ; Indicates done processing request

; error values when STATUS_ERROR is set (byte)
ERROR_UNKNOWNUNIT       EQU 01h     ; unknown (or unsupported) unit specified
ERROR_DEVICENOTREADY    EQU 02h     ; device not ready
ERROR_UNKNOWNCMD        EQU 03h     ; unknown (or unsupported) command
ERROR_SECTORNOTFOUND    EQU 08h     ; sector not found
ERROR_READERROR         EQU 0Bh     ; error reading
ERROR_GENERALFAILURE    EQU 0Ch     ; general failure
ERROR_INVLDDSKCHNGE     EQU 0Fh     ; invalid disk change

; offsets in the device request header's data section for init request
DEVREQI_UNITS     EQU 00h     ; # of subunits, not set by character devices (or set to 0) (byte)
DEVREQI_ENDADDR   EQU 01h     ; end of resident code (dword)
DEVREQI_PBPB      EQU 05h     ; pointer to BPB array, not set by character devices (dword)
DEVREQI_CMDLINE   EQU 05h     ; on entry BPB points to info in CONFIG.SYS after = (ends with \n,\r, or \r\n)
DEVREQI_DRIVE     EQU 09h     ; drive number [block device number, 0 for character device] (byte)
DEVREQI_CFGFLG    EQU 10h     ; CONFIG.SYS Error Message control flag (word)

; offsets in the device request header's data section for read/prefetch requests
DEVREQR_ADDRMODE  EQU 00h     ; addressing mode (byte)
DEVREQR_TRANSADDR EQU 01h     ; transfer address (dword)
DEVREQR_SECTCNT   EQU 05h     ; number (count) of sectors to read in (word)
DEVREQR_START     EQU 07h     ; starting sector number (dword)
DEVREQR_MODE      EQU 0Bh     ; read mode (byte)
DEVREQR_ILSIZE    EQU 0Ch     ; interleave size
DEVREQR_ILSKIP    EQU 0Dh     ; interleave skip factor

; offsets in the device request header's data section for seek requests
DEVREQS_ADDRMODE  EQU 00h     ; addressing mode (byte)
DEVREQS_TRANSADDR EQU 01h     ; transfer address (dword) == 0
DEVREQS_SECTCNT   EQU 05h     ; number (count) of sectors to read in (word) == 0
DEVREQS_START     EQU 07h     ; starting sector number

; offsets from control block (specified by transfer address in device request)
; for both Cmd_IOCTL_input and Cmd_IOCTL_output requests
CBLK_CMDCODE      EQU 00h     ; command code (control block code), i.e. command to perform

; for Cmd_IOCTL_input's command codes, offset in control block, note 00h is always CBLK_CMDCODE
; format is:  code (number of bytes to transfer, i.e. sizeof(control block)) description

; GetDevHdr code 0 (5 bytes) return address of device header
CBLK_DEVHDRADDR   EQU 01h     ; offset to place address of device driver header
; HeadLoc code 1 (6 bytes) location of head
CBLK_ADDRMODE     EQU 01h     ; byte specifying addressing mode
CBLK_LOCHEAD      EQU 02h     ; DD specifying current location of the drive head
; Reserved code 2 (? bytes) reserved, return unknown command
; ErrStats code 3 (? bytes) error statistics
; AudioChannel code 4 (9 bytes) audio channel information
; ReadDrvB code 5 (130 bytes) read drive bytes
; DevStatus code 6 (5 bytes) device status
; SectorSize code 7 (4 bytes) return size of sectors
; VolumeSize code 8 (5 bytes) return size of volume
; MediaChanged code 9 (2 bytes) media changed
CBLK_MEDIABYTE    EQU 01h     ; byte specifying if disc changed (-1 yes, 0 don't know, 1 no)
; AudioDisk code 10 (7 bytes) audio disk information
CBLK_LOWTRACKNUM  EQU 01h     ; byte, lowest track number  (binary, not BCD)
CBLK_HIGHTRACKNUM EQU 02h     ; byte, highest track number (binary, not BCD)
CBLK_STARTLOTRACK EQU 03h     ; DD (MSF) of Starting point of the lead-out track
; AudioTrack code 11 (7 bytes) audio track information
CBLK_TRACKNUM     EQU 01h     ; byte, track number
CBLK_STARTTRACK   EQU 02h     ; DD (MSF) of starting point of the track
CBLK_TRACKCTRL    EQU 06h     ; byte, track control information
; AudioQCh code 12 (11 bytes) audio Q-Channel information
; AudioSubCh code 13 (13 bytes) audio Sub-Channel information
; UPC code 14 (11 bytes) UPC code
; AudioStatus code 15 (11 bytes) audio status information
; codes 16-255 (? bytes) reserved, return unknown command


; Address Mode
AM_HSG	EQU 0h	; HSG addressing mode (logical block address as defined by High Sierra)
AM_RED	EQU 01h	; Red Book addressing mode (minute/second/frame)
; 02h through 0FFh are reserved
; logical block (sector) = 75 * (minute*60 + second) + frame - 150

; Data Mode
DM_COOKED	EQU 0h	; cooked, 2048 byte sectors, device handles EDC/ECC (errors)
DM_RAW	EQU 01h	; as much data of raw sectors as device can give aligned to 2352 byte sectors
; 02h through 0FFh are reserved



ALIGN 16
; Jump table for requests
jumpTable:
      DW Cmd_Init             ; Cmd 0  - init (required)
      DW Cmd_NA               ; Cmd 1  - media check (block)
      DW Cmd_NA               ; Cmd 2  - Build BPB (block)
      DW Cmd_IOCTL_input      ; Cmd 3  - IOCTL input (required)
      DW Cmd_NA               ; Cmd 4  - input read
      DW Cmd_NA               ; Cmd 5  - nondestructive input (no wait)
      DW Cmd_NA               ; Cmd 6  - input status
      DW Cmd_InputFlush       ; Cmd 7  - input flush (required)
      DW Cmd_NA               ; Cmd 8  - output write
      DW Cmd_NA               ; Cmd 9  - output with verify
      DW Cmd_NA               ; Cmd 10 - output status
      DW Cmd_NA               ; Cmd 11 - output flush (erasable CD-ROM)
      DW Cmd_IOCTL_output     ; Cmd 12 - IOCTL output (required)
      DW Cmd_DevOpen          ; Cmd 13 - device open (required)
      DW Cmd_DevClose         ; Cmd 14 - device close (required)
      ; 15-24 route to Cmd_NA
                              ; Cmd 15 - removable media (block)
                              ; Cmd 16 - output until busy
                              ; Cmd 17-18 Reserved ???
                              ; Cmd 19 - Generic IOCTL request (if attribute bit 6 set)
                              ; Cmd 20-22 Reserved ???
                              ; Cmd 23 - get logical device (if attribute bit 6 set) (block)
                              ; Cmd 24 - set logical device (if attribute bit 6 set) (block)
                              ; Cmd 25-127 Reserved ???
extJumpTable:
      DW Cmd_ReadLong         ; Cmd 128 - read long (required)
      DW Cmd_NA               ; Cmd 129 - Reserved
      DW Cmd_ReadLongPrefetch ; Cmd 130 - read long prefetch (required)
      DW Cmd_Seek             ; Cmd 131 - seek (required)
      DW Cmd_PlayAudio        ; Cmd 132 - play audio (optional)
      DW Cmd_StopAudio        ; Cmd 133 - stop audio (optional)
      DW Cmd_NA               ; Cmd 134 - write long (erasable CD-ROM)
      DW Cmd_NA               ; Cmd 135 - write long verify (erasable CD-ROM)
      DW Cmd_ResumeAudio      ; Cmd 136 - resume audio (optional)


;*******************************************************************************

; Strategy routine - simply store request (ES:BX == Device Request Block)
StrategyProc proc far
      mov  word ptr [CS:devRequest], BX
      mov  word ptr [CS:devRequest+2], ES
      retf
endp


;*******************************************************************************

; Interrupt routine - process stored request 
; (CS:devRequest == Device Request Block), moved into DS:BX then
; calls command to perform action 
; all registers (except CS & stack) may return modified by command called
InterruptProc proc far
      cli               ; disable interrupts while we modify stack

      ; set to my stack
      push AX
      mov  [CS:oldSS], SS
      mov  [CS:oldSP], SP
      mov  AX, CS
      mov  SS, AX
      mov  SP, OFFSET mystacktop

      sti               ; reenable interrupts

      pushf             ; save flags
      push BX           ; save registers (to local stack)
      push CX
      push DX
      push DI
      push SI
      push DS
      push ES
      push BP

      cld               ; set direction flag for increments

      ; load Device Request Block into DS:BX
      lds  BX, [CS:devRequest]

      ; initialize status (for shsucdx which will continue to fail once set otherwise)
      mov  word ptr [BX+DEVREQ_STATUS], 0	; clear status

      ; verify if unit action requested for is valid (note: units==-1 before init call)
      mov  AL, [BX+DEVREQ_UNIT]     ; get unit action requested for
      cmp  AL, [CS:units]           ; compare with our count of units we handle
      jb   @@getRequestedCmd        ; if less than our count then proceed (jb on purpose so ok for units==-1)
      mov  AL,  ERROR_UNKNOWNUNIT   ; set failure status of unknown unit
      call Cmd_Error                ; we must call instead of jmp so its retn works
      jmp @@done                    ; and return/exit

@@getRequestedCmd:
      mov  [CS:unitReq], AL         ; store unit action requested for
      call ldsregDSDprep            ; precalculate seg:off into dev data for unitReq

      xor  AH, AH                   ; prepare so can extend AL into word
      mov  AL, [BX+DEVREQ_CMD]	
      DEBUG_PrintChar DBGENABLE, '?' ; print out command request as "?#" eg ?0 for init request
      DEBUG_PrintNumber DBGENABLE, al

IF 0	; docs indicate we should do this, but seems unneeded (for shsucdx) and slows things down
      ; also probably will screw up at least some other programs
      ; query device and determine if disk change has occurred, if so report this and exit
      push AX                             ; don't loose command
      cmp  AL, 0                          ; 1st make sure this isn't an init request
      je   @@endCheck                     ; if so proceed without check

      DEBUG_PrintChar DBGCDB, '|'
      call getMediaStatus                 ; issue ATAPI media event status request
      DEBUG_PrintChar DBGCDB, '|'
      jc   @@endCheck                     ; if error of any sort then ignore this check
      cmp  AL, STATUS_MEDIA_NOCHANGE      ; media has not changed since last time we checked
      je   @@endCheck
      pop  AX                             ; pop off stack
      ;setMediaChanged 01h                 ; indicate next check should indicate changed
      mov  AL,  ERROR_INVLDDSKCHNGE       ; yes it has, so exit with an error and let CDEX retry
      call Cmd_Error                      ; we must call instead of jmp so its retn works
      jmp  @@done
      @@endCheck:
      pop  AX                             ; restore command
ENDIF

      shl  AL, 1                          ; get index into jump table (multiply by 2)
                                          ; also makes it easy to determine if is one of 
                                          ; new commands (128+)

      jnc  @@normalCmd                    ; shl sets carry to high bit, if not carry then cmd < 128
      cmp  AL, 16                         ; Commands 128-136 use extended jump table 
                                          ; Offset/Cmd: 0/128,2/129,4/130,6/131,...
      ja   @@unknownCmd                   ; if above 16 then cmd is greater than 136 so unknown
      add  AL, (extJumpTable - jumpTable) ; update index to refer to extended
                                          ; jump table (add length of jumpTable)
      jmp  @@callCommand

@@normalCmd:
      cmp  AL, 30                         ; (30=2*15) Commands 0-14 use jump table
      jb   @@callCommand

@@unknownCmd:
      call Cmd_NA                         ; consider invalid command
      jmp  @@done

@@callCommand:
      mov  DI, AX



      add  DI, OFFSET jumpTable           ; add index to start of jumpTable
      callw [CS:,DI]                      ; perform the call

@@done:
      ; re-load Device Request Block into DS:BX
      lds  BX, [CS:devRequest]

      or  word ptr [BX+DEVREQ_STATUS], STATUS_DONE ; mark request as completed.
      DEBUG_PrintChar DBGENABLE, '/'       ; print out return status
      DEBUG_PrintNumber DBGENABLE, [BX+DEVREQ_STATUS+1]
      DEBUG_PrintNumber DBGENABLE, [BX+DEVREQ_STATUS]
	DEBUG_PrintChar DBGENABLE, '/'

      pop  BP           ; restore registers (from local stack)
      pop  ES
      pop  DS
      pop  SI
      pop  DI
      pop  DX
      pop  CX
      pop  BX
      popf			; restore flags

      ; restore original stack
      cli
      mov  AX, [CS:oldSS]
      mov  SS, AX
      mov  SP, [CS:oldSP]
      sti

      pop  AX

      retf
endp  ;InterruptProc


;*******************************************************************************
;*******************************************************************************

ALIGN 16
; jump table for IOCTL input's command
IOCTLI_jumpTable:
      DW    CmdI_GetDevHdr    ; 0  = return the address of the device header (required)
      DW    CmdI_HeadLoc      ; 1  = location of head (required)
      DW    Cmd_NA            ; 2  = reserved, return unknown command
      DW    CmdI_ErrStats     ; 3  = error statistics
      DW    CmdI_AudioChannel ; 4  = audio channel information
      DW    CmdI_ReadDrvB     ; 5  = read drive bytes
      DW    CmdI_DevStatus    ; 6  = device status
      DW    CmdI_SectorSize   ; 7  = return size of sectors
      DW    CmdI_VolumeSize   ; 8  = return size of volume
      DW    CmdI_MediaChanged ; 9  = media changed (required)
      DW    CmdI_AudioDisk    ; 10 = audio disk information
      DW    CmdI_AudioTrack   ; 11 = audio track information
      DW    CmdI_AudioQCh     ; 12 = audio Q-Channel information
      DW    CmdI_AudioSubCh   ; 13 = audio Sub-Channel information
      DW    CmdI_UPC          ; 14 = UPC code
      DW    CmdI_AudioStatus  ; 15 = audio status information
                              ; commands 16-255 are reserved and call Cmd_NA

; jump table for IOCTL outputs's command
IOCTLO_jumpTable:
      DW    CmdO_EjectDisk    ; 0  = eject the disk (opens the tray)
      DW    CmdO_DoorLock     ; 1  = lock or unlock the door
      DW    CmdO_ResetDrive   ; 2  = resets the drive and this driver
      DW    CmdO_AudioChCtrl  ; 3  = audio channel control
      DW    CmdO_DirectCtrl   ; 4  = allows programs to have direct control
      DW    CmdO_CloseTray    ; 5  = close the tray (opposite of eject disk)
                              ; commands 6-255 are reserved and call Cmd_NA



; Command to perform input or get information from the device driver
; Command called with DS:BX pointing to device request and sets
; ES:DI pointing to transfer address (subcommand request information)
; DS:BX, ES:DI, SI, AX, and CX may all be altered.
Cmd_IOCTL_input proc near
      DEBUG_PrintChar DBGENABLE, '@'                  ; print IOCTL input subcommand
      mov  SI, OFFSET IOCTLI_jumpTable                ; load offset of jump table
      mov  AH, 15                                     ; 0-15 valid subcommands
      
Cmd_IOCTL_invoke:
      les  DI, [BX+DEVREQ_DATA+DEVREQR_TRANSADDR]     ; load ES:DI with transfer address
      mov2 AL, [ES:,DI]      ; get subcommand code
      DEBUG_PrintNumber DBGENABLE, al
      cmp  AL, AH             ; see if valid (one of the 0 to [AH] commands supported)
      ja   Cmd_NA

      xor  AH, AH             ; zero AH so can extend AL into word
      shl  AX, 1              ; convert command code into index in jump table
      add  SI, AX             ; add index to start of jump table
      callw [CS:,SI]          ; perform the command

      retn
endp


; Command to perform output or control the cd drive
; Command called with DS:BX pointing to device request and
; ES:DI pointing to transfer address (subcommand request information)
; DS:BX, ES:DI, SI, AX, and CX may all be changed.
Cmd_IOCTL_output proc near
      DEBUG_PrintChar DBGENABLE, '&'                  ; print IOCTL output subcommand 
      mov  SI, OFFSET IOCTLO_jumpTable			; load offset of jump table
      mov  AH, 5                                      ; 0-5 valid subcommands

      jmp Cmd_IOCTL_invoke
endp


;***************************************
;***************************************


; When an error occurs and the driver needs to return an
; error message, the command should set AL with the proper
; error value and then either jmp to Cmd_Error or call it
; and upon return immediately retn itself to return processing
; to InterruptProc which will cleanly end the device request.
; Given its common use, ERROR_UNKNOWNCMD should be returned
; by simply jmp'ing or call'ing Cmd_NA

; example error conditions
;
; called when disk change and need to return invalid disk change
; ie if since last request the CD-ROM in the drive has changed,
; MSCDEX or equiv should determine if it should retry the request or abort it
;     mov  AL,  ERROR_INVLDDSKCHNGE       ; set error to invalid disk change
;     jmp  Cmd_Error
;
; indicates an unknown or otherwise general error occurred
;     mov  AL,  ERROR_GENERALFAILURE      ; set error to general failure
;     jmp  Cmd_Error
;
; Unit specified is invalid, return error
;     mov  AL,  ERROR_UNKNOWNUNIT         ; set error to unknown/invalid unit
;     jmp  Cmd_Error
;
; drive not ready
;     mov  AL,  ERROR_DEVICENOTREADY      ; set error to indicate no disc/drive not ready
;     jmp  Cmd_Error


; Command not available - simply return error
Cmd_NA proc near
      DEBUG_PrintMsg DBGENABLE, DbgMsg_Cmd_NA 
      mov  AL,  ERROR_UNKNOWNCMD    ; set error to unknown cmd
endp  ; fall through to Cmd_Error

; indicate driver should return with error,
; error code should be placed in AL before call
; all registers (except CS & stack) undefined after this call
Cmd_Error proc near
      lds  BX, [CS:devRequest]                 ; re-load Device Request Block into DS:BX
                                               ; assume AL set to appropriate error code
      mov AH, byte ptr [BX+DEVREQ_STATUS+1]    ; get current status value (high byte)
      or  AX, STATUS_ERROR                     ; set error flag
      mov [BX+DEVREQ_STATUS], AX               ; set returned status value
      retn
endp


;*******************************************************************************
;*******************************************************************************

; 3  = error statistics - the return format is undefined.
; so we can only return command unknown.  Should not cause problems.
; If format found then we can implement.
CmdI_ErrStats proc near
      DEBUG_PrintMsg DBGENABLE, DbgMsg_CmdI_ErrStats

      jmp Cmd_NA
endp


;***************************************

; IOCTL Input subcommand 0  = return the address of the device header
; ES:DI points to control block on entry
CmdI_GetDevHdr proc near
      ; point (ES:DI) to return address (DD size) field of control block
      INC DI                  ; ADD  DI, CBLK_DEVHDRADDR

      ; store device header addr at DD pointed to by ES:DI
      xor  AX, AX             ; offset should be 0 == OFFSET devHdr
      stosw                   ; store into ES:DI and increment DI
      mov  AX, CS             ; segment should be same as CS == SEG devHdr
      stosw                   ; store into ES:DI

      retn
endp  ;CmdI_GetDevHdr


;*******************************************************************************

; Command to indicate caller is using this driver - does not have to do anything
Cmd_DevOpen proc near
	inc  word ptr [CS:devAccess]	; increment count of those accessing
	retn
endp  ;Cmd_DevOpen


;***************************************

; Command to indicate caller is no longer using this driver - may do nothing
Cmd_DevClose proc near
	dec  word ptr [CS:devAccess]	; decrement count of those accessing
	retn
endp  ;Cmd_DevClose


;*******************************************************************************
;*******************************************************************************


; Command to perform initalization
; Note: On the first call this command calls the nonresident init code,
; further calls will simply return
Cmd_Init proc near
      mov  AL, [CS:units]
      cmp  AL, -1
      jne  @@done
      call Init
@@done:
      retn
endp


;*******************************************************************************
