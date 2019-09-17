extern __Z10IntDefaultv, __Z7IntInt0v, __Z7IntInt1v, __Z7IntInt2v, __Z7IntInt3v, __Z7IntInt4v, __Z7IntInt5v, __Z7IntInt6v, __Z7IntInt7v, __Z7IntInt8v, __Z7IntInt9v, __Z8IntInt10v, __Z8IntInt11v, __Z8IntInt12v, __Z8IntInt13v, __Z8IntInt14v, __Z8IntInt15v, __Z8IntInt16v, __Z8IntInt17v, __Z8IntInt18v, __Z8SysCallDv, __Z7IntIrq0v, __Z7IntIrq1v, __Z7IntIrq2v, __Z7IntIrq3v, __Z7IntIrq4v, __Z7IntIrq5v, __Z7IntIrq6v, __Z7IntIrq7v, __Z7IntIrq8v, __Z7IntIrq9v, __Z8IntIrq10v, __Z8IntIrq11v, __Z8IntIrq12v, __Z8IntIrq13v, __Z8IntIrq14v, __Z8IntIrq15v

global _default_int, _k_int0, _k_int1, _k_int2, _k_int3, _k_int4, _k_int5, _k_int6, _k_int7, _k_int8, _k_int9, _k_int10, _k_int11, _k_int12, _k_int13, _k_int14, _k_int15, _k_int16, _k_int17, _k_int18, _k_irq0, _k_irq1, _k_irq2, _k_irq3, _k_irq4, _k_irq5, _k_irq6, _k_irq7, _k_irq8, _k_syscall, _k_irq9, _k_irq10, _k_irq11, _k_irq12, _k_irq13, _k_irq14, _k_irq15

_default_int:
	cli
	call __Z10IntDefaultv
	sti
	iret

_k_int0:
	cli
	call __Z7IntInt0v
	sti
	iret

_k_int1:
	cli
	call __Z7IntInt1v
	sti
	iret

_k_int2:
	cli
	call __Z7IntInt2v
	sti
	iret

_k_int3:
	cli
	call __Z7IntInt3v
	sti
	iret

_k_int4:
	cli
	call __Z7IntInt4v
	sti
	iret

_k_int5:
	cli
	call __Z7IntInt5v
	sti
	iret

_k_int6:
	cli
	call __Z7IntInt6v
	sti
	iret

_k_int7:
	cli
	call __Z7IntInt7v
	sti
	iret

_k_int8:
	cli
	call __Z7IntInt8v
	sti
	iret

_k_int9:
	cli
	call __Z7IntInt9v
	sti
	iret

_k_int10:
	cli
	call __Z8IntInt10v
	sti
	iret

_k_int11:
	cli
	call __Z8IntInt11v
	sti
	iret

_k_int12:
	cli
	call __Z8IntInt12v
	sti
	iret

_k_int13:
	cli
	call __Z8IntInt13v
	sti
	iret

_k_int14:
	cli
	call __Z8IntInt14v
	sti
	iret

_k_int15:
	cli
	call __Z8IntInt15v
	sti
	iret

_k_int16:
	cli
	call __Z8IntInt16v
	sti
	iret

_k_int17:
	cli
	call __Z8IntInt17v
	sti
	iret

_k_int18:
	cli
	call __Z8IntInt18v
	sti
	iret

_k_syscall:
	call __Z8SysCallDv
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
	call __Z7IntIrq0v
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
	call __Z7IntIrq1v
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
	call __Z7IntIrq2v
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
	call __Z7IntIrq3v
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
	call __Z7IntIrq4v
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
	call __Z7IntIrq5v
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
	call __Z7IntIrq6v
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
	call __Z7IntIrq7v
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
	call __Z7IntIrq8v
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
	call __Z7IntIrq9v
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
	call __Z8IntIrq10v
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
	call __Z8IntIrq11v
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
	call __Z8IntIrq12v
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
	call __Z8IntIrq13v
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
	call __Z8IntIrq14v
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
	call __Z8IntIrq15v
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
