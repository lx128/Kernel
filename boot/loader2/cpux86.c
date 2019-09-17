int CpuCopyStringW(void * str1, void * str2, int comp)
{
    comp = comp /2;
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
		:"=g" (comp):"g" (comp), "g" (str1), "g" (str2));
    return comp;
}

int CpuCmpStringC(void * str1, void * str2, int comp)
{
    asm (	"push %%ecx \n"
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
		:"=g" (comp):"g" (comp), "g" (str1), "g" (str2));
    return comp;
}

void CpuIdle(void)
{
	asm("hlt");//met en pause le cpu jusqu'au prochain evenement.
}
