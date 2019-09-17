#ifndef CPUX86_H
#define CPUX86_H

#include<clr.h>

#define outb(port,value) \
	asm volatile ("outb %%al,%%dx"::"d" (port), "a" (value));

#define outbp(port,value) \
	asm volatile ("outb %%al,%%dx; jmp 1f; 1:"::"d" (port), "a" (value));

#define inb(port) ({	\
	unsigned char _v;	\
	asm volatile ("inb %%dx,%%al" : "=a" (_v) : "d" (port)); \
	_v;	\
})

#define inw(port) ({	\
	unsigned int _v;	\
	asm volatile ("inw %%dx,%%ax" : "=a" (_v) : "d" (port)); \
	_v;	\
})
#define outw(port,value) ({	\
	unsigned int _v;	\
	asm volatile ("outw %%ax,%%dx" : "=a" (_v) : "d" (port), "a" (value)); \
	_v;	\
})

//met en pause le cpu jusqu'au prochain evenement.
#define CpuIdle(void) ({asm volatile ("hlt");})

void CpuPagingOn(uint);
void CpuPagingOff(void);
uint CpuCmpStringC(void*, void*, uint);
uint CpuCmpStringW(void*, void*, uint);
uint CpuCopyStringC(void*, void*, uint);
uint CpuCopyStringW(void*, void*, uint);
void CpuSuspendIrq(void);
void CpuRestaureIrq(void);
void CpuIrqOff(void);
void CpuIrqOn(void);

/*void inb(char);
void inw(short);
char outb(short, char);
short outbp(short, char);
*/
struct cr0
{
	uint pe:1;		//Protected mode 0=real 1=protected
	uint mp:1;		//Monitor coprossecor
	uint es:1;		//Coprocessor 0=present 1=not
	uint ts:1;		//Task switch for FPU
	uint et:1;		//1=support 387 inst. On P5 is reserved. P6 is =1
	uint ne:1;		//1=enable internal FPU report
	uint reserved:9;
	uint wp:1;		//0=kernel level write on RO page 1= kernel can't
	uint _reserved:1;
	uint am:1;		//1=enable automatic alignement check
	uint __reserved:11;
	uint nw:1;		//1=no write-through(386-486) write-back(pentium - p6)
	uint cd:1;		//1=disable caching
	uint pg:1;		//1=enable paging
}__attribute__ ((packed));

/*Cr1 Reserved*/
/*Cr2 Page fault linear address*/

struct cr3
{
	uint reserved:3;
	uint pwt:1;		//write-through or write-back for internal cache
	uint pcd:1;		//internal page caching L1 & L2
	uint _reserved:7;
	uint base:20;
}__attribute__ ((packed));

struct cr4
{
	uint vme:1;		//Virtual mode extention enable
	uint pvi:1;
	uint tse:1;
	uint de:1;
	uint pse:1;
	uint pae:1;
	uint mce:1;
	uint pge:1;
	uint pce:1;
	uint osfxsr:1;
	uint osxmmexcpt:1;
	uint reserved:21;
}__attribute__ ((packed));

#endif
