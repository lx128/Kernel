#ifndef FAT_H
#define FAT_H

#include<clr.h>

struct fat_current_file {
       unsigned char name [12];
       unsigned int entry;
       unsigned int currententry;
       unsigned int filesize;
       unsigned int nbclust;
       unsigned int * clustlst[16];
       unsigned int LastClust;
}__attribute__ ((packed));

struct fat_boot {
    unsigned char jmp[3];
    unsigned char name[8];
    unsigned short sect_size;
    unsigned char sect_per_cluster;
    unsigned short res_boot;
    unsigned char fat_nb;
    unsigned short max_root_entry;
    unsigned short sect_nb;
    unsigned char support;
    unsigned short fat_size;
    unsigned short sect_per_track;
    unsigned short head;
    unsigned int hiden;
    unsigned int sect_nb_32;
}__attribute__ ((packed));

struct fat1216_boot {
    unsigned char media;		//FD = 0 HDD = 0x80 or 0x81
    unsigned char reserved;
    unsigned char magic;
    unsigned int serial;
    unsigned char vol_name[11];
    unsigned char fat_type[8];
}__attribute__ ((packed));

struct fat32_boot {
    unsigned int fat_size;
    unsigned short ExtFlags;
    unsigned short FSVer;
    unsigned int RootClus;
    unsigned short FSInfo;
    unsigned short BkBootSec;
    unsigned char reserved [12];
    unsigned char media;
    unsigned char reserved2;
}__attribute__ ((packed));

struct FatDirectoryEntry {
unsigned char DIR_Name [11];
unsigned char DIR_Attr; //ATTR_READ_ONLY 0x01 ATTR_HIDDEN 0x02 ATTR_SYSTEM 0x04
               //ATTR_VOLUME_ID 0x08 ATTR_DIRECTORY 0x10 ATTR_ARCHIVE 0x20
               //ATTR_LONG_NAME ATTR_READ_ONLY |ATTR_HIDDEN |ATTR_SYSTEM |ATTR_VOLUME_ID
unsigned char DIR_NTRes; //Reserved for use by Windows NT. Set value to 0 when a file is
                //created and never modify or look at it after that.
unsigned char DIR_CrtTimeTenth;
unsigned short DIR_CrtTime; //Time file was created.
unsigned short DIR_CrtDate; //Date file was created.
unsigned short DIR_LstAccDate;
unsigned short DIR_FstClusHi;
unsigned short DIR_WrtTime;
unsigned short DIR_WrtDate; //Date of last write. Note that file creation is considered a write.
unsigned short DIR_FstClusLo; //Low word of this entry’s first cluster number.
unsigned int DIR_FileSize; //32-bit DWORD holding this file’s size in bytes.
}__attribute__ ((packed));

struct fat_current_fat {
       unsigned char FatType;        //0=error, 1=Fat12 2=Fat16CHS 3=Fat32CHS 4=Fat16LBA 5=Fat32LBA
       unsigned int FatStart;
       unsigned int FatSize;
       unsigned int RootStart;
       unsigned int DataStart;
       unsigned int MaxRootEntry;
       unsigned short SectorSize;
       unsigned char ClusterSize ;   //in sectors
}__attribute__ ((packed));

#endif
