#include <archx86.h>
#include <cpux86.h>
#include <fat.h>
#include <drive.h>
#include <fonctions.h>

unsigned int FatDispatch (struct drive_command*);
unsigned int FatDetect(void);
unsigned int FatOpenFile(unsigned char *);
unsigned int FatReadFile(unsigned long long, unsigned char*);
unsigned int FatNextCluster(unsigned int);
unsigned int FatReadEntry(void);
unsigned int FatScanEntry(void);

static unsigned int res = 0;  //To resolve a bug
static unsigned char buffer [512] = {0};

//Current data
static struct fat_current_fat CurrentFat = {0,0,0,0,0,0};
static struct fat_current_file CurrentFile = {{0},0,0,0,0,{0},0};

//unsigned int FatDispatch (unsigned char opcode, unsigned long long source, unsigned char* dest)
unsigned int FatDispatch (struct drive_command *current)
{
//kprint("FatDispatch enter\n");
    switch (current->command)
    {
    case 0:
         res = FatDetect();
    break;
    case 1:
         res = FatOpenFile(current->name);
    break;
    case 2:
         res = FatReadFile(current->offset, current->buffer);
    break;
    }
//kprint("FatDispatch exit\n");
return res;
}

unsigned int FatDetect(void)
{
     static struct fat_boot *fat_b = 0;
     static struct fat1216_boot *fat1216_b = 0;
     static struct fat32_boot *fat32_b = 0;
     static unsigned long RootDirSectors = 0, FATSz = 0, TotSec = 0, DataSec = 0, CountofClusters=0;
     static unsigned char partnb=0;

     fat_b = (struct fat_boot *)buffer;
     fat1216_b = (struct fat1216_boot *)(buffer+36);
     fat32_b = (struct fat32_boot *)(buffer+36);

     ReadPartFsType(&partnb);
//hex(partnb);
//while(1);
//     if ((partnb != 0) &(partnb != 0x6)&(partnb != 0xb)&(partnb != 0xc)&(partnb != 0xe))
     switch (partnb)
//           Aucune       Fat 16 norm      Fat32 Norm       Fat32 LBA       Fat16 LBA
             {
             case 0:
             kprint("Fat.c Not Fat FS\n");
             CurrentFat.FatType = 0;
             return 0;
             case 0x1:
             kprint("Fat.c Fat12 ?\n");
             CurrentFat.FatType = 1;
             break;
             case 0x4:
             kprint("Fat.c Fat16 CHS <32Mo ?\n");
             CurrentFat.FatType = 2;
             break;
             case 0x6:
             kprint("Fat.c Fat16 CHS ?\n");
             CurrentFat.FatType = 2;
             break;
             case 0xb:
             kprint("Fat.c Fat32 CHS ?\n");
             CurrentFat.FatType = 3;
             break;
             case 0xc:
             kprint("Fat.c Fat32 LBA ?\n");
             CurrentFat.FatType = 5;
             break;
             case 0xe:
             kprint("Fat.c Fat16 LBA ?\n");
             CurrentFat.FatType = 4;
             break;
             
             
             case 0x11:
             kprint("Fat.c Hiden Fat12 ?\n");
             CurrentFat.FatType = 1;
             break;
             case 0x14:
             kprint("Fat.c Hiden Fat16 CHS <32Mo ?\n");
             CurrentFat.FatType = 2;
             break;
             case 0x16:
             kprint("Fat.c Hiden Fat16 CHS ?\n");
             CurrentFat.FatType = 2;
             break;
             case 0x1b:
             kprint("Fat.c Hiden Fat32 CHS ?\n");
             CurrentFat.FatType = 3;
             break;
             case 0x1c:
             kprint("Fat.c Hiden Fat32 LBA ?\n");
             CurrentFat.FatType = 5;
             break;
             case 0x1e:
             kprint("Fat.c Hiden Fat16 LBA ?\n");
             CurrentFat.FatType = 4;
             break;
             }
//hex(partnb);
//kprint ("FatDetect2\n");
     ReadPart(0,buffer);
//hexl (&fat_b->fat_nb);
//hexl (fat_b->fat_nb);
//hexl (&fat_b); //0x407650
//kprint(fat_b->name);
//hexl(buffer);
//while(1);
//kprint ("FatDetect3\n");
     CurrentFat.ClusterSize = (long)fat_b->sect_per_cluster;
     CurrentFat.SectorSize = (long)fat_b->sect_size;
//kprint ("FatDetect4\n");

//kprintbuff(buffer, 512);
//Find Fat Type
RootDirSectors = ((fat_b->max_root_entry * 32) + (fat_b->sect_size - 1)) / fat_b->sect_size;
//kprint ("Root Dir: ");
//hexl (RootDirSectors);
//kprint ("Max root entry: ");
//hexl (fat_b->max_root_entry);
//kprint ("Sector size: ");
//hexl (fat_b->sect_size);

//14.99 = 224*32 + 512-1 / 512

//taille de la fat
if(fat_b->sect_size != 0)
{
                   FATSz = fat_b->fat_size;
}
else
{
                   FATSz = fat32_b->fat_size;
}
//kprint ("FatSize: ");
//hexl (FATSz);

//Total sectors on volume.
if(fat_b->sect_nb != 0)
{
                TotSec = fat_b->sect_nb;
}
else
{
                TotSec = fat_b->sect_nb_32;
}
//kprint ("Total Sector nb: ");
//hexl (TotSec);

DataSec = TotSec - (fat_b->res_boot + (fat_b->fat_nb * FATSz) + RootDirSectors);
//kprint ("Secteurs Zone Data: ");
//hexl (DataSec);
//hexl (TotSec);
//hexl (fat_b->res_boot);
//hexl (fat_b->fat_nb);
//hexl (FATSz);
//hexl (RootDirSectors);
//hexl (fat_b->sect_per_cluster);
/*if (fat_b->sect_per_cluster == 0)
{
          kprint ("Fat.c *** Fat not detected ***\n");
      CurrentData.FatType = 0;
          return 0;
                            }*/

       CountofClusters = DataSec / fat_b->sect_per_cluster;
kprint ("Nombre de clusters: ");
hexl (CountofClusters);

//       if((CountofClusters <4085) & (CountofClusters < 65525))
       if(CountofClusters <4085)
       {
/* Volume is FAT12 */
          kprint ("Fat.c *** Fat12 detected ***\n");
      CurrentFat.FatType = 1;
      CurrentFat.FatStart = 1;
      CurrentFat.FatSize = fat_b->fat_size;
      CurrentFat.ClusterSize = (long)fat_b->sect_per_cluster;
      CurrentFat.RootStart = (long)(CurrentFat.FatStart + fat_b->fat_size*fat_b->fat_nb);
      CurrentFat.DataStart = (long)(CurrentFat.RootStart + ((fat_b->max_root_entry*0x20)/fat_b->sect_size));
      CurrentFat.MaxRootEntry = (long)fat_b->max_root_entry;
//hexl (CurrentData.RootStart);
//hexl (CurrentData.ClusterSize);
//hexl (fat_b->sect_size);
//hexl (fat_b->max_root_entry);
//hexl (CurrentData.DataStart);
//while(1);
}
//else if((4085 < CountofClusters) & (CountofClusters < 65525)) {
else if(CountofClusters < 65525) {
/* Volume is FAT16 */
          kprint ("Fat.c *** Fat16 detected ***\n");
      CurrentFat.FatType = 2;
      CurrentFat.FatStart = 1;
      CurrentFat.FatSize = fat_b->fat_size;
      CurrentFat.ClusterSize = (long)fat_b->sect_per_cluster;
      CurrentFat.RootStart = (long)(CurrentFat.FatStart + fat_b->fat_size*fat_b->fat_nb);
      CurrentFat.DataStart = (long)(CurrentFat.RootStart + ((fat_b->max_root_entry*0x20)/fat_b->sect_size));
      CurrentFat.MaxRootEntry = (long)fat_b->max_root_entry;
}
 else {
/* Volume is FAT32             ******Under construction****** */
          kprint ("Fat.c *** Fat32 detected UNDER CONSTRUCTION***\n");
      CurrentFat.FatType = 3;
      CurrentFat.FatStart = 1;
      CurrentFat.FatSize = fat32_b->fat_size;
      CurrentFat.ClusterSize = (long)fat_b->sect_per_cluster;
      CurrentFat.RootStart = (long)(CurrentFat.FatStart + fat32_b->fat_size*2);
      CurrentFat.DataStart = (long)(CurrentFat.RootStart + ((fat_b->max_root_entry*0x20)/fat_b->sect_size));
      CurrentFat.MaxRootEntry = (long)fat_b->max_root_entry;
      return 0;
}
//while(1);

    return 1;
}

unsigned int FatOpenFile(unsigned char * string)
{
    static int comp=0, comp2=0, sroot=0, sdata=0;

kprint("Fat.c FatOpenFile enter\n");
    comp = 0;
    comp2 = 0;
//    CurrentFile.currententry = CurrentFat.RootStart;
//    CurrentFile.filesize = CurrentFat.DataStart;
    sroot = CurrentFat.RootStart;
    sdata = CurrentFat.DataStart;

//effacer pour pouvoir compiler
//    CurrentFile.nbclust = ((CurrentFat.MaxRootEntry*32)CurrentFat.ClusterSize);
    while (comp != 255)
    {
          switch (string[comp])
          {
          case 0x2f:       // Detect Slash = Directory
                    CurrentFile.name[comp2]=0;
                    comp2 = 0;
                    comp++;
kprint("Fat.c Read Directory :");
kprint(CurrentFile.name);
kprint("\n");
             break;
          case 0:          //detect end of string
          case 0x10:          //detect end of string
          case 0x13:          //detect end of string
                    CurrentFile.name[comp2]=0;
                    comp2 = 0;
                    comp++;

kprint("Fat.c Read File entry :");
kprint(CurrentFile.name);
kprint("\n");
               while(sroot < CurrentFile.nbclust)
               {
               sroot++;
kprint("while\n");
             ReadPart(sroot, buffer);
             res = FatReadEntry();
                   if (res != 0)
                   {
kprint("Fat.c Trouvé !\n");
                   return 1;
                   }
             CurrentFile.entry++;
               }
kprint("Fat.c Non Trouvé\n");
//while(1);
          return 0;

          case 0x2e:
          break;      //Jump if "." detected
          default:            //Continue if other char
               if ((string[comp]<0x7a)&(string[comp]>0x61)){
               CurrentFile.name[comp2] = string[comp] - 0x20;
               }
               else {
               CurrentFile.name[comp2] = string[comp];
               }
               comp2++;
          break;
          }
          if (comp2 > 11){
                    kprint("Fat.c File or directory name too long. Max 11 char\n");
                    return 0;
                    }
          comp++;
    }

kprint("Fat.c Path too long. Max 255 char\n");
return 0;
}

unsigned int FatReadFile(unsigned long long offset, unsigned char *buff)
{
         static unsigned page = 0;

         
         res = ReadPart(page, buff);
         if (res ==0){return res;}

     return 0;
}

unsigned int FatNextCluster(unsigned int clust)
{
         static unsigned int *fatptr=0, comp=0, page=0, pagereaded=0;

//hexl(clust);
         comp = 0;
         pagereaded=-1;
         fatptr = (unsigned int *) buffer;

         while(comp < 16){
         page = (clust/CurrentFat.ClusterSize) / 256;
                if (page > CurrentFat.FatSize){
kprint("Fat.c Entry in Fat is out\n");
                     return 0;
                     }
                if (page != pagereaded){
kprint("Fat.c Changing Fat page\n");
                         res = ReadPart((page +CurrentFat.FatStart), buffer);
                         if (res ==0){kprint("Fat.c FatNextCluster Read Error\n");return 0;}
                         pagereaded = page;
                         }
                if (fatptr[clust] != 0xfff){
kprint("Fat.c End Of File\n");
                        return 1;
                 }
         clust = fatptr[clust];
         CurrentFile.clustlst[comp]= (unsigned int*)clust;
         CurrentFile.LastClust++;
//hexl(fatpos);
         comp++;
         }
kprint("Fat.c Cluster list Full\n");
//while(1);
         return 1;
}

unsigned int FatReadEntry(void)
{
       static struct FatDirectoryEntry* FDE = 0;
       static unsigned char comp = 0, comp2 = 0;
//       static unsigned int pos = 0;

         FDE = (struct FatDirectoryEntry*) buffer;
         CurrentFile.LastClust=0;

         while(FDE < (struct FatDirectoryEntry*)(buffer + CurrentFat.SectorSize -1))
         {
         comp=0;
         comp2=0;
         if (FDE->DIR_Name[0] == 0 ){
//kprint("Fin\n");
                              return 0;
            }
         if (FDE->DIR_Name[0] != 0xE5){
//kprint("Pas libre ou effacé\n");
         if (FDE->DIR_Attr != 0x0f){
//kprint("Pas nom long\n");
               while(comp < 12){
//kprint("Pas plus de 11\n");
                          if (FDE->DIR_Name[comp] != 0x20){
//kprint("Pas d'espace\n");
//hex(comp);
//hex(comp2);
//hex(name[comp2]);
//hex(FDE->DIR_Name[comp]);
//kprint(FDE->DIR_Name);
                             if (CurrentFile.name[comp2] == FDE->DIR_Name[comp]){
//kprint("Egal\n");
                                comp++;
                                comp2++;
                             }
                             else{comp = 0xff;}
//hex(comp);
                          }
                          else{comp++;}
               }
               if(comp == 12){
//kprint("Trouve\n");
               CurrentFile.entry = (FDE->DIR_FstClusHi<<16) + FDE->DIR_FstClusLo;
               CurrentFile.currententry = CurrentFile.entry;
               CurrentFile.filesize = FDE->DIR_FileSize;
               CurrentFile.nbclust = (CurrentFile.filesize/CurrentFat.ClusterSize);
               if (CurrentFile.filesize%CurrentFat.ClusterSize){CurrentFile.nbclust++;}

//hexl(FDE->DIR_FstClusHi);
//hexl(FDE->DIR_FstClusLo);
               return 1;
//               while(1);
                    }
                             
         }
         }
//kprint("Repeat\n");
         FDE = FDE + 1;
//hexl(buffer);
//hexl(FDE);
         }
//while(1);
//kprint(FDE->DIR_Name);
//hexl(FDE);
//FDE = FDE + 1;
//kprint(FDE->DIR_Name);
         return 0;
}

unsigned int FatScanEntry()
{
         return 0;
}
