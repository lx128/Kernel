#ifndef SYSCALL_H
#define SYSCALL_H

#include<clr.h>

#define SYS_SIZE	0x20	/*max. descriptors number in table*/

struct SysCallStruct
{
	uint manager:16;
	uint fuction:16;
}__attribute__ ((packed));

ushort SysCallAdd (uint);
uint SysCall(uint,uint,uint,uint,uint,uint);

#endif
