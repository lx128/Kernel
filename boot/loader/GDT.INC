;--------------------------------------------------------------------
; CalcAdr
; Entree:
;	ax -> segment
;	bx -> offset
; Sortie:
;	ecx -> adresse lineaire sur 32 bits
; Modifie: partie haute de ebx
;--------------------------------------------------------------------
calcadr:
	xor ecx,ecx
	mov cx,ax
	shl ecx,4
	and ebx,0x0000FFFF
	add ecx,ebx
	ret
;--------------------------------------------------------------------

;--------------------------------------------------------------------
; descInit
;--------------------------------------------------------------------
%macro descInit	5	; base(32),limite(20/32),acces(8),flags(4/8),adresse(32)
	push eax
; base :
	mov eax,%1
	mov word [%5+2],ax
	shr eax,16
	mov byte [%5+4],al
	shr eax,8
	mov byte [%5+7],al
; limite :
	mov eax,%2
	and eax,0x000FFFFF
	mov word [%5],ax	; ecrit (0..15)
	shr eax,16			; place (16..19) sur le nibble inferieur
	mov byte [%5+6],0	; initialise flags+lim(16..19) a 0
	or [%5+6],al		; ecrit (16..19)
; flags :
	mov al,%4
	and al,0x0F
	shl al,4
	or [%5+6],al
; acces :
	mov byte [%5+5],%3
	pop eax
%endmacro
;--------------------------------------------------------------------


%define LDT_FLAG	0x2		; ldt
%define TASK_FLAG	0x5		; task gate
%define TSS_FLAG	0x9		; TSS
%define CALL_FLAG	0xC		; call gate
%define INT_FLAG	0xE		; interrupt gate
%define TRAP_FLAG	0xF		; trap gate

%define PRESENT_F	0x80	; present flag
%define	DPL0	0x00
%define	DPL1	0x20
%define	DPL2	0x40
%define	DPL3	0x60

;--------------------------------------------------------------------
; intdesc
;--------------------------------------------------------------------
%macro gateInit	5	; selecteur(16),offset(32),type (8),wc(5/8),adresse(32)
	push eax
; selecteur :
	mov word [%5+2],%1
; offset :
	mov eax,%2
	mov word [%5],ax
	shr eax,16
	mov word [%5+6],ax
; type :
	mov byte [%5+5],%3
; wc :
	mov byte [%5+4],%4
	and byte [%5+4],00011111b
	pop eax
%endmacro
;--------------------------------------------------------------------
