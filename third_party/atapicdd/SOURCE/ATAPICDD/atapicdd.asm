;*******************************************************************************
;
;  ATAPICDD - ATAPI CD Driver for DOS
;  Written by Kenneth J. Davis <jeremyd@computer.org>, 2001-2003
;  Released to public domain  [ U.S. Definition ]
;
;  ATAPI code based on public domain C code from Hale Landis' ATADRVR
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
   ReleaseStr    EQU 'v0.2.14 ALPHA'
   DRIVERVERSION EQU 0,2,14
;  Version # = major.minor.developement (0 for dev == released version)
;
;  Still to do, add support for optional commands to play/stop/resume audio
;  Also add support for buffering/prefetching to enhance performance and
;  support for direct control strings to be sent to CD
;  Future, enhance for DVD and/or CD/DVD writers
;  WARNING not re-entrant! presently relies on global data & own single stack,
;  i.e. it must complete the current IOCTL request before receiving another one
;
;*******************************************************************************
MINBUILD EQU 1

; Enable or disable various parts of driver (assemble time options)
; Recommend for a minimal functioning driver should have
; one of OPT_ATAPI or OPT_SCSI (both only if you actually have both)
; only enable OPT_AUDIO and OPT_READ* to play audio CDs or extracting data
; only enable OPT_WRITE for CD-Writers and you want support through the CDEX
; only enable OPT_8020ONLY for older drives to save memory; reduced functionality
; enable OPT_UPC, OPT_RESET, OPT_DRVINFO if you have an application that uses it
; OPT_DRVCTRL may generally remain enabled (for eject/lock/etc)
; OPT_DIRECTCTRL is optional, but should be enabled for CD Writers & public releases
; OPT_8086 should only be enabled if you need to support 8086s, default requires
;          186 for rep insw/rep outsw which are replaced by slower in/out loops
IFNDEF MINBUILD
OPT_DEBUG      EQU 1    ; enable DEBUG support (actual debugging output still must be enabled)
OPT_ATAPI      EQU 1    ; support ATA based ATAPI devices
OPT_SCSI       EQU 1    ; support ASPI based SCSI devices
;OPT_AUDIO      EQU 1    ; support 'optional for basic usage' audio driver calls
OPT_READRAW    EQU 1    ; support raw read driver calls (assuming CD drive supports it)
OPT_READMSF    EQU 1    ; support msf read driver calls (assuming CD drive supports it)
OPT_WRITE      EQU 1    ; support optional cd writer driver calls
;OPT_8020ONLY   EQU 1   ; limit packets to those within the obsolete INF-8020i document
OPT_UPC        EQU 1    ; support get UPC call
OPT_RESET      EQU 1    ; support reset call
OPT_DRVCTRL    EQU 1    ; support useful drive control operations, e.g. open/close tray, ...
OPT_DRVINFO    EQU 1    ; support possibly useful drive information operations
OPT_DIRECTCTRL EQU 1    ; enable limited device & driver control via external programs
;OPT_8086       EQU 1   ; enable 8086 compatible build

; maximum number of subunits we can handle
; (defaults are 2 for min build and 10 otherwise)
; 8 is maximum when using the 4 standard ATA controller base
; I/O address (2 devices, master/slave, per controller)
; also include extra for any additional SCSI based CD drives
; This is a somewhat arbitrary limit, effects disk size and max used RAM
; not runtime memory (which is determined by actual # devices found,
; other than setting maximum that can possibly be used/devices supported)
MAX_DEVICES EQU 10

ELSE   ;MINBUILD
OPT_DEBUG      EQU 1
OPT_ATAPI      EQU 1
MAX_DEVICES    EQU 2
ENDIF  ;predefined build setups


; to handle segment override syntax differences between NASM & TASM
; NASM uses:    mov byte ptr [segovr:reg], val    (where ptr is replaced by emptry string via %define ptr)
; TASM uses:    mov byte ptr segovr:[reg], val
; instead of explicit mov byte call, use movb ptr [segovr:,reg], val	e.g. movb ptr [ES:,DI], -1
movb MACRO p, segovr, reg, val
      mov byte ptr segovr[reg], val		; e.g. mov byte ptr ES:[DI], -1
ENDM
movw MACRO p, segovr, reg, val
      mov word ptr segovr[reg], val		; e.g. mov word ptr ES:[DI], -1
ENDM
cmpw MACRO p, segovr, reg, val
      cmp word ptr segovr[reg], val		; e.g. mov word ptr ES:[DI], -1
ENDM
mov2 MACRO dst, segovr, reg
      mov dst, segovr[reg]			; e.g. mov AL, CS:[BX]
ENDM
callw MACRO segovr, reg
      call word ptr segovr[reg]            ; e.g. call word ptr CS:[DI]
ENDM
testb MACRO p, segovr, reg, val
	test byte ptr segovr[reg], val      ; e.g. test byte ptr SS:[BP+2], STATUS_NEA
ENDM

LOCALS      ; needed by TASM to enable @@ local symbols to work
JUMPS       ; automatically handle case where conditional jump short is too far away

code segment para public 'CODE' use16
      assume cs:code, ds:nothing, es:nothing

INCLUDE devhdr.inc      ; DOS device driver header + CD-ROM extension, & global data
INCLUDE debug.inc       ; debugging related functions, such as DEBUG_PrintMsg
INCLUDE devstrat.inc    ; DOS device driver interrupt and strategy routines
INCLUDE devdata.inc
INCLUDE ioPktInt.inc    ; generic interface to peform packet commands
INCLUDE ioATA.inc       ; command functions for ATA controller / ATAPI devices
INCLUDE ioATArst.inc
INCLUDE ioATApkt.inc
INCLUDE ioASPI.inc      ; command functions for ASPI access to SCSI devices
INCLUDE ioASPIrst.inc
INCLUDE ioASPIpkt.inc
INCLUDE cdb.inc
INCLUDE cdbevnot.inc
INCLUDE cdbident.inc
INCLUDE cdblock.inc
INCLUDE cdbread.inc
INCLUDE cdbsense.inc
INCLUDE cdbsrtsp.inc
INCLUDE cdbtsrdy.inc
INCLUDE data.inc        ; data related IOCTLs implementation
INCLUDE dirctrl.inc
INCLUDE drvctrl.inc
INCLUDE drvinfo.inc
INCLUDE audio.inc       ; audio related IOCTLs implementation
INCLUDE stack.inc       ; marks end of resident code and has reserved stack + data space
INCLUDE initATA.inc     ; initialization functions for ATA/ATAPI devices
INCLUDE initASPI.inc    ; initialization functions for ASPI access to SCSI devices
INCLUDE initcmdl.inc
INCLUDE initmsg.inc
INCLUDE init.inc        ; device driver init, including cmd line parsing and device init

code ends

end devHdr

;*******************************************************************************
