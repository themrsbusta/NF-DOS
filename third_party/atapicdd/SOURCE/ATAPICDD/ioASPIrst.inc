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
;  ioASPIrst.inc
;  ASPI based device reset
;
;*******************************************************************************
IFDEF OPT_SCSI

IFDEF OPT_RESET
;TODO
PerformASPIResetCmd proc near
	retn
endp ;PerformASPIResetCmd
ENDIF ;OPT_RESET


ENDIF ;OPT_SCSI
;*******************************************************************************
