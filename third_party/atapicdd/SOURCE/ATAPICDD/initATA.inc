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
;  initATA.inc
;  initialization routines (and data) for ATA (IDE) CD-ROM drives
;  all code and data that need not be resident but specific to ATA based
;  devices should be here
;
;*******************************************************************************
IFDEF OPT_ATAPI

; Default locations to look for ATA/ATAPI devices
;	controller    base    interrupt
;	primary       0x1F0   0x76 (IRQ14)
;	secondary     0x170   0x77 (IRQ15), or maybe 0x72 (IRQ10)
;	tertiary      0x1E8   0x74 (IRQ12), or maybe 0x73 (IRQ11)
;	quaternary    0x168   0x72 (IRQ10), or maybe 0x71 (IRQ9)
; default base locations we look for (base2 == base + 0x200,
; i.e. status == base + 0x206)
; we know about 4 standard base I/O addresses to search for controllers
MAX_CONTROLLERS	EQU	04h	

FLG_NONE		EQU	00h	; used in flags to indicate if device exists
FLG_MASTER		EQU	01h
FLG_SLAVE		EQU	02h
FLG_MATAPI		EQU	04h	; master is an ATAPI device
FLG_SATAPI		EQU	08h	; slave is an ATAPI device
FLG_MATA		EQU	10h	; master is an ATA (not ATAPI) device
FLG_SATA		EQU	20h	; slave is an ATA device (not ATAPI)

; may be overridden by command line,
; set of base I/O addresses (and how many to actually check)
ata_base_io_list dw 1F0h, 170h, 1E8h, 168h
ata_base_io_list_cnt db 4

; Determines if device exists & if so if its an ATAPI one
; Given base I/O address, <irq>, <dma>, will check for
; any devices and return if devices found and if they are
; ATAPI or not.  (reg_config)
; no registers changed, expects BX set on entry to start of DSD
init_ata_search proc near
	push AX

	mov  AX, [BX+DSD_NDX_BASEIO]
      DEBUG_PrintChar DBGATA, 'i'
      DEBUG_PrintChar DBGATA, 'o'
	DEBUG_PrintNumber DBGATA, ah
	DEBUG_PrintNumber DBGATA, al

	; set up Device Control register
	call_pio_outbyte CB_DC, devCtrl

	; lets see if there is a device (master)
	call_pio_outbyte CB_DH, CB_DH_DEV0	; select device 0 (master)
	DELAY400NS;
	call_pio_outbyte CB_SC, 055h
	call_pio_outbyte CB_SN, 0aah
	call_pio_outbyte CB_SC, 0aah
	call_pio_outbyte CB_SN, 055h
	call_pio_outbyte CB_SC, 055h
	call_pio_outbyte CB_SN, 0aah
	call_pio_inbyte CB_SC			; al = sector count
	mov  AH, AL
	call_pio_inbyte CB_SN			; al = sector number
	cmp  AX, 55AAh				; sector count == 0x55  &&  sector number == 0xaa
	jne  @@notfound_1				; device not found, check again after reset
	; indicate a master device was found
	or   byte ptr [BX+DSD_NDX_DEVCHANNEL], FLG_MASTER	
	DEBUG_PrintChar DBGATA, 'M'
	@@notfound_1:

	; lets see if there is a device (slave)
	call_pio_outbyte CB_DH, CB_DH_DEV1	; select device 1 (slave)
	DELAY400NS;
	call_pio_outbyte CB_SC, 055h
	call_pio_outbyte CB_SN, 0aah
	call_pio_outbyte CB_SC, 0aah
	call_pio_outbyte CB_SN, 055h
	call_pio_outbyte CB_SC, 055h
	call_pio_outbyte CB_SN, 0aah
	call_pio_inbyte CB_SC			; al = sector count
	mov  AH, AL
	call_pio_inbyte CB_SN			; al = sector number
	cmp  AX, 55AAh				; sector count == 0x55  &&  sector number == 0xaa
	jne  @@notfound_2				; device not found, check again after reset
	; indicate a slave device was found
	or   byte ptr [BX+DSD_NDX_DEVCHANNEL], FLG_SLAVE	
	DEBUG_PrintChar DBGATA, 'S'
	@@notfound_2:

	; now we think we know which devices, if any are there,
	; so lets try a soft reset (ignoring any errors).
	call_pio_outbyte CB_DH, CB_DH_DEV0
	DELAY400NS;
	DEBUG_PrintChar DBGATA, '{'
	call reg_reset
	DEBUG_PrintChar DBGATA, '}'

	; lets check device 0 again, is the device really there?
	; is it ATA or ATAPI?
	call_pio_outbyte CB_DH, CB_DH_DEV0	; select device 0 (master)
	DELAY400NS;
	call_pio_inbyte CB_SC		; al = sector count
	mov  AH, AL
	call_pio_inbyte CB_SN		; al = sector number
	; if ( (sector count == 0x01) && (sector number == 0x01) ) // device found, check type
	DEBUG_PrintChar DBGATA, 'D'
	DEBUG_PrintChar DBGATA, '0'
	DEBUG_PrintChar DBGATA, 'S'
	DEBUG_PrintChar DBGATA, 'C'
	DEBUG_PrintNumber DBGATA, ah
	DEBUG_PrintChar DBGATA, 'S'
	DEBUG_PrintChar DBGATA, 'N'
	DEBUG_PrintNumber DBGATA, al
	cmp  AX, 0101h
	je   @@checktypedev0		; see if its really an ATAPI device

	; indicate not really a device there
	and  byte ptr [BX+DSD_NDX_DEVCHANNEL], NOT FLG_MASTER
	jmp  @@checkdev1			; device 0 not found, proceed to recheck device 1

	@@checktypedev0:
	DEBUG_PrintChar DBGATA, 'm'
	; we are sure there is a device, mark it (in case not already done so)
	or   byte ptr [BX+DSD_NDX_DEVCHANNEL], FLG_MASTER

	; see if ATAPI
	call_pio_inbyte CB_CL		; al = cylinder low byte
	mov  AH, AL
	call_pio_inbyte CB_CH		; al = cylinder high byte
	; if ( (cyl low == 0x14) && (cyl high == 0xEB) ) // ATAPI device found
	DEBUG_PrintChar DBGATA, 'D'
	DEBUG_PrintChar DBGATA, '0'
	DEBUG_PrintChar DBGATA, 'C'
	DEBUG_PrintChar DBGATA, 'H'
	DEBUG_PrintNumber DBGATA, al
	DEBUG_PrintChar DBGATA, 'C'
	DEBUG_PrintChar DBGATA, 'L'
	DEBUG_PrintNumber DBGATA, ah
	cmp  AX, 14EBh
	jne  @@isatafounddev0		; ignore, either unknown or ATA device found

	; we found an ATAPI device, so mark it as such
	or   byte ptr [BX+DSD_NDX_DEVCHANNEL], FLG_MATAPI
	jmp  @@checkdev1

	@@isatafounddev0:
      ;if ( ( cl == 0x00 ) && ( ch == 0x00 ) && ( st != 0x00 ) )
	cmp  AX, 0				; compare cyl low and cyl high to 0x00
	jne  @@checkdev1			; no, then some unknown device
	call_pio_inbyte CB_STAT
	DEBUG_PrintChar DBGATA, 'S'
	DEBUG_PrintChar DBGATA, 'T'
	DEBUG_PrintNumber DBGATA, al
	cmp  AL, 0
	jz   @@checkdev1			; if not zero then we found an ATA device (e.g. hard drive)
	or   byte ptr [BX+DSD_NDX_DEVCHANNEL], FLG_MATA	
	;jmp @@ checkdev1

	@@checkdev1:
	; lets check device 1 again, is the device really there?
	; is it ATA or ATAPI?
	call_pio_outbyte CB_DH, CB_DH_DEV1	; select device 1 (slave)
	DELAY400NS;
	call_pio_inbyte CB_SC		; al = sector count
	mov  AH, AL
	call_pio_inbyte CB_SN		; al = sector number
	; if ( (sector count == 0x01) && (sector number == 0x01) ) // device found, check type
	DEBUG_PrintChar DBGATA, 'D'
	DEBUG_PrintChar DBGATA, '1'
	DEBUG_PrintChar DBGATA, 'S'
	DEBUG_PrintChar DBGATA, 'C'
	DEBUG_PrintNumber DBGATA, ah
	DEBUG_PrintChar DBGATA, 'S'
	DEBUG_PrintChar DBGATA, 'N'
	DEBUG_PrintNumber DBGATA, al
	cmp  AX, 0101h
	je   @@checktypedev1		; see if its really an ATAPI device

	; indicate not really a device there
	and  byte ptr [BX+DSD_NDX_DEVCHANNEL], NOT FLG_SLAVE
	jmp  @@done				; device 1 not found, proceed to end

	@@checktypedev1:
	DEBUG_PrintChar DBGATA, 's'
	; we are sure there is a device, mark it (in case not already done so)
	or   byte ptr [BX+DSD_NDX_DEVCHANNEL], FLG_SLAVE

	; see if ATAPI
	call_pio_inbyte CB_CL		; al = cylinder low byte
	mov  AH, AL
	call_pio_inbyte CB_CH		; al = cylinder high byte
	; if ( (cyl low == 0x14) && (cyl high == 0xEB) ) // ATAPI device found
	DEBUG_PrintChar DBGATA, 'D'
	DEBUG_PrintChar DBGATA, '1'
	DEBUG_PrintChar DBGATA, 'C'
	DEBUG_PrintChar DBGATA, 'H'
	DEBUG_PrintNumber DBGATA, al
	DEBUG_PrintChar DBGATA, 'C'
	DEBUG_PrintChar DBGATA, 'L'
	DEBUG_PrintNumber DBGATA, ah
	cmp  AX, 14EBh
	jne  @@isatafounddev1		; ignore, either unknown or ATA device found

	; we found an ATAPI device, so mark it as such
	or   byte ptr [BX+DSD_NDX_DEVCHANNEL], FLG_SATAPI
	jmp  @@done

	@@isatafounddev1:
      ;if ( ( cl == 0x00 ) && ( ch == 0x00 ) && ( st != 0x00 ) )
	cmp  AX, 0				; compare cyl low and cyl high to 0x00
	jne  @@done				; no, then some unknown device
	call_pio_inbyte CB_STAT
	DEBUG_PrintChar DBGATA, 'S'
	DEBUG_PrintChar DBGATA, 'T'
	DEBUG_PrintNumber DBGATA, al
	cmp  AL, 0
	jz   @@done				; if not zero then we found an ATA device (e.g. hard drive)
	or   byte ptr [BX+DSD_NDX_DEVCHANNEL], FLG_SATA	
	;jmp @@done

	@@done:

	DEBUG_PrintChar DBGATA, 0Dh			; print newline to seperate debug prints from normal messages
	DEBUG_PrintChar DBGATA, 0Ah

	; select a device that exists (if possible), preferably device 0

	; 1st select slave, if exists
	mov  AL, [BX+DSD_NDX_DEVCHANNEL]		; get flag that indicates if master & slave (probably) exist
	test  AL, FLG_SLAVE
	jz   @@next
	call_pio_outbyte CB_DH, CB_DH_DEV1
	DELAY400NS;
	@@next:
	; now try selecting master, if exists
	mov  AL, [BX+DSD_NDX_DEVCHANNEL]		; get flag that indicates if master & slave (probably) exist
	test  Al, FLG_MASTER
	jz   @@next2
	call_pio_outbyte CB_DH, CB_DH_DEV0
	DELAY400NS;
	@@next2:

	pop  AX
	retn
endp  ;init_ata_search


; perform ATA software reset, this may not be used as a normal reset
; as it may reset non-ATAPI devices as well, see ATAPI software reset
; for device reset to use during normal operations
; reset done on both devices (if exists) as determined by base of [unitReq]
; ;devRtn (which device is set in CB_DH) is determined by which unitReq actually refers to
; no registers are modified, expects DS:BX set to start of DSD on entry
reg_reset proc near
	push AX
	push DX

	; ensure a small delay before accessing any ATA registers
	DELAY400NS;

	; initialize timeout value
	MAXSECS EQU 10				; max seconds, originally 20
	mov  AX, MAXSECS*18			; max seconds * 18 clock ticks / second == clock ticks to wait
	call set_BIOS_timeout
	
	; Set and then reset the soft reset bit in the Device Control
	; register.  This causes device 0 be selected.
	; callee may want this skipped
	mov  AL, devCtrl
	or   AL, CB_DC_SRST
      call_pio_outbyte CB_DC, AL		; devCtrl | CB_DC_SRST
      DELAY400NS;
      call_pio_outbyte CB_DC, devCtrl
      DELAY400NS;

	; if there is a MASTER device
	mov  AL, [BX+DSD_NDX_DEVCHANNEL]	; get flag that indicates if master & slave (probably) exist
	DEBUG_PrintChar DBGATA, 'f'
	DEBUG_PrintNumber DBGATA, al
	test AL, FLG_MASTER
	jz   @@checkslave

	;call atapi_delay

	@@loopwhilebusyMaster:			; loop while busy flag set or until timeout
	call_pio_inbyte CB_STAT
	test AL, CB_STAT_BSY
	jz   @@endbusyloopMaster
	; check for timeout
	call check_BIOS_timeout
	jc   @@timeout_1
	jmp @@loopwhilebusyMaster
	@@timeout_1:
;            reg_cmd_info.to = 1;		; indicate time-out
;            reg_cmd_info.ec = 1;		; error code = 1
	@@endbusyloopMaster:


@@checkslave:
	; if there is a SLAVE device
	mov  AL, [BX+DSD_NDX_DEVCHANNEL]	; get flag that indicates if master & slave (probably) exist
	test AL, FLG_SLAVE
	jz   @@resetdone

	;call atapi_delay

	@@loopwhilebusySlave:			; wait until allows register access
	call_pio_outbyte CB_DH, CB_DH_DEV1
	DELAY400NS;
	call_pio_inbyte CB_SC
	xchg AH, AL
	call_pio_inbyte CB_SN
	cmp  AX, 0101h
	je @@endbusyloopSlave
	; check for timeout
	call check_BIOS_timeout
	jc   @@timeout_2
	jmp @@loopwhilebusySlave
	@@timeout_2:
;            reg_cmd_info.to = 1;		; indicate time-out
;            reg_cmd_info.ec = 2;		; error code = 2
	@@endbusyloopSlave:

	; if no error or timeout (error code still 0) so far, then check if drive 1 set BSY=0
	call_pio_inbyte CB_STAT
	test AL, CB_STAT_BSY
	jz   @@resetdone
;		 // reg_cmd_info.to = 0;
;            reg_cmd_info.ec = 3;

	@@resetdone:			;RESET_DONE:


	; done, but select caller expected device
	;call_pio_outbyte CB_DH, CB_DH_DEV?
	DELAY400NS;

	; select a device that exists (if possible), preferably device 0

	; 1st select slave, if exists
	mov  AL, [BX+DSD_NDX_DEVCHANNEL]	; get flag that indicates if master & slave (probably) exist
	test  AL, FLG_SLAVE
	jz   @@next1
	call_pio_outbyte CB_DH, CB_DH_DEV1
	DELAY400NS;
	@@next1:
	; now try selecting master, if exists
	mov  AL, [BX+DSD_NDX_DEVCHANNEL]	; get flag that indicates if master & slave (probably) exist
	test  Al, FLG_MASTER
	jz   @@next2
	call_pio_outbyte CB_DH, CB_DH_DEV0
	DELAY400NS;
	@@next2:

	pop  DX
	pop  AX
	retn
endp  ;reg_reset


search_for_atapi_drives proc near
      push CX
      push BX

	DEBUG_PrintChar DBGATA, '<'

      ;	// cycle through all possible combinations supported,
      ;	// detecting master & slave at roughly the same time
      ;	for (int i=0; (i < ata_base_io_list_cnt[<= MAX_CONTROLLERS]); i++)
      xor  CX, CX					; i = 0

@@loop_finddrives:

      ; store (for use by pio io functions) which register set we are currently testing
      mov  AL, [CS:units]
      mov  byte ptr [CS:unitReq], AL
      mov  BX, CX
      shl  BX, 1					; adjust for word size entries
      add  BX, OFFSET ata_base_io_list
      mov2 AX, [CS:,BX]				; get I/O address from array

      ; load DS:BX with address of device specific data for current unit
      call ldsregDSDprep            ; precalculate seg:off into dev data for unitReq
      lds BX, [CS:unitReqDevData]

	mov word ptr [BX+DSD_NDX_FLAGS], 01h
	mov word ptr [BX+DSD_NDX_BASEIO], AX
	mov word ptr [BX+DSD_NDX_DEVCHANNEL], 0 ; init only value*** CB_DH_DEV#
	mov word ptr [BX+DSD_NDX_DMA], 0
	mov word ptr [BX+DSD_NDX_IRQ], 0

      ; check for device(s) specified by unitReq
      call init_ata_search

      mov  AL, [BX+DSD_NDX_DEVCHANNEL]	; get init device detection flags
      DEBUG_PrintChar DBGATA, 'A'
      DEBUG_PrintNumber DBGATA, AL

IFDEF OPT_DEBUG
	; tell user what we found, including non-ATAPI and no devices
      call printUnitInfo
ENDIF

      ; see if any drives detected
      and AL, FLG_MATAPI OR FLG_SATAPI
      jz   @@loop_continue			; if 0 then we didn't find a device

IFNDEF OPT_DEBUG
      ; we found 1 or more, so tell the user
      call printUnitInfo
ENDIF

      ; if master and slave then we need to duplicate entry
      cmp AL, FLG_MATAPI OR FLG_SATAPI	
      jne @@master_atapi

      ; mark this device as the master
      mov  byte ptr [BX+DSD_NDX_DEVCHANNEL], CB_DH_DEV0

      ; now duplicate and mark as the slave
      inc  byte ptr [CS:unitReq]		; point to next DSD entry
      mov  BX, CX
      shl  BX, 1
      add  BX, OFFSET ata_base_io_list
      mov  word ptr AX, [BX]			; get I/O address from array

	mov word ptr [BX+DSD_NDX_FLAGS], 01h
	mov word ptr [BX+DSD_NDX_BASEIO], AX
	mov word ptr [BX+DSD_NDX_DEVCHANNEL], CB_DH_DEV1
	mov word ptr [BX+DSD_NDX_DMA], 0
	mov word ptr [BX+DSD_NDX_IRQ], 0

      ; increment our counter by two
      add  byte ptr [CS:units], 2		; increment counter, units += 2
      jmp  @@loop_continue

      ; was a master device found?
      @@master_atapi:
      test AL, FLG_MATAPI
      jz   @@slave_atapi

      ; mark this device as the master
      mov  byte ptr [BX+DSD_NDX_DEVCHANNEL], CB_DH_DEV0
      jmp  @@update_counter

      ; then it must have been a slave device found
      @@slave_atapi:
      ; mark this device as the slave
      mov  byte ptr [BX+DSD_NDX_DEVCHANNEL], CB_DH_DEV1

      @@update_counter:
      ; increment our counter
      inc  byte ptr [CS:units]            ; increment counter, units++

@@loop_continue:
      ; check for loop termination conditions
      inc  CX                             ; i++
      cmp  CL, [ata_base_io_list_cnt]     ; i < MAX_CONTROLLERS
      jae  @@loop_end
      mov  AL, [CS:units]                 ; units < N
      cmp  AL, [CS:lookfor_N_devices]
      jae  @@loop_end
      jmp  @@loop_finddrives
@@loop_end:

	DEBUG_PrintChar DBGATA, '>'

      pop BX
      pop CX
      retn
endp ;search_for_atapi_drives


ENDIF ;OPT_ATAPI
;*******************************************************************************
