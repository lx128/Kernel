#include <archx86.h>
#include <textmode.h>
#include <fonctions.h>

/* 
 * 'scrollup' scrolle l'ecran (la console mappee en ram) vers le haut
 * de n lignes (de 0 a 25).
 */

static char kX = 16; /* position courante du curseur a l'ecran */
static char kY = 16;
static uchar kattr = 0x07; /* attributs video des caracteres a afficher */

static void scrollup (unsigned int n) 
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
    if(kY<0) 
    kY=0;
}

/*
 * 'print' affiche a l'ecran, a la position courante du curseur, une chaine
 * de caracteres terminee par \0.
 */
void kprint (char* string)
{
    static char* ptr = 0;			/* pointeur sur la chaine de caractere */
    ptr = string;

    while(*ptr!=0)
    {		/* tant que le caractere est different de 0x0 */
    	putcar(*ptr);
    	ptr++;
    }
}

void putcar(unsigned char c)
{
    static char* video = 0;

    if(c==10)
    {	/* CR-NL */
    	kX=0;
    	kY++;
    }
    else
    {		/* autres caracteres */ 
	video = (char*) (RAMSCREEN+2*kX+160*kY);
	*video = c;
	*(video+1) = kattr;

	kX++;
	if(kX>79)
	{
		kX = 0;
		kY++;
	}
    }
    if(kY>24)
	scrollup(kY-24);
}

void init_text(void)
{
//     while(1);
    kY=*(char*)(CONF + 0x8);
    kX=*(char*)(CONF + 0x9);
}

void SetTextConfig(void)
{
    *(char*)(CONF + 0x8)=kY;
    *(char*)(CONF + 0x9)=kX;
}
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
