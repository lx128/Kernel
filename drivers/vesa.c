#include <archx86.h>
#include <cpux86.h>
#include <vesa.h>
#include <fonctions.h>
#include <bioscall.h>

static short int *comp = 0, color = 0xF000, page = 0;
static struct bioscall_reg reg = {0,0,0,0,0,0,0,0x10,0};
static struct vesa_current config = {0,0,0};

//static struct info1 binfo1 = {{0},0,0,0,0,0};

uint status(struct bioscall_reg* reg)
{
	if ((reg->ax&0xff)!=0x4F)
	{
		kprint("Vesa.c Unsupported function in ");
		return 0;
	}
	if ((reg->ax&0xff00)==0x100)
	{
		kprint("Vesa.c Function call unsuccefull in ");
		return 0;
	}
	return 1;
}

uint vesa(void)
{
//static char *buffinfo = (char*)CONF;
//while(1);
	VesaInit();
	VesaSetMode(0x111);//640*480*16b

	comp=(short*)config.fenetre1;

	while(1)
	{
		comp++;
//        color++;
		if (comp >= (short*)(config.fenetre1 + config.granul))
		{
		comp=(short*)config.fenetre1;

		page++;
//getchar();
		if (page >config.nbpage)
		{
		page =0;
		color=color+0x1;
		}
		reg.ax=0x4f05;
		reg.bx=0;
		reg.dx=page;
		BiosCall(&reg);
	if (!status(&reg))
	{
	   kprint("Vesa\n");
//       while(1);
	}
		}
		*comp = color;
	}
}

uint VesaInit(void)
{
	static struct info1 *info = (struct info1 *)CONF;
	reg.ax=0x4f00;
	reg.di= CONF;
	BiosCall(&reg); //int,*ax,*bx,*cx,*dx,*bp,*si,*di
	if (!status(&reg))
	{
	   kprint("VesaInit\n");
	   while(1){CpuIdle();};
	}
hexl(info->version);
	if (info->version <0x102)
	{
	   kprint("Vesa.c Version must be 1.2 or highter\n");
	   return 0;
	}
	kprint("Vesa.c Vesa supported. Version: \n");
//    CpuCopyStringC((char*)CONF, (char*)&binfo1, 0x13);
//while(1);
	return 1;
}

uint VesaSetMode(ushort mode)
{
	static struct info2 *buffinfo = (struct info2*) CONF;
/*    kprint("vesa0");
kprintbuff ((char*)binfo1.signature,0x14);
hexl(binfo1.mode);

	while ((binfo1.mode[a]!=0) &(binfo1.mode[a]!=mode))
	{
hexl(binfo1.mode[a]);
getchar();
		  a++;
	}
//    kprint("vesa1");
	if (binfo1.mode[a]!=mode){
	kprint("vesa.c Unsuported mode\n");
	return 0;
	a=0;
	}
	a=0;*/

	if (mode < 0x100)//Change l'écran en mode BIOS
	{
		reg.ax=mode;
		BiosCall(&reg); //int,*ax,*bx,*cx,*dx,*bp,*si,*di
		return 1;
	}

//Test si le mode est possible
	reg.ax=0x4f01;
	reg.cx=mode;
	reg.di= CONF;
	BiosCall(&reg); //int,*ax,*bx,*cx,*dx,*bp,*si,*di
	if (!status(&reg))
	{
	   kprint("VesaSetMode\n");
	   while(1){CpuIdle();};
	}
	if (!(buffinfo->info & 0x1))
	{
	   kprint("Vesa.c Mode not supported by video card\n");
	   return 0;
	}

	config.granul = buffinfo->granul * 0x400;
//hexl(config.granul);
	if ((buffinfo->fenetre1 & 0x5)== 0x5)
	{
	   config.fenetre1 = (uchar*)((buffinfo->addf1)*0x10);
	}
	   config.fenetre1 = (uchar*)VESA_BANK;


	if ((buffinfo->fenetre2 & 0x5)== 0x5)
	{
	   config.fenetre2 = (uchar*)((buffinfo->addf2)*0x10);
	}
	   config.fenetre2 = 0;

	   if (buffinfo->info & 0x2)
	   {
//hexl(buffinfo->hor);
//hexl(buffinfo->ver);
//hexl(buffinfo->bitsparpoint);
	   config.hor=buffinfo->hor;
	   config.ver=buffinfo->ver;
	   if (buffinfo->bitsparpoint == 0xF){config.bits=2;}
	   else {config.bits=buffinfo->bitsparpoint/8;}
	   config.nbpage=((config.bits*config.ver*config.hor)/config.granul)-1;
	   }
	   else
	   {
		   kprint("Vesa.c VesaSetMode Extra Info not present\n");
		   return 0;
	   }

//hexl(buffinfo->taillebloc);
//hexl(config.nbpage);
//getchar();
//hexl(buffinfo->granul);
//hexl(config.granul);
//getchar();


//Set mode
	reg.ax=0x4f02;
	reg.bx=mode;
	BiosCall(&reg); //int,*ax,*bx,*cx,*dx,*bp,*si,*di
	if (!status(&reg))
	{
	   kprint("VesaSetMode 2\n");
	   while(1){CpuIdle();};
	}
	return 1;
}

void VesaSetBank(uchar bank)
{
	reg.ax=0x4f05;
	reg.bx=0;
	reg.dx=bank;
	BiosCall(&reg);
}

