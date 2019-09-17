#include <clr.h>
#include <archx86.h>
#include <fonctions.h>
#include <cpux86.h>
#include <pic.h>

/*
1st control		h1f0+x
    control register	h3f0+y
2nd			h170+x
    control reg		h370+y
3rd 			hf0+x
    c r			h2f0+y
4th			h70+x
    cr			h270+y
x = 0 to 7
y = 6 to 7
*/
void IntAdd(char, void*);
void AtaIrq(void);

struct hdd_str
{
    ushort cylinders;
    ushort heads;
    ushort sectors;
    uchar rm :1;
    uchar dma :1;
    uchar atapi :1;
    uchar multirw :1;
    uchar lba :1;
    uchar _reserved :3;
    ulong lba_size;
	uchar reg_etat;
	uchar reg_erreur;
}__attribute__ ((packed));

static uchar irq = 0;
static struct hdd_str hdd[2] = {{0,0,0,0,0,0,0,0,0,0}};

//copie des données du disque vers le buffer
static void write_buf(short *buffer)//ok
{
    static int comp = 0;
    for(comp = 0;comp<256;comp++)
    {
		buffer[comp]=inw(0x1f0);
    }
}

//copie des données du buffer vers le disque
static void read_buf(short *buffer)//ok
{
    static int comp = 0;
    for(comp = 0;comp<256;comp++)
    {
		outw(0x1f0,buffer[comp]);
    }
}

void AtaIrq(void)//ok
{
    irq=1;
}

//Envoie d'une commande sur le disque
static void AtaSendCommand(unsigned char drive, unsigned char val)//ok
{
    static unsigned int comp = 0;

    for (comp = 0;comp<1000;comp++)
    {
		if (~(inb(0x3f0+6)&0x80))
		{
			outb (0x1f0+7,val);
			kprint("Ata: Command Sended\n");
			return;
		}
    }

    kprint ("Ata: Command send time out\n");
}

//teste si le controleur est pret à recevoir une commande
static void AtaReady(void)//ok
{
    static unsigned int comp = 0;

    for (comp = 0;comp<1000;comp++)
    {
		if (~(inb(0x3f0+6)&0xA0))
		{
			//while(irq==0);
			//irq=0;
			kprint ("Ata: Disk Ready\n");
			return;
		}
    }
    kprint ("Ata: Time out on output\n");
}

//teste si l'interruption est arrivé
static void AtaIrqReceved(void)//ok
{
    static unsigned int comp = 0;

    for (comp = 0;comp<1000;comp++)
    {
		if (irq!=0)
		{
			irq=0;
			kprint ("Ata: AtaIrqReceved\n");
			return;
		}
    }
    kprint ("Ata: Time out on AtaIrqReceved\n");
}

void AtaInit(void)//ok
{
    static ushort * buffer = 0;
//    static short comp = 0;

    kprint ("Ata: Init\n");

	IntAdd(14, AtaIrq);//enregistre la routine d'irq
	PicIrqOn(14);

    outb (0x3f0+6,(8));//ecriture dans le registre de controle
	//bit correspondant dans le registre de controle
	//8:	1=lecteur de + de 8 tetes
	//4:	1=Initialisation 0=normal
	//2:	1=N'autorise pas les interruption 0=Autorise

	//choix du buffer
	buffer = (char*) 0x2000;
/*print("Etat: ");
binaire(inb(0x1f0+7));
*/
//    outb (0x1f6,32|64|128);//Bit 0,1,2,3 is head 4 is master0/slave1 5,7 must
    outb (0x1f6,32|128);//Bit 0,1,2,3 is head 4 is master0/slave1 5,7 must
			//be set to 1 and 6 is CHS0 or LBA1

    kprint ("Ata: Get disk info\n");
	AtaSendCommand (0,0xec);//Obtenir les informations sur le disque
    AtaReady();
    write_buf((short *)buffer);

kprint(buffer+0x1b);
    //removable media ?
    if (*buffer & 64)
    {
		hdd[0].rm = 0;
    }
    else
    {
		hdd[0].rm = 1;
    }

    hdd[0].cylinders = *(buffer+1);	//Cylinders 1
    //Reserved 2
    hdd[0].heads = *(buffer+3);		//Heads 3
    //Reserved 4
    //Reserved 5
    hdd[0].sectors = *(buffer+6);		//word 6
    //Reserved 7
    //Reserved 8
    //Reserved 9
    //Serial, 10 words long
    //Reserved 20
    //Reserved 21
    //Serial 22
    //23 to 26 firmware revision (4 ascii)
    //27 to 46 Model number (40 ascii)
    hdd[0].multirw = (*(buffer+47)&255);		//word 47
    hdd[0].lba = ((*(buffer+49)&256)>>8)&1;	//word 49
    hdd[0].dma = ((*(buffer+49)&512)>>9)&1;	//word 49
	hdd[0].lba_size = (long)(*(buffer+60));

	if (hdd[0].lba_size>16515072)
	{
		hdd[0].cylinders = 16383;	//Cylinders 1
		hdd[0].heads = 16;			//Heads 3
		hdd[0].sectors = 63;		//word 6
	}

kprint("Ata: LBA support: ");
TextModePutcar(0x30+hdd[0].lba);
TextModePutcar(0xA);
kprint("Ata: DMA support: ");
TextModePutcar(0x30+hdd[0].lba);
TextModePutcar(0xA);

	kprint ("Ata: Init end\n");
}

//Lire un seul secteur en CHS sans DMA
uchar AtaReadSector(void* buffer, uint size ,uchar lecteur ,uint secteur)//ok
{
	static ushort s=0,c=0,h=0;
kprint ("Ata: ReadSector\n");

	//conversion de LBA vers CHS
	s = 1 + (secteur % hdd[lecteur].sectors);
	h = (secteur / s) % hdd[lecteur].heads;
	c = secteur / (s * hdd[lecteur].heads);

	AtaReady();
	outb (0x3f6,0);//configure
	outb (0x1f2,1);//nb of sectors to read
	outb (0x1f3,(uchar)s);//sector nb
	outb (0x1f4,(uchar)c);//cylinder nb octet inferieur
	outb (0x1f5,(uchar)(c>>8));//cylinder nb octet supérieur
	outb (0x1f6,
	h|					//bit 0,1,2,3 head
	0x10*(lecteur%2)|	//bit4  master=0 slave=1
	0xA0				//5,7 must be set to 1
	);					//6 CHS=0 or LBA=1
	AtaSendCommand (0,0x20);//envoie read command

	AtaIrqReceved();
	AtaReady();
	write_buf((short *)buffer);

kprint("Ata: Test etat\n");
	hdd[lecteur].reg_etat=inb(0x1f0+7);
	if(hdd[lecteur].reg_etat&1)//Ereur si Etat = XXXXXXX1
	{
		kprint("Ata: Releve erreur\n");
		hdd[lecteur].reg_erreur=inb(0x1f0+1);
		return 1;
	}

kprint ("Ata: ReadSector end\n");
	return 0;//success
}

//Lire un seul secteur en CHS sans DMA
uchar AtaWriteSector(void* buffer, uint size ,uchar lecteur ,uint secteur)//ok
{
	static ushort s=0,c=0,h=0;
kprint ("Ata: WriteSector\n");

	//conversion de LBA vers CHS
	c = secteur / (hdd[lecteur].sectors * hdd[lecteur].heads);
	h = secteur % (hdd[lecteur].sectors * hdd[lecteur].heads) / hdd[lecteur].sectors;
	s = 1 + (secteur % (hdd[lecteur].sectors * hdd[lecteur].heads) % hdd[lecteur].sectors);

	AtaReady();
	outb (0x3f6,0);//configure
	outb (0x1f2,1);//nb of sectors to read
	outb (0x1f3,(uchar)s);//sector nb
	outb (0x1f4,(uchar)c);//cylinder nb octet inferieur
	outb (0x1f5,(uchar)(c>>8));//cylinder nb octet supérieur
	outb (0x1f6,
	h|					//bit 0,1,2,3 head
	0x10*(lecteur%2)|	//bit4  master=0 slave=1
	0xA0				//5,7 must be set to 1
	);					//6 CHS=0 or LBA=1
	AtaSendCommand (0,0x30);//envoie read command
	AtaReady();
	read_buf((short *)buffer);

kprint("Ata: Test etat\n");
	hdd[lecteur].reg_etat=inb(0x1f0+7);
	if(hdd[lecteur].reg_etat&1)//Ereur si Etat = XXXXXXX1
	{
		kprint("Ata: Releve erreur\n");
		hdd[lecteur].reg_erreur=inb(0x1f0+1);
		return 1;
	}

kprint ("Ata: WriteSector end\n");
	return 0;//success
}
