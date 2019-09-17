#ifndef PARTITION_H
#define PARTITION_H

#include<clr.h>

struct PartLine //16 octets
{
       unsigned char State;     //0x80 = active
       unsigned char SHead;
       unsigned short SCylSec;
       unsigned char Type;      //System file
       unsigned char EHead;
       unsigned short ECylSec;
       unsigned long MbrTo1;    //Number of sector between mbr and partition
       unsigned long SecNb;     //Number of sector of the partition
}__attribute__ ((packed));

#endif
