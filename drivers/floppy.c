#include <archx86.h>

//fd 1.44 cyl:80 sect:18 head:2 base minor: 28 b/sect: 512
//data rate=0=500Ko/s spec=0xCF gap=0x1B
//DMA Mode unique

//Registre
#define		F_BASE		0x3F0
#define		F_DOR		0x2
#define		F_MSRDSR	0x4
#define		F_FIFO		0x5


//Commandes controleur
#define		FC_VERSION	0x10
#define		FC_RESET	0x04
#define		FC_SEEK		0x0F

#define 	SIZE		0x1FF

static void f_out(unsigned char);
static unsigned char f_in(void);
static void moteur_on(void);
static void moteur_off(void);
static void fd_int_status(void);
static void wait_int (void);
static void reset(void);
//static void seek(void);
static void recal(void);
static void config(void);
static void fd_detect(char);
static void fd_copy_buff(char *,char *);
static int fd_read(int, int, char *);

void dma_on(char, int, int);
void idt_add_irq(char,int);
void idt_del_irq(char);
int malloc_drv(int);

struct floppy {
    char present:1;
    char reserved:7;
    int size;
    char sect_size;
    char gap;
    char spec;
}__attribute__ ((packed));

static unsigned char int_floppy = 0;
//static unsigned char fd_conf[2] = {0};
static struct floppy fd_conf[2] = {{0,0,0,0,0,0}};
static char *buffer = 0;

int floppy(int command, int p1, int p2, int p3)
{
    static char *buff = 0;
    switch(command)
    {
    case 0:		//read config of driver p1=dest p2=offset
	buff = (char *)p2;
	*buff = (char)"FLOPPY\n";
	*(buff+7) = 1;				//drive detected
	*(buff+8) = (char)"Init\n";		//detect command
    break;
    case 0x1:
	fd_detect((char)p1);
	return 0xff;
    break;
    case 0x2:
	fd_read(p1, p2, (char *)p3);
//int fd_read(char drive, int block, char *dest)
    break;
    }
    return 0;
}

void fd_detect(char cmos_fd)
{
    static unsigned int comp = 0;
    static unsigned char fd = 0;

//type de controleur
    idt_add_irq(6,(int)fd_int_status);
    f_out(FC_VERSION);
    fd = f_in();
    switch (fd)
    {
        case 0x80:
	    kprint("Standard Floppy Controller detected.");
	break;
        case 0x90:
	    kprint("Enhenced Floppy Controller detected.");
	break;
        default:
	    kprint("Unknow Floppy Controller detected.");
	break;
        case 0:
	    kprint("FD Time out or not present.\n");
	    idt_del_irq(6);
	    return;
	break;
    }
    moteur_off();
    TextModePutcar(10);

//type de lecteur de disquette en lisant dans cmos
    for (comp=0;comp<2;comp++)
    {
    if (comp == 0)
    {
    fd = (cmos_fd >> 4 )& 0xF;
    kprint("FD1: ");
    }
    if (comp == 1)
    {
    fd = cmos_fd & 0xF;
    kprint("FD2: ");
    }
    switch (fd)
    {
	case 0x0:
	    kprint("not present or not detected.");
	    fd_conf[comp].present = 0;
	break;
	case 0x1:
	    kprint("360K 5' detected. NOT SUPPORTED");
	    fd_conf[comp].present = 0;
	break;
	case 0x2:
	    kprint("1.2M 5' detected. NOT SUPPORTED");
	    fd_conf[comp].present = 0;
	break;
	case 0x3:
	    kprint("720K 3.5' detected. NOT SUPPORTED");
	    fd_conf[comp].present = 0;
	break;
	case 0x4:
	    kprint("1.44M 3.5' detected.");
	    fd_conf[comp].present = 1;
	    fd_conf[comp].size = 2880;
	    fd_conf[comp].sect_size = 0x2;
	    fd_conf[comp].gap = 0x1B;
	    fd_conf[comp].spec = 0xCF;
	break;
	case 0x5:
	    kprint("2.88M 3.5' detected. SUPPORTED AS 1.44M");
	    fd_conf[comp].present = 1;
	    fd_conf[comp].size = 2880;
	    fd_conf[comp].sect_size = 0x2;
	    fd_conf[comp].gap = 0x1B;
	    fd_conf[comp].spec = 0xCF;
	break;
    }
    TextModePutcar(10);
    }
    if (fd_conf[0].present | fd_conf[1].present)
    {
        buffer = (char *)(malloc_drv(0) * 4096);
    }
}

static void moteur_on(void)
{
    static unsigned char fd = 0;
    static int i = 0;

    outb(F_BASE+F_DOR,12);	//select drive A
    fd=inb(F_BASE+F_DOR)|16;
    outb(F_BASE+F_DOR,fd);
for(i=0;i>1000;i++);
    kprint("Moteur on");
    TextModePutcar(10);
}

static void moteur_off(void)
{
    static unsigned char fd = 0;
    
    fd=inb(F_BASE+F_DOR)^16;
    outb(F_BASE+F_DOR,fd);

    kprint("Moteur off");
    TextModePutcar(10);
}

void fd_int_status(void)
{
    TextModePutcar(10);
    int_floppy=6;
}

static void wait_int (void)
{
    while (int_floppy!=6)
    {
    }
    kprint ("Interruption recu !\n");
    int_floppy=0;
}

static void reset(void)
{
    outb(1010,28);//reset commande
}

/*static void seek(void)
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
}*/

static void recal(void)
{
    kprint ("Recal Command");
    TextModePutcar(10);
    f_out(7);//commande 7 00000111b
    f_out(0);//choix lecteur A
}

static void config(void)
{
//current spec
    f_out(0x03);//spec command
    f_out(0xCF);//spec
    f_out(6);//load time 6ms
//current rate
    outb(F_BASE+7,0);//rate = 500ko/s
}

static void f_out(unsigned char val)
{
    static unsigned int comp = 0;
    static unsigned char in = 0;

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

    kprint("Time out error on Output");
    TextModePutcar(10);
}

static unsigned char f_in(void)
{
    static int comp = 0;
    static unsigned char out = 0;

    while (comp<1000)
    {
	out=(inb(F_BASE+F_MSRDSR)&192);
	if (out==192)
	{
	    return inb (F_BASE+F_FIFO);
	}
	comp++;
    }

    kprint("Time out error on Input");
    TextModePutcar(10);
    return 0;
}

int fd_read(int drive, int block, char *dest)
{
    static char *bufferd = 0;
    block++;

    if (drive > 2){return 0;}

    if (~fd_conf[(int)drive].present)
    {return 0;}

    if (block > (fd_conf[(int)drive].size + 1)){return 0;}

    reset();
    moteur_on();
    config();
    recal();
//    seek();

    dma_on(2,(int)buffer,SIZE);

    f_out(230);//commande 230 11100110b
    f_out(drive);//choix lecteur A
    f_out(block/36);//Piste 0 a 80	c
    block = block%36;
    f_out(block/18);//Tete 0 a 1	h
    f_out(block %18);//Adresse du secteur 1 a 18	r
    f_out(fd_conf[(int)drive].sect_size);//Sector size 00=128 b01=256b 02=512b 03=1024b 07=16 kb	n
    f_out(18);//Numero du dernier secteur de la piste
    f_out(fd_conf[(int)drive].gap);//GAP 0x1B pour 1.44 3,5"
    f_out(0xFF);//DTL Taille du secteur = 255

    wait_int ();
    f_in();
    f_in();
    f_in();
    f_in();
    f_in();
    f_in();
    f_in();
    
//print("fd_c");
    bufferd = (char *)dest;
//print("fd_d");
//binairel(buffer);
//binairel(bufferd);
//binairel(dest);
    fd_copy_buff((char *)buffer,(char *)bufferd);
/*    binaire(f_in());
    binaire(f_in());
    binaire(f_in());
    binaire(f_in());
    binaire(f_in());
    binaire(f_in());
    binaire(f_in());*/

//print("fd_f");
    return 0;
}

void fd_copy_buff(char *p1,char *p2)	//source ->dest
{
    static unsigned int comp = 0;
    for (comp = 0;comp<512;comp++)
    {
	*(p2+comp) = *(p1+comp);
    }
}
