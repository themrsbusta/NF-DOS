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
;  initmsg.inc
;  print routines and messages displayed during init phase (nonresident)
;
;*******************************************************************************

; Strings displayed at startup
AtapiCDDMsg		DB "Public Domain ATAPI CD-ROM Driver ", ReleaseStr, ", KJD 2001-2003"
newlineMsg		DB 0Dh, 0Ah, "$"

DrivesFoundMsg	DB " CD-ROM Drive(s) found.", 0Dh, 0Ah, "$"
NoDrivesFoundMsg	DB "Failure!  No CD-ROM Drives found!", 0Dh, 0Ah, "$"

Dev0Msg		DB "Master: $"
Dev1Msg		DB "Slave:  $"
NoDevMsg		DB "No device$"
ATAPIFoundMsg	DB "ATAPI device$"
ATAFoundMsg		DB "ATA, not ATAPI device$"
UnknownMsg		DB "Phantom device$"
FoundMsg		DB " found", 0Dh, 0Ah, "$"
BaseIOMsg		DB "  base I/O: 0x", "$"
StatusIOMsg		DB 0Dh, 0Ah, "  status I/O: 0x", "$"
DMAMsg            DB 0Dh, 0Ah, "  dma: N/A", "$"
IRQMsg            DB 0Dh, 0Ah, "  irq: N/A", "$"


; Prints a character using BIOS int 10h video teletype function
; All registers are preserved except AX and flags
; Expects character to print in AL
PrintChar proc near
      push BP           ; for BIOSes that screw it up if text scrolls window
      push BX
      push AX

	; get current active page
      push AX
	mov  AH, 0Fh	; get current video mode
	int  10h		; perform Video - BIOS request
				; on return AH=screen width (# of columns),
				; AL=display mode, and *** BH=active page # ***
	mov  BL, 07h	; ensure sane color in case in graphics mode
	pop  AX
	mov  AH, 0Eh	; Video - Teletype output
	int  10h		; invoke request

      pop AX
	pop BX
	pop BP
	retn
endp  ;PrintChar


; Uses PrintChar to display the number in AL
; Expects hex number (00-FF) to print in AL
PrintHexValue proc near
	push AX		; store value so we can process a nibble at a time

	; print upper nibble
	shr  AL, 4		; move upper nibble into lower nibble
	cmp  AL, 09h	; if greater than 9, then don't base on '0', base on 'A'
	jbe @@printme
	add  AL, 7		; convert to character A-F
	@@printme:
	add  AL, '0'	; convert to character 0-9
	call PrintChar

	pop  AX		; restore for other nibble
      push AX           ; so we can restore its original value

	; print lower nibble
	and  AL, 0Fh	; ignore upper nibble
	cmp  AL, 09h	; if greater than 9, then don't base on '0', base on 'A'
	jbe @@printme2
	add  AL, 7		; convert to character A-F
	@@printme2:
	add  AL, '0'	; convert to character 0-9
	call PrintChar

      pop  AX           ; restore called in value
	retn
endp  ;PrintHexValue


; similar to print HexValue, except prints as decimal without leading 0s
; TODO change to print leading tens/hundreds digit(s) instead of hex digit(s)
PrintDecNumber proc near
	push AX		; store value so we can process a nibble at a time

	; print upper nibble
	shr  AL, 4		; move upper nibble into lower nibble
	cmp  AL, 0
	je   @@skip1      ; don't print leading 0
	cmp  AL, 09h	; if greater than 9, then print value mod 10
	jbe @@printme
	add  AL, 7		; convert to character A-F
	@@printme:
	add  AL, '0'	; convert to character 0-9
	call PrintChar

	@@skip1:
	pop  AX		; restore for other nibble

	; print lower nibble
	and  AL, 0Fh	; ignore upper nibble
	cmp  AL, 09h	; if greater than 9, then don't base on '0', base on 'A'
	jbe @@printme2
	add  AL, 7		; convert to character A-F
	@@printme2:
	add  AL, '0'	; convert to character 0-9
	call PrintChar

	retn
endp  ;PrintDecNumber


; display string using PrintChar
; we don't use DOS print string as it trashes our request header
; expects CS:DX to point to $ terminated string and AH to channel
; all registers preserved except flags
PrintMsg proc near
      push BX		; save registers modified
      push AX
      mov  BX, DX       ; copy offset into bx register for use as index value

      @@next:
      mov2 AL, [CS:,BX]  ; copy current character into AL, AH already has channel value
      inc  BX
      cmp  AL, '$'      ; if reached end of string marker then end loop before printing it
      je   @@done
      call PrintChar
      jmp  @@next

      @@done:
      pop  AX
      pop  BX           ; restore registers we changed
      retn
endp  ;PrintMsg


; prints the info stored about the unit
; [unitReq] specifies device to print info about
; no registers changed, on entry DS:BX points to DSD
; on ATAPI devices includes both MASTER & SLAVE
printUnitInfo proc near
	push DX
	push BX
	push AX

	; print master or slave & ATAPI or ATA
	mov  AL, [BX+DSD_NDX_DEVCHANNEL]	; device (master, slave, or none)

	; 1st print master information
	mov  DX, OFFSET Dev0Msg			; channel 0, master
	call PrintMsg
	test AL, FLG_MASTER			; was a master found?
	jnz  @@m1
	mov  DX, OFFSET NoDevMsg		; no device was found
	call PrintMsg
	jmp  @@printSlave

	@@m1:
	test AL, FLG_MATAPI			; is master ATAPI
	jnz  @@mAtapi
	test AL, FLG_MATA				; is master general ATA
	jnz  @@mAta
	mov  DX, OFFSET UnknownMsg		; neither ATAPI nor ATA, but still found
	call PrintMsg
	jmp  @@printSlave

	@@mAtapi:
	mov  DX, OFFSET ATAPIFoundMsg		; ATAPI device found
	call PrintMsg
	jmp  @@printSlave

	@@mAta:
	mov  DX, OFFSET ATAFoundMsg		; ATA device (such as hard drive) found
	call PrintMsg
	;jmp @@printSlave

	; now print slave information
	@@printSlave:
	mov  DX, OFFSET FoundMsg		; (end master portion)
	call PrintMsg
	mov  DX, OFFSET Dev1Msg			; channel 1, slave
	call PrintMsg				; and print it
	test AL, FLG_SLAVE			; was a slave found?
	jnz  @@s1
	mov  DX, OFFSET NoDevMsg		; no device was found
	call PrintMsg
	jmp  @@printBaseIO

	@@s1:
	test AL, FLG_SATAPI			; is slave ATAPI
	jnz  @@sAtapi
	test AL, FLG_SATA				; is slave general ATA
	jnz  @@sAta
	mov  DX, OFFSET UnknownMsg		; neither ATAPI nor ATA, but still found
	call PrintMsg
	jmp  @@printBaseIO

	@@sAtapi:
	mov  DX, OFFSET ATAPIFoundMsg		; ATAPI device found
	call PrintMsg
	jmp  @@printBaseIO

	@@sAta:
	mov  DX, OFFSET ATAFoundMsg		; ATA device (such as hard drive) found
	call PrintMsg
	;jmp @@printBaseIO


	@@printBaseIO:
	mov  DX, OFFSET FoundMsg		; (end slave portion)
	call PrintMsg
	; print the base I/O address
	mov  DX, OFFSET BaseIOMsg		; load base register I/O address message
	call PrintMsg

	; use same method other code uses, base I/O address same as register 0
	mov  DX, 0
	call get_pio_port
	mov  AX, DX
	xchg AH, AL				; print the high byte 1st
	call PrintHexValue
	xchg AL, AH				; now the low byte
	call PrintHexValue

IF 0
	mov  DX, OFFSET StatusIOMsg	; print the status register I/O address (base2)
	call PrintMsg			; note this is status register base I/O address
						; NOT the more common status register itself I/O address (differ by 6)
	add  AX, 200h			; presently we assume 0x200 from base I/O address
	xchg AL, AH				; print the high byte 1st
	call PrintHexValue
	xchg AL, AH				; now the low byte
	call PrintHexValue
ENDIF

IF 0  ; TODO reneable once we support DMA and/or interrupts
	mov  DX, OFFSET DMAMsg		; print DMA channel used
	call PrintMsg
	mov  DX, OFFSET IRQMsg		; print interrupt used
	call PrintMsg
ENDIF

	mov  DX, OFFSET newlineMsg	; and of course move the cursor to the next line for pretty output :-)
	call PrintMsg

	pop  AX
	pop  BX
	pop  DX
	retn
endp  ;printUnitInfo


;*******************************************************************************
