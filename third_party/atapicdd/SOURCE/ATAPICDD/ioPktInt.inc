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
;  ioPktInt.inc
;  Input/Output Packet Interface
;  Defines a generic method of transfering a CDB (command description block)
;  to a SCSI device and sending/receiving the corresponding data if any.
;
;*******************************************************************************

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
PerformPacketCmd proc near
     push DS
     push BX

     ; load DS:BX with address of device specific data for current unit
     lds BX, [CS:unitReqDevData]
     
     ; invoke appropriate handler for unit type (ASPI or ATAPI)
     mov  AL, [BX]                ; get flag byte
     and  AL, DSD_DEVTYPE         ; ignore all but how we should access the unit
IFDEF OPT_ATAPI
     cmp  AL, DSD_DEVTYPE_ATA     ; is it an ATAPI drive?
     je   doATAPIpacketCmd
ENDIF
IFDEF OPT_SCSI
     cmp  AL, DSD_DEVTYPE_ASPI    ; well is it an SCSI drive then?
     je   doSCSIpacketCmd
ENDIF
     jmp @@done                   ; give up, unknown drive type
     
IFDEF OPT_ATAPI
     doATAPIpacketCmd:
     call PerformATAPacketCmd
     jmp @@done
ENDIF
     
IFDEF OPT_SCSI
     doSCSIpacketCmd:
     call PerformASPIPacketCmd
     jmp @@done
ENDIF

     ; if we reach here then no method known to send command
     xor  AX, AX
     stc
     
     @@done:
     
     pop  BX
     pop  DS
     retn
endp ;PerformPacketCmd


IFDEF OPT_RESET
; reset device using proper controller method
PerformResetCmd proc near
     push DS
     push BX

     ; load DS:BX with address of device specific data for current unit
     lds BX, [CS:unitReqDevData]
     
     ; invoke appropriate handler for unit type (ASPI or ATAPI)
     mov  AL, [BX]                ; get flag byte
     and  AL, DSD_DEVTYPE         ; ignore all but how we should access the unit
IFDEF OPT_ATAPI
     cmp  AL, DSD_DEVTYPE_ATA     ; is it an ATAPI drive?
     je   doATAPIresetCmd
ENDIF
IFDEF OPT_SCSI
     cmp  AL, DSD_DEVTYPE_ASPI    ; well is it an SCSI drive then?
     je   doSCSIresetCmd
ENDIF
     jmp @@done                   ; give up, unknown drive type
     
IFDEF OPT_ATAPI
     doATAPIresetCmd:
     call PerformATAResetCmd
     jmp @@done
ENDIF
     
IFDEF OPT_SCSI
     doSCSIresetCmd:
     call PerformASPIResetCmd
     jmp @@done
ENDIF

     ; if we reach here then no method known to send command
     xor  AX, AX
     stc
     
     @@done:
     
     pop  BX
     pop  DS
     retn
endp ;PerformResetCmd
ENDIF ; OPT_RESET


;*******************************************************************************
