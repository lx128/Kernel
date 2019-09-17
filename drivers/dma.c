#include <archx86.h>
#include <dma.h>

static uint dmamap = 0;

void DmaInit(void)
{
}

void DmaOn(uchar channel,uint buffer,uint size)
{
    cli;

    dmamap = dmamap | (1<<channel);
/*    dmaoutb (0x70,0x10);
    dmaoutb (0xa,0x6);
    dmaoutb (0xc,0x0);
    dmaoutb (0x4,(BUFFER)&0xFF);
    dmaoutb (0x4,(BUFFER>>8)&0xFF);
    dmaoutb (0xc,0x0);
    dmaoutb (0x5,(SIZE)&0xFF);
    dmaoutb (0x5,(SIZE>>8)&0xFF);
    dmaoutb (0xb,0x46);
    dmaoutb (0x81,0x0);
    dmaoutb (0xa,0x2);*/

//    print ("DMA 2");
    dmaoutb(13,1);//reset
    dmaoutb(10,4|channel);//mask dma channel

//    dmaoutb(11,64|4|2);//Registre Mode. IO to mem, auto init.
    __asm__("outb %%al,$12\n\tjmp 1f\n1:\tjmp 1f\n1:\t"
    "outb %%al,$11\n\tjmp 1f\n1:\tjmp 1f\n1:"::
    "a" ((uchar) (0x46)));
    //0x46 = 70 = 01000110
    dmaoutb(0xC,0);//Flip Flop = 0
    dmaoutb(4,(buffer)&0xFF);//Poid faible adresse debut
    dmaoutb(4,(buffer>>8)&0xFF);//Poid fort adresse debut
    dmaoutb(0x81,(buffer>>16)&0xFF);//page de memoire
    dmaoutb(5,(size)&0xFF);//Poid faible longueur -1
    dmaoutb(5,(size>>8)&0xFF);//Poid fort longueur -1
    dmaoutb(10,0|channel);//Demasque dma 2

/*
    outb(10,4|2);//masque dma 2

    outb(11,64|4|2);//Registre Mode. IO to mem, auto init.
    __asm__("outb %%al,$12\n\tjmp 1f\n1:\tjmp 1f\n1:\t"
    "outb %%al,$11\n\tjmp 1f\n1:\tjmp 1f\n1:"::
    "a" ((char) (0x46)));
    //0x46 = 70 = 01000110
    outb(0x81,(BUFFER>>16)&0xFF);//page de memoire
    outb(0xC,0);//Flip Flop = 0
    outb(4,(BUFFER)&0xFF);//Poid faible adresse debut
    outb(4,(BUFFER>>8)&0xFF);//Poid fort adresse debut
    outb(5,(SIZE)&0xFF);//Poid faible longueur -1
    outb(5,(SIZE>>8)&0xFF);//Poid fort longueur -1
    outb(10,0|2);//Demasque dma 2*/
    sti;
}

void DmaOff(uchar channel)
{
    dmamap = dmamap ^ (1<<channel);
}
