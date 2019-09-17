#ifndef PIC_H
#define PIC_H

#include<clr.h>

uint PicIrqOff(uchar);
uint PicIrqOn(uchar);
void PicInit(uchar);// 0 = reel; 1 = protected

#endif
