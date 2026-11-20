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
;  drvhdr.inc
;  device driver header with CDEX extensions and global data (not unit specific)
;
;*******************************************************************************


; Normal device driver header with CD-ROM character device extension
devHdr:
      DD -1             ; point to next driver in chain, -1 for end of chain
      DW 0C800h         ; device attributes
                        ;  Bit 15         1       - Character device
                        ;  Bit 14         1       - IOCTL supported
                        ;  Bit 13         0       - Output 'till  busy
                        ;  Bit 12         0       - Reserved
                        ;  Bit 11         1       - OPEN/CLOSE/RM supported
                        ;  Bit 10-4       0       - Reserved
                        ;  Bit  3         0       - Dev is CLOCK
                        ;  Bit  2         0       - Dev is NUL
                        ;  Bit  1         0       - Dev is STO (standard output)
                        ;  Bit  0         0       - Dev is STI (standard input)
      DW OFFSET StrategyProc  ; device driver Strategy entry point
      DW OFFSET InterruptProc ; device driver Interrupt entry point

; device name (overridden by /D:name command line option, e.g. /D:FDCD0001)
devName     DB "FDCD0000"           ; aka "ATAPICDD"

CDDevHdrExt:
            DW 0        ; reserved (should be 0)
driveLetter DB 0        ; the 1st CD-ROM's drive letter (initially 0, set by MSCDEX or equiv)
units       DB -1       ; how many drives found (number of units)
                        ; -1 for us indicates not yet initalized

; mark this driver as mine
myMarker	DB 'KJD PD$',0
myVersion	DW DRIVERVERSION


; Other DOS device driver related resident data
devRequest  DD 0        ; stores the device request address (DOS, set by strategy, used by int proc)

oldSS DW 0              ; store original stack, replaced with local one during invocation
oldSP DW 0


; Other CD driver implementation specific resident data
devAccess	DW 0		; number of active users of device driver, serves no
				; purpose other than a counter, for DevOpen & DevClose

unitReq     DB 0        ; which unit (index into our data structures) action requested for
            DB 0        ; padding
unitReqDevData DD 0     ; precalculated (seg:off for lds bx) start of device specific data for unitReq

datadir       DB 0      ; indicate if pkt cmd inputs or outputs data (data transfer direction)
cdbLen        DB 0      ; indicate packet length in bytes (generally should be 12)
packetcdb:              ; alternate DD reference for packet cdb buffer
packetcdboff  DW 0      ; actual packet data sent to the CD drive
packetcdbseg  DW 0      ; hold pointer (seg:offset) to command description block, i.e. the
packetbuf:              ; alternate DD reference for packet data transfer buffer
packetbufoff  DW 0      ; offset of buffer,  we assume it's large enough for requested data
packetbufseg  DW 0      ; segment of buffer, for data transfer (input or output)


;*******************************************************************************
