#ifndef ARCHX86_H
#define ARCHX86_H

#include<clr.h>

#define cli             asm("cli"::)
#define sti             asm("sti"::)
#define CONF            0x8000
#define BIOSCALL	    0x9000
#define INT86_R         0x9000
#define INT86           0x900E
#define BUFFER_DMA      0xA000
#define STACK_32        0x24000
#define STACK_32_END    0x2BFFF

#define MAP_POS         0x100000

#define MAP_POS_PAGE    0x100

#define INDEX           0x30000     //Loader index
#define CODE_START      0x30030     //Loader code start

#define PORT_ATA_COMMANDE_1    0x1F0
#define PORT_ATA_CONTROLE_1    0x3F0
#define PORT_ATA_COMMANDE_2    0x170
#define PORT_ATA_CONTROLE_2    0x370
#define PORT_ATA_COMMANDE_3    0xF0
#define PORT_ATA_CONTROLE_3    0x2F0
#define PORT_ATA_COMMANDE_4    0x70
#define PORT_ATA_CONTROLE_4    0x270

#define IRQ_ATA_1    0xE
#define IRQ_ATA_2    0xF

#endif
