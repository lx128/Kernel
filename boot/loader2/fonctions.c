#include <fonctions.h>
#include <consolepc.h>

void hex2 (unsigned char);

void binaire(unsigned char a)
{
    static char comp=0;
    comp = 0;
    putcar (((a & 128) >> 7) + 48);
    while (comp < 7)
    {
	a = a << 1;
	comp++;
	putcar (((a & 128) >> 7) + 48);
    }
    putcar (10);
}

void binairel(unsigned long a)
{
    static char comp=0;
    comp = 0;
    putcar (((a & 0x80000000) >> 31) + 48);
    while (comp < 31)
    {
	a = a << 1;
	comp++;
	putcar (((a & 0x80000000) >> 31) + 48);
	if (comp == 7)
	{
	    putcar(32);
	}
	if (comp == 15)
	{
	    putcar(32);
	}
	if (comp == 23)
	{
	    putcar(32);
	}
    }
    putcar (10);
}

void hex(unsigned char a)
{
    static unsigned char comp=0, b = 0;
    b = 0;

    kprint("0x");
    for (comp=2;comp >0;comp--)
    {
	b = ((a >>((comp-1)*4)) &0xf);
	hex2(b);
    }
    putcar (10);
}

void hexl(unsigned long a)
{
    static unsigned char comp=0, b = 0;
    b = 0;

    kprint("0x");
    for (comp=8;comp >0;comp--)
    {
	b = ((a >>((comp-1)*4)) &0xf);
	hex2(b);
    }
    putcar (10);
}

void hex2 (unsigned char a)
{
    if(a < 10)
    {
	putcar (a + 48);
    }
    else
    {
	putcar (a + 0x37);
    }
}
