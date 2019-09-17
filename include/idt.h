#ifndef IDT_H
#define IDT_H

#include<clr.h>

#define IDTBASE	0x1000		/* addr. physique ou doit resider la gdt */
#define IDTSIZE	255		/* nombre max. de descripteurs dans la table */

#define CALLGATE_16 0x4
#define TASKGATE 0x5 /* utilise pour commuter des taches */
#define INTGATE_16 0x6
#define TRAPGATE_16 0x7
#define CALLGATE_32 0xC
#define INTGATE_32 0xE  /* utilise pour gerer les interruptions */
#define TRAPGATE_32 0xF /* utilise pour faire des appels systemes */

/*segment descriptor*/
struct idtdesc {
    short offset0_15;	
    short selector;
    char _reserved;
    char type :5;
    char dpl :2;
    char present :1;
    short offset16_31;	
} __attribute__ ((packed));

/*IDT register*/
struct idtr {
    short limite;
    long base;
} __attribute__ ((packed));

void IdtInit(void);

#endif
