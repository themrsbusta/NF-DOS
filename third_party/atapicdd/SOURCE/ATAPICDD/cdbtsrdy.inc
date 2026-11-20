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
;  cdbtsrdy.inc
;  cdb test unit ready
;
;*******************************************************************************


PKT_TESTUNITREADY db AC_TESTUNITREADY, 0,0,0,0,0  ,0,0,0,0,0,0
; 0 = check if unit is ready
; 1 = lun=0 (obsolete), reserved
; 2 = reserved
; 3 = reserved
; 4 = reserved
; 5 = vendor reserved

; Command to test for unit ready
; on exit AL is set to 0 for ready or nonzero error value
; and carry is either cleared (ready) or set (not ready)
testUnitReady proc near
	push BX
      allocateBuffer 3					; allocate 6 bytes for cdb
	mov  BX, SS
	mov  word ptr [CS:packetcdbseg], BX		; setup cdb seg:off
	mov  word ptr [CS:packetcdboff], BP
	callInitPacket PKT_TESTUNITREADY, 6
	call PerformPacketCmd				; do request
	freeBuffer						; free allocated data
	pop  BX
	retn
endp  ;testUnitReady


;*******************************************************************************
