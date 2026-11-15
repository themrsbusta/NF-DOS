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
;  audio.inc
;  our local stack, marks end of resident section == end of
;  where drive specific data is keep
;
;*******************************************************************************

;*******************************************************************************
; IOCTL input subcommands relating to Audio
;*******************************************************************************

; 4  = audio channel information
; TODO: implement me
CmdI_AudioChannel proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
	DEBUG_PrintMsg DBGAUDIO, DbgMsg_CmdI_AudioChannel
      call Cmd_NA
	retn
ENDIF
endp  ;CmdI_AudioChannel


;***************************************

; 8  = return size of volume
; TODO: implement me
CmdI_VolumeSize proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
	DEBUG_PrintMsg DBGAUDIO, DbgMsg_CmdI_VolumeSize
      call Cmd_NA
	retn
ENDIF
endp  ;CmdI_VolumeSize


;***************************************

IFDEF OPT_AUDIO
; returns in AX the offset of current lun's Table Of Contents buffer
getTOC proc near
	push CX
	push DX
	xor  CH, CH							; get lun into CX
	mov  CL, [CS:unitReq]
	mov  AX, 816						; size of TOC buffer itself
	mul  CX							; get appropriate one (816 * unitReq)
	add  AX, OFFSET TOCbuffer				; AX = OFFSET TOCbuffer + (unitReq * 816)
	pop  DX							; ignore DX, 816*MAX_DEVICES should never > 65535
	pop  CX
	retn
endp  ;getTOC

; no registers modified, clear TOC of current requested unit (lun)
clearTOC proc near
	push AX
	push CX

	call getTOC							; set AX to start (offset) of TOC for unitReq
	xchg AX, BX							; store BX in AX, and set BX to AX
	mov  CX, 408						; size of TOC buffer in words (816/2)

	@@looptop:							; loop CX times setting each word to zero (0)
	movw ptr [CS:,BX], 0
	inc  BX
	inc  BX
	loop @@looptop

	xchg AX, BX							; restore BX and other registers
	pop  CX
	pop  AX
	retn
endp  ;clearTOC
ENDIF

; 10 = audio disk information
CmdI_AudioDisk proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
	push BX

	DEBUG_PrintMsg DBGAUDIO, DbgMsg_CmdI_AudioDisk

	call_clearPacket 12					; initialize ATAPI command packet
	call clearTOC						; clear the stored TOC for [unitReq]

	mov  byte ptr [CS:packet], AC_READTOC		; read TOC, may fail during play if drive doesn't cache TOC
	mov  byte ptr [CS:packet+1], 02h			; indicate MSF format (bit 1 set)
	mov  byte ptr [CS:packet+2], 00h			; format 00b
	mov  byte ptr [CS:packet+6], 0AAh			; starting track/session #, track must be 0-99 track or AAh leadout
	mov  byte ptr [CS:packet+7], 03h			; allocation length MSB (support up to 816(0x330) bytes data)
	mov  byte ptr [CS:packet+8], 30h			; allocation length LSB
	;mov  byte ptr [CS:packet+9], 00h			; old format field, high two bits, now part of vendor reserved

	mov  byte ptr [CS:datadir], APC_INPUT		; indicate pkt cmd inputs data
	mov  AX, CS
	mov  [CS:packetbufseg], AX				; set buffer to our working buffer
	call getTOC							; returns offset in AX of [unitReq]'s Table of Contents buffer
	mov  [CS:packetbufoff], AX


	; to simplify logic here a little, we issue the request twice, 1st we
	; get the TOC with only the lead out track (which is all the information we need for this command)
	; then we issue it again with starting track # (we still use 0 so we get all of them)
	; so our buffer contains full TOC (for use by CmdI_AudioTrack and perhaps others)

	call PerformATAPIPacketCmd	; handles sending packet to device and reading returned data into buffer
	jc   @@assumeNotSupported

	call getTOC							; BX should be TOC offset since we pushed AX/popped BX it
	mov  BX, AX
	cmpw ptr [CS:,BX], 10				      ; ensure we actually got data (size of data returned - 2 for cnt)
	jl   @@assumeNotSupported				; didn't get remaining 2 bytes for header + 8 bytes for lead out data
	mov  AX, word ptr [CS:2+BX]				; mov start into AL and end into AH (low addr=start, high addr=end)
	movb ptr [ES:,DI+CBLK_LOWTRACKNUM], AL	      ; set starting track #
	DEBUG_PrintNumber DBGAUDIO, AL
	movb ptr [ES:,DI+CBLK_HIGHTRACKNUM], AH	      ; set last track #
	DEBUG_PrintNumber DBGAUDIO, AH
	mov  AX, word ptr [CS:4+BX]				; MSB bytes
	xchg AH, AL							; swap for little endian order
	movw ptr [ES:,DI+CBLK_STARTLOTRACK+2], AX	      ; set MSF of lead out track (MSB)
	mov  AX, word ptr [CS:6+BX]				; MSB bytes
	xchg AH, AL							; swap for little endian order
	movw ptr [ES:,DI+CBLK_STARTLOTRACK], AX	      ; set MSF of lead out track (LSB)

	; load full TOC
	DEBUG_PrintChar DBGAUDIO, 'T'
	DEBUG_PrintChar DBGAUDIO, 'O'
	DEBUG_PrintChar DBGAUDIO, 'C'
	mov  byte ptr [CS:packet+6], 00h			; starting track/session #, track must be 0-99 track or AAh leadout
	mov  byte ptr [CS:datadir], APC_INPUT		; indicate pkt cmd inputs data
	call PerformATAPIPacketCmd
	jnc  @@done
	
	@@assumeNotSupported:
	DEBUG_PrintChar DBGAUDIO, 'a'
	movb ptr [ES:,DI+CBLK_LOWTRACKNUM], 0		; clear data and return error
	movb ptr [ES:,DI+CBLK_HIGHTRACKNUM], 0
	movw ptr [ES:,DI+CBLK_STARTLOTRACK], 0
	movw ptr [ES:,DI+CBLK_STARTLOTRACK+2], 0
	call Cmd_NA

	@@done:
	pop  BX
	retn
ENDIF
endp  ;CmdI_AudioDisk


;***************************************

; 11 = audio track information
;CBLK_TRACKNUM	EQU 01h	; byte, track number
;CBLK_STARTTRACK	EQU 02h	; DD (MSF) of starting point of the track
;CBLK_TRACKCTRL	EQU 06h	; byte, track control information
CmdI_AudioTrack proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
      ; get audio track information here
	DEBUG_PrintMsg DBGAUDIO, DbgMsg_CmdI_AudioTrack
      call Cmd_NA
	retn
ENDIF
endp  ;CmdI_AudioTrack


;***************************************

; 12 = audio Q-Channel information
; TODO: implement me
CmdI_AudioQCh proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
      ; get audio Q-Channel stuff here
      call Cmd_NA
	retn
ENDIF
endp  ;CmdI_AudioQCh


;***************************************

; 13 = audio Sub-Channel information
; TODO: implement me
CmdI_AudioSubCh proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
      ; get audio Sub-Channel stuff here
      call Cmd_NA
	retn
ENDIF
endp  ;CmdI_AudioSubCh


;***************************************

; 14 = UPC code
; TODO: implement me, possibly initially returning 0
CmdI_UPC proc near
IFNDEF OPT_UPC
	jmp Cmd_NA
ELSE
      ; do UPC stuff here
      call Cmd_NA
	retn
ENDIF
endp  ;CmdI_UPC


;***************************************

; 15 = audio status information
; TODO: implement me
CmdI_AudioStatus proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
      ; do Audio Status stuff here
      call Cmd_NA
	retn
ENDIF
endp  ;CmdI_AudioStatus


;*******************************************************************************
;*******************************************************************************

;***************************************

; 3  = audio channel control
; TODO: implement me
CmdO_AudioChCtrl proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
      call Cmd_NA
	retn
ENDIF
endp  ;CmdO_AudioChCtrl


;*******************************************************************************
;*******************************************************************************


; plays audio
Cmd_PlayAudio proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
      ; add audio play stuff here
	call Cmd_NA
	retn
ENDIF
endp  ;Cmd_PlayAudio
 ; pauses if currently playing, else stops
Cmd_StopAudio proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
      ; add audio stop stuff here
	call Cmd_NA
	retn
ENDIF
endp  ;Cmd_StopAudio

; resumes if paused
Cmd_ResumeAudio proc near
IFNDEF OPT_AUDIO
	jmp Cmd_NA
ELSE
	push CX
	push BX

	; device call to pause/resume, however CDEX docs indicate pause/resume should
	; work slightly differently, stop when playing is pause with saved location,
	; then resume issues play with saved value
	call_clearPacket 12
	mov  byte ptr [CS:packet], AC_PAUSERESUME		; indicate we want audio playing to pause or resume
	mov  byte ptr [CS:packet+8], 1			; bit 0, if set then resume, if clear then pause
	call PerformATAPIPacketCmd	; handles sending packet to device and reading returned data into buffer

	pop  BX
	pop  CX

	retn
ENDIF
endp  ;Cmd_ResumeAudio


;*******************************************************************************
