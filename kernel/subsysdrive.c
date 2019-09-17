#include <subdrive.h>
#include <clr.h>
#include <archx86.h>
#include <fonctions.h>

static uint res = 0, hdd=0;  //res = To resolve a bug
static struct drive_command current={0,0,0,0,0,0,0,0};

uint InitLogicalDrive(uchar drive, uchar partnb)
{
kprint("Drive.c DetectDrive\n");
    //detect if drive exist
    current.drive=drive;
    current.command=0;
    res = BiosDispatch(&current);
    if (res == 0)
    {
                      kprint ("Drive.c *** Drive not present ***\n");
                      return 0;
    }


    //floppy media ?
    if (drive&0x80){hdd = 1;}
    else{hdd = 0;}

    //detect if partition exist
    if(hdd)
    {
kprint("Drive.c DetectPartition\n");
    current.part=partnb;
    res = PartDispatch(&current);
/*    if ((res == 2) & ((drive&0x80) == 0x80))
    {
                      kprint ("Drive.c *** Partition not present ***\n");
                      return 0;
    }
    if (res == 2)
    {
                      kprint ("Drive.c *** Removable media ***\n");
    }*/
    if (res == 0){
                      kprint ("Drive.c *** Read Error ***\n");
                      return 0;
                 }
    }

    //detect file system on it
//kprint ("File system search\n");
       res = FatDispatch(&current);
//kprint ("File system search2\n");
    if (res == 0)
    {
                      kprint ("Drive.c *** File system not found or unknow ***\n");
                      return 0;
    }
//kprint("InitLogicalDrive Exit");
    return 1;
}

uint OpenFile(uchar * file)
{
    //Open routine
    current.command=1;
    current.name=file;
    res = FatDispatch(&current); // opcode OPEN, 0, File name
    //Done/
    return res;
}

uint ReadFile(ullong offset, uchar* buff)
{
    //Call correct file system with good parameters
    current.command=2;
    current.offset=offset;
    current.buffer=buff;
      res = FatDispatch(&current);
    //Done
    return res;
}

uint ReadPart(ullong offset, uchar* buffer)
{
static struct drive_command current2={0,0,0,0,0,0,0,0};
     //Call partition driver
//kprint("ReadPart1");
//hexl(res);
//while(1);
    current2.command=2;
    current2.offset=offset;
    current2.buffer=buffer;

      if (hdd)
      {
//kprint("Drive.c");
      res = PartDispatch(&current2);
      }
      else{
      ReadDrive(offset, buffer);
      res=1;
      }
//      while(1);//0x4038e7
     //Done
     return res;
}

uint ReadPartFsType(uchar *partnb)
{
     //Call partition driver and return partition type.
static struct drive_command current2={0,0,0,0,0,0,0,0};
//kprint("ReadPartFs\n");
    current2.command=3;
//hexl(&current2);
      if (hdd)
      {
//while(1);
      res = PartDispatch(&current2);
      *partnb = current2.filesys;
      }
      else{
      *partnb=1;
      res=1;
      }
     //Done
//hex(res);
     return res;
}

uint ReadDrive(ullong offset, uchar* buffer)
{
    //Read correct drive. 512 bytes
//kprint("ReadDrive1");
//while(1);
static struct drive_command current2={0,0,0,0,0,0,0,0};
    current2.command=2;
    current2.offset=offset;
    current2.buffer=buffer;
      res = BiosDispatch(&current2);
//kprint("ReadDrive2\n");
    return res;
}
