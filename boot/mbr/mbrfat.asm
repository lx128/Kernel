%define BOOT 0x07C0	;original boot address
%define STACK 0x200	;stack address
%define LOADER 0x2000	;loader address
;%define LOADER 0x7E0	;debug


[BITS 16]
[ORG 0x0]

jmp start	;FAT 12

    db "Kernel  "	;8 char
    dw 0x200		;Sectors's size
    cluster: db 0x1	;Sector number per Cluster
    dw 0x1		;Reserved sector for boot
    db 0x2		;FAT Number on the disk
    rootentry: dw 0xE0	;Max Root entry
    dw 0x0B40		;Sector number on the disk
    db 0xF0		;Support type 1.44 3'1/2 18sectors per track
    comp: dw 0x9	;FAT size in sector
    dw 0x12		;Sectors number per track
    dw 0x2		;Heads number
    dw 0		;Hiden sectors
    dw 0		;Hiden sectors
    dw 0		;Sectors number if media > 32 Mo
    dw 0		;Sectors number if media > 32 Mo
    db 0		;Media type 0=floppy 0x80 or 0x81 = HDD
    db 0		;Reserved
    db 0x29		;Signature
    dw 0x0000		;Serial number
    dw 0x0000		;Serial number
    db "Kernel_DISK"	;11 char Volum name
    dw 0x4146		;FAT12 in ASCII for the type
    dw 0x3154		;FAT12 in ASCII for the type
    dw 0x2032		;FAT12 in ASCII for the type
    dw 0x2020		;FAT12 in ASCII for the type
;end fat 12 descriptor

    fat: dw 1		;address of fat table
    root: dw 19		;root table address
    dataarea: dw 31	;data area address

start:

    mov ax,BOOT
    mov ds,ax		;data

    mov	[bootdrv],dl	;boot device

    mov ax,STACK	;stack at 0x2000
    mov ss,ax

    mov ax,0xFFE	;init stack pointer
    mov sp,ax		;stack limit is 0x3000

;***************Load LOADER Image***************
    xor		ax,ax		;ax=0
    xor		bx,bx		;bx=0
    int		0x13		;call floppy int

    mov cx,0			;find XPLDR.BIN
    mov ax,0x400
    mov es,ax
    mov ax,[root]		;load root map
    jmp find2
find:
    add cx,1
    cmp cx,15
    jne run1
    pop ax
    add ax,1
    cmp ax,[dataarea]
    je err1			;ax = dataarea ?
find2:
    mov bx,0x400
    call readfd
    push ax
    xor cx,cx

run1:
    mov ax,cx
    mov bx,0x20			;entry size
    mul bx
    mov bx,ax

    mov ax,[es:bx]
    cmp al,0
    je err1
    cmp al,0xE5			;deleted file ?
    je find

    push cx			;compare string "XPLDR.BIN"
    mov si,xpldr
    mov di,bx
    mov cx,12
    repe
    cmpsb
    mov ax,cx
    pop cx			;end compare
    cmp ax,0
    jne find			;if not equal, go find

    mov al,[es:bx+11]		;first cluster in the rootentry
    cmp al,0xF
    je find

    pop ax
    mov ax,[es:bx+26]		;first cluster in the rootentry
    push ax

    mov ax,[fat]
    mov bx,0x400
    jmp loop2b
err1:				;*
    mov si,err1msg		;*Pour economiser une instruction
    call print			;*
keyb:				;*
    mov ax,0			;*
    int 0x16			;*
    cmp ax,0			;*
    je keyb			;*
    int 0x19			;*
;    jmp start
loop2:
    add ax,0x1
    add bx,0x20
loop2b:
    call readfd
    cmp ax,[comp]
    jne loop2			;jmp if not equal

    mov ax,0x400
    mov fs,ax
    mov ax,LOADER
    mov es,ax
    pop ax


docp:
;jmp docp
    cmp ax,0xFF8		;end of file ?
    jnb donec
    push ax
    add ax,[dataarea]
    mov bx,es
    call readfd
    add bx,0x20
    mov es,bx
    pop ax

    xor dx,dx
    mov bx,12
    mul bx
    mov bx,8
    div bx
    cmp dx,0
    mov bx,ax
    mov ax,[fs:bx]
    je pair
    shr ax,4
    jmp docp
pair:
    and ax,0x0FFF
    jmp docp

donec:

    mov bl,bootdrv
    jmp dword LOADER:4		;jump to new start

end:
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

    .fin
    pop dx
    pop cx
    pop bx
    pop ax
ret

readfd:		;(sector number, dest) ****only for 1.44 Mb 1,5"****

    push ax
    push bx
    push cx
    push dx
	mov es,bx		;dest address

;	mov bx,1
;	add ax,bx
	mov bx,36
	xor dx,dx
	div bx
	mov [trackc],al

	mov ax,dx
	xor dx,dx
	mov bx,18
	div bx
	mov [facec],al
	mov al,1
        add dl,al
	mov [sectorc],dl

;mov al,[trackc]
;add ax,0x30
;mov [msg2],ax
	mov si,msg2
	call print

        mov bx,0		;??
        mov ah,2		;2h command read
        mov al,1		;nb sector to read normaly 18
        mov ch,[trackc]		;track max=80
        mov cl,[sectorc]	;sector max=18
        mov dh,[facec]		;face nb max=2
        mov dl,[bootdrv]	;disk nb
	int 0x13

    pop dx
    pop cx
    pop bx
    pop ax
ret


;********************************************************************
err1msg db "Error: XPLDR.BIN not present",13,10,"Change disk & press any key",13,10,10,0
msg2 db ".",0
xpldr db "XPLDR   BIN"

bootdrv: db 0

;kernel position
sectorc: db 0			;sector to read
trackc: db 0
facec: db 0

times 510-($-$$) db 144
dw 0xAA55
