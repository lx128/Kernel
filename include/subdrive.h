#ifndef SUBDRIVE_H
#define SUBDRIVE_H

#include<clr.h>

uint InitLogicalDrive(uchar, uchar);

uint OpenFile(uchar *);
uint ReadFile(ullong, uchar*);

uint ReadPart(ullong, uchar *);
uint ReadPartFsType(uchar*);

uint ReadDrive(ullong offset, uchar* buff);

struct drive_command {
	uchar command;
	uchar drive;        //0=error, 1=Fat12 2=Fat16CHS 3=Fat32CHS 4=Fat16LBA 5=Fat32LBA
	uchar part;
	uchar filesys;
	uchar* name;
	ullong offset;
	ullong bread;
	uchar* buffer;
}__attribute__ ((packed));

#endif
