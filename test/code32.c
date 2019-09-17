#include "../../include/idt.h"
#include "../../include/gdt.h"

void start(void);
//void print(char *);

void start(void)
{
    static struct idtr kidtr = {0,0};
    static int taskptr[2]={0,0}, sbp=0, ssp=0, reg[5]= {0,0,0,0,0};
//Pointeur vers section 16 bits
    taskptr[0]=0xE;
    taskptr[1]=0x500;

//Sauve registre SP BP
     asm ("mov %esp,%eax");
     asm ("mov %ebp,%ebx");
     asm ("mov %%eax, %0"::"g"(ssp));
     asm ("mov %%ebx, %0"::"g"(sbp));

/*Mask interrupt*/
     asm ("cli");

/*Disable paging*//*To do*/

/*Load IDT in real mode*/
      kidtr.limite = 0x3ff;
      kidtr.base = 0x0;
      asm("lidtl %0"::"g"(kidtr));//Idt en mode BIOS

/*Save CS DS ES FS GS*/
     asm ("mov %cs,%eax");
     asm ("mov %ds,%ebx");
     asm ("mov %%eax, %0"::"g"(reg[0]));
     asm ("mov %%ebx, %0"::"g"(reg[1]));
     asm ("mov %es,%eax");
     asm ("mov %fs,%ebx");
     asm ("mov %%eax, %0"::"g"(reg[2]));
     asm ("mov %%ebx, %0"::"g"(reg[3]));
     asm ("mov %gs,%eax");
     asm ("mov %%eax, %0"::"g"(reg[4]));

/*Load new DS ES FS GS*/
    asm("	movw $0x30,%ax	\n \
		movw %ax, %ds	\n \
		movw %ax, %es	\n \
		movw %ax, %fs	\n \
		movw %ax, %gs	\n");

//Passage en mode 16 bits
     asm ("ljmp	0x28, $switch_to_real16");
     asm ("switch_to_real16:");
     asm ("mov %cr0,%eax");
     asm ("and $0xFFFFFFFE,%eax");
     asm ("mov %eax,%cr0");
     asm ("ljmp *%0"::"g" (*taskptr));   //0x603f
while(1);
                //longueur de l'instruction ljmp: 6 octets
//Restaure registre SP BP
     asm ("mov %0, %%eax"::"g"(ssp));
     asm ("mov %0, %%ebx"::"g"(sbp));
     asm ("mov %eax,%esp");
     asm ("mov %ebx,%ebp");

//Idt en mode 32 bits
//    kidtr.limite = IDTSIZE*8;
//    kidtr.base = IDTBASE;
//      asm("lidtl %0"::"g"(kidtr));

     return;
}
