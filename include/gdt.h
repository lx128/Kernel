#ifndef GDT_H
#define GDT_H

#include<clr.h>

#define GDTBASE	0x1800	/*GDT physic Address*/
#define GDTSIZE	0xFF	/*max. descriptors number in table*/
#define STACKSIZE 0x3FFF/*max. descriptors number in table*/

/***********************System desc*/
//0x0 reserved
#define D_TSS16_A 0x1		/*A=AVIABLE*/
#define D_LDT 0x2
#define D_TSS16_B 0x3
#define D_CALL16 0x4
#define D_TASK 0x5
#define D_INT16 0x6
#define D_TRAP16 0x7
//0x8 reserved
#define D_TSS32_A 0x9		/*A=AVIABLE*/
//0xA reserved
#define D_TSS32_B 0xB
#define D_CALL32 0xC
//0xD reserved
#define D_INT32 0xE
#define D_TRAP32 0xF
/***********************Data desc*/
#define D_DATA_RO 0x10
#define D_DATA_RO_A 0x11
#define D_DATA_RW 0x12
#define D_DATA_RW_A 0x13	/*For DATA A=ACCESSED*/
#define D_DATA_RO_EX 0x14
#define D_DATA_RO_A_EX 0x15
#define D_DATA_RW_EX 0x16
#define D_DATA_RW_A_EX 0x17	/*For STACK EX=EXPEND DOWN*/

#define D_CODE_EO 0x18      /*EO = Execute Only*/
#define D_CODE_EO_A 0x19
#define D_CODE_ER 0x1A      /*ER = Execute and Read **For CODE**/
#define D_CODE_ER_A 0x1B
#define D_CODE_EO_CONF_EX 0x1C	/*CONF = CONFORM*/
#define D_CODE_EO_CONF_A_EX 0x1D
#define D_CODE_ER_CONF_EX 0x1E
#define D_CODE_ER_CONF_A_EX 0x1F

/*Other param aviable*/
#define D_OTHER_16 0x0    //16bits
#define D_OTHER_32 0x2    //32bits
#define D_OTHER_16_G 0x4    //16bits granularity 4kb
#define D_OTHER_32_G 0x6    //32bits granularity 4kb

/*segment descriptor*/
struct gdtdesc
{
    short lim0_15;
    short base0_15;
    char base16_23;

    char type : 5;
    char dpl : 2;	//0=kernel 1,2,3=user
    char present : 1;	//0=not 1=present

    char lim16_19 : 4;
    char aviable : 1;	//Aviable for os
    char other : 3;

    char base24_31;
} __attribute__ ((packed));//evite que la taille de la structure soit plus grand
			//que la taille theorique a cause du paddling
			//paddling: octet en plus pour compenser l'erreur de taille

/*segment descriptor 32*/
struct gdtdesc32
{
    short lim0_15;
    short base0_15;
    char base16_23;


	char access : 1;//cpu le met ) 1 si utilisé
	char rw : 1;//segment rw
    char dc : 1;//0=execution que dans son niveau de privilege 0=execution dans niveau supérieur autorisé
				//exemple= si dpl=3 et dc=0, un jump depuis le niveau 0 vers ce bloc est interdit.
    char execute_byte : 1;//nx bit
    char always_1 : 1;
    char dpl : 2;	//0=kernel 1,2,3=user
    char present : 1;	//0=not 1=present


    char lim16_19 : 4;
    char aviable : 1;	//Aviable for os
    char other : 3;

    char base24_31;
} __attribute__ ((packed));

/*GDTR register*/
struct gdtr {
    short limite;
    long base;
} __attribute__ ((packed));

void GdtInit(void);

#endif
