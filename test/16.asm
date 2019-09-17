section bioscall

[BITS 16]

start:
    mov si,startmsg
    call print

   jmp dword 0x8:0

print:
    push ax
    push bx
    push cx
    push dx
    .debut:
    lodsb		;ds:si -> al
    cmp al,0		;end ?
    jz .fin
    mov ah,0x0E		;call BIOS service 0x0E
    mov bx,0x07		;bx=color
    int 0x10
    jmp .debut
    .fin
    pop dx
    pop cx
    pop bx
    pop ax
ret

startmsg db 13,10,"Real mode",13,10,0
