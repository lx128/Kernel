
#include<clr.h>

#define GDTBASE	0x1800	/*GDT physic Address*/
#define GDTSIZE	0xFF	/*max. descriptors number in table*/
#define STACKSIZE 0x3FFF/*max. descriptors number in table*/

#define D_DATA_RO 0x10
#define D_DATA_RO_A 0x11
#define D_DATA_RW 0x12
#define D_DATA_RW_A 0x13	/*For DATA A=ACCESSED*/
#define D_DATA_RO_EX 0x14
#define D_DATA_RO_A_EX 0x15
#define D_DATA_RW_EX 0x16
#define D_DATA_RW_A_EX 0x17	/*For STACK EX=EXPEND DOWN*/

#define D_CODE_EO 0x18
#define D_CODE_EO_A 0x19
#define D_CODE_ER 0x1A
#define D_CODE_ER_A 0x1B	/*For CODE*/
#define D_CODE_EO_CONF_EX 0x1C	/*CONF = CONFORM*/
#define D_CODE_EO_CONF_A_EX 0x1D
#define D_CODE_ER_CONF_EX 0x1E
#define D_CODE_ER_CONF_A_EX 0x1F

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

#define D_OTHER 0x6

/*segment descriptor*/
struct gdtdesc
{
	ushort lim0_15;
	ushort base0_15;
	uchar base16_23;

	uchar type : 5;
	uchar dpl : 2;	//0=kernel 1,2,3=user
	uchar present : 1;	//0=not 1=present

	uchar lim16_19 : 4;
	uchar aviable : 1;	//Aviable for os
	uchar other : 3;

	uchar base24_31;
} __attribute__ ((packed));//evite que la taille de la structure soit plus grand
			//que la taille theorique a cause du paddling
			//paddling: octet en plus pour compenser l'erreur de taille

/*GDTR register*/
struct gdtr {
	ushort limite;
	ulong base;
} __attribute__ ((packed));
