#include <archx86.h>
#include <fonctions.h>
#include <syscall.h>

static uint sysmap[SYS_SIZE] = {0};

ushort SysCallAdd (uint driver)
{
    static uint comp = 0;

    for (comp=0;comp<SYS_SIZE;comp++)
    {
        if (sysmap [comp]==0)
        {
			sysmap [comp]=driver;
			return comp;
		}
    }
    return 0;
}

void SysCallD(void)
{
	static uint rax = 0,rbx = 0,rcx = 0,rdx = 0,rsi = 0,rdi = 0;
	static uint (*pdriver) (uint,uint,uint,uint,uint,uint) = 0;
	static struct SysCallStruct* scs=(struct SysCallStruct*)&rax;

//	cli;
//	asm("nop":"=eax" (rax), "=ebx" (rbx),"=ecx" (rcx),"=edx" (rdx));
	asm ("movl %%eax, %0;" : "=r" ( rax ));
	asm ("movl %%ebx, %0;" : "=r" ( rbx ));
	asm ("movl %%ecx, %0;" : "=r" ( rcx ));
	asm ("movl %%edx, %0;" : "=r" ( rdx ));
	asm ("movl %%esi, %0;" : "=r" ( rsi ));
	asm ("movl %%edi, %0;" : "=r" ( rdi ));
/*
kprint("Syscall: Call\n");
hexl (rax);
hexl (rbx);
hexl (rcx);
hexl (rdx);
*/
	pdriver=(uint (*)(uint, uint, uint, uint, uint, uint))sysmap[scs->manager];
	if (pdriver)
	{
	//	*rax = (*pdriver) ((int) rax, (int) rbx, (int) rcx, (int) rdx);
		rax = (*pdriver) (scs->fuction,rbx,rcx,rdx,rsi,rdi);
	}
	else
	{
		rax=0;
		//kprint("Syscall: Unknow Syscall\n");
	}

//	return rax;
	asm ("movl %0, %%eax;" : "=r" ( rax ));
//	sti;
//	asm ("int $0x20"::);
}
