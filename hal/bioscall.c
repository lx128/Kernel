#include <bioscall.h>
#include <archx86.h>
#include <cpux86.h>
#include <pic.h>
#include <fonctions.h>

static void (*bios_call_f)(void) = 0;
static ushort* Registers = 0;

void BiosCallInit(void* bioscall)
{
	static uint * fonction = 0;

	fonction = (uint *)(BIOSCALL+BIOSCALL_START);
	bios_call_f = (void (*)(void))fonction;

	Registers = (ushort *)bioscall;
}

void BiosCall(struct bioscall_reg *reg)
{
//kprint("BiosCall\n");
	CpuSuspendIrq();
	PicInit(0);
	Registers[0]= reg->ax;
	Registers[1]= reg->bx;
	Registers[2]= reg->cx;
	Registers[3]= reg->dx;
	Registers[4]= reg->si;
	Registers[5]= reg->di;
	Registers[6]= reg->bp;
	Registers[7]= reg->int86;
//while (1);
	(*bios_call_f)(); //Jump to BIOSCALL 
	PicInit(1);
	reg->ax = Registers[0];
	reg->bx = Registers[1];
	reg->cx = Registers[2];
	reg->dx = Registers[3];
	reg->si = Registers[4];
	reg->di = Registers[5];
	reg->bp = Registers[6];
	reg->flag = Registers[8];
	CpuRestaureIrq();
//     sti;
//kprint("Int86 Exit\n");
}

/*void int86_buff(struct int86_reg *reg, char *buff, int size)
{
     if (size > 0x1000)
     {
              kprint ("**Stop**\nBuffer too big\n");
              while (1);
     }
     cpu_copy_string_c(buff, (char*)CONF, size);
     int86 (reg);
}*/
