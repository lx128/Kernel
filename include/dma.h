#ifndef DMA_H
#define DMA_H

#include<clr.h>

//Commande DMA
#define dmaoutb(port,val) \
__asm__("outb %0,%1\n\tjmp 1f\n1:\tjmp 1f\n1:"::"a" ((char) (val)),"i" (port))

void DmaInit(void);
void DmaOn(uchar,uint,uint);	//cannal, buffer, size of buffer
void DmaOff(uchar);			//cannal

#endif
