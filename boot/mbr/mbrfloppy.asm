%define BOOT 0x07C0	;original boot address
%define STACK 0x200	;stack address
%define LOADER 0x3000	;loader address

[BITS 16]
[ORG 0x0]

    jmp start
start:

    mov ax,BOOT
    mov ds,ax		;data

    mov	[bootdrv],dl	;boot device

    mov ax,STACK	;stack at 0x2000
    mov ss,ax

    mov ax,0xFFE	;init stack pointer
    mov sp,ax		;stack limit is 0xFFE

    mov si,StartMsg
    call print

;***************Load Kernel Image***************
    xor		ax,ax		;ax=0
    xor		bx,bx		;bx=0
    int		0x13		;call floppy int

      call docopy
      mov ax,LOADER
      mov es,ax
      mov eax, [es:0]
      mov ebx,0x200
      div ebx
      inc ax
      mov cx,ax
      cld

loopc:
        push cx

             mov cl,[sectorc]
             mov dh,[facec]
             mov ch,[trackc]

	    inc cl
        cmp cl,19		;comp sector
        jne dcopy

        mov cl,1
        inc dh
        cmp dh,2		;comp face
        jne dcopy

        xor dh,dh
        inc ch
        cmp ch,80		;comp track
        je errorcopy
dcopy:
           mov [sectorc],cl
           mov [facec],dh
           mov [trackc],ch
      
      call docopy

        pop cx
loop loopc
jmp donec

errorcopy:
    mov si,ErrorMsg
    call print
    jmp end

donec:

    mov si,EndMsg
    call print

    mov bl,[bootdrv]

    jmp dword LOADER:48		;jump to new start

end:
	hlt
    jmp end

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

    .fin:
    pop dx
    pop cx
    pop bx
    pop ax
ret

docopy:		;****only for 1.44 Mb 3,5"****

        mov ax,[address]	;kernel address to es
        mov es,ax		;dest address

        mov bx,0		;??
        mov ah,2		;2h command read
        mov al,1		;nb sector to read normaly 18
        mov ch,[trackc]			;track max=80
        mov cl,[sectorc]		;sector max=18
        mov dh,[facec]			;face nb max=2
        mov dl,[bootdrv]	;disk nb
        int 0x13
        mov ax,[address]	;kernel address to es
        add ax,0x20
        mov [address],ax
        mov si,msg2
        call print
ret

StartMsg db "Loading boot",13,10,0
EndMsg db 13,10,"Done",13,10,0
msg2 db ".",0

ErrorMsg db "Fatal: IO error",13,10,0

bootdrv: db 0

;kernel position
sectorc: db 2			;init comp variable
trackc: db 0
facec: db 0
address: dw LOADER


times 510-($-$$) db 144
dw 0xAA55
