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
;  devdata.inc
;  Device Specific Data
;  defines, etc. useful for accessing data that is stored per device
;
;*******************************************************************************

;Data stored per device:  
;  (stored at end of resident code, just before init code
;  so top of memory returned is just above last device data object)
;  52 paragraphs (832 bytes) per device found {assuming all OPTional code}
;  Without optional audio code we can omit the 804 byte TOC buffer
IFDEF OPT_AUDIO
DEVDATASIZE EQU 832
ELSE
DEVDATASIZE EQU 32      ; 28 bytes + 4 padd bytes for para alignment
ENDIF

;DW  flags     - 0 for invalid, 
;                byte 0:
;                bit 0-1 (controller): 00=unknown,01=ATA,10=SCSI,11 reserved
;                bit 2 (dma): should be 0 for SCSI, 1 if DMA mode is enabled for
;                       ATA (i.e. use DMA & dma dw is valid)
;                bit 3 (dma type): should be 0 for SCSI, 0 if ISA (dma dw is
;                       channel), 1 if PCI (dma dw is I/O address)
;                bit 4 (interrupt): should be 0 for SCSI, 0 for poll mode only,
;                       1 to interrupt mode enabled (irq valid)
;                bit 5 (useCachedSenseData): 1 if next call to requestSense
;                       should return cached data, 0 force pkt cmd call
;                bit 6 (noEventNotification): flag to disable event notification
;                       (media changed) checks on older drives
;                bit 7 (media changed): nonzero indicates media changed since
;                       last device request call
;                byte 1:
;                bit 0 (lock status): nonzero indicates drive locked, thus
;                       we can assume media will not change
;                bit 1-7 reserved
;DB 3 dup (?)  - device control address, three bytes
;    ASPI
;      host#(DB)   - the host adapter # for use with the ASPI call
;      target(DB)  - indicates which device on host adapter the request is for
;      lun(DB)     - the logical unit number of CD drive on the target device
;    ATA
;      baseIO(DW)  - the base I/O port of the ATA controller for the device
;      devchnl(DB) - CB_DH_DEV0 (master) or CB_DH_DEV1 (slave) device channel
;DB  reserved  - pad byte/reserved for future device or adapter information
;DD  asd       - adapter specific data, double word
;    ASPI
;      ASPI_entry(DD) - the seg:off for entry point when making a call to the
;                       ASPI manager
;    ATA
;      dma(DW)        - for ATA ISA DMA mode indicate DMA channel to use, 
;                       ATA PCI DMA BMCR reg base I/O addr
;      irq(DW)        - for ATA indicates the interrupt number for interrupt
;                       requests
;DB? sensedata - 18 bytes reserved for sense data
;DB? TOC       - 804 bytes for buffered Table Of Contents used for Audio
;                commands 4 byte header + 99*8 byte Track data + 8 byte Lead
;                Out data


;*******************************************************************************

; defines for accessing our Device Specific Data

; index into various parts of DSD
DSD_NDX_FLAGS           EQU 00h     ; location of flag word
DSD_NDX_FLAGLOW         EQU 00h     ; low byte
DSD_NDX_FLAGHIGH        EQU 01h     ; high byte
DSD_NDX_HOST            EQU 02h     ; for ASPI controlled devices, host ID
DSD_NDX_TARGET          EQU 03h     ; for ASPI controlled devices, target #
DSD_NDX_LUN             EQU 04h     ; for ASPI controlled devices, lun
DSD_NDX_BASEIO          EQU 02h     ; for ATA controlled devices, Base I/O addr
DSD_NDX_DEVCHANNEL      EQU 04h     ; for ATA controlled devices, master/slave
DSD_NDX_RESDA           EQU 05h     ; reserved for device or adapter data
DSD_NDX_ASPIENTRY       EQU 06h     ; ASPI Entry point
DSD_NDX_DMA             EQU 06h     ; ATA DMA channel or base I/O register
DSD_NDX_IRQ             EQU 08h     ; ATA interrupt value
DSD_NDX_SENSEDATA       EQU 0Ah     ; information returned by Request Sense
DSD_NDX_TOC             EQU 1Ch     ; buffered Table Of Contents for Audio

; masks and such for various parts of the flags byte
DSD_DEVTYPE             EQU 03h     ; low two bits indicate if use ASPI or ATA
DSD_DEVTYPE_ATA         EQU 01h
DSD_DEVTYPE_ASPI        EQU 02h

DSD_MEDIACHANGED        EQU 80h	; high bit of low byte, force media change

DSD_LOCKED              EQU 01h     ; low bit of high byte, is drive (un)locked

;*******************************************************************************

; calculates the segment:offset of device specific data for unitReq,
; then stores the address for quicker access, lds BX, [CS:unitReqDevData]
ldsregDSDprep proc near
     push AX                        ; save registers we modify
     push DX

     ; 1st calc and store the offset (of devdata[unitReq] )
     mov  AX, DEVDATASIZE           ; calc offset of start of this device's data
     xor  DH, DH                    ; sizeof(datachunk) * unitReq index value
     mov  DL, [unitReq]
     mul  DX                        ; offset is now in DX:AX, but we ignore DX
     add  AX, OFFSET devdata        ; add to beginning of device data pointer
     mov  word ptr [CS:unitReqDevData], AX   ; override DD with explicit word ptr

     ; then the segment
     mov  AX, CS                    ; get segment, should be same as code (CS)
     mov  word ptr [CS:unitReqDevData+2], AX ; store segment

     pop  DX                        ; restore modified registers
     pop  AX
     retn
endp ;ldsbxDSDprep


IFDEF __NASM_MAJOR__
%macro CHECKDSD 0
%endmacro
ELSE
CHECKDSD MACRO
IFDEF OPT_DEBUG
	; safety check
	push AX
	push BX
	mov  AX, CS
	mov  BX, DS
	cmp  AX, BX
	je  @@offcheck

	DEBUG_PrintChar DBGALL, '!'
	DEBUG_PrintChar DBGALL, '!'
	DEBUG_PrintChar DBGALL, '!'

	@@offcheck:
	pop  BX
	mov  AX, OFFSET endOfResidentSectionMarker
	jae  @@endcheck

	DEBUG_PrintChar DBGALL, '*'
	DEBUG_PrintChar DBGALL, '*'
	DEBUG_PrintChar DBGALL, '*'
	
	@@endcheck:
	pop  AX
ENDIF
ENDM
ENDIF


;*******************************************************************************
