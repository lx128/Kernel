#include <gdt.h>
#include <archx86.h>

static int find_free_gdt_desc(void);
//void binaire(char);

/*Add a descriptor in GDT table*/
int GdtAddDesc(long base, long limite, char acces, char dpl, char other)
{
    static struct gdtdesc *desc = 0;
    static unsigned int gdtptr = 0;

    gdtptr = find_free_gdt_desc();
//binaire (gdtptr);
//print ("OK\n");
        desc = (struct gdtdesc *)(gdtptr*8+GDTBASE);

        desc->lim0_15 = (limite & 0xffff);
        desc->base0_15 = (base & 0xffff);
        desc->base16_23 = (base & 0xff0000)>>16;
        desc->type = acces;
        desc->dpl = dpl;
        desc->present = 1;
        desc->lim16_19 = (limite & 0xf0000)>>16;
        desc->aviable = 0;
        desc->other = other;
        desc->base24_31 = (base & 0xff000000)>>24;

        return gdtptr;
}

/*Free a descriptor*/
/*void free_gdt_desc(char desc)
{
    gdtmap[desc]=0;
}*/

/*Find a free descriptor*/
static int find_free_gdt_desc(void)
{
    static char *ptr = (char*)GDTBASE;
    static int comp = 13;

    for (comp=13; comp<(GDTSIZE*8);comp=comp+8)
    {
	if (ptr[comp]==0){return (comp/8);}
    }
    return 0;
}

/*Init kernel gdt*/
void GdtInit(void)
{
    static struct gdtr kgdtr = {0,0};
//init_gdt_desc(Base, Limit, Type, CPL Level);

    //Empty descriptor segment
    GdtAddDesc(0x0, 0xFFFFF, D_CODE_ER_A, 0, D_OTHER_32_G);//code segment descriptor
    GdtAddDesc(0x0, 0xFFFFF, D_DATA_RW, 0, D_OTHER_32_G);//data segment descriptor
    GdtAddDesc(0x0, (STACK_32/0x1000), D_DATA_RW_A_EX, 0, D_OTHER_32_G);//stack segment descriptor
    GdtAddDesc(0x0, 0xFFFF, D_CODE_ER_CONF_EX, 0, D_OTHER_16);//Code Real mode
    GdtAddDesc(0x0, 0xFFFF, D_DATA_RW, 0, D_OTHER_16);//Data Real mode

/*init GDTR structure*/
    kgdtr.limite = GDTSIZE*8;//on initialise 8 descripteurs taille de la GDT=255*8=2040
    kgdtr.base = GDTBASE;//addresse de base ou commance la GDT=0x800 = 2048
//while (1);

/*load GDTR register*/
    asm("lgdtl %0":: "m" (kgdtr));

/*segments initialisation*/
    asm("	movw $0x10,%ax	\n \
		movw %ax, %ds	\n \
		movw %ax, %es	\n \
		movw %ax, %fs	\n \
		movw %ax, %gs	\n \
		movw $0x18,%ax	\n \
		movw %ax, %ss	\n \
		nop	\n \
		nop	\n \
		ljmp $0x08,$next	\n \
		next:	\n");
}
