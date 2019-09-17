%define CS_ACCES	10011011b
%define DS_ACCES	10010011b
%define CONFSEG 0x800

%define BOOT 0x3003
%define LOADER 0x30230
%define NEWSTACK 0x2BFFF


[BITS 16]
[ORG 0x0]

;dw 0
;dw 0

jmp start
    %include "GDT.INC"
start:

    mov ax,BOOT
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov [BootDrv], bl

    mov si,startmsg
    call print

;open gate A20

    call .testa20
    mov al,0xD1
    out 0x64,al
    call .testa20
    mov al,0xDF
    out 0x60,al
    call .testa20
    jmp .endA20
.testa20:           ;8042 Ready ?
    in al,0x64
    test al,2
    jnz .testa20
    ret
.endA20:
;end gate A20

;Init config segment
    mov ax,CONFSEG
    mov es,ax
    mov si,sign
    mov di,0
    mov cx,8
    rep
    movsb		;copy signature

    mov ah,0x3
    mov bh,0
    int 0x10		;get cursor position
    mov [es:8],dh	;ligne
    mov [es:9],dl	;colonne
    mov al,[BootDrv]
    mov [es:10],al	;Boot Disk

;Init simple gdt
    descInit 0,0xFFFFF,CS_ACCES,1101b,gdt_cs
    descInit 0,0xFFFFF,DS_ACCES,1101b,gdt_ds

;Init gdt ptr
    mov ax,gdtend
    mov	bx,gdt
    sub ax,bx
    mov word [gdtptr],ax
    xor eax,eax
    mov ax,ds
    mov bx,gdt
    call calcadr
    mov dword [gdtptr+2],ecx

;going protected mode
    cli
    lgdt [gdtptr]
    mov eax,cr0
    or ax,1
    mov cr0,eax
    jmp next

next:

;init new segment
    mov ax,0x10
    mov ds,ax
    mov fs,ax
    mov gs,ax
    mov es,ax
    mov ss,ax
    mov esp,NEWSTACK	;Stack start

    jmp dword 0x8:LOADER
end:
	hlt
	jmp end


print:
    push ax
    push bx
    .debut:
    lodsb		;ds:si -> al
    cmp al,0		;end ?
    jz .fin
    mov ah,0x0E		;call BIOS service 0x0E
    mov bx,0x07		;bx=color
    int 0x10
    jmp .debut
    .fin:
    pop bx
    pop ax
ret

startmsg db "Loading step 2 succeed!",13,10,0
sign db "CLR00001"

BootDrv db 0

gdt:
gdt_null:
        dw 0,0,0,0
gdt_cs:
	dw 0,0,0,0
gdt_ds:
	dw 0,0,0,0
gdtend:

gdtptr:
	dw	0x0000
	dd	0
times 512-($-$$) db 0 ;144 ancienne valeur
