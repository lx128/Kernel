#ifndef TEXTMODE_H
#define TEXTMODE_H

#include<clr.h>

#define RAMSCREEN 0xB8000	/* debut de la memoire video */
#define SIZESCREEN 0xFA0 	/* 4000, nombres d'octets d'une page texte */
#define SCREENLIM 0xB8FA0	/* fin de la memoire video */

void TextModeLoad(uchar, uchar);
void kprint (char*);
void TextModePutcar(uchar);
void TextModeInit(void);
uchar TextModeLoadBuffer(ushort, uchar*, ushort);
uchar TextModeSaveBuffer(ushort, uchar*, ushort);

#endif
