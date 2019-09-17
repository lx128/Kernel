#include <archx86.h>
#include <cpux86.h>
#include <fonctions.h>

void init_text(void);

void SetTextConfig(void);

char CmosRead(char);
void exec(long);

void start(void)
{
	static unsigned long mem=0, size=0;
	static unsigned long *map = 0;

	init_text();
	kprint("Loading step 3 succeed!\n");

/*Test memory size*/
	mem = (CmosRead(8)>>2)+1;	//Limited to 64Mb
	if ((mem < 8) && (mem != 0))
	{
		kprint("This system haven't enough memory\n");
		kprint("It must have 8 Mb and it have ");
		putcar(mem+48);
		kprint(" Mb\n");
		while (1){CpuIdle();};
	}
	SetTextConfig();

//Add Int86 registers area for BiosCall
	map = (unsigned long *)(INDEX);
	size = map[3];
	mem = map [1] + map [2] + CODE_START;

	CpuCopyStringW((char*)mem,(char*)INT86_R,(int)(size+1));

//Charger loader3 et execution
	mem = mem + map [3];
	exec((long)mem);

	while (1){CpuIdle();}; //A Loader can't exit !
}
