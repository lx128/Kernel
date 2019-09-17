#ifndef _TASK_H_
#define _TASK_H_

#include<clr.h>

#define TSS_SIZE 104
#define TSS_BASE 0x101240	// After the bios shadow at 1053248
#define TSS_NB 10

//extern void task1();

struct TSS {
   u16 OldTss, __reserved0;
   u32 esp0;
   u16 ss0, __reserved1;
   u32 esp1;
   u16 ss1, __reserved2;
   u32 esp2;
   u16 ss2, __reserved3;
   u32 cr3, eip, eflags;
   u32 eax,ecx,edx,ebx,esp,ebp,esi,edi;
   u16 es, __reserved4;
   u16 cs, __reserved5;
   u16 ss, __reserved6;
   u16 ds, __reserved7;
   u16 fs, __reserved8;
   u16 gs, __reserved9;
   u16 ldt, __reserved10;
   u16 debugtrap, iomapbase;
};// __attribute__ ((packed));

struct TSS_DS {
   u16 limit;
   u16 base;
   u8 base2;
   u8 type;
   u8 limit2 : 4;
   u8 other : 4;
   u8 base3;
};// __attribute__ ((packed));

struct GATE_DS {
   u16 __reserved;
   u16 TssSeg;
   u8 __reserved2;
   u8 type;
   u16 __reserved3;
};// __attribute__ ((packed));


//free TSS and Task state map
struct TSS_MAP {
    u8 tss : 1;		//1 = tss used
    u8 state : 2;	//0 = ready, 1 = pass, 2 = busy on I/O, 3 = reserved
    u8 __reserved : 5;
    u32 time;
    u16 gdt_desc;
};// __attribute__ ((packed));

//static struct TSS_MAP tssmap[TSS_NB] = {0,0};
struct TSS_MAP tssmap[TSS_NB] = {0,0};
#endif
