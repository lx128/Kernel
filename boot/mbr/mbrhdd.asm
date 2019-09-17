;HDD version 23/02/2013
%define BOOT 0x07C0	;original boot address
%define STACK 0x200	;stack address
%define LOADER 0x3000	;loader address

[BITS 16]
[ORG 0x0]

    jmp start		;charge cs avec le segment courrant
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
    int		0x13		;init drive au cas ou

	call docopy		;copie du premier secteur
	mov ax,LOADER	;adresse du loader

	push ax			;save
	add ax,0x20		;Loader + 1 pour ne pas copier une deuxieme fois le premier secteur
	mov [dap_dest_seg],ax	;copie dans dap_dest_seg
	pop ax			;restore

	mov es,ax		;adresse LOADER dans es
	mov eax, [es:0]	;copie de la taille du fichier dans eax
	mov ebx,0x200	;division par 512
	div ebx			;division par 512
	;pas de decrementation pour copier le dernier secteur du fichier
	mov [dap_nb_to_read],ax		;déplacement dans dap_nb_to_read

	;demarrage 1 secteur plus loin sur le disque
	mov dword[dap_src1],2		;déplacement dans dap_src1

	call docopy		;copie

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

docopy:		;hdd version
	push ax
	push dx
	push si

	mov si,dap_size	;dap structure adress
	;mov si,ax		;dap structure adress

	mov ah,0x42		;42h extended read command
	mov dl,[bootdrv]	;disk nb
	clc				;clear carryflag
	int 0x13

	jnc .suite		;jmp si carryflag=0
    mov si,ErrorMsg
    call print
	jmp end

	.suite:
	pop si
	pop dx
	pop ax
ret

StartMsg db "Loading step 1 ",0
EndMsg db "succeed!",13,10,0

ErrorMsg db "Read error",13,10,0

bootdrv: db 0

;kernel position
address: dw LOADER

dap_size: db 16
dap_unused: db 0
dap_nb_to_read: dw 1
dap_dest_offset: dw 0
dap_dest_seg: dw LOADER
dap_src1: dd 1
dap_src2: dd 0

times 510-($-$$) db 0	;144 était l'ancienne valeur
dw 0xAA55
