#ifndef VESA_H
#define VESA_H

#include<clr.h>

#define VESA_BANK    0xA0000
#define VESA_BANK_SIZE 0x10000

uint VesaInit(void);
uint VesaSetMode(ushort);
void VesaSetBank(uchar);

struct vesa_current
{
       uint granul;
       uchar* fenetre1;
       uchar* fenetre2;
       ushort hor;
       ushort ver;
       uchar bits;
       uchar nbpage;
} __attribute__ ((packed));

struct info1       //la structure est incomplete
{
       uchar signature[4];
       ushort version;
       uchar * fabricant;
       uint perf;
       ushort* mode;
       ushort nb_page;//nombre de page video
} __attribute__ ((packed));

struct info2       //la structure est incomplete
{
       ushort info;
       uchar fenetre1;
       uchar fenetre2;
       ushort granul;
       ushort taille2fenetres;
       ushort addf1;
       ushort addf2;
       uint fenetrage;
       ushort byteperline;

       ushort hor;
       ushort ver;
       uchar largeur_matrice;
       uchar hauteur_matrice;
       uchar nbplanmemoire;
       uchar bitsparpoint;
       uchar nbpage;
       uchar modelemem;
       uchar taillebloc;
       uchar nbpageimage;
       uchar reserved;

} __attribute__ ((packed));

#endif
