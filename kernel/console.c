#include <console.h>
#include <textmode.h>

static uchar c[8000];//2 ecrans de 4000
static uchar c1x,c1y;
static uchar c2x,c2y;
static uchar current;

void ConsoleInit(void)
{
}

void ConsoleSelect(uchar nb)
{
}

static void testscrollup (int n) 
{
    static char* video = 0, *tmp = 0;

    for(video=(char*)RAMSCREEN ; video<(char*)SCREENLIM ; video++)
    {
		tmp = (char*) (video+n*160);

		if(tmp<(char*)SCREENLIM)
			*video = *tmp;
		else
			*video = 0;
    }

    kY-=n;
    if(kY<0)kY=0;
}

void print(uchar* string)
{
    static char* ptr = 0;			/* pointeur sur la chaine de caractere */
    ptr = string;

    while(*ptr!=0)
    {		/* tant que le caractere est different de 0x0 */
    	putcar(*ptr);
    	ptr++;
    }
}

void putcar(uchar c)
{
    static uchar* video = 0;

    if(c==10)
    {	/* CR-NL */
    	kX=0;
    	kY++;
    }
    else
    {		/* autres caracteres */ 
		video = (uchar*) (RAMSCREEN+2*kX+160*kY);
		*video = c;
		*(video+1) = kattr;

		kX++;
		if(kX>79)
		{
			kX = 0;
			kY++;
		}
    }
    if(kY>24)testscrollup(kY-24);
}