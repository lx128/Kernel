#include<clr.h>

uint SysCall(uint rax, uint rbx, uint rcx, uint rdx, uint rsi, uint rdi)
{
	asm(
	"push %%eax\n\
	push %%ebx\n\
	push %%ecx\n\
	push %%edx\n\
	push %%esi\n\
	push %%edi\n\
	mov %0, %%eax\n\
	mov %1,%%ebx\n\
	mov %2,%%ecx\n\
	mov %3,%%edx\n\
	mov %4,%%esi\n\
	mov %5,%%edi\n\
	int $0x80\n"
	::"m" (rax),"m" (rbx),"m" (rcx),"m" (rdx),"m" (rsi),"m" (rdi));

	asm("mov %%eax, %0":"=m" (rax));

/*    "push %%eax\n\
	push %%ebx\n\
	push %%ecx\n\
	push %%edx\n\
	mov %1, %%eax\n\
	mov %2,%%ebx\n\
	mov %3,%%ecx\n\
	mov %4,%%edx\n\
	int $0x80\n"

	:"=eax" (rax)
	:"m" (rax),"m" (rbx),"m" (rcx),"m" (rdx));*/

	asm(
	"pop %edi\n\
	pop %esi\n\
	pop %edx\n\
	pop %ecx\n\
	pop %ebx\n\
	pop %eax\n"
	);
	return rax;
}
