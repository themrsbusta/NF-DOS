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
;  cdb.inc
;  Command Description Block
;  general routines and defines used in setting up a cdb structure
;
;*******************************************************************************

; ATAPI/SCSI Packet Commands	
; see: 
; INF-8090i [5.4]  ATA Packet Interface for DVD (and CD) ROM, RAM, R/RW devices
; (obsolete) INF-8020i [E]  ATA Packet Interface for CD-ROMs
; SCSI-3 specificiation, SPC (primary commands) and MMC (multi-media commands)
; SCSI-2 specification X3T9.2/375R revision 10L
; M = mandatory according to SCSI 2 spec
; P = if audio play commands implemented, all of these must be
; * in SCSI 2 spec, but may not be implemented by ATAPI devices
; the remaining are optional commands
AC_BLANK                EQU 0A1h
AC_CLOSETRACKSESSION    EQU 5Bh
AC_COMPARE              EQU 39h
AC_ERASE10              EQU 2Ch
AC_FORMATUNIT           EQU 04h
AC_GETCONFIGURATION     EQU 46h
AC_GETEVENTSTATUSNOTIFY EQU 4Ah
AC_GETPERFORMANCE       EQU 0ACh
AC_INQUIRY              EQU 12h           ; M
AC_LOADUNLOADMEDIUM     EQU 0A6h
AC_LOCKUNLOCKCACHE      EQU 36h
AC_LOGSELECT            EQU 4Ch
AC_LOGSENSE             EQU 4Dh
AC_MECHANISMSTATUS      EQU 0BDh
AC_MODESELECT           EQU 55h
AC_MODESENSE            EQU 5Ah
AC_PAUSERESUME          EQU 4Bh
AC_PLAYAUDIO10          EQU 45h           ; P
AC_PLAYAUDIO12          EQU A5h           ; P *
AC_PLAYAUDIOMSF         EQU 47h           ; P
AC_PLAYAUDIOTRACKINDEX  EQU 48h           ; P *
AC_PLAYTRACKREL10       EQU 49h           ; P *
AC_PLAYTRACKREL12       EQU A9h           ; P *
AC_PLAYCD               EQU 0BCh
AC_PREFETCH             EQU 34h
AC_PREVENTMEDIUMREMOVAL EQU 1Eh
AC_READ10               EQU 28h           ; M
AC_READ12               EQU 0A8h
AC_READBUFFER           EQU 3Ch
AC_READBUFFERCAPACITY   EQU 5Ch
AC_READCAPACITY         EQU 25h           ; M
AC_READCD               EQU 0BEh
AC_READCDMSF            EQU 0B9h
AC_READDISCINFO         EQU 51h
AC_READDVDSTRUCTURE     EQU 0ADh
AC_READFORMATCAPACITIES EQU 23h
AC_READHEADER           EQU 44h
AC_READSUBCHANNEL       EQU 42h
AC_READTOC              EQU 43h
AC_READTRACKINFO        EQU 52h
AC_RECEIVEDIAGRESULTS   EQU 1Ch
AC_RELEASE6             EQU 17h           ; M
AC_RELEASE10            EQU 57h
AC_REPAIRRZONE          EQU 58h
AC_REPORTKEY            EQU 0A4h
AC_REQUESTSENSE         EQU 03h           ; M
AC_RESERVE6             EQU 16h           ; M
AC_RESERVE10            EQU 56h
AC_RESERVETRACK         EQU 53h
AC_SCAN                 EQU 0BAh
AC_SEEK6                EQU 0Bh
AC_SEEK10               EQU 2Bh
AC_SENDCUESHEET         EQU 5Dh
AC_SENDDIAGNOSTIC       EQU 1Dh           ; M
AC_SENDDVDSTRUCTURE     EQU 0BFh
AC_SENDEVENT            EQU 0A2h
AC_SENDKEY              EQU 0A3h
AC_SENDOPCINFO          EQU 54h
AC_SETCDSPEED           EQU 0BBh
AC_SETREADAHEAD         EQU 0A7h
AC_SETSTREAMING         EQU 0B6h
AC_STARTSTOPUNIT        EQU 1Bh
AC_STOPPLAYSCAN         EQU 4Eh
AC_SYNCCACHE            EQU 35h
AC_TESTUNITREADY        EQU 00h           ; M
AC_VERIFY10             EQU 2Fh
AC_VERIFY12             EQU AFh           ;   *
AC_WRITE10              EQU 2Ah
AC_WRITE12              EQU 0AAh
AC_WRITEANDVERIFY10     EQU 2Eh
AC_WRITEBUFFER          EQU 3Bh

; ATAPI Packet Command causes Input or Output of data
APC_INPUT	EQU 0h
APC_OUTPUT	EQU 1h
APC_DONE	EQU -1
APC_NONE    EQU APC_DONE



;*******************************************************************************

; each packet file has a data block e.g.
;PKT_STARTSTOPUNIT db AC_STARTSTOPUNIT, 0,0,0,0...,0
; description 0 = cmd
;             1 = blah ...
;proc ???? near
;   ; set constant values
;   callInitPacket OFFSET PKT_STARTSTOPUNIT, 12
;   ; now set variable data passed in as parameters
;   mov [packet+?], ???   
;endp

; copy packet setup (template) data to packet cdb buffer
; ipktDataOff is offset of template data
; pktSize is true (6/10/12) number of bytes of this packet
; expects packetcdbseg/off to be set to a large enough buffer
; on return packet cdb data is set to template, cdbLen
; is set to cdb length, datadir and packetbufseg/off are
; initialized for no data transfer
IFDEF __NASM_MAJOR__
%macro callInitPacket 2
	push SI				; store registers
	push CX
	mov  CL, %2				; set BL to true packet size
	mov  SI, %1				; set SI to offset of template data
	call InitPacket			; actually copy the data & init stuff
	pop  CX				; restore registers
	pop  SI
%endmacro
ELSE
callInitPacket MACRO ipktDataOff, pktSize
	push SI				; store registers
	push CX
	mov  CL, pktSize			; set BL to true packet size
	mov  SI, OFFSET ipktDataOff	; set SI to offset of template data
	call InitPacket			; actually copy the data & init stuff
	pop  CX				; restore registers
	pop  SI
ENDM
ENDIF
InitPacket proc near
      push DS				; store registers
      push ES
      push DI
      push AX

      ; misc setup
      mov  [CS:cdbLen], CL                  ; indicate packet length (6/10/12 etc)
      mov  byte ptr [CS:datadir], APC_DONE  ; indicate pkt cmd transfers no data
      mov  word ptr [CS:packetbufseg], 0    ; set buffer to 0x0000:0x000
      mov  word ptr [CS:packetbufoff], 0
	
      ; setup for template data copy
      les  DI, dword ptr [CS:packetcdb]     ; set ES:DI to packet cdb buffer
      mov  AX, CS							; set DS:SI to packet template
      mov  DS, AX							; assumes SI preset on entry
      xor  CH, CH							; set CX = pktSize / 2, clear high byte
      shr  CX, 1							; assumes on entry CL=pktSize, CX=CX/2

      ; actually copy the data
      rep movsw

      pop  AX				; restore registers
      pop  DI
      pop  ES
      pop  DS
      retn
endp  ;InitPacket


; sets 1st N words of our working buffer to 0
; no registers modified except CX, which is count of words to clear on input
; expects buffer to be at SS:BP
clearBuffer proc near
      push BP

      @@looptop:
      movw ptr [SS:,BP], 0
      add  BP, 2
      loop @@looptop

      pop  BP
      retn
endp  ;clearBuffer

IFDEF __NASM_MAJOR__

%macro call_clearBuffer 1
      push CX
      mov  CX, %1
      call clearBuffer
      pop  CX
%endmacro

; saves BP, decrements stack pointer, and sets BP to start of buffer
; expects as argument number of words to reserve (times 2 == byte count)
%macro allocateBuffer 1
      push BP                   ; save current BP in case in use somewhere
      mov  BP, SP               ; maintain current SP for quick cleanup
      sub  SP, %1*2             ; reserve buffer space on stack, size in bytes
      push BP                   ; store original SP (restored by freeBuffer)
      sub  BP, %1*2             ; store address of buffer in SS:BP
%endmacro

; increments stack pointer and restores BP
; VERY IMPORTANT, stack must be at same state it was before right after
; call to allocateBuffer i.e.  no pushes without a pop between allocateBuffer X ; freeBuffer
; MAY NOT ALTER FLAGS!
%macro freeBuffer 0
      pop  BP                   ; retrieve stored original SP
      mov  SP, BP               ; return stack to just after 1st push BP
                                ; same as  add SP, bufferSize*2 but no flags changed
      pop  BP                   ; restore original BP
%endmacro

ELSE

call_clearBuffer MACRO N
	push CX
	mov  CX, N
	call clearBuffer
	pop  CX
ENDM call_clearBuffer

; saves BP, decrements stack pointer, and sets BP to start of buffer
; expects as argument number of words to reserve (times 2 == byte count)
allocateBuffer MACRO bufferSize
      push BP                   ; save current BP in case in use somewhere
	mov  BP, SP               ; maintain current SP for quick cleanup
      sub  SP, bufferSize*2     ; reserve buffer space on stack, size in bytes
	push BP                   ; store original SP (restored by freeBuffer)
	sub  BP, bufferSize*2     ; store address of buffer in SS:BP
ENDM

; increments stack pointer and restores BP
; VERY IMPORTANT, stack must be at same state it was before right after
; call to allocateBuffer i.e.  no pushes without a pop between allocateBuffer X ; freeBuffer
; MAY NOT ALTER FLAGS!
freeBuffer MACRO
      pop  BP                   ; retrieve stored original SP
	mov  SP, BP               ; return stack to just after 1st push BP
                                ; same as  add SP, bufferSize*2 but no flags changed
      pop  BP                   ; restore original BP
ENDM

ENDIF
	

;*******************************************************************************
