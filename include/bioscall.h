#ifndef BIOSCALL_H
#define BIOSCALL_H

#include<clr.h>

#define BIOSCALL_START	0x12	//14 octets

struct bioscall_reg
{
	ushort ax;
	ushort bx;
	ushort cx;
	ushort dx;
	ushort si;
	ushort di;
	ushort bp;
	ushort int86;
	ushort flag;
} __attribute__ ((packed));

void BiosCallInit(void*);
void BiosCall(struct bioscall_reg*); //*ax,*bx,*cx,*dx,*si,*di,bp,int,flags
//void BiosCallBuff(struct bioscall_reg*, char *, int);

#endif
