#include <textmode.h>
#include <archx86.h>
#include <fonctions.h>
#include <bioscall.h>
#include <cpux86.h>

static char kX = 16; /* position courante du curseur a l'ecran */
static char kY = 16;
static uchar kattr = 0x07; /* attributs video des caracteres a afficher */

/* 
 * 'scrollup' scrolle l'ecran (la console mappee en ram) vers le haut
 * de n lignes (de 0 a 25).
 */

void TextModeInit()
{
	static struct bioscall_reg reg = {0,0,0,0,0,0,0,0x10,0};
	reg.ax=3;	//Mode texte + effacement ecran
				//+128 n'efface pas l'ecran mais sans grand interret car la memoire contient d'autres données que les précédentes
	BiosCall(&reg); //int,*ax,*bx,*cx,*dx,*bp,*si,*di
	kX=kY=0;
}

static void scrollup (int n) 
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
    	TextModePutcar(*ptr);
    	ptr++;
    }
}

void TextModePutcar(uchar c)
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
    if(kY>24)scrollup(kY-24);
}

void TextModeLoad(uchar x, uchar y)
{
    kY=y;
    kX=x;
}

uchar TextModeLoadBuffer(ushort start,uchar* buf, ushort size)
{
	uchar* ptr=0;
	ptr=(uchar *)(RAMSCREEN+start);
	if((int)(ptr+size)<SCREENLIM)
	{
		CpuCopyStringW((uchar*) buf, ptr, size/2);
		return 0;
	}
	return 1;//erreur
}

uchar TextModeSaveBuffer(ushort start,uchar* buf, ushort size)
{
	uchar* ptr=0;
	ptr=(uchar *)(RAMSCREEN+start);
	if((int)(ptr+size)<SCREENLIM)
	{
		CpuCopyStringW(ptr, (uchar*) buf, size/2);
		return 0;
	}
	return 1;//erreur
}
