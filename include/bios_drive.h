/*Format of disk address packet:
Offset	Size	Description	(Table 00272)
 00h	BYTE	size of packet (10h or 18h)
 01h	BYTE	reserved (0)
 02h	WORD	number of blocks to transfer (max 007Fh for Phoenix EDD)
 04h	DWORD	-> transfer buffer
 08h	QWORD	starting absolute block number
		(for non-LBA devices, compute as
		  (Cylinder*NumHeads + SelectedHead) * SectorPerTrack +
		  SelectedSector - 1
 10h	QWORD	(EDD-3.0, optional) 64-bit flat address of transfer buffer;
		  used if DWORD at 04h is FFFFh:FFFFh*/
#ifndef BIOS_DRIVE_H
#define BIOS_DRIVE_H

#include<clr.h>

struct dap_str
{
	uchar size;                  //size of dap structure
	uchar _reserved;
	ushort sector_nb;             //sector to be read
	ulong buffer;
	ullong start;            //absolute start sector
} __attribute__ ((packed));//evite que la taille de la structure soit plus grand

struct bios_drive_data
{
	uchar drive_nb;
	ulong cylinders;
	ulong heads;
	ulong sectors;
	ushort bytes_sector;
//	uchar extend : 1;
//	uchar reserved : 7;
} __attribute__ ((packed));//evite que la taille de la structure soit plus grand

struct drive_spec
{
	ushort buff_size;//(001Ah for v1.x, 001Eh for v2.x, 42h for v3.0)
	ushort If;
	ulong cylinders;
	ulong heads;
	ulong sectors;
	ullong sectors_total;
	ushort bytes_sector;
} __attribute__ ((packed));//evite que la taille de la structure soit plus grand

#endif
