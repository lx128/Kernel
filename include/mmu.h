#ifndef MMU_H
#define MMU_H

#include<clr.h>

//#define B_SIZE 0x1000	//4096 bytes

#define PAGE_SIZE            0x1000

struct pde
{
    ulong present:1;	//0=not 1=present (page or page table)
    ulong rw:1;		//0=RO 1=RW
    ulong us:1;		//0=Kernel 1=User privilege (run whith WP flag in cr0)
    ulong pwt:1;		//0=Write-back 1=Write through (run whith CD flag in cr0)
    ulong cache:1;	//0=can be cached 1=prevented (set 1 for buffer)
    ulong accessed:1;	//0=not 1=readed

    ulong dirty:1;	//Page is written(modified) ?
			//Thist is only for PTE. Reserved for PDE

    ulong p_size:1;	//0=4kbytes 1=4Mbytes(Pentium pro and after)
			//Thist is only for PDE. Reserved for PTE

    ulong global:1;	//For Pentium Pro and more. Entry no deleted
			//in TLB.
			//Thist is only for PTE. Ignored for PDE

    ulong no_used:3;	//Aviable for mmu
    ulong base:20;	//multiple of 4096
} __attribute__ ((packed));
/*
pde pointe sur pte qui pointe sur page.

Addresse sur 32 bits:	31-22: pointeur pde
			21-12: pointeur pte
			11-0: offset dans la page
*/

void MmuAddToBitmap(ulong, ulong);		//Ajoute Y pages de RZ à partir de l'adresse X pour le monde Physique
//long MmuPAlloc(long, long);				//Alloue Y pages de RZ à partir de l'adresse X pour le monde Physique Retourne la position de la page
//void MmuPFree(long, long);				//Libère Y pages de RZ à partir de l'adresse X pour le monde Physique
//long MmuAllocV(long, char);			//Alloue X pages de RZ au hazard pour le contexte courant User Retourne la position de la page

//char MmuLastError();				//retourne la dernière erreur.

//Fonction existante
uint AllocVM(uint, uint, uint); //alloue une page virtuelle.
void MmuEraseBlock(uint, uint);      //position, pos virtuelle, blocks
//static int CheckPage(int, int);    //teste si des page sont libres
uint FindFreePage(uint);      //cherche des pages libre
uint AllocPage(uint, uint);    //alloue des pages

//Fonction non existante
//static int MmuFindFreePage(int);   //cherche des pages libre

//int MmuFindFreePage(int);      //alloc x pages de RS2 dans VS1
//int MmuFindFreePage(int);      //cherche des pages libre

                               //Alloue x pages dans VS

uint MmuRAllocV(ulong, uchar, uchar);//alloc une page au hazard dans VS
                               //long = size char = zone

uint MmuRAllocR(uint);      //cherche des pages libre
void MmuInitMemorySize(void);
uint MmuGetMemorySize(void);


//int malloc_drv(int);             //alloue des pages
//static void allocm2(int);
//static int free_b(int, int);

#endif
