#ifndef ATA_H
#define ATA_H

#include<clr.h>

void AtaInit(uchar,ushort,ushort);
uchar AtaReadSector(void*,uint,uchar,uint);
uchar AtaWriteSector(void*,uint,uchar,uint);

#endif
