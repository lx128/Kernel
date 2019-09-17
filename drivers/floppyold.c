#include "../include/sys.h"

//fd 1.44 cyl:80 sect:18 head:2 base minor: 28 b/sect: 512
//data rate=0=500Ko/s spec=0xCF gap=0x1B
//DMA Mode unique

//Commande DMA
#define dmaoutb(port,val) \
__asm__("outb %0,%1\n\tjmp 1f\n1:\tjmp 1f\n1:"::"a" ((char) (val)),"i" (port))

#define		F_BASE		0x3F0
#define		F_DOR		0x2
#define		F_MSRDSR	0x4
#define		F_FIFO		0x5

#define		FC_1		0x80
#define		FC_2		0x90

//Commandes controleur
#define		FC_VERSION	0x10
#define		FC_RESET	0x04
#define		FC_SEEK		0x0F

//Registre & commande cmos pour fd
#define		RTC_SEL		0x70
#define		RTC_DATA	0x71
#define		CMOS_FD		0x10

//type de lecteur
#define		FD_NONE		0x0
#define		FD_360		0x1
#define		FD_1200		0x2
#define		FD_720		0x3
#define		FD_1440		0x4
#define		FD_2880		0x5

#define		BUFFER		56320	//must be < 0xA0000 because dma limitation
#define 	SIZE		0x1FF	//1024=0x4 1023=0x3FF

void f_out(unsigned char);
unsigned char f_in(void);
void fd_detect(void);
void moteur_on(void);
void moteur_off(void);
void int_status(void);
void wait_int (void);
void reset(void);
void seek(void);
void recal(void);
void buf(void);
void config(void);

unsigned char int_floppy=0;

void floppy (void)
{
    //Read command 9 octets
    int comp=0;
    unsigned char fd;
    
    print("READ Sector with dma");
    TextModePutcar(10);
//while (1);
    reset();
    config();
//    moteur_on();
//    recal();		//problem whith most 486 architecture
    seek();
    print("DMA 1");
    dma2_on(2,BUFFER,SIZE);

    print("Read Command");
    TextModePutcar(10);
    f_out(230);//commande 230 11100110b
    f_out(0);//choix lecteur A
    f_out(0);//Cylindre 0 a 80	c
    f_out(0);//Tete 0 a 1	h
    f_out(1);//Adresse du secteur 0 a 18	r
    f_out(2);//Sector size 00=128 b01=256b 02=512b 03=1024b 07=16 kb	n
    f_out(18);//Numero du dernier secteur de la piste
    f_out(0x1B);//GAP 0x1B pour 1.44 3,5"

//    f_out(0);//Numero du dernier secteur de la piste
//    f_out(0);//GAP 0x1B pour 1.44 3,5"

    f_out(0xFF);//DTL Taille du secteur = 255

    print("Wait interrupt2");
    TextModePutcar(10);
    wait_int ();
    binaire(f_in());
    binaire(inb(F_BASE+F_FIFO));
    binaire(inb(F_BASE+F_FIFO));
    binaire(inb(F_BASE+F_FIFO));
    binaire(inb(F_BASE+F_FIFO));
    binaire(inb(F_BASE+F_FIFO));
    binaire(inb(F_BASE+F_FIFO));

    moteur_off();
    buf();
while (1);
}

void f_out(unsigned char val)
{
    unsigned int comp=0;
    unsigned char in;

    while (comp<1000)
    {
	in=(inb(F_BASE+F_MSRDSR)&192);
	if (in==128)
	{
	    outb (F_BASE+F_FIFO,val);
	    return;
	}
	comp++;
    }

    print("Time out error on Output");
    TextModePutcar(10);
}

unsigned char f_in(void)
{
    unsigned int comp=0;
    unsigned char out;

    while (comp<1000)
    {
	out=(inb(F_BASE+F_MSRDSR)&192);
	if (out==192)
	{
	    return inb (F_BASE+F_FIFO);
	}
	comp++;
    }

    print("Time out error on Input");
    TextModePutcar(10);
    return 0;
}

void fd_detect()
{
    unsigned int comp;
    unsigned char version,cmos_fd,fd;

    for (comp=0;comp<10000;comp++);
    //init fd
    //type de controleur
    f_out(FC_VERSION);
    version=f_in();
    switch (version)
    {
        case FC_1:
	    print("Standard Floppy Controller detected !");
	break;
        case FC_2:
	    print("Enhenced Floppy Controller detected !");
	break;
        default:
	    print("Unknow Floppy Controller detected !");
	break;
        case 0:
	    print("FD Time out! :(");
	break;
    }
    TextModePutcar(10);
    //type de lecteur de disquette en lisant dans cmos
    outb(RTC_SEL, CMOS_FD);
    fd = (inb (RTC_DATA)>> 4 )& 0xF;
    switch (fd)
    {
	case FD_NONE:
	    print("FD1 not present or not detected! :(");
	break;
	case FD_360:
	    print("FD1 360K 5' detected !");
	break;
	case FD_1200:
	    print("FD1 1.2M 5' detected !");
	break;
	case FD_720:
	    print("FD1 720K 3.5' detected !");
	break;
	case FD_1440:
	    print("FD1 1.44M 3.5' detected !");
	break;
	case FD_2880:
	    print("FD1 2.88M 3.5' detected !");
	break;
    }
    TextModePutcar(10);
    //type de lecteur de disquette en lisant dans cmos
    outb(RTC_SEL, CMOS_FD);
    fd = (inb (RTC_DATA))& 0xF;
    switch (fd)
    {
	case FD_NONE:
	    print("FD2 not present or not detected! :(");
	break;
	case FD_360:
	    print("FD2 360K 5' detected !");
	break;
	case FD_1200:
	    print("FD2 1.2M 5' detected !");
	break;
	case FD_720:
	    print("FD2 720K 3.5' detected !");
	break;
	case FD_1440:
	    print("FD2 1.44M 3.5' detected !");
	break;
	case FD_2880:
	    print("FD2 2.88M 3.5' detected !");
	break;
    }
    TextModePutcar(10);
}

void moteur_on()
{
    unsigned char fd;

    outb(F_BASE+F_DOR,12);	//select drive A
    fd=inb(F_BASE+F_DOR)|16;
    outb(F_BASE+F_DOR,fd);

    print("Moteur on");
    TextModePutcar(10);
}

void moteur_off()
{
    unsigned char fd;
    
    fd=inb(F_BASE+F_DOR)^16;
    outb(F_BASE+F_DOR,fd);

    print("Moteur off");
    TextModePutcar(10);
}

void int_status()
{
    TextModePutcar(10);
    int_floppy=6;
}

void wait_int ()
{
    while (int_floppy!=6)
    {
    }
    print ("Interruption recu !\n");
    int_floppy=0;
}

void reset()
{
    outb(1010,28);//reset commande
}

void seek()
{
    print ("SEEK Command");
    TextModePutcar(10);
    f_out(15);//commande 15 00001111b
    f_out(0);//choix lecteur A
    f_out(0);//Nouveau Cylindre 0 a 80
    print ("Wait int");
    TextModePutcar(10);
    wait_int ();
    print ("Int OK !");
    TextModePutcar(10);
    f_out(8);//commande 8 Interrupt status
    binaire(f_in());
    binaire(f_in());
}

void recal()
{
    print ("Recal Command");
    TextModePutcar(10);
    f_out(7);//commande 7 00000111b
    f_out(0);//choix lecteur A
}

void buf()
{
    unsigned char * buf;
    unsigned long comp;

    (int)buf=BUFFER;
    comp=0;
    while (comp<SIZE)
//    while (comp<0xB93FF)
    {
	TextModePutcar(*buf);
	comp++;
	buf++;
    }
    print ("\nRead buffer OK\n");
}

void config()
{
//current spec
    f_out(0x03);//spec command
    f_out(0xCF);//spec
    f_out(6);//load time 6ms
//current rate
    outb(F_BASE+7,0);//rate = 500ko/s
}