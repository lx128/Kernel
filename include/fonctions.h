#ifndef FONCTIONS_H
#define FONCTIONS_H

#include<clr.h>

#define true 1
#define false 0

void putcar(uchar); 
void kprint(char *);
void binaire(uchar);
void binairel(ulong);
void hex(uchar);
void hexl(ulong);
void kprintbuff(uchar *,uint);
uchar getchar(void);
void printbuff(uchar*, uint);
void end(void);

void lifo_push(uint *, uint *, uint, uint);
uint lifo_pop(uint *, uint *, uint);

struct fifo
{
	uint granul;
	uchar* fenetre1;
	uchar* fenetre2;
	ushort hor;
	ushort ver;
	uchar bits;
	uchar nbpage;
};

#endif
