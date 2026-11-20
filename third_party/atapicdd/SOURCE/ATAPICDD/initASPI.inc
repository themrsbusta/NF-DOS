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
;  initASPI.inc
;  initialization routines (and data) for ASPI (SCSI) CD-ROM drives
;  all code and data that need not be resident but specific to ASPI based
;  devices should be here
;
;*******************************************************************************
IFDEF OPT_SCSI


; blah
search_for_scsi_drives proc near
endp ;search_for_scsi_drives


ENDIF ;OPT_SCSI
;*******************************************************************************
