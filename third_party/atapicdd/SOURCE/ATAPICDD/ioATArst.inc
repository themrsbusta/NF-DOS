;*******************************************************************************
;
;  ATAPICDD - ATAPI CD Driver for DOS
;  Written by Kenneth J. Davis <jeremyd@computer.org>, 2001-2003
;  Released to public domain  [ U.S. Definition ]
;
;  ATA/ATAPI code based on public domain C code from Hale Landis' ATADRVR
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
;  ioATArst.inc
;  ATA specific device reset
;
;*******************************************************************************
IFDEF OPT_ATAPI

IFDEF OPT_RESET
; perform ATAPI software reset (this is different from the normal
; ATA setting device control SRST bit, which may adversely effect
; other ATA device on controller, and must be used for device reset)
; issue ATA ATAPI Soft Reset 08h
; TODO
PerformATAResetCmd proc near
      retn
endp ;PerformATAResetCmd
ENDIF ;OPT_RESET


ENDIF ;OPT_ATAPI
;*******************************************************************************
