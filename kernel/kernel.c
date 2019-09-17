#include <archx86.h>
#include <cpux86.h>
#include <fonctions.h>
#include <ata.h>
#include <bioscall.h>
#include <syscall.h>
#include <textmode.h>
#include <mmu.h>
#include <interrupt.h>
#include <pic.h>
#include <gdt.h>
#include <idt.h>
#include <i8042.h>

void Exemple(void);
void test(uint);
void test2(uint,char*);

void start(void)
{
    static uchar boot = 0, buffer[512] = {0}, message[]="Kprint marche en appel systeme\n";

    TextModeLoad(*(uchar*)(CONF + 0x9),*(uchar*)(CONF + 0x8));//Envoie la position du curseur
    MmuInitMemorySize();
    kprint("Start Kernel\n");

//    print("Load GDT\n");
    GdtInit();//Initialisation du mode mémoire Flat
    asm("	movl %0,%%eax \n\
		movl %%eax,%%esp \n":: "g"(STACK_32_END));
//    print("New GDT loaded\n");

//    print("Load IDT\n");
    IdtInit();//Initialisation des tables d'interruption pour X86
//    print("New IDT loaded\n");

/*It's also important to know that dl contains the drive that was
booted (in other words, the drive from which this code originated).
This is 0x00 or 0x01 for the first or second floppy drives or 0x80,
0x81, 0x82, or 0x83 for the first through fourth hard drives.
If this code originated from a hard drive and the master boot code
loaded this code, then the partition table entry that corresponds to
the booted partition is passed in es:si.
*/

/*Find Memory size*/
	kprint("Kernel.c Memory size: ");
	hexl (MmuGetMemorySize());

/*Read disk boot and find boot.ini*/
	boot = *(unsigned char*) (CONF + 0xA);

	IntInit();//initialisation du gestionnaire de redirection d'interruption
	PicInit(1);//initialisation du controleur d'IRQ pour le mode protégé (1)
	CpuIrqOn();
//	MmuInit();



	BiosCallInit((void*)BIOSCALL);//initialisation du service d'interruption en mode réel.
	i8042Init ();//clavier et souris ps/2
	Exemple();

	AtaInit(IRQ_ATA_1,PORT_ATA_COMMANDE_1,PORT_ATA_CONTROLE_1);//disque dur ide
	if(AtaReadSector(buffer,512,0,0))
	{
		kprint("Read error\n");
	}
	printbuff((uchar*) buffer, 512);
	TextModePutcar(0xA);

	if(AtaWriteSector(buffer,512,0,0x80))
	{
		kprint("Write error\n");
	}

	SysCallAdd ((uint)test);
	SysCallAdd ((uint)test);
	SysCallAdd ((uint)test2);
	SysCall(0x10000,0,0,0,0,0);
	SysCall(0x50001,0,0,0,0,0);
	SysCall(0x30002,(uint)message,0,0,0,0);
	SysCall(3,0,0,0,0,0);



/*
kprint("Kernel.c InitDrive\n");



//getchar();
     if (InitLogicalDrive(boot, 0) == 0)//Drive, partition
     {
            kprint("Kernel.c  *** Drive Error *** \n");
			while(1){CpuIdle();}
     }
//while(1);
//getchar();
kprint("Kernel.c OpenFile\n");
     if (OpenFile("ldr.ini\0") == 0)
     {
            kprint("Kernel.c *** Can read File ldr.ini *** \n");
			while(1){CpuIdle();}
     }
//getchar();
kprint("Kernel.c ReadFile\n");
//     ReadFile(0, buffer);
*/
	kprint("Kernel.c Shutdown\n");
    while(1){CpuIdle();}
}

void test(uint reax)
{
	kprint("Appel systeme\n");
}

void test2(uint reax,char*message)
{
	kprint(message);
}
