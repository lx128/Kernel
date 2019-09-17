#include<fonctions.h>
#include<clr.h>
#include<cpux86.h>
#include<textmode.h>

void binaire(uchar a)
{
    static uchar comp=0;
    comp = 0;
    TextModePutcar ((uchar)((a & 128) >> 7) + 48);
    while (comp < 7)
    {
	a = a << 1;
	comp++;
	TextModePutcar (((a & 128) >> 7) + 48);
    }
    TextModePutcar (10);
}

void binairel(ulong a)
{
    static uchar comp=0;
    comp = 0;
    TextModePutcar (((a & 0x80000000) >> 31) + 48);
    while (comp < 31)
    {
	a = a << 1;
	comp++;
	TextModePutcar (((a & 0x80000000) >> 31) + 48);
	if (comp == 7)
	{
	    TextModePutcar(32);
	}
	if (comp == 15)
	{
	    TextModePutcar(32);
	}
	if (comp == 23)
	{
	    TextModePutcar(32);
	}
    }
    TextModePutcar (10);
}

static void hex2 (uchar a)
{
    if(a < 10)
    {
	TextModePutcar (a + 48);
    }
    else
    {
	TextModePutcar (a + 0x37);
    }
}

void hex(uchar a)
{
    static uchar comp=0, b = 0;
    b = 0;

    kprint("0x");
    for (comp=2;comp >0;comp--)
    {
	b = ((a >>((comp-1)*4)) &0xf);
	hex2(b);
    }
    TextModePutcar (10);
}

void hexl(ulong a)
{
    static uchar comp=0, b = 0;
    b = 0;

    kprint("0x");
    for (comp=8;comp >0;comp--)
    {
	b = ((a >>((comp-1)*4)) &0xf);
	hex2(b);
    }
    TextModePutcar (10);
}

void lifo_push(uint liste[], uint *pointeur, uint taille, uint var)//la liste est vide si pointeur = 0
{
	if ((*pointeur)==taille)
	{
		kprint("Lifo_push: Buffer Plein\n");
		hexl(*pointeur);
		hexl(taille);
	}
	else
	{
//hexl(var);
		liste[*pointeur]=var;
		*pointeur = *pointeur + 1;
	}
}

uint lifo_pop(uint liste[], uint *pointeur, uint taille)//la liste est vide si pointeur = 0
{
	if ((*pointeur)==0)
	{
		kprint("Lifo_pop: Buffer vide\n");
		return 0;
	}
	else
	{
		*pointeur = *pointeur - 1;
		return liste[*pointeur];
	}
}
void fifo_push(uint liste[], uint *pointeur, uint *reste_a_lire, uint taille, uint var)//la liste est vide si pointeur = 0
{
	static uint actuel=0;

	actuel=*pointeur+ *reste_a_lire;
	if (actuel>taille)
	{
		actuel-=taille;
	}

	if ((actuel+1)==*pointeur)
	{
		kprint("Fifo_push: Buffer Plein\n");
		hexl(*reste_a_lire);
		hexl(taille);
	}
	else
	{
//hexl(var);
		liste[actuel+1]=var;
		*reste_a_lire += 1;
	}
}

uint fifo_pop(uint liste[], uint *pointeur, uint *reste_a_lire, uint taille)//la liste est vide si pointeur = 0
{
	static uint actuel=0;

	actuel=*pointeur+ *reste_a_lire;
	if (actuel>taille)
	{
		actuel-=taille;
	}

	if (actuel==*pointeur)
	{
		kprint("Fifo_pop: Buffer vide\n");
		return 0;
	}
	else
	{
		*reste_a_lire -= 1;
		return liste[actuel];
	}
}

void printbuff(uchar* buff, uint size)
{
	static uint comp;
	for(comp = 0;comp<512;comp++)//copy to buffer
	{
		TextModePutcar(*(uchar *)(buff+comp));
	}
}

void kprintbuff(uchar * buff, uint size)
{
     static uint comp = 0;
     while (comp < size)
     {
      comp++;
      TextModePutcar (buff[comp]);
      }
      comp = 0;
      TextModePutcar(0xA);
}

/*static bool LibBitContigue(long* add, long start, long size, bool type)//tableau à tester, adresse en bit de début, nombre de bit à tester, valeur à trouver 0 ou 1
{
	static unsigned long comp=0;
	static unsigned char var1=0,var2=0;

	var1=start%sizeof(long);
	var2=sizeof(long)-var1;

	if(var1)
	{
		for(comp=0;comp<var2;comp++)//teste le premier mot
		{
			if(((add[0]>>(var1+comp))&1)!=type)
			{
				return false;
			}
		}
	}
	for(comp=0;comp<size;comp=comp+sizeof(long))//teste le millieu du tableau
	{
//		add[comp]
	}
}

fonction crc(tableau de bits bitString[1..longueur], entier polynome)
{
    shiftRegister := valeur_initiale  // Généralement tous les bits à 0 ou 1
    pour i de 1 à longueur
    {
        si bit de poids fort de shiftRegister xor bitString[i] vaut 1
        {
            // décaler d'1 bit vers la gauche équivaut à multiplier par 2
            shiftRegister := (shiftRegister décalé d'1 bit vers la gauche) xor polynome
        }
        sinon
        {
            shiftRegister := (shiftRegister décalé d'1 bit vers la gauche)
        }
    }
    retourne shiftRegister
}
*/
