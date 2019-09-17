#include <idt.h>
#include <archx86.h>

void default_int(void);
void k_int0(void);
void k_int1(void);
void k_int2(void);
void k_int3(void);
void k_int4(void);
void k_int5(void);
void k_int6(void);
void k_int7(void);
void k_int8(void);
void k_int9(void);
void k_int10(void);
void k_int11(void);
void k_int12(void);
void k_int13(void);
void k_int14(void);
void k_int15(void);
void k_int16(void);
void k_int17(void);
void k_int18(void);
void k_irq0(void);
void k_irq1(void);
void k_irq2(void);
void k_irq3(void);
void k_irq4(void);
void k_irq5(void);
void k_irq6(void);
void k_irq7(void);
void k_irq8(void);
void k_irq9(void);
void k_irq10(void);
void k_irq11(void);
void k_irq12(void);
void k_irq13(void);
void k_irq14(void);
void k_irq15(void);
void k_syscall(void);

//Add a descriptor in the table
void IdtAddDesc(ulong offset, ushort select, uchar type, ushort idtptr, uchar dpl)
{
    static struct idtdesc *desc = 0;

    if (idtptr>IDTSIZE)
    {
	return;
    }
	desc = (struct idtdesc *)(idtptr*8+IDTBASE);

	desc->offset0_15 = (offset & 0xffff);
	desc->selector = select;
	desc->_reserved = 0;
	desc->type = type;
	desc->dpl = dpl;
	desc->present = 1;
	desc->offset16_31 = (offset & 0xffff0000) >> 16;
}

void IdtInit(void) {

    static struct idtr kidtr = {0,0};
    static uint i = 0;

/*System descriptors initialisation*/
    for(i=0;i<=IDTSIZE;i++)
    {
        IdtAddDesc((uint)default_int, 0x08, INTGATE_32, i, 0);
    }

/*Init default idt interruption*/
    IdtAddDesc((uint)k_int0, 0x08, INTGATE_32, 0, 0);
    IdtAddDesc((uint)k_int1, 0x08, INTGATE_32, 1, 0);
    IdtAddDesc((uint)k_int2, 0x08, INTGATE_32, 2, 0);
    IdtAddDesc((uint)k_int3, 0x08, INTGATE_32, 3, 0);
    IdtAddDesc((uint)k_int4, 0x08, INTGATE_32, 4, 0);
    IdtAddDesc((uint)k_int5, 0x08, INTGATE_32, 5, 0);
    IdtAddDesc((uint)k_int6, 0x08, INTGATE_32, 6, 0);
    IdtAddDesc((uint)k_int7, 0x08, INTGATE_32, 7, 0);
    IdtAddDesc((uint)k_int8, 0x08, INTGATE_32, 8, 0);
    IdtAddDesc((uint)k_int9, 0x08, INTGATE_32, 9, 0);
    IdtAddDesc((uint)k_int10, 0x08, INTGATE_32, 10, 0);
    IdtAddDesc((uint)k_int11, 0x08, INTGATE_32, 11, 0);
    IdtAddDesc((uint)k_int12, 0x08, INTGATE_32, 12, 0);
    IdtAddDesc((uint)k_int13, 0x08, INTGATE_32, 13, 0);
    IdtAddDesc((uint)k_int14, 0x08, INTGATE_32, 14, 0);
    IdtAddDesc((uint)k_int15, 0x08, INTGATE_32, 15, 0);
    IdtAddDesc((uint)k_int16, 0x08, INTGATE_32, 16, 0);
    IdtAddDesc((uint)k_int17, 0x08, INTGATE_32, 17, 0);
    IdtAddDesc((uint)k_int18, 0x08, INTGATE_32, 18, 0);

/*Init default idt IRQ*/
//    add_idt_desc(0, 0x20, TASKGATE, 32, 0);
    IdtAddDesc((uint)k_irq0, 0x08, INTGATE_32, 32, 0);
    IdtAddDesc((uint)k_irq1, 0x08, INTGATE_32, 33, 0);
    IdtAddDesc((uint)k_irq2, 0x08, INTGATE_32, 34, 0);
    IdtAddDesc((uint)k_irq3, 0x08, INTGATE_32, 35, 0);
    IdtAddDesc((uint)k_irq4, 0x08, INTGATE_32, 36, 0);
    IdtAddDesc((uint)k_irq5, 0x08, INTGATE_32, 37, 0);
    IdtAddDesc((uint)k_irq6, 0x08, INTGATE_32, 38, 0);
    IdtAddDesc((uint)k_irq7, 0x08, INTGATE_32, 39, 0);
    IdtAddDesc((uint)k_irq8, 0x08, INTGATE_32, 112, 0);//8
    IdtAddDesc((uint)k_irq9, 0x08, INTGATE_32, 113, 0);//9
    IdtAddDesc((uint)k_irq10, 0x08, INTGATE_32, 114, 0);//10
    IdtAddDesc((uint)k_irq11, 0x08, INTGATE_32, 115, 0);//11
    IdtAddDesc((uint)k_irq12, 0x08, INTGATE_32, 116, 0);//12
    IdtAddDesc((uint)k_irq13, 0x08, INTGATE_32, 117, 0);//13
    IdtAddDesc((uint)k_irq14, 0x08, INTGATE_32, 118, 0);//14
    IdtAddDesc((uint)k_irq15, 0x08, INTGATE_32, 119, 0);//15 reserved

    IdtAddDesc((int)k_syscall, 0x08, TRAPGATE_32, 0x80, 0);//128 syscall

/*IDT stucture initialisation*/
    kidtr.limite = IDTSIZE*8;
    kidtr.base = IDTBASE;

/*load IDT register*/
    asm("lidtl %0"::"m"(kidtr));
//while (1);
}
