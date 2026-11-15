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
;  stack.inc
;  our local stack, marks end of resident section == end of
;  where drive specific data is keep
;
;*******************************************************************************

ALIGN 16

; the stack must be large enough for all our pushes, one or more buffers
; for temp data transfer with device, and any data ASPI driver may push
; and room for packet CDB / SRB (command description block, SCSI request blk)
STACKSIZE  EQU  1024       ; size of stack in words


; reserve room for stack
IFDEF __NASM_MAJOR__
mystack times STACKSIZE DW 0
ELSE
mystack DW STACKSIZE dup (0)
ENDIF
mystacktop:

; device specific data is at the end of the resident section,
; however we only reserve as much room as we will use
; i.e. [units]*DEVDATASIZE
endOfResidentSectionMarker:

; reserve room for device specific data
IFDEF __NASM_MAJOR__
devdata times MAX_DEVICES*DEVDATASIZE DB 0
ELSE
devdata DB MAX_DEVICES*DEVDATASIZE dup (0)
ENDIF

;*******************************************************************************
; End of Resident Code
;*******************************************************************************
