#include <archx86.h>
#include <clr.h>
#include <i8042.h>
#include <fonctions.h>
#include <vesa.h>
#include <cpux86.h>
#include <textmode.h>
#include "exemple.h"

void souris_texte(void);
void souris_graph(void);
void clavier(void);
void souris_graph_init(void);

uint exit_flag=1;

void Exemple(void)
{
	VesaInit();
	VesaSetMode(0x115);//800*600*16b
	souris_graph_init();

//partie dispatch
	while(exit_flag)
	{
		if(i8042CheckKeybState())
		{
			clavier();
		}
		if(i8042CheckMouseState())
		{
			souris_graph();
//			souris_texte();
		}
		CpuIdle();//met en pause le cpu jusqu'au prochain evenement.
	}
	TextModeInit();//texte
}

void souris_graph_init()
{
    static char *buffer= (char *)VESA_BANK;//buffer graphique
	static unsigned char bank=0, bank_now=0;
    static int pos=0, buf_pos=0;//buf_pos=position dans le buffer

	for(pos=0;pos<480000;pos++)
	{
		bank=(pos*3)/VESA_BANK_SIZE;
		buf_pos=(pos*3)%VESA_BANK_SIZE;
		if (bank!=bank_now)
		{
			VesaSetBank(bank);
			bank_now=bank;
		}
		if (buf_pos!=VESA_BANK_SIZE)
		{
			buffer[buf_pos] = 0x80;//bleu
			buffer[buf_pos+1] = 0x80;//vert
			buffer[buf_pos+2] = 0x0;//rouge
		}
	}
	VesaSetBank(0);
}
void souris_graph()
{
	//mode video 800x600
	//mode texte 80x25 = 2000 caractères
	static struct mousestate mstate = {0,0,0,0,0,0,0,0,0,0,0};//dernier etat de la souris
    static char *buffer= (char *)VESA_BANK;//buffer graphique
	static unsigned char bank=0, bank_now=0;
    static int pos=0, new_pos=0, xx=0, yy=0, last_xx=0, last_yy=0
	, res_x =800, res_y =600, compx=0, compy=0;//pos=position dans le buffer, xx=pos x, yy= pos y
	static char img_souris[21][12]={{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
									{0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
									{0x00,0xFF,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
									{0x00,0xFF,0xFF,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
									{0x00,0xFF,0xFF,0xFF,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
									{0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x01,0x01,0x01,0x01,0x01},
									{0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x01,0x01,0x01,0x01},
									{0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x01,0x01,0x01},
									{0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x01,0x01},
									{0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x01},
									{0x00,0xFF,0xFF,0xFF,0xff,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x01},
									{0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00},
									{0x00,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0x01,0x01,0x01,0x01},
									{0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x00,0x01,0x01,0x01,0x01},
									{0x00,0xFF,0x00,0x01,0x01,0x00,0xFF,0xFF,0x00,0x01,0x01,0x01},
									{0x00,0x00,0x01,0x01,0x01,0x00,0xFF,0xFF,0x00,0x01,0x01,0x01},
									{0x00,0x01,0x01,0x01,0x01,0x01,0x00,0xFF,0xFF,0x00,0x01,0x01},
									{0x01,0x01,0x01,0x01,0x01,0x01,0x00,0xFF,0xFF,0x00,0x01,0x01},
									{0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0xFF,0xFF,0x00,0x01},
									{0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0xFF,0xFF,0x00,0x01},
									{0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01}};

	i8042GetMouseState(&mstate);//conversion des données du controleur en données perso

//calcul de la position du pointeur après mouvement de la souris
	if(mstate.sx)//x négatif
	{
		xx=xx-(0xFF - mstate.x);
		if (xx < 0)//limite basse boucle
		{
			xx=0;
		}
	}
	else//x positif
	{
		xx=xx+mstate.x;
		if (xx >(res_x-1))//limite haute xx
		{
			xx=(res_x-1);
		}
	}

	if(mstate.sy)//y positif
	{
		yy=yy+(0xFF - mstate.y);
		if (yy > res_y)//limite basse boucle
		{
			yy=(res_y-1);
		}
	}
	else//y negatif
	{
		yy=yy-mstate.y;
		if (yy <0)//limite haute yy
		{
			yy=0;
		}
	}

//effacement de l'ancienne image du pointeur
	for(compy=0;compy<21;compy++)
	{
		for(compx=0;compx<12;compx++)
		{
			if ((last_xx+compx)<res_x && (last_yy+compy)<res_y)//ne pas dépasser la zone d'affichage
			{
				pos = ((last_xx+compx)*3) + ((last_yy+compy)*2400);//3 bits x 800p
				bank=pos/VESA_BANK_SIZE;
				new_pos=pos%VESA_BANK_SIZE;
				if (bank!=bank_now)
				{
					VesaSetBank(bank);
					bank_now=bank;
				}
				if (img_souris[compy][compx]!=0x1)//transparence si 0x1
				{
					buffer[new_pos] = 0x80;//bleu
					buffer[new_pos+1] = 0x80;//vert
					buffer[new_pos+2] = 0;//rouge
				}
			}
		}
	}

//affichage de la nouvelle image du pointeur
	for(compy=0;compy<21;compy++)
	{
		for(compx=0;compx<12;compx++)
		{
			if ((xx+compx)<res_x && (yy+compy)<res_y)//ne pas dépasser la zone d'affichage
			{
				pos = ((xx+compx)*3) + ((yy+compy)*2400);//3 bits x 800p
				bank=pos/VESA_BANK_SIZE;
				new_pos=pos%VESA_BANK_SIZE;
				if (bank!=bank_now)
				{
					VesaSetBank(bank);
					bank_now=bank;
				}
				if (img_souris[compy][compx]!=0x1)//transparence si 0x1
				{
					buffer[new_pos] = img_souris[compy][compx];//bleu
					buffer[new_pos+1] = img_souris[compy][compx];//vert
					buffer[new_pos+2] = img_souris[compy][compx];//rouge
				}
			}
		}
	}
	last_xx=xx;
	last_yy=yy;
}

void souris_texte()
{
	//mode video 800x600
	//mode texte 80x25 = 2000 caractères
	static struct mousestate mstate = {0,0,0,0,0,0,0,0,0,0,0};//dernier etat de la souris
    static char *buffer= (char *)0xb8000;//buffer graphique
//	static char ii=0;//retour clavier
    static int pos=0, xx=0, yy=0, last_xx=0, last_yy=0, last_char=0, last_color=0
	, res_x =80, res_y =25;//pos=position dans le buffer, xx=pos x, yy= pos y

/*	switch (var)
	{
	case 1:
		ii = i8042GetKeybState();
		//a complete
	break;
	case 12:
		i8042GetMouseState(&mstate);//conversion des donnée du controleur en donné perso
	break;
	default:
		return;
	break;
	}*/
	i8042GetMouseState(&mstate);//conversion des donnée du controleur en donné perso
//kprint("1a");

	if(mstate.sx)//x négatif
	{
//kprint("a");
//hex((0xFF - mstate.x));
//kprint("aaa");
		xx=xx-(0xFF - mstate.x);
		if (xx < 0)//limite basse boucle
		{
			xx=0;
		}
	}
	else//x positif
	{
//kprint("b");
		xx=xx+mstate.x;
		if (xx >(res_x-1))//limite haute xx
		{
			xx=(res_x-1);
		}
	}

	if(mstate.sy)//y positif
	{
//binaire(mstate.py);
//hex(mstate.y);
		yy=yy+(0xFF - mstate.y);
		if (yy > res_y)//limite basse boucle
		{
			yy=(res_y-1);
		}
	}
	else//y negatif
	{
//binaire(mstate.py);
		yy=yy-mstate.y;
		if (yy <0)//limite haute yy
		{
			yy=0;
		}
	}
//binaire(mstate.y);
//binaire(yy/10);
	pos = (last_xx*2) + (last_yy*160);
	buffer[pos] = last_char;
	buffer[pos+1] = last_color;

	pos = (xx*2) + (yy*160);
//binaire(pos);
//binairel(xx);
//binairel(yy);
	last_char=buffer[pos];
	last_color=buffer[pos+1];
	buffer[pos] = 0x1;
	buffer[pos+1] = 0xA;
	last_xx=xx;
	last_yy=yy;
}

void clavier(void)
{
	static unsigned char ii = 0;

	ii = i8042GetKeybState();

	ii--;
	if(ii<0x80){
		TextModePutcar(kbdmap[ii*4]);
		if(ii==15)
		{
			exit_flag=0;
		}
	}
}
