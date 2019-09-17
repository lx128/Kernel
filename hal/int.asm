extern _IntDefault, _IntInt0, _IntInt1, _IntInt2, _IntInt3, _IntInt4, _IntInt5, _IntInt6, _IntInt7, _IntInt8, _IntInt9, _IntInt10, _IntInt11, _IntInt12, _IntInt13, _IntInt14, _IntInt15, _IntInt16, _IntInt17, _IntInt18, _SysCallD, _IntIrq0, _IntIrq1, _IntIrq2, _IntIrq3, _IntIrq4, _IntIrq5, _IntIrq6, _IntIrq7, _IntIrq8, _IntIrq9, _IntIrq10, _IntIrq11, _IntIrq12, _IntIrq13, _IntIrq14, _IntIrq15

global _default_int, _k_int0, _k_int1, _k_int2, _k_int3, _k_int4, _k_int5, _k_int6, _k_int7, _k_int8, _k_int9, _k_int10, _k_int11, _k_int12, _k_int13, _k_int14, _k_int15, _k_int16, _k_int17, _k_int18, _k_irq0, _k_irq1, _k_irq2, _k_irq3, _k_irq4, _k_irq5, _k_irq6, _k_irq7, _k_irq8, _k_syscall, _k_irq9, _k_irq10, _k_irq11, _k_irq12, _k_irq13, _k_irq14, _k_irq15

_default_int:
	cli
	call _IntDefault
	sti
	iret

_k_int0:
	cli
	call _IntInt0
	sti
	iret

_k_int1:
	cli
	call _IntInt1
	sti
	iret

_k_int2:
	cli
	call _IntInt2
	sti
	iret

_k_int3:
	cli
	call _IntInt3
	sti
	iret

_k_int4:
	cli
	call _IntInt4
	sti
	iret

_k_int5:
	cli
	call _IntInt5
	sti
	iret

_k_int6:
	cli
	call _IntInt6
	sti
	iret

_k_int7:
	cli
	call _IntInt7
	sti
	iret

_k_int8:
	cli
	call _IntInt8
	sti
	iret

_k_int9:
	cli
	call _IntInt9
	sti
	iret

_k_int10:
	cli
	call _IntInt10
	sti
	iret

_k_int11:
	cli
	call _IntInt11
	sti
	iret

_k_int12:
	cli
	call _IntInt12
	sti
	iret

_k_int13:
	cli
	call _IntInt13
	sti
	iret

_k_int14:
	cli
	call _IntInt14
	sti
	iret

_k_int15:
	cli
	call _IntInt15
	sti
	iret

_k_int16:
	cli
	call _IntInt16
	sti
	iret

_k_int17:
	cli
	call _IntInt17
	sti
	iret

_k_int18:
	cli
	call _IntInt18
	sti
	iret

_k_syscall:
	call _SysCallD
	iret

_k_irq0:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq0
	call _reset_master
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq1:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq1
	call _reset_master
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq2:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq2
	call _reset_master
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq3:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq3
	call _reset_master
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq4:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq4
	call _reset_master
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq5:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq5
	call _reset_master
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq6:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq6
	call _reset_master
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq7:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq7
	call _reset_master
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq8:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq8
	call _reset_masterslave
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq9:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq9
	call _reset_masterslave
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq10:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq10
	call _reset_masterslave
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq11:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq11
	call _reset_masterslave
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq12:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq12
	call _reset_masterslave
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq13:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq13
	call _reset_masterslave
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq14:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq14
	call _reset_masterslave
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_k_irq15:
	cli
	push ebp
	push gs
	push fs
	push es
	push ss
	push ds
	pusha
	call _IntIrq15
	call _reset_masterslave
	popa
	pop ds
	pop ss
	pop es
	pop fs
	pop gs
	pop ebp
	sti
	iret

_reset_master:
	mov al,0x20
	out 0x20,al
	ret

_reset_masterslave:
	mov al,0x20
	out 0x20,al
	out 0xA0,al
	ret
