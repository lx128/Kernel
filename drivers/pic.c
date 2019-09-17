/*
i8259a twin Driver
Support Master & Slave
Support real & protected mode
*/
#include <archx86.h>
#include <cpux86.h>

static uchar state1=0xfb, state2=0xff;

void PicInit(uchar mode)// 0 = reel; 1 = protected
{
/* MASTER Ne rien Toucher ! */
    /* Initialisation de ICW1 Ne pas Toucher = 0x11 */
    outbp(0x20,0x11);

    /* Initialisation de ICW2 - vecteur d'interruption*/
    /*Vecteur d'interruption mode reel = 0x8 */
    /*Vecteur d'interruption mode protégé = 0x20 */
    if (mode == 0)
    {
       outbp(0x21,0x8);
    }
    else if (mode == 1)
    {
       outbp(0x21,0x20);
    }
    /* Initialisation de ICW3 Déclare le controleur slave = 0x4 */
    outbp(0x21,0x04);

    /* Initialisation de ICW4 = 0x1 :0x1=no buffered 0x9=buffered*/
//    outbp(0x21,0x01);
    outbp(0x21,0x9);

    /* masquage des interruptions sauf la No 2 pour activé le 2e PIC*/
//    outbp(0x21,0xFb);
    if (mode == 0)
    {
    outbp(0x21,0x0);
    }
    else if (mode == 1)
    {
    outbp(0x21,state1);
    }

/* SLAVE Ne rien Toucher ! */
    /* Initialisation de ICW1 Ne pas Toucher = 0x11 */
    outbp(0xA0,0x11);

    /* Initialisation de ICW2 - vecteur d'interruption*/
    /*Vecteur d'interruption mode reel = 0x70 */
    /*Vecteur d'interruption mode protégé = 0x70 */
    outbp(0xA1,0x70);

    /* Initialisation de ICW3 Déclare le controleur slave = 0x2 */
    outbp(0xA1,0x02);

    /* Initialisation de ICW4 = 0x1: 0x1=no buffered 0x9=buffered*/
//    outbp(0xA1,0x01);
    outbp(0xA1,0x9);

    /* masquage des interruptions */
//    outbp(0xA1,0xFF);
    if (mode == 0)
    {
    outbp(0xA1,0x0);
    }
    else if (mode == 1)
    {
    outbp(0xA1,state2);
    }
//    state1=0xfb;
//    state2=0xff;
    /* Demasquage des irqs */
//    outbp(0x21,0x0);	//demask all
//    outbp(0x21,0x1);	//normal

//    outbp(0xA1,0x0);	//demask all
//    outbp(0xA1,0xFF);	//normal
//kprint("End init\n");
}

uint PicIrqOn(uchar irq)
{
     if (irq < 8)
     {
             state1=state1 & (0xff-(0x1<<irq));
             outbp(0x21,state1);	//normal
//kprint("Pic1 ");
//binaire(state1);
//while(1);
             return irq;
     }
     else if (irq<16)
     {
             state2=state2 & (0xff-(0x1<< (irq-8)));
             outbp(0xA1,state2);	//demask all
//kprint("Pic2 ");
//binaire(state2);
             return irq;
      }
      return 0;
}

uint PicIrqOff(uchar irq)
{
//kprint("Pic3 ");
     if (irq < 8)
     {
             state1=state1 | (0x1<<irq);
             outbp(0x21,state1);	//normal
//binaire(state1);
//binaire((0x1<<irq));
             return irq;
     }
     else if ((7<irq)&(irq<16))
     {
             state2=state2 | (0x1<< (irq-8));
             outbp(0xA1,state2);	//demask all
//kprint("Pic4 ");
//binaire(state2);
             return irq;
      }
      return 0;
}
