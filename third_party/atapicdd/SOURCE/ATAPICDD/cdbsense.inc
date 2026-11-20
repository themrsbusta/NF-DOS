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
;  cdbsense.inc
;  cdb REQUEST SENSE, gets error/status information
;
;*******************************************************************************

; issues REQUEST SENSE packet
; and stores data in our cache buffer
; call first checks if useCachedSenseData is set, if so we
; clear and return immediately (as the data is already there)
; otherwise we issue the packet call using device specific
; data sense data buffer.
requestSenseCmd proc near
	; setup packet
	; including storing CX so proper amount of data returned
	; mov into packetbufseg/off
      call Cmd_NA
      retn
endp ;requestSenseCmd


;*******************************************************************************
