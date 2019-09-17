#include <archx86.h>
#include <drive.h>
#include <partition.h>
#include <fonctions.h>

unsigned int PartDetect(char);
unsigned int PartRead(unsigned long long, unsigned char*);
unsigned int PartReturnFs(struct drive_command*);

static unsigned int res = 0;  //To resolve a bug
static unsigned char CurrentFS = 0;
static unsigned char buffer [512] = {0};
static unsigned int CurrentStart = 0, CurrentSize = 0;

//unsigned int PartDispatch (unsigned char opcode, unsigned long long source, unsigned char* dest)
unsigned int PartDispatch (struct drive_command *current)
{
//kprint ("PartDispatch\n");
//hex(opcode);
//while(1);//0x40396c
    switch (current->command)
    {
    case 0:
//kprint ("PartDispatch1\n");
         res = PartDetect(current->part);
//kprint ("PartDispatch2\n");
    break;
    case 2:
         res = PartRead(current->offset, current->buffer);
//while(1);//0x4039c0               //4039a2
    break;
    case 3:
         res = PartReturnFs(current);
    break;
    }
//Erreur a 0x403b79
//while(1);//0x4039d0
//kprint ("PartDispatch Exit\n");
//hex(current->filesys);
//getchar();
return res;
}
/*
Fat debut: 0x7e00 ou 0x3f secteurs    fin: 0x9d8000 ou 0x4ec0 secteurs
    taille: 0x9d0200 ou 0x4e81 secteurs
*/
unsigned int PartDetect(char partnb)
{
         static struct PartLine *ptr = 0;
//kprint ("PartDetect\n");

         res = ReadDrive(0,buffer);
//kprint("partition.c 1\n");
         if (res == 0){return 0;}
//while(1);
//kprintbuff(buffer, 512);
//hex(buffer[510]);
//hex(buffer[511]);
//hexl(buffer);
//kprint ("PartDetect0\n");
         if (partnb>4)
         {
                  kprint("Partition.c Partition number > 4\n");
                  return 0;
         }
         if ((buffer[510] != 0x55) | (buffer[511] != 0xAA))
         {
                  kprint("Partition.c No Valid MBR\n");
//while(1);
                        return 0;
         }
//kprint ("PartDetect1\n");
         ptr = (struct PartLine *)(buffer + 0x1be +(0x20* partnb));
//hexl(ptr);
//kprint ("PartDetect2\n");
         if (ptr->State != 0x80)
         {
         kprint("Partition.c Warning: Partition no active\n");
         }
//hex(ptr->State);
//hexl(ptr->MbrTo1);
//hexl(ptr->SecNb);
//hexl(ptr->Type);
//kprint ("PartDetect Exit\n");
         CurrentFS = ptr->Type;
         CurrentStart = ptr->MbrTo1;
         CurrentSize = CurrentStart + ptr->SecNb;
//while(1);
         return 1;
}

unsigned int PartRead(unsigned long long source, unsigned char* dest)
{
//kprint("PartRead\n");
         if (source>CurrentSize)
         {
                    kprint("Partition.c Try to read out of partition\n");
                    return 0;
         }
         source = source + CurrentStart;
//while(1);//0x403b5f
         res = ReadDrive(source, dest);
         return res;
}

unsigned int PartReturnFs(struct drive_command * current)
{
         current->filesys = CurrentFS;
         return 1;
}
