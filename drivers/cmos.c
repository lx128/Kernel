#include <archx86.h>
#include <cmos.h>
#include <cpux86.h>
#include <irq.h>

//char cmos_read(char);
unsigned int CmosReady(void);
void CmosInt(void);
//void cmos_test_rate(void);
//void get_time(struct times *);

void IdtAddIrq(char,int);

static int r1024 = 0;
static unsigned char irate = 0;

//void FdDetect(char);
//void MmuInit(unsigned char);
//extern int allocm1(int,int);

void CmosEnableInt(void)
{
    IntAdd(8,(void*)CmosInt);
    irate = 0;
    outb (0x70,STATUS_B);
    outb (0x71,inb(0x71) | 0x40);
    outb (0x70,STATUS_C);
//    binaire(inb(0x71));
}

void CmosDisableInt(void)
{
    outb (0x70,STATUS_B);
    outb (0x71,inb(0x71) & 0xBF);
}

void CmosInt(void)
{
    irate = 1;
    outb (0x70,STATUS_C);
    inb(0x71);
}

void CmosGetTime(struct times *tm)
{
    CmosReady();
    outb(0x70, 0);
    tm->second = inb (0x71);			//Second

    CmosReady();
    outb(0x70, 0x2);
    tm->minute = inb (0x71);			//Minute

    CmosReady();
    outb(0x70, 0x4);
    tm->hour = inb (0x71);			//Hour

    CmosReady();
    outb(0x70, 0x6);
    tm->day_wk = inb (0x71);			//Day week

    CmosReady();
    outb(0x70, 0x7);
    tm->day = inb (0x71);			//Day

    CmosReady();
    outb(0x70, 0x8);
    tm->month = inb (0x71);			//Month

    CmosReady();
    outb(0x70, 0x9);
    tm->year = inb (0x71);			//Year
}

unsigned int CmosReady(void)
{
    static unsigned int comp = 0;

    outb(0x70, STATUS_A);
    for (comp=0; comp<1000; comp++)
    {
	if (~(inb (0x71))&128)
	{
	    return 255;
	}
    }
    return 0;
}

void CmosTestRate(void)
{
    r1024 = 0;
    irate = 0;
    static unsigned char comp = 0;

    CmosEnableInt();
//    sti;
    for(comp=0;comp<3;comp++)
    {
	while(irate==0)
	{
	}
	irate = 0;
	while(irate==0)
	{
	    r1024++;
	}
	irate = 0;
    }
//    cli;
    r1024 = r1024 /220;		// div per 3 and div 164 for Mhz
    CmosDisableInt();
}

unsigned char CmosRead(char offset)
{
    outb(0x70, offset);			//Byte to read
    return inb (0x71);			//Read it
}

unsigned int CmosRate(void)
{
    return r1024;
}

unsigned int CmosFloppy (void)
{
    return CmosRead (0x10);
}
