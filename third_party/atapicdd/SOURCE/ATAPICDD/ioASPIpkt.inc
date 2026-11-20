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
;  cmdASPI.inc
;  controller specific defines and routines for SCSI devices
;  accessed via ASPI (Adaptec's Advanced SCSI Programming Interface)
;
;*******************************************************************************
IFDEF OPT_SCSI


; Sends Packet command to device (unitReq), 
; then gets (or sends) data for (from) buffer
; on success AX == 0 and carry is clear
; on error carry is set and AX is the error code
; AH is the sense key, AL is the additional sense code
; (a call to request sense may return additional error information)
; all other registers preserved
;
; on input expects the following variables to be set appropriately
; unitReq (so we know which device to communicate with & if its SCSI or ATA)
; datadir (APC_INPUT, APC_OUTPUT, or APC_NONE depending on command to be issued)
; cdbLen ( in most cases will be 12, may also be 6,10,16,... depends on command)
; packetcdbseg/off  (far pointer to actual command description block)
; packetbufseg/off  (far pointer to buffer for input/output, or NULL for none)
;
; on output AX has error value and if APC_INPUT specified then packetbuf should
; contain the requested data
;
; this procedure calls appropriate handler depending on how the CD drive 
; physically connected (i.e. SCSI or ATAPI calls ASPI handler or ATA handler)
; with DS:BX pointing to device specific data for unitReq (where CS==DS)
PerformASPIPacketCmd proc near
	retn
endp ;PerformASPIPacketCmd


ENDIF ;OPT_SCSI
;*******************************************************************************
