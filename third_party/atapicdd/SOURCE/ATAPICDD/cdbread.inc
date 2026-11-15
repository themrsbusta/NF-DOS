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
;  cdbread.inc
;  cdb READ request
;
;*******************************************************************************

PKT_READ10 db AC_READ10, 0,0,0,0,0,0,0,0,0
; 0 = read (10 byte version)
; 1 = various flag fields
; 2 = LBA of start address high word (MSB)
; 3 = "" high word (LSB)
; 4 = LBA of start address low word (MSB)
; 5 = "" low word (LSB)
; 6 = reserved
; 7 = number (count) of sectors to read in (MSB)
; 8 = "" (LSB)
; 9 = vendor reserved


PKT_SEEK10 db AC_SEEK10, 0,0,0,0,0
; 0 = request unit seek to LBA address
; 1 = lun (0), reserved field
; 2 = LBA address to seek to (MSB)
; 3 = ""
; 4 = ""
; 5 = "" (LSB)
; 6 = reserved
; 7 = reserved
; 8 = reserved
; 9 = vendor reserved

; issue SEEK packet command
; on input expects DX:AX to be (intel) double word LBA address
; on return carry set on error (clear otherwise) and AX is error or 0 on success
seekCmd proc near
	push BX						; save register
      allocateBuffer 5					; allocate 10 bytes for cdb
	mov  BX, SS
	mov  word ptr [CS:packetcdbseg], BX		; setup cdb seg:off
	mov  word ptr [CS:packetcdboff], BP
	callInitPacket PKT_SEEK10, 10
	xchg AH, AL						; flip to big endian format
	xchg DH, DL
	movw ptr [SS:,BP+2], DX				; set LBA address (high word)
	movw ptr [SS:,BP+4], AX				; set LBA address (low word)
	call PerformPacketCmd				; do request
	freeBuffer						; free allocated data
	pop  BX						; restore register
	retn
endp  ;seekCmd


;*******************************************************************************
