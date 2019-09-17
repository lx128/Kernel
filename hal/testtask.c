#include "../include/sys.h"

void task1(void)
{
    char * buf;
    int comp=0;
    (int)buf = 56320;

    locate(30,3);
    readfl(buf,0);

    while (comp<512)
    {
	if (*(buf+comp)==0)
	{
	    *(buf+comp)=32;
	}
	comp++;
    }
    *(buf+512)=0;

    printf(buf);

    readfl(buf,0);

    while (comp<512)
    {
	if (*(buf+comp)==0)
	{
	    *(buf+comp)=32;
	}
	comp++;
    }
    *(buf+512)=0;

    printf(buf);
    exit();
}


void task2(void)
{
    static int a=0,a2=0;
    static char * screen;
    (int)screen = 0xB80A0;

    a:
//    lputcar (97+a2+a);
    *(screen + a*2) = (97+a2+a);
    *(screen + a*2 +1) = 5;
    a++;
    if (a>=20)
    {
	a=0;
	a2++;
	if (a2==10){a2=0;}
//	locate (0,2);
//    exit();
    }
//    printf ("Et oui!");
    goto a;
}