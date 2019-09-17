#include <archx86.h>
#include <cpux86.h>

uchar state = 0;

void CpuPagingOn(uint pde)
{
    static uint ccr0 = 0;
    static struct cr0 *ccr0p = 0;

    pde = pde & 0xfffff000;

    asm ("movl %%eax,%%cr3"::"a" (pde));
    asm ("movl %%cr0,%%eax":"=a" (ccr0));

    ccr0p = (struct cr0 *)&ccr0;
    ccr0p->cd = 1;
    ccr0p->pg = 1;

    asm ("movl %%eax,%%cr0"::"a" (ccr0));
}

void CpuPagingOff(void)
{
    static uint ccr0 = 0;
    static struct cr0 *ccr0p = 0;

    asm ("movl %%cr0,%%eax":"=a" (ccr0));

    ccr0p = (struct cr0 *)&ccr0;
    ccr0p->cd = 0;
    ccr0p->pg = 0;

    asm ("movl %%eax,%%cr0"::"a" (ccr0));
}

uint CpuCmpStringC(void * str1, void * str2, uint comp)
{
    comp++;
    asm (
        "push %%ecx \n"
		"push %%esi \n"
		"push %%edi \n"

		"mov %1,%%ecx \n"
		"mov %2,%%esi \n"
		"mov %3,%%edi \n"

		"rep \n"
		"cmpsb \n"
		"mov %%ecx, %0 \n"

	    "pop %%edi \n"
		"pop %%esi \n"
		"pop %%ecx \n"
		:"=m" (comp):"m" (comp), "m" (str1), "m" (str2));
    return comp;
}

uint CpuCmpStringW(void * str1, void * str2, uint comp)
{
    comp++;
    asm (
		"push %%ecx \n"
		"push %%esi \n"
		"push %%edi \n"

		"mov %1,%%ecx \n"
		"mov %2,%%esi \n"
		"mov %3,%%edi \n"

		"rep \n"
		"cmpsw \n"
		"mov %%ecx, %0 \n"

		"pop %%edi \n"
		"pop %%esi \n"
	    "pop %%ecx \n"
		:"=m" (comp):"m" (comp), "m" (str1), "m" (str2));
    return comp;
}

uint CpuCopyStringC(void * str1, void * str2, uint comp)
{
    asm (
		"push %%ecx \n"
		"push %%esi \n"
		"push %%edi \n"

		"mov %1,%%ecx \n"
		"mov %2,%%esi \n"
		"mov %3,%%edi \n"

		"rep \n"
		"movsb \n"
		"mov %%ecx, %0 \n"

		"pop %%edi \n"
		"pop %%esi \n"
	    "pop %%ecx \n"
		:"=m" (comp):"m" (comp), "m" (str1), "m" (str2));
    return comp;
}
/*source, dest, nb of word (char x 2 = word)*/
uint CpuCopyStringW(void * str1, void * str2, uint comp)
{
//hexl((int)str1);
//hexl((int)str2);
//hexl((int)comp);
//while(1);//0x4026b2
    asm (
		"push %%ecx \n"
		"push %%esi \n"
		"push %%edi \n"

		"mov %1,%%ecx \n"
		"mov %2,%%esi \n"
		"mov %3,%%edi \n"

		"rep \n"
		"movsw \n"
		"mov %%ecx, %0 \n"

		"pop %%edi \n"
		"pop %%esi \n"
	    "pop %%ecx \n"
		:"=m" (comp):"m" (comp), "m" (str1), "m" (str2));
    return comp;
}

void CpuSuspendIrq(void)
{
	asm("cli"::);
}

void CpuRestaureIrq(void)
{
	if (state & 0x1)
	{
		asm("sti"::);
	}
	else
	{
		asm("cli"::);
	}
}

void CpuIrqOff(void)
{
	asm("cli"::);
	state = state & 0xFE;
}

void CpuIrqOn(void)
{
	asm("sti"::);
	state = state | 0x1;
}

void CpuId(void)
{
    uint eax,ebx,ecx,edx,op=0;
    asm ("cpuid"
                : "=a" (eax),
                  "=b" (ebx),
                  "=c" (ecx),
                  "=d" (edx)
                : "a" (op));
}
