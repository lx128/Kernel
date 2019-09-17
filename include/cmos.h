#ifndef CMOS_H
#define CMOS_H

#include<clr.h>

#define STATUS_A 0xA
#define STATUS_B 0xB
#define STATUS_C 0xC
#define STATUS_D 0xD

struct times {
	uchar second;
	uchar minute;
	uchar hour;
	uchar day_wk;
	uchar day;
	uchar month;
	uchar year;
}__attribute__ ((packed));

#endif
