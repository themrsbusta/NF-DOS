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
;  drvstrat.inc
;  device driver interrupt and strategy routines 
;  (forward references to DATA and AUDIO specific IOCTL handlers)
;
;*******************************************************************************


; init time options, defaults to 1 device, check all controllers
; see also devName DB 8 dup (?) which corresonds with /D:<name> option
;lookfor_N_devices DB 1			; corresponds with /N:# option
lookfor_N_devices DB MAX_DEVICES
; see also ata_base_io_list & ata_base_io_list_cnt 



; processes command line as specified in DS:BX
; no registers modified
; assumes default values already set
; will update variables as switches encountered going from left to right
;USAGE:
;Load device driver from CONFIG.SYS (or dynamic device loader such as DEVLOAD)
;device=[drive][path]ATAPICDD.SYS <options>
;
;device=        standard method of loading a device in CONFIG.SYS
;[drive][path]  drive and/or path to device driver file
;ATAPICDD.SYS   the driver itself, if renamed use appropriate filename
;<options>      supported command line options
;  Options are processed left to right, with rightmost one overriding any before it
;  invalid options will be silently ignored (and may screw up rest of processing)
;  Only /D and /N are specified by CDEX document, the remaining are extensions
;  and subject to change in future revisions (to avoid conflicts with existing practice)
;  Currently supported options:
;  /D:<name>
;    - will set the device name field (DOS device) to <name>
;    - default is 'FDCD0000'
;    - if <name> is less than 8 characters then space ' ' padded on right to 8 bytes
;    - if <name> is more than 8 characters, the rest are ignored
;  /N:#
;    - specifies to look for # devices
;    - default is 1 (as per spec)
;    - maximum value is 15 (F) (up to 8==MAX_CONTROLLERS*2 ATAPI devices are supported, 
;      additional ASPI/SCSI CD-ROM are also supported if enabled via /ASPI option)
;    - a value of 0 indicates search for maximum devices
;    - # is a single HEX digit, remaining characters are ignored
;  /P:<baseIO>[,<irq>[,<dma>]]
;  /S:<baseIO>[,<irq>[,<dma>]]
;    - (/S and /P are aliases for compatibility with other drivers)
;    - speed up startup by {S}pecifying the controller ({P}ort) to check for CDROM on
;    - <baseIO> is HEX base I/O port address, e.g. 1F0
;    - <irq> determines the interrupt to use; presently ignored as we don't use one
;    - <dma> ignored, future versions it will specify dma channel or other information
;    - Defaults correspond with:
;        /S:1F0,14 /S:170,15 /S:1E8,12 /S:168,10
;  /ASPI[:<device name>]
;    - enables support of SCSI CD-ROM devices, accessed via ASPI
;    - <device name> indicates to open a device other than 'SCSIMGR$' to get ASPI
;      entry point, example '$ATAMGR$' may allow it to work with ATASPI
;    - SCSI code is largely untested or experimental, thus must be explicitly enabled.
;  /I
;    - reserved, [dis]enable interrupt mode once we support interrupt based I/O
;  /V
;    - reserved, verbose
;  /UDMA
;    - reserved, enable DMA/Ultra DMA mode
;  /L:XX
;    - reserved, XX is language code (e.g. EN, ES, ...)
;
processCmdLine proc near
	push AX
	push BX
	push DI

	@@loopstart:
	mov  AL, byte ptr [BX]
	DEBUG_PrintChar DBGINIT, al
	DEBUG_PrintChar DBGINIT, ' '
	cmp  AL, '/'			; check for switch character (TODO: can we use DOS switchar in CONFIG.SYS?)
	je   @@checkswitch
	cmp  AL, 0Dh			; \r end of line marker
	je   @@loopend
	cmp  AL, 0Ah			; \n end of line marker
	je   @@loopend
	cmp  AL, 0				; safety check for \0, not valid so exceeded end of line
	je   @@loopend

	; we silently ignore everything except end of line marker and switch character!
	@@next:
	inc  BX				; check next character
	jmp  @@loopstart

	@@checkswitch:
	inc  BX				; point to character after switch
	mov  AL, byte ptr [BX]
	cmp  AL, 'D'		; device name
	je   @@optD
	cmp  AL, 'N'		; number of devices
	je   @@optN
	cmp  AL, 'S'		; select controller base I/O port address
	je   @@optP
	cmp  AL, 'P'		; select controller base I/O port address
	je   @@optP
	cmp  AL, 'A'            ; enable ASPI support
      je   @@optASPI
	cmp  AL, 'I'		; change interrupt # to use
	je   @@optI
	jmp  @@loopstart			; not a supported option, so check it for EOL or start of new switch markers

	@@optD:
	inc  BX				; point to : after /D
mov al, [bx]
DEBUG_PrintChar DBGINIT, al
	xor  DI, DI				; counter, we copy at most 8 characters (or pad to 8 characters)
	@@Dloop:
	inc  BX				; point to next character on command line
	mov  AL, [BX]
	cmp  AL, ' '			; space marks end of device name (all spaces is ok with us, but may mess up others)
	je   @@pad
DEBUG_PrintChar DBGINIT, al
	mov  [CS:devName+DI], AL	; store portion of new name
	inc  DI				; increment our counter so we know when devName is full (8 characters max)
	cmp  DI, 8				; see if we've maxed out our array
	jb   @@Dloop			; not yet, so get next character
	@@pad:
	cmp  DI, 8				; see if we need to pad
	jae  @@next				; so we inc BX before continuing
DEBUG_PrintChar DBGINIT, '.'
	mov  byte ptr [CS:devName+DI], ' '	; pad with a space
	inc  DI
	jmp  @@pad

	@@optN:
	inc  BX				; point to : after /N
mov al, [bx]
DEBUG_PrintChar DBGINIT, al
	inc  BX				; point to # after :
mov al, [bx]
DEBUG_PrintChar DBGINIT, al
	mov  AL, [BX]			; get the number, '0'-'9' are 0-9, >= 'A'&0FDh are 10+, so G is valid sorta
	cmp  AL, 'A'			; see if letter (A-F) or number (0-9) specified
	jae  @@NcapLetter
	cmp  AL, 'a'
	jae  @@NlowLetter
	sub  AL, '0'			; hex digit is 0-9, so adjust based on letter zero (0)
	or   AL, AL				; see if 0, in which case set to maximum supported
	jnz  @@NsetN
	mov  AL, MAX_DEVICES
	jmp  @@NsetN
	@@NcapLetter:
	sub  AL, 'A'+10			; hex digit is A-F, so adjust based on A
	jmp  @@NsetN
	@@NlowLetter:
	sub  AL, 'a'+10			; hex digit is A-F, so adjust based on A
	@@NsetN:				; store the new value for max units to look for
	mov  [CS:lookfor_N_devices], AL
DEBUG_PrintChar DBGINIT, '*'
DEBUG_PrintNumber DBGINIT, AL
DEBUG_PrintChar DBGINIT, '*'
	jmp  @@next				; so we inc BX before continuing

	@@optP:
	inc  BX				; point to : after /P or /S
	inc  BX				; point to # after :
	mov  AL, [BX]			; get the number, '0'-'9' are 0-9, >= 'A'&0FDh are 10+, so G is valid sorta
	cmp  AL, 'A'			; see if letter (A-F) or number (0-9) specified
	jae  @@KcapLetter
	cmp  AL, 'a'			; see if letter (a-f)
	jae  @@KlowLetter
	sub  AL, '0'			; hex digit is 0-9, so adjust based on letter zero (0)
	jmp  @@KsetIgnore
	@@KlowLetter:
	sub  AL, 'a'+10			; hex digit is A-F, so adjust based on A
	jmp  @@KsetIgnore
	@@KcapLetter:
	sub  AL, 'A'+10			; hex digit is A-F, so adjust based on A
	@@KsetIgnore:			; mark flag to ignore controller in AL
	cmp  AL, MAX_CONTROLLERS	; ignore if invalid valid
	ja   @@next
	cbw					; sign extend AL into AX
	mov  DI, AX
	jmp  @@next				; so we inc BX before continuing

	@@optI:				; not currently used/supported, set interrupt
	inc  BX				; point to : after /I
	; TODO, implement me
	jmp  @@next				; so we inc BX before continuing

	@@optV:				; verbose mode
	inc  BX				; point to : after /V
	; TODO, implement me
	jmp  @@next				; so we inc BX before continuing

	@@optASPI:
      inc BX 				; point to S in /ASPI
      ; TODO, implement me
      jmp  @@next				; so we inc BX before continuing

	@@loopend:


	DEBUG_PrintChar DBGINIT, 0Dh
	DEBUG_PrintChar DBGINIT, 0Ah

	pop  DI
	pop  BX
	pop  AX
	retn
endp  ;processCmdLine


;*******************************************************************************
