#include <archx86.h>
#include <cpux86.h>
#include <bios_drive.h>
#include <drive.h>
#include <fonctions.h>

unsigned int CmosFloppy (void);
unsigned int BiosDispatch(struct drive_command *);

static unsigned int BiosRead(unsigned long long, unsigned char*, unsigned char);
static unsigned int BiosInitDrive(unsigned char);

static struct int86_reg reg = {0,0,0,0,0,0,0,0,0};
static struct bios_drive_data data = {0,0,0,0,0};
static struct dap_str *dap = (struct dap_str *)CONF;

//unsigned int BiosDispatch(unsigned char opcode, unsigned long long offset, unsigned char* dest)
unsigned int BiosDispatch(struct drive_command *current)
{
    static unsigned int res=0;  //To resolve a bug
    static unsigned char drive = 0;//To resolve a bug
        switch (current->command)
    {
    case 0:
//kprint ("BiosDispatch\n");
         drive = current->drive;
         res = BiosInitDrive(drive); //BUG avec "offset"***
//kprint ("BiosDispatch 1\n");
//while(1);
    break;
    case 2:
//kprint ("BiosDispatch 2\n");
         res = BiosRead(current->offset, current->buffer, current->bread);
//kprint ("BiosDispatch 3\n");
    break;
    }
//kprint ("BiosDispatch Exit\n");
//while(1);
return res;
}

unsigned int BiosInitDrive(unsigned char drive)
{
/*INT 13 - IBM/MS INT 13 Extensions - INSTALLATION CHECK
	AH = 41h
	BX = 55AAh
	DL = drive (80h-FFh)
Return: CF set on error (extensions not supported)
	    AH = 01h (invalid function)
	CF clear if successful
	    BX = AA55h if installed
	    AH = major version of extensions
		01h = 1.x
		20h = 2.0 / EDD-1.0
		21h = 2.1 / EDD-1.1
		30h = EDD-3.0
	    AL = internal use
	    CX = API subset support bitmap (see #00271)
	    DH = extension version (v2.0+ ??? -- not present in 1.x)
Note:	the Phoenix Enhanced Disk Drive Specification v1.0 uses version 2.0 of
	  the INT 13 Extensions API
SeeAlso: AH=42h"INT 13 Ext",AH=48h"INT 13 Ext"

Bitfields for IBM/MS INT 13 Extensions API support bitmap:
Bit(s)	Description	(Table 00271)
 0	extended disk access functions (AH=42h-44h,47h,48h) supported
 1	removable drive controller functions (AH=45h,46h,48h,49h,INT 15/AH=52h)
	  supported
 2	enhanced disk drive (EDD) functions (AH=48h,AH=4Eh) supported
	extended drive parameter table is valid (see #00273,#00278)
 3-15	reserved (0)*/
static unsigned char typefd = 0;
//hex(drive);
//kprint("Detect drive\n");//0x403e1d
//while(1);
switch (drive)
{
case 0:
case 1:
//Drive is floppy
//kprint("Drive is floppy\n");
        typefd = CmosFloppy ();
//hex (typefd);
        if (drive == 0)
        {
                  typefd = 4>>typefd;
        }
//1.44 3.5"
//hex (typefd);
        switch (typefd & 0xF)
        {
        case 0x4:
        data.drive_nb = drive;
        data.cylinders = 80;
        data.heads = 2;
        data.sectors = 18;
        data.bytes_sector = 512;
        break;
        default:
                kprint ("Bios_drive.c *** Drive unknow ***\n");
        return 0;
        }
        reg.ax = 0;         //Reset drive
        reg.dx = drive;
        reg.int86 = 0x13;
        Int86(&reg);//404194

/*  Bits 7-4 - First Floppy Disk Drive Type
   0h      No Drive
   1h      360 KB 5 1/4 Drive
   2h      1.2 MB 5 1/4 Drive - note: not listed in PS/2 technical manual
   3h      720 KB 3 1/2 Drive
   4h     1.44 MB 3 1/2 Drive
   5h-Fh  unused (??? 5h: 2.88 Mb 3 1/2 Drive ???)

   Bits 3-0 Second Floppy Disk Drive Type (bit settings same as A)

   Hence a PC having a 5 1/4 1.2 Mb A: drive and a 1.44 Mb B: drive will
   have a value of 24h in byte 10h. With a single 1.44 drive: 40h.
*/
break;
case 0x80:
case 0x81:
case 0x82:
case 0x83:
//Drive is HDD
//while(1);
//kprint("Drive is floppy\n");
     reg.ax = 0x4100;         //Extention search
     reg.bx = 0x55aa;
     reg.dx = drive;
     reg.int86 = 0x13;
//hexl(reg.ax);
//hexl(reg.flag);
//hex(drive);
     Int86(&reg);
//hexl(reg.ax);
//hexl(reg.flag);
     if (reg.flag & 0x1)
     {
     kprint ("**STOP** Bios_drive.c Extended read not supported\n");
     while (1);
     }
//     kprint ("Extended read supported\n");

//Check if drive exist.
//hexl(reg.bx);
//hexl(0xAA55);
     if (reg.bx == (short)0xAA55 )
     {
//     kprint ("Found drive\n");
     data.drive_nb = drive;
     }
     else
     {
     kprint ("**STOP** Bios_drive.c\nDrive not found or error\n");
     while (1);
     }
break;
default:
                      kprint ("Bios_drive.c *** Drive not supported ***\n");
                      return 0;
break;
}
return 1;
}

/*offset=location on disk, dest=buffer, drive=bios drive*/
unsigned int BiosRead(unsigned long long offset, unsigned char *dest, unsigned char nblocks)
{
    static unsigned short cyl=0, head=0, sector=0, offsetd=0;
//kprint("BiosRead\n");
//hexl(data.drive_nb);
//while(1);
    switch(data.drive_nb & 0x80)
    {
//Floppy
    case 0x0:
//kprint("FDDRead\n");
    offsetd = (unsigned short)offset;
    /*int 13h extended*/
    cyl = offsetd / 36;
    offsetd = offsetd % 36;
    head = offsetd / 18;
    sector = (offsetd % 18) +1 ;

    reg.ax = 0x200 + 1;         //Read
    reg.bx = (unsigned short)BUFFER_DMA;         //Read
    reg.cx = ((cyl & 0xFF)<<8) + (cyl & 0x300) + sector;
    reg.dx = (head<<8) + (unsigned short)data.drive_nb;
//binaire(reg.dx);
    reg.int86 = 0x13;
//hex(data.drive_nb);
//kprint("FDDRead2\n");
    Int86(&reg);//404194
//kprint("FDDRead3\n");
//binairel(reg.flag);
    if (reg.flag & 0x1)
    {
       kprint("**Stop** Bios_drive.c Disk Read Error\n");
hex(reg.ax);
while(1);
    return 0;
    }
//kprint("FDDRead4\n");
    CpuCopyStringW((unsigned char *)BUFFER_DMA, dest, (unsigned long)256);
//kprint("FDDRead5\n");
//hexl(dest);
//hex(dest[510]);
//hex(dest[511]);
//while(1);
    return 1;

//HDD
    case 0x80:
//kprint ("HDD read\n");
    /*INT 13 - IBM/MS INT 13 Extensions - EXTENDED READ
	AH = 42h
	DL = drive number
	DS:SI -> disk address packet (see #00272)
Return: CF clear if successful
	    AH = 00h
	CF set on error
	    AH = error code (see #00234)
	    disk address packet's block count field set to number of blocks
	      successfully transferred
*/
/*int 13h extended*/
//while(1);
     reg.ax = 0x4200;         //Extended Read
     reg.bx = 0;
     reg.cx = 0;
     reg.dx = (unsigned short)data.drive_nb;
//hexl (data.drive_nb);
     reg.si = CONF;
     reg.int86 = 0x13;

     dap->size = 0x10;          //dap structure size
     dap->_reserved = 0;
     dap->sector_nb = 0x1;
     dap->buffer = BUFFER_DMA;
//hexl (dest);
     dap->start = (unsigned long long)offset;

//hexl (offset);
//while(1);
    Int86(&reg);//404277
//kprint ("HDD read2\n");
//while(1);//403aca
     CpuCopyStringW((char*)BUFFER_DMA, dest, (unsigned long)256);
//    CpuCopyStringW((unsigned char *)CONF, dest, (unsigned long)256);
//while(1);//0x40371b
//kprint ("HDD read Exit\n");
//hexl(reg.flag);
//hexl(reg.ax);
//while(1);
    //Done
    default:
    break;
}
    return 1;
}
