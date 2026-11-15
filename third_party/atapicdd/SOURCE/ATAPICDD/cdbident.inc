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
;  cdbident.inc
;  cdb INQUIRY (or identification/information) request
;
;*******************************************************************************


PKT_INQUIRY db AC_INQUIRY, 0, 0, 0FFh, 0, 0
; 0 = inquire about device
; 1 = LUN, reserved, CmdDt(bit 1==0), and Enable Vital Producet Data (bit 0)
; 2 = page or operation code, only valid if EVPD bit set
; 3 = reserved
; 4 = max bytes to return
; 5 = vender specific misc flags

; issues IDENTIFY DEVICE command and fills in buffer accordingly
; expects packetbufseg/off to be set upon entry and [unitReq] valid
; with packet buffer large enough for at least 255 bytes
identifyDevice proc near
	mov  AX, CS						; set cdb buffer
	mov  word ptr [CS:packetcdbseg], AX
	mov  word ptr [CS:packetcdboff], OFFSET PKT_INQUIRY
	mov  byte ptr [CS:cdbLen], 6			; set cdb length
	mov  byte ptr [CS:datadir], APC_INPUT	; indicate returns data
	call PerformPacketCmd				; do request & get data
	retn
endp  ;identifyDevice


;*******************************************************************************
