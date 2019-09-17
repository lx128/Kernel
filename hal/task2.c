#include "../include/sys.h"
#include "../include/types.h"
#include "../include/task.h"
#include "../include/gdt.h"

#define D_ACCESS_TASK	0x89
#define D_OPTION	0x09

void task_management(void);
void add_tss(short, short, short, int, int, int);
void task_ready(int);
char tss_find ();

static int current = 1, last = 1;

int thread(unsigned int base,unsigned int stackp)
{
    unsigned char desc;
    desc = tss_find ();
    tssmap[desc].gdt_desc = add_gdt_desc(((desc*TSS_SIZE)+TSS_BASE),0xFFFFF, D_ACCESS_TASK, D_OPTION, desc);
    tssmap[desc].time = 0;
    last = desc;
//binaire (desc);
    add_tss(1, 2, 3,base, desc, stackp);
    return desc;
}

int rm_thread(unsigned int sig)
{
    if (last != current)
    {
    tssmap[current].tss = tssmap[last].tss;
    tssmap[current].state = tssmap[last].state;
    tssmap[current].time = tssmap[last].time;
    tssmap[current].gdt_desc = tssmap[last].gdt_desc;
    }
    tssmap[last].tss = 0;
    sti;
    asm ("int $0x20"::);
    return current;
}


/*int load_task(unsigned int base)
{
    unsigned char desc;
    desc = tss_find ();
    tssmap[desc].gdt_desc = add_gdt_desc(((desc*TSS_SIZE)+TSS_BASE),0xFFFFF, D_ACCESS_TASK, D_OPTION, desc);
    add_tss(1, 2, 3,base, desc);
    return desc;
}*/

void kernel_task(unsigned int task)
{
    add_gdt_desc(TSS_BASE,0xFFFFF, D_ACCESS_TASK, D_OPTION, "Kernel");//Kernel TSS descriptor
    add_tss(1, 2, 3,(int)task_management, 0, 0);//kernel task tss must be 0 position!
    tssmap[0].state = 2;

        asm ("	movw $0x20,%ax \n\
		ltr %ax\n");
}

void task_management(void)
{
    static int comp, taskptr[2], time = 0;
    unsigned char tasklist[TSS_NB];

    taskptr[0]=0;

    while (1)
    {
	comp = 1;
	while (tssmap[comp].tss != 0)
	{
	    if (tssmap[comp].time < tssmap[current].time)
	    {
		current = comp;
	    }
	    comp++;
	}
        taskptr[1] = tssmap[current].gdt_desc * 8;

//putcar_drv ('A');
//binaire (current);
//putcar_drv (tasknb2 + 48);
//putcar_drv ((taskptr[1] / 8) + 48);
//while (1);


//taskptr[1]=0x28;
	time ++;
	tssmap[current].time = time;

        __asm__ __volatile__ (
        "ljmp *%0\n"

        "movb $0x20, %%al\n"
        "outb %%al,$0x20\n"
        :
        :"m" (*taskptr));

//print ("Task end\n");
//while(1);

	task_ready(current);
    }
}

void add_tss(short code, short data, short stack, int eeip, int tssptr, int stackp)
{
    struct TSS * tss;

    if (tssptr==TSS_NB+1)
    {
	return;
    }

    (int)tss = (tssptr*TSS_SIZE)+TSS_BASE;

    tss->OldTss = 0;
    tss->esp0 = 0;
    tss->ss0 = 0;
    tss->esp1 = 0;
    tss->ss1 = 0;
    tss->esp2 = 0;
    tss->ss2 = 0;
    tss->cr3 = 0;
    tss->eip = eeip;
    tss->eflags = 0x200;
//    tss->eflags = 0x7202;
    tss->eax = 0;
    tss->ecx = 0;
    tss->edx = 0;
    tss->ebx = 0;

//    tss->esp = 0xFFFF;
//    asm ("	movw %esp,%eax \n\
//	        movw %eax,a");
    tss->esp = (stackp + 0x1FFFF);
    tss->ebp = 0;
    tss->esi = 0;
    tss->edi = 0;
    tss->es = data * 8;
    tss->cs = code * 8;
    tss->ss = stack * 8;
    tss->ds = data * 8;
    tss->fs = data * 8;
    tss->gs = data * 8;
    tss->ldt = 0;
    tss->debugtrap = 0;
    tss->iomapbase = 0;


    tssmap[tssptr].tss = 1;
    tssmap[tssptr].state = 1;
    return;
}

void task_ready(int task)
{
    struct gdtdesc * desc;

    if (task==GDTSIZE+1)
    {
	return;
    }

    (int)desc = tssmap[task].gdt_desc * 8+GDTBASE;
//desc = (task * 8)+GDTBASE;

    desc->acces = D_ACCESS_TASK;

    return;
}

char tss_find ()
{
    unsigned char tasknb;

    for (tasknb = 0; tasknb <= TSS_NB; tasknb++)
    {
        if (tssmap[tasknb].tss == 0)
        {
	    return tasknb;
	}
    }
    return 255;
}

void task_wait()
{
//    tssmap[current].time = tssmap[current].time - 1;
}