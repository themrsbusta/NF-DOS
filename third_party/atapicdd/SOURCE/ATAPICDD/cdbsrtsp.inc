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
;
;  cdbsrtsp.inc
;  cdb START STOP request, prepare device for use
;  may also be used to open/close tray
;
;*******************************************************************************

PKT_STARTSTOPUNIT db AC_STARTSTOPUNIT, 0,0,0,0,0
; 0 = enable or disable unit access
; 1 = set immediate flag [lun=0,reserved=0,immed=1/wait=0]
; 2 = reserved
; 3 = reserved
; 4 = eject/load flags [power=0,res=0,LoEj=?,start=?]
; 5 = vendor reserved

; issue STARTSTOP packet command
; on input expects AX to indicate eject/close flags
; on return carry set on error (clear otherwise) and AX is error or 0 on success
startStopCmd proc near
	push BX						; save register
      allocateBuffer 3					; allocate 6 bytes for cdb
	mov  BX, SS
	mov  word ptr [CS:packetcdbseg], BX		; setup cdb seg:off
	mov  word ptr [CS:packetcdboff], BP
	callInitPacket PKT_STARTSTOPUNIT, 6
	movb ptr [SS:,BP+4], AL				; set eject/load flags
	call PerformPacketCmd				; do request
	freeBuffer						; free allocated data
	pop  BX						; restore register
	retn
endp  ;startStopCmd


;*******************************************************************************
