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
;  ioATA.inc
;  ATA specific commands and definitions
;
;*******************************************************************************
IFDEF OPT_ATAPI

; ATA commands see Information Technology 
; - AT Attachment with Packet Interface - 6 (ATA/ATAPI-6) [draft 1410 rev 3a]
CMD_IDENTIFY_DEVICE		EQU 0ECh
CMD_IDENTIFY_DEVICE_PACKET	EQU 0A1h
CMD_NOP				EQU 00h
CMD_PACKET				EQU 0A0h
CMD_ATAPI_SOFT_RESET          EQU 08h


; ATA register set, command block & control block regs, pio register offsets
; Note: this driver only works with ATA devices whose pio_base_addr2
; is at pio_base_addr1 + 0x200
CB_DATA	EQU 0   ; data reg         in/out pio_base_addr1+0
CB_ERR	EQU 1   ; error            in     pio_base_addr1+1
CB_FR		EQU 1   ; feature reg         out pio_base_addr1+1
CB_SC		EQU 2   ; sector count     in/out pio_base_addr1+2
CB_SN		EQU 3   ; sector number    in/out pio_base_addr1+3
CB_CL		EQU 4   ; cylinder low     in/out pio_base_addr1+4
CB_CH		EQU 5   ; cylinder high    in/out pio_base_addr1+5
CB_DH		EQU 6   ; device head      in/out pio_base_addr1+6
CB_STAT	EQU 7   ; primary status   in     pio_base_addr1+7
CB_CMD	EQU 7   ; command             out pio_base_addr1+7
CB_ASTAT	EQU 8   ; alternate status in     pio_base_addr2+6
CB_DC		EQU 8   ; device control      out pio_base_addr2+6
CB_DA		EQU 9   ; device address   in     pio_base_addr2+7

; device control reg (CB_DC) bits
CB_DC_HOB	EQU 80h	; High Order Byte (48-bit LBA)
CB_DC_HD15	EQU 00h	; bit 3 is reserved, (old definition was 08h)
CB_DC_SRST	EQU 04h	; soft reset
CB_DC_NIEN	EQU 02h	; disable interrupts

; value for device control register, presently we don't use/enable interrupts
devCtrl EQU CB_DC_HD15 OR CB_DC_NIEN

; ATAPI Interrupt Reason bits in the Sector Count reg (CB_SC)
CB_SC_P_TAG	EQU 0F8h	; ATAPI tag (mask)
CB_SC_P_REL	EQU 04h	; ATAPI release
CB_SC_P_IO	EQU 02h	; ATAPI I/O
CB_SC_P_CD	EQU 01h	; ATAPI C/D

; bits 7-4 of the device/head (CB_DH) reg
CB_DH_LBA		EQU 40h	; LBA bit
CB_DH_DEV0		EQU 00h	; select device 0 (master), formally 0A0h
CB_DH_DEV1		EQU 10h	; select device 1 (slave),  formally 0B0h

; status reg (CB_STAT and CB_ASTAT) bits
CB_STAT_BSY		EQU 80h	; busy
CB_STAT_RDY		EQU 40h	; ready
CB_STAT_DF		EQU 20h	; device fault
CB_STAT_WFT		EQU 20h	; write fault (old name)
CB_STAT_SKC		EQU 10h	; seek complete
CB_STAT_SERV	EQU 10h	; service
CB_STAT_DRQ		EQU 08h	; data request
CB_STAT_CORR	EQU 04h	; corrected
CB_STAT_IDX		EQU 02h	; index
CB_STAT_ERR		EQU 01h	; error (ATA)
CB_STAT_CHK		EQU 01h	; check (ATAPI)


;*******************************************************************************
;*******************************************************************************

; Perform programmed I/O with IDE (ATA/ATAPI) device

; given which device register to use in DX and which device to access in
; [unitReq], then sets DX to actual port # to use, no other registers modified
; assume DS:BX already points to device specific data 
; (i.e. lds BX, [CS:unitReqDevData] as previously done by PerformPktCommand)
get_pio_port proc near
	CHECKDSD
	; determine if register requested is in base (command block)
      ; or status (control block == command block+0x200) register set
	cmp  DX, 7					; register 8 & 9 map to 6 & 7 of control block set
	jbe  @@addbaseIO				; if 0-7 then skip ahead
	add  DX, 200h - 2h			; register I/O address == base + 0x200 + (DX - 2)
	@@addbaseIO:
	add  DX, [BX+DSD_NDX_BASEIO]        ; add base I/O address to get port #
	retn						; return to caller :-)
endp  ;get_pio_port

; input a byte from given register in ATA register set
; unitReq (DS:BX) is which device to access
; returns with AL=data byte inputted, all other registers unchanged
IFDEF __NASM_MAJOR__
%macro call_pio_inbyte 1
	push DX					; store it for restoration after call
	mov  DX, %1
	call get_pio_port				; convert DX to actual port #
	in  AL, DX					; get data byte (AL) from device on I/O port (DX)
	pop  DX					; restore DX to device register & not true port
%endmacro
ELSE
call_pio_inbyte MACRO reg_ndx
	; AL = pio_inbyte( reg_ndx );
	push DX					; store it for restoration after call
	mov  DX, reg_ndx
	call get_pio_port				; convert DX to actual port #
	in  AL, DX					; get data byte (AL) from device on I/O port (DX)
	pop  DX					; restore DX to device register & not true port
ENDM call_pio_inbyte
ENDIF


; output a byte to given register in ATA register set
; AL=data to output, DX=which device register to use, unitReq is which device to access
; all other registers unchanged
IFDEF __NASM_MAJOR__
%macro call_pio_outbyte 2
	push AX
	push DX					; save it
	mov  DX, %1
	mov  AL, %2
	call get_pio_port				; convert DX to actual port #
	out  DX, AL					; send data byte (AL) to device on I/O port (DX)
	pop  DX					; and restore it
	pop  AX
%endmacro
ELSE
call_pio_outbyte MACRO reg_ndx, data
	; pio_outbyte( reg_ndx, data );
	push AX
	push DX					; save it
	mov  DX, reg_ndx
	mov  AL, data
	call get_pio_port				; convert DX to actual port #
	out  DX, AL					; send data byte (AL) to device on I/O port (DX)
	pop  DX					; and restore it
	pop  AX
ENDM call_pio_outbyte
ENDIF


;// This macro provides a small delay that is used in several
;// places in the ATA command protocols:
;// 1) It is recommended that the host delay 400ns after
;//    writing the command register.
;// 2) ATA-4 has added a new requirement that the host delay
;//    400ns if the DEV bit in the Device/Head register is
;//    changed.  This was not recommended or required in ATA-1,
;//    ATA-2 or ATA-3.  This is the easy way to do that since it
;//    works in all PIO modes.
;// 3) ATA-4 has added another new requirement that the host delay
;//    after the last word of a data transfer before checking the
;//    status register.  This was not recommended or required in
;//    ATA-1, ATA-2 or ATA-3.  This is the easy to do that since it
;//    works in all PIO modes.
; assumes [unitReq] refers to a valid device, which is what delay based upon
; no registers modified
IFDEF __NASM_MAJOR__

%macro DELAY400NS 0
	push AX
		call_pio_inbyte CB_ASTAT;
		call_pio_inbyte CB_ASTAT;
		call_pio_inbyte CB_ASTAT;
		call_pio_inbyte CB_ASTAT;
	pop  AX
%endmacro

ELSE

DELAY400NS MACRO
	push AX
	REPT 4
		call_pio_inbyte CB_ASTAT;
	ENDM
	pop  AX
ENDM DELAY400NS

ENDIF


; return BIOS time in DX:AX, no other registers modified
getBIOStime proc near
	push ES				; save other registers
	push BX

	mov  BX, 40h			; BIOS timer value, a DD at 0040:006C
	mov  ES, BX
	mov  BX, 6Ch

	pushf					; disable interrupts
	cli

	mov2 AX, [ES:,BX]	            ; get low word into AX
	add  BX, 2				; point to high word
	mov2 DX, [ES:,BX]	            ; get high word into DX

	popf					; reenable interrupts if enabled before calling us

	pop  BX				; and restore them
	pop  ES

	retn
endp  ;getBIOStime


; sets a timeout value, number of timer ticks relative to current BIOS time (timer ticks)
; later a call to check_BIOS_timeout can be used to see if its expired
; only a single timeout value is maintained, so if called again then all
; calls to check_BIOS_timeout will refer to new timeout value!
; NOTE: the single timeout maintained below is meant for use within a single
;       device request call, as the next call may refer to a different device
; input AX is number of timer ticks before timeout expires
; no registers modified, updates [timeout] with new expiration count
timeout DW 0,0
set_BIOS_timeout proc near
	push DX				; save modified registers
	push BX
	push AX

	mov  BX, AX				; store timeout count
	call getBIOStime			; get BIOS timer value DD into DX:AX
	add  AX, BX				; wait at least 2 timer ticks
	adc  DX, 0				; handle carry in addition
						; now DX:AX has timeout value

	; TODO check crossing over midnight case!!!

	mov  [CS:timeout+2], DX		; store timeout count DX:AX into [timeout]
	mov  [CS:timeout], AX	

	pop  AX				; restore registers
	pop  BX
	pop  DX
	retn
endp  ;set_BIOS_timeout

; determines if timeout has expired, see set_BIOS_timeout
; no registers modified, sets carry if timeout expired, carry cleared otherwise
check_BIOS_timeout proc near
	push DX					; save modified registers
	push BX
	push AX

	call getBIOStime				; get current time

	cmp  DX, word ptr [CS:timeout+2]	; see if its passed our timeout, high word 1st
	jb   @@notyet				; high word of current time still less than timeout
	ja   @@timeoutexpired			; if high word is greater, assume timeout passed
	cmp  AX, word ptr [CS:timeout]	; now check low word (least significant portion)
	jb   @@notyet				; jmp if current time still < set timer value

	; set carry flag here, remember cmp will alter it!!!
	@@timeoutexpired:
	DEBUG_PrintChar DBGATA, 'x'
	stc						; mark timeout expired
	jmp  @@done

	@@notyet:
	;DEBUG_PrintChar DBGATA, 'o'
	clc						; mark timeout not expired yet

	@@done:
	pop  AX					; restore them
	pop  BX
	pop  DX
	retn
endp  ;check_BIOS_timeout


; delay for at least 2 timer ticks, ~110ms == ~1/9 second (2 ticks / 18.2 ticks per second)
; we dont use set_BIOS_timeout & check_BIOS_timeout so can be intermixed with atapi_delay calls
; no registers changed
atapi_delay proc near
	push DX				; save modified registers
	push CX
	push BX
	push AX

	call getBIOStime			; get BIOS timer value DD into DX:AX
	add  AX, 2				; wait at least 2 timer ticks
	adc  DX, 0				; handle carry in addition
						; now DX:AX has timeout value
	; TODO check crossing over midnight case!!!

	mov  CX, DX				; store in CX:BX
	mov  BX, AX

	@@delayloop:
	call getBIOStime			; get current time
	cmp  DX, CX				; see if its passed our timeout, high word 1st
	jb   @@delayloop
	ja   @@delaydone			; if high word is greater, assume timeout passed
	cmp  AX, BX				; now check low word (least significant portion)
	jb   @@delayloop			; keep looping until current time > set timer value
	@@delaydone:

	pop  AX				; restore them
	pop  BX
	pop  CX
	pop  DX
	retn
endp  ;atapi_delay


; wait for an interrupt/error or poll til not busy/error  (error == timeout)
; only modifies AL
; on entry DH contains error code for interrupt error and DL error code for
; poll error and time-out value should already be set via set_BIOS_timeout
; on return AL is 0 for all's ok, otherwise
; if a interrupt error/timeout occurred, AL is set to DH
; if an timeout/error occurred during polling, AL is set to DL
reg_wait_poll proc near
	; if (interruptErr && use interrupts)
	; TODO do that loop when we add interrupt support!

	@@loop_wait_not_busy:
	call_pio_inbyte CB_ASTAT			; get status, check for not busy
	test AL, CB_STAT_BSY
	jz   @@done_ok

	call check_BIOS_timeout				; if timeout, set error code
	jnc  @@loop_wait_not_busy

	mov  AL, DL						; set error
	jmp  @@done

	@@done_ok:
	mov  AL, 0						; indicate no error

	@@done:
	ret
endp  ;reg_wait_poll

IFDEF __NASM_MAJOR__

%macro call_reg_wait_poll 2
	push DX
	mov  DH, %1
	mov  DL, %2
	call reg_wait_poll
	pop  DX
%endmacro

ELSE

call_reg_wait_poll MACRO timeoutErr, pollErr
	push DX
	mov  DH, timeoutErr
	mov  DL, pollErr
	call reg_wait_poll
	pop  DX
ENDM call_reg_wait_poll

ENDIF


; selects device (0 or 1 -- master or slave)
; no registers modified
; Waits for not BUSY, selects drive, then waits for READY and SEEK
; COMPLETE status.
; This should only be called for devices that are known to exist
; otherwise it needs modified to not check the device for status
; assume DS:BX already points to device specific data 
; (i.e. lds BX, [CS:unitReqDevData] as previously done by PerformPktCommand)
SelectDevice proc near
	push AX
	push DX

      ; wait for whatever device currently selected (which we don't know)
      ; to no longer be busy (which in most cases it won't be)
	@@loop_wait_not_busy:
	call_pio_inbyte CB_STAT				; get status, check for not busy
	test AL, CB_STAT_BSY
	jz   @@doselection

	call check_BIOS_timeout				; if timeout, set error code
	jnc  @@loop_wait_not_busy

	; set error
	DEBUG_PrintChar DBGATA 't'
	DEBUG_PrintChar DBGATA '-'
	jmp  @@done

	@@doselection:
	; set AL to CB_DH_DEV# where # is 0 or 1 (master or slave)
	mov AL, [BX+DSD_NDX_DEVCHANNEL]

	; issue device device select request
	call_pio_outbyte CB_DH, AL
      DELAY400NS;


      ; wait for selected device to be ready
	@@loop_wait_not_busy2:
	call_pio_inbyte CB_STAT				; get status, check for not busy
	test AL, CB_STAT_BSY
	jz   @@done

	call check_BIOS_timeout				; if timeout, set error code
	jnc  @@loop_wait_not_busy2

	; set error
	DEBUG_PrintChar DBGATA 't'
	DEBUG_PrintChar DBGATA '+'
	;jmp  @@done


	@@done:
	pop  DX
	pop  AX
	retn
endp  ;SelectDevice


ENDIF ;OPT_ATAPI
;*******************************************************************************
