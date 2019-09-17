%define _32BCODE   0x8
%define _32BDATA	0x10
%define _32BSTACK	0x18
%define _16BCODE	0x20
%define _16BDATA	0x28
%define _16BSTACK	0x7fff

[ORG 0x9000]
[BITS 32]

regax dw 0x0000
regbx dw 0x0000
regcx dw 0x0000
regdx dw 0x0000
regsi dw 0x0000
regdi dw 0x0000
regbp dw 0x0000
fonc dw 0x00
regflag dw 0x0

bioscall16:
	pop eax
	mov [return32],eax
	call go_real
[BITS 16]
	push ax ;Error at this point 0x9032
	push bx
	push cx
	push dx
	push si
	push di
	push bp

	mov al,[fonc]
	mov [number],al
	mov ax, [regax]
	mov bx, [regbx]
	mov cx, [regcx]
	mov dx, [regdx]
	mov si, [regsi]
	mov di, [regdi]
	mov bp, [regbp]
	sti
	opcode: db 0xcd	;0x9046
	number: db 0x0	;0x9047
;ing:
;jmp ing
	cli
	mov [regax],ax
	mov [regbx],bx
	mov [regcx],cx
	mov [regdx],dx
	mov [regsi],si
	mov [regdi],di
	mov [regbp],bp
	lahf
	mov [regflag],ah
	pop bp
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
	call go_prot
[BITS 32]
	mov eax,[return32]
	push eax
ret

go_real:
;Get return add
	pop ax
	mov [ret_add],ax
;Save SP
	mov eax,esp
	mov [Sesp],eax

;Set Cs limit for 16bits mode
	jmp dword _16BCODE: switch_to_real16
switch_to_real16:

[BITS 16]

;Load DS ES FS GS*/
	mov	ax,_16BDATA
	mov	ds,ax
	mov	es,ax
	mov	fs,ax
	mov	gs,ax
	mov	ss,ax

;Switch Real Mode
	mov	eax,cr0
	and	eax,0xFFFFFFFE
	mov	cr0,eax
	jmp	dword 0: inrmode
inrmode:

	mov	ax,cs
	mov	ds,ax
	mov	es,ax
	mov	fs,ax
	mov	gs,ax
	mov	ss,ax

	xor	esp,esp
	mov	sp,_16BSTACK

;Put the return address back onto the stack
    mov ax,[ret_add]
	push ax

;Load IDTR with real mode value
	lidt	[rmode_idtptr]

;Exit
	ret




go_prot:
[BITS 16]
    mov    ax,0x0
	mov    ds,ax
	mov    es,ax
	mov     fs,ax
	mov     gs,ax
	mov     ss,ax
;Get return add
     pop ax
     mov [ret_add],ax
;save 16bits stack
      mov [stack16],sp
;Load the GDT
	lgdt	[gdtptr]
;Load the IDT
	lidt	[idtptr]

;Enable Protected Mode
	mov	eax,cr0
	or	eax,0x1
	mov	cr0,eax

;	/* Clear prefetch queue & correct CS */
	jmp	dword _32BCODE: inprotectmode


[BITS 32]
inprotectmode:
;Setup segment selectors
	mov      ax,_32BDATA
	mov      ds,ax
	mov      es,ax
	mov      fs,ax
	mov      gs,ax

	mov      ax,_32BSTACK
	mov      ss,ax
	mov      esp, [Sesp]

;Put the return address back onto the stack
    mov ax,	[ret_add]
	push ax
	ret

Sesp: dd 0
ret_add: dd 0
stack16: dw 0
return32: dd 0

rmode_idtptr:
	dw	0x3ff		; Limit
	dd	0			; Base Address

idtptr:
	dw	0x7ff		; Limit
	dd	0x1000		; Base Address

gdtptr:
	dw	0x7ff		; Limit
	dd	0x1800		; Base Address
