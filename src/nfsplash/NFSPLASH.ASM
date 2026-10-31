bits 16
org 100h

start:
    push cs
    pop ds

    ; BIOS returns current mode in AL, columns in AH, active page in BH.
    mov ah, 0Fh
    int 10h
    mov [cs:active_page], bh
    cmp ah, 80
    jne unsupported

    cmp al, 02h
    je color_text
    cmp al, 03h
    je color_text
    cmp al, 07h
    je mono_text
    jmp unsupported

color_text:
    mov ax, 0B800h
    jmp draw

mono_text:
    mov ax, 0B000h

draw:
    mov es, ax
    xor bx, bx
    mov bl, [cs:active_page]
    mov cl, 12
    shl bx, cl                  ; 4 KiB per 80x25 display page
    mov di, bx

    ; Hide earlier boot messages once, then leave the splash in place.
    mov ax, 0720h                ; black background, white-on-black text attr
    mov cx, 2000                 ; 80 columns x 25 rows
    cld
    rep stosw
    mov di, bx

    push cs
    pop ds
    mov si, screen_data
    mov cx, 1600                ; 80 columns x 20 rows, character+attribute
    rep movsw

    ; Four VIS.BAT description lines occupy rows 20-23; FreeCOM starts on 24.
    mov ah, 02h
    mov bh, [cs:active_page]
    mov dh, 20
    xor dl, dl
    int 10h

    mov ax, 4C00h
    int 21h

unsupported:
    push cs
    pop ds
    mov dx, unsupported_message
    mov ah, 09h
    int 21h
    mov ax, 4C01h
    int 21h

active_page db 0
unsupported_message db 'NFSPLASH needs an 80-column text display.$'
screen_data:
    incbin "generated/screen.bin"
