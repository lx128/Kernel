[BITS 16]
[ORG 0x0]

%define BOOT	0x500
%define PSTACK	0xFFE

regax dw 0x0e64
regbx dw 0x0007
regcx dw 0x0000
regdx dw 0x0000
regsi dw 0x0000
regdi dw 0x0000
fonc dw 0x10

    mov ax,BOOT
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    mov ax,PSTACK	;init stack pointer
    mov sp,ax		;stack limit is 0xFFE
    mov bp,ax		;stack limit is 0xFFE
    sti

    push ax
    push bx
    push cx
    push dx
    push si
    push di
    mov ax,[regax]
    mov bx,[regbx]
    mov cx,[regcx]
    mov dx,[regdx]
    mov si,[regsi]
    mov di,[regdi]
;    push ax
;    mov al,[fonc]
;    mov [0x50],al
;    pop ax
    int 0x10
az:
jmp az
    cli
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax


;going protected mode
    mov eax,cr0
    or ax,1
    mov cr0,eax
    jmp next

next:

;init new segment
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ax,0x18
    mov ss,ax

    jmp dword 0x8:0x6045
end:
jmp end

gdtptr:
	dw	0x1000
	dd	0xFF
