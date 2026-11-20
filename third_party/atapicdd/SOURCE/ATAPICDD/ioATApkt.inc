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
;  ioATApkt.inc
;  ATA implementation to perform given SCSI/ATAPI command
;
;*******************************************************************************
IFDEF OPT_ATAPI


; TODO, if there is an ATAPI error we need to automatically issue
; a request sense or at least adjust AX to indicate ATA error
; or closes ATAPI sense error
; [flag should be updated so next request sense call returns
; cached data; this is so ATA & ASPI behave similarly]


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
; when called we know the device is an ATAPI one and DS:BX points
; to device specific data for this particular unit, with DS==CS
PerformATAPacketCmd proc near
	push BX
	push CX
	push DX
	push DS
	push ES
	push SI
	push DI

	; initialize timeout counter
	mov  AX, 20*18				; default 20 seconds * 18 ticks/second
	call set_BIOS_timeout			; read BIOS 

	@@selectdev:				; indicate if command for master or slave
	call SelectDevice				; uses unitReq and issues device select call

	@@setupregs:				; send data to all registers except command regiser
	call_pio_outbyte CB_DC, CB_DC_NIEN	; change to 0 if we add support for interrupts

	; below varies depending on if communicating in LBA 28, LBA 48 mode, or ATA CHS/ATAPI LBA32 mode
	; but since we are an ATAPI device we of course only support ATA CHS/ATAPI LBA32 mode
	call_pio_outbyte CB_FR, 0				; if support DMA OR with 0x01  (feature register)
	call_pio_outbyte CB_SC, 0				; sector count, tag # for command queuing when supported, 0-31 only
	call_pio_outbyte CB_SN, 0				; sector number, not applicable
	mov  AX, 0FFFFh						; set cyl value to max transmit size per PIO DRQ transfer (==0xFFFE)
	call_pio_outbyte CB_CH, AH				; i.e. max value transfered per rep insw/outsw request
	call_pio_outbyte CB_CL, AL				; must be even if < data count, odd ok if >= data count
	mov  AL, byte ptr [BX+DSD_NDX_DEVCHANNEL]		; AL = CB_DH_DEV#
	call_pio_outbyte CB_DH, AL				; CB_DH_DEV# | ( reg_atapi_reg_dh & 0x0f /* i.e. only low bits */ )

	; if support interrupt mode, then
	; Take over INT 7x and initialize interrupt controller and reset interrupt flag.
	; call int_save_int_vect

	; Start the command by setting the Command register.  The drive
	; should immediately set BUSY status.
	call_pio_outbyte CB_CMD, CMD_PACKET

	; Waste some time by reading the alternate status a few times.
	; This gives the drive time to set BUSY in the status register on
	; really fast systems.  If we don't do this, a slow drive on a fast
	; system may not set BUSY fast enough and we would think it had
	; completed the command when it really had not even started the
	; command yet.

	DELAY400NS;

	; Command packet transfer...
	; Check for protocol failures,
	; the device should have BSY=1 or
	; if BSY=0 then either DRQ=1 or CHK=1.
	call atapi_delay
	call_pio_inbyte CB_ASTAT
	test AL, CB_STAT_BSY
	jnz  @@next1				; if busy is set (BSY==1) then all's ok
	test AL, CB_STAT_DRQ OR CB_STAT_ERR
	jnz  @@next1				; if not busy but DRQ or ERR set then all's ok

	; some sort of failure
	; reg_cmd_info.failbits |= FAILBIT0;  // not OK
	DEBUG_PrintChar DBGATA, '#'
	DEBUG_PrintChar DBGATA, '0'


	@@next1:
	; Command packet transfer...
	; Poll Alternate Status for BSY=0.
	call_pio_inbyte CB_ASTAT		; poll for not busy
	test AL, CB_STAT_BSY
	jz   @@next2
	call check_BIOS_timeout			; carry set if our timer expired
	jc   @@timeout1
	jmp  @@next1

	@@timeout1:
	; reg_cmd_info.to = 1;
	DEBUG_PrintChar DBGATA, 't'
	DEBUG_PrintChar DBGATA, '1'
	mov  AL, 51					      ; set error code
	mov  byte ptr [datadir], APC_DONE	; indicate command done
	jmp  @@done

	@@next2:  ; Command packet transfer...
	; Check for protocol failures... no interrupt here please!
	; Clear any interrupt the command packet transfer may have caused.

	; if ( int_intr_flag )
	;   reg_cmd_info.failbits |= FAILBIT1;
	; int_intr_flag = 0;


	@@next3:  ; Command packet transfer...
	; If no error, transfer the command packet.

	; Read the primary status register and the other ATAPI registers.
	call_pio_inbyte CB_STAT
	mov  DL, AL					; status
	call_pio_inbyte CB_SC
	mov  DH, AL					; reason
	call_pio_inbyte CB_CL
	mov  CL, AL					; lowCyl
	call_pio_inbyte CB_CH
	mov  CH, AL					; highCyl

	; check status: must have BSY=0, DRQ=1 now
	and  DL, CB_STAT_BSY OR CB_STAT_DRQ OR CB_STAT_ERR
	test DL, CB_STAT_DRQ
	jnz  @@next4

	DEBUG_PrintChar DBGATA, 'e'
	DEBUG_PrintNumber DBGATA, 52
	mov  AL, 52					      ; set error code
	mov  byte ptr [datadir], APC_DONE	; indicate command done
	jmp  @@done

	@@next4:  ; Command packet transfer...
	; Check for protocol failures...
	; check: C/nD=1, IO=0.
	test DH, CB_SC_P_TAG OR CB_SC_P_REL OR CB_SC_P_IO
	jnz   @@next4_fail1
	test DH, CB_SC_P_CD
	jnz  @@next4a

	@@next4_fail1:
	; reg_cmd_info.failbits |= FAILBIT2;
	DEBUG_PrintChar DBGATA, '#'
	DEBUG_PrintChar DBGATA, '2'

	@@next4a:
	cmp  CX, 0FFFFh				; are low & high cyl same as value we inputted, ~min(data buffer size, 0xffff)
	je   @@next4b

	@@next4_fail2:
	; reg_cmd_info.failbits |= FAILBIT3;
	DEBUG_PrintChar DBGATA, '#'
	DEBUG_PrintChar DBGATA, '3'
	DEBUG_PrintChar DBGATA, '-'
	DEBUG_PrintNumber DBGATA, ch
	DEBUG_PrintNumber DBGATA, cl

	@@next4b:
	; xfer the command packet (the cdb)
	; pio_drq_block_out( CB_DATA, cpseg, cpoff, cpbc >> 1 );
	mov  DX, CB_DATA				; get I/O address of data register
	call get_pio_port				; into DX
	xor  CX, CX
	mov  CL, [cdbLen]				; set CX to the buffer count, in _words_
	shr  CX, 1
DEBUG_PrintChar DBGATA, 'c'
DEBUG_PrintChar DBGATA, 'L'
DEBUG_PrintNumber DBGATA, CL

	push BX					; pad packets less than 12 bytes
	mov  BX, 0					; assume larger packets are properly
	cmp  CL, 6					; padded if so required
	jae   @@nopad
	mov  BX, 6					; 12 byte packet (6 words)
	sub  BX, CX					; how many 0 words to send
DEBUG_PrintChar DBGATA, 'p'
DEBUG_PrintChar DBGATA, 'D'
DEBUG_PrintNumber DBGATA, BL
	@@nopad:

      push DS                             ; save original DS value
	lds  SI, dword ptr [packetcdb]	; set DS:SI to the address of the buffer
	; cld						; direction flag should already be set

;----
	push bx
      shl  cx, 1  ;bytes
	mov  bx, offset tmp
DEBUG_PrintChar DBGATA, ':'
mov AX, DS
DEBUG_PrintNumber DBGATA, AH
DEBUG_PrintNumber DBGATA, AL
DEBUG_PrintChar DBGATA, ':'
mov AX, SI
DEBUG_PrintNumber DBGATA, AH
DEBUG_PrintNumber DBGATA, AL
DEBUG_PrintChar DBGATA, '<'
      l1:
	mov al, [si]
      mov CS:[bx], al
DEBUG_PrintNumber DBGATA, AL
	inc si
	inc bx
	loop l1
	jmp el
tmp db 12 dup (?)
	el:
	pop cx	; pushed bx
	cmp cx, 0
	je nx
	shl cx, 1
	mov al, 0
	l3:
	mov CS:[bx], al
DEBUG_PrintNumber DBGATA, AL
	inc bx
	loop l3
	nx:
DEBUG_PrintChar DBGATA, '>'
	mov bx, 0  ; no pad
	mov cx, 6  ; force 12 bytes
	push cs
	pop  ds
	mov  si, offset tmp
;---

IFDEF OPT_8086	; for 8086 compatibility, enable this:
	;push ax
	oldOutswLoop:
	lodsw
	out dx,ax
	loop oldOutswLoop
	;pop ax
ELSE
.186	; REQUIRED for outsw, for 8086 compatible driver replace with out logic
	rep  outsw					; actually transfer the data, a word at a time
.8086	; RETURN to 8086 compatible mode
ENDIF 

	pop  DS                             ; restore DS:BX

	cmp  BX, 0
	je   @@skipsendpad
	mov  CX, BX					; copy stored 0 word count to CX
	xor  AX, AX					; obtain the 0
	rep  out dx,ax				; send them
	@@skipsendpad:
	pop  BX					; restore DS:BX


	@@waitcmddescblksent:
	; once all the data written, give the device a chance to process it
	DELAY400NS;    				; delay so device can get the status updated


	@@next5:  ; Data transfer loop...
	; If there is no error, enter the data transfer loop.
	; First adjust the I/O buffer address so we are able to
	; transfer large amounts of data (more than 64K).

	mov  AX, [packetbufoff]			; get segment:offset of data buffer into ES:SI and normalize it
	shr  AX, 4					; drop lower 4 bits
	add  AX, [packetbufseg]			; adjust segment value
      mov  ES, AX                         ; place into ES
	mov  SI, [packetbufoff]			; load offset
	and  SI, 0Fh				; use only the lower 4 bits (upper bits part of segment)


	@@transferdata_looptop:			; loop while no errors & data to send

	; Wait for INT 7x -or- wait for not BUSY -or- wait for time out.
	call atapi_delay
	call_reg_wait_poll 53, 54

	; If there was a time out error, exit the data transfer loop.
	cmp  AL, 0					; if ( reg_cmd_info.ec )
	jz   @@tdl_1
	
	mov  byte ptr [datadir], APC_DONE	; indicate command done
	jmp  @@done

	@@tdl_1:


	; Read the primary status register and the other ATAPI registers.
	call_pio_inbyte CB_STAT
	mov  DL, AL					; status
	call_pio_inbyte CB_SC
	mov  DH, AL					; reason
	call_pio_inbyte CB_CL
	mov  CL, AL					; lowCyl
	call_pio_inbyte CB_CH
	mov  CH, AL					; highCyl


	; Exit the read data loop if the device indicates this is the end of the command.
	test DL, CB_STAT_BSY OR CB_STAT_DRQ
	jnz  @@tdl_2

	mov  byte ptr [datadir], APC_DONE	; indicate command done
	jmp  @@transferdata_loopend

	@@tdl_2:


	; The device must want to transfer data...
	; check status: must have BSY=0, DRQ=1 now.
	and  DL, CB_STAT_BSY OR CB_STAT_DRQ
	cmp  DL, CB_STAT_DRQ
	je  @@tdl_3

	DEBUG_PrintChar DBGATA, 'e'
	DEBUG_PrintNumber DBGATA, 55
	mov  AL, 55					      ; set error code
	mov  byte ptr [datadir], APC_DONE		; indicate command done
	jmp  @@done

	@@tdl_3:
	; Check for protocol failures...
	; check: C/nD=0, IO=1 (read) or IO=0 (write).
;*** TODO ***
;         if ( ( reason &  ( CB_SC_P_TAG | CB_SC_P_REL ) )
;              || ( reason &  CB_SC_P_CD )
;            )
;            reg_cmd_info.failbits |= FAILBIT4;
	@@tdl_4:
;         if ( ( reason & CB_SC_P_IO ) && dir )
;            reg_cmd_info.failbits |= FAILBIT5;
	@@tdl_5:


	; do the slow data transfer thing
;      if ( reg_slow_xfer_flag )
;      {
;         slowXferCntr ++ ;
;         if ( slowXferCntr <= reg_slow_xfer_flag )
;         {
;            sub_xfer_delay();
;            reg_slow_xfer_flag = 0;
;         }
;      }
	@@tdl_6:


	; get the byte count, check for zero...
	cmp  CX, 1						; if ( ((highCyl << 8)|lowCyl) < 1 )
	jae   @@tdl_7

	DEBUG_PrintChar DBGATA, 'e'
	DEBUG_PrintNumber DBGATA, 59
	mov  AL, 59					      ; set error code
	mov  byte ptr [datadir], APC_DONE		; indicate command done
	jmp  @@done

	@@tdl_7:

	; and check protocol failures...
;      if ( byteCnt > dpbc )
;         reg_cmd_info.failbits |= FAILBIT6;
	@@tdl_8:
;      reg_cmd_info.failbits |= prevFailBit7;
;      prevFailBit7 = 0;
;      if ( byteCnt & 0x0001 )
;         prevFailBit7 = FAILBIT7;
	@@tdl_9:


	; quit if buffer overrun.
;*** TODO ***
;      if ( ( reg_cmd_info.totalBytesXfer + byteCnt ) > reg_buffer_size )
;      {
	jmp  @@tdl_10
	DEBUG_PrintChar DBGATA, 'e'
	DEBUG_PrintNumber DBGATA, 61
	mov  AL, 61					      ; set error code
	mov  byte ptr [datadir], APC_DONE		; indicate command done
	jmp  @@done
;      }
	@@tdl_10:


	; increment number of DRQ packets
;      reg_cmd_info.drqPackets ++ ;


	; transfer the data and update the i/o buffer address
	; and the number of bytes transfered.

	DEBUG_PrintChar DBGATA, 'c'
	DEBUG_PrintNumber DBGATA, ch
	DEBUG_PrintNumber DBGATA, cl
	DEBUG_PrintChar DBGATA, ' '
	push CX					; save byte count
	mov  DX, CX					; wordCnt = ( byteCnt >> 1 ) + ( byteCnt & 0x0001 );
	and  DX, 01h
	shr  CX, 1
	add  CX, DX
      
;      reg_cmd_info.totalBytesXfer += ( wordCnt << 1 );

	; do actual transfer
	; CX is the word count and ES:SI is the address of buffer with data to transfer
	; cld						; direction flag should already be set
	mov  DX, CB_DATA				; get I/O address of data register
	call get_pio_port

	mov  AL, [datadir]			; do input or output based on direction indicated
	cmp  AL, APC_INPUT			; or do nothing on other values (e.g. APC_DONE)
	je   @@do_pio_drq_block_in
	cmp  AL, APC_OUTPUT
	je   @@do_pio_drq_block_out
	jmp  @@updatecount			; important, make sure we pop cx

	; The maximum ATA DRQ block is 65536 bytes or 32768 words.
	; The maximun ATAPI DRQ block is 131072 bytes or 65536 words.
	; REP INSW/REP OUTSW will fail if wordCnt > 65535,
	; so the transfer should be split in such cases
	; however, since our byte count is limited to CX, our word count is always <= 65535

	@@do_pio_drq_block_out:
	; pio_drq_block_out( CB_DATA, dpseg, dpoff, wordCnt );
	push SI					; don't loose original starting pointer
	push DS					; save DS (for DS:BX)
	push ES					; mov DS, ES, outsw uses DS:SI
	pop  DS
IFDEF OPT_8086	; for 8086 compatibility, enable this:
	push ax
	oldOutsw2Loop:
	lodsw
	out dx,ax
	loop oldOutsw2Loop
	pop ax
ELSE
.186	; REQUIRED for outsw, for 8086 compatible driver replace with outsb logic
	rep  outsw					; actually transfer the data, a word at a time
.8086	; RETURN to 8086 compatible mode
ENDIF
	pop  DS					; restore DS (for DS:BX)
	pop  SI					; restore original start offset
	jmp @@updatecount

	@@do_pio_drq_block_in:
	; pio_drq_block_in( CB_DATA, dpseg, dpoff, wordCnt );
	mov  DI, SI					; insw places data into ES:DI
IFDEF OPT_8086	; for 8086 compatibility, enable this:
	push ax
	oldInswloop:
	in ax,dx
	stosw
	loop oldInswloop
	pop ax
ELSE
.186	; REQUIRED for outsw, for 8086 compatible driver replace with outsb logic
	rep  insw					; actually transfer the data, a word at a time
.8086	; RETURN to 8086 compatible mode
ENDIF
	;jmp @@updatecount


	; dpaddr = dpaddr + byteCnt;
	@@updatecount:
	pop  CX					; restore byte count
	add  SI, CX					; add byte count to offset
	mov  AX, SI					; normalize segment:offset
	shr  AX, 4					; adjust segment
	mov  DX, ES
	add  AX, DX
	mov  ES, AX
	and  SI, 0Fh				; adjust offset

      DELAY400NS;    // delay so device can get the status updated

	jmp  @@transferdata_looptop
	@@transferdata_loopend:


	; End of command...
	; Wait for interrupt or poll for BSY=0,
	; but don't do this if there was any error or if this
	; was a commmand that did not transfer data.
IF 0	; comment out as test should always be true
	cmp  [datadir], APC_DONE
	je  @@finalchk

	call atapi_delay
	call_reg_wait_poll 56, 57

	cmp  AL, 0
	jnz  @@done
ENDIF

	; Final status check, only if no previous error.
	@@finalchk:

	; Read the primary status register and the other ATAPI registers.
	call_pio_inbyte CB_STAT
	mov  BL, AL					; status
	call_pio_inbyte CB_SC
	mov  BH, AL					; reason
	call_pio_inbyte CB_CL
	mov  CL, AL					; lowCyl
	call_pio_inbyte CB_CH
	mov  CH, AL					; highCyl

	; check for any error.
	test BL, CB_STAT_BSY OR CB_STAT_DRQ OR CB_STAT_ERR
	jz   @@finalchk_protocol

IFDEF OPT_DEBUG
	DEBUG_PrintChar DBGATA, 'e'
	DEBUG_PrintNumber DBGATA, 58
	DEBUG_PrintChar DBGATA, ':'
	DEBUG_PrintNumber DBGATA, bl
	test BL, CB_STAT_BSY
	jz @@1
	DEBUG_PrintChar DBGATA, 'B'
	@@1:
	test BL, CB_STAT_DRQ
	jz @@2
	DEBUG_PrintChar DBGATA, 'D'
	@@2:
	test BL, CB_STAT_ERR
	jz @@3
	DEBUG_PrintChar DBGATA, 'E'
	@@3:
	call_pio_inbyte CB_ERR	; see what error is
	DEBUG_PrintChar DBGATA, 'v'
	DEBUG_PrintNumber DBGATA, AL
ENDIF
	mov  AL, 58			; most often means seems to indicate device not ready yet
	;mov AL, 0			; force success
	jmp  @@done

	@@finalchk_protocol:
	; Check for protocol failures...
	; check: C/nD=1, IO=1.
	test BH, CB_SC_P_TAG OR CB_SC_P_REL
	jnz  @@fail_finalchk_protocol
	test BH, CB_SC_P_IO
	jz   @@fail_finalchk_protocol
	test BH, CB_SC_P_CD
	jnz  @@end_finalchk_protocol
	
	@@fail_finalchk_protocol:
	; reg_cmd_info.failbits |= FAILBIT8;
	DEBUG_PrintChar DBGATA, '#'
	DEBUG_PrintChar DBGATA, '8'

	@@end_finalchk_protocol:
	; indicate no errors (though may have failed some checks)
	mov  AL, 0

	; at this point, AL == error code, if AL is NOT 0 then set carry and return
	@@done:

	; For interrupt mode, restore the INT 7x vector.
	; int_restore_int_vect();

	DEBUG_PrintChar DBGATA, '{'
	DEBUG_PrintNumber DBGATA, al
	DEBUG_PrintChar DBGATA, '}'

	pop  DI
	pop  SI
	pop  ES
	pop  DS
	pop  DX
	pop  CX
	pop  BX

	; set return status and error (carry) flag
	cmp  AL, 0
	jnz   @@failure
	clc						; carry clear unless error occurred
	retn
	@@failure:
	stc						; error occurred
	retn
endp  ;PerformATAPacketCmd


ENDIF ;OPT_ATAPI
;*******************************************************************************
