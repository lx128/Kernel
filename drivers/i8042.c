/*CLR All rights Reserved*/
//Fonctionnel													=1
//Controle overflow                                             =0
//Compatible 64 bits (long au lieu de int là où necessaire)     =0
//Controle unicode                                              =0
//Code d'erreur                                                 =0
//Sécurité                                                      =0

#include <archx86.h>
#include <clr.h>
#include <cpux86.h>
#include <i8042.h>
#include <pic.h>
#include <fonctions.h>
#include <irq.h>

static unsigned char mouse_state[4]={0};//sauvegarde de l'etat de la souris
static unsigned char kbd_state=0, mouse_state_size=0,mouse_state_ptr=0;//sauvegarde de l'etat du clavier

unsigned char i8042Read(char add)//ok
{
	static unsigned int timeout=1000000;

    for(timeout=1000000; timeout>0;timeout--)
    {
		if(inb(STATUS_REG)&1)
        {
           return inb(add);
        }
    }
    kprint("i8042: Read Time Out\n");
    return 0;
}

void i8042Write(char port, char byte)//ok
{
	static unsigned int timeout=1000000;

    for(timeout=1000000; timeout>0;timeout--)
    {
//binaire(inb(STATUS_REG));
		if((inb(STATUS_REG)&2)==0)
        {
			outb(port, byte);
			return;
        }
    }
    kprint("i8042: Write Time Out\n");
}

void i8042IrqOn(void)//ok
{
	PicIrqOn(12);
	PicIrqOn(1);
}

void i8042IrqOff(void)//ok
{
	PicIrqOff(12);
	PicIrqOff(1);
}

void i8042MouseInterrupt(void)//ok
{
	static uchar data=0;
	i8042IrqOff();
//kprint("Mouse interrupt\n");	

	data=i8042Read(DATA_REG);
//hex(data);
	if (mouse_state_ptr==0 && (data&8)==0)//test si le bit 3 du premier paquet est bien a 1
	{
		kprint("Mouse erreur de syncronisation\n");
		i8042IrqOn();
		return;
	}
	mouse_state[mouse_state_ptr]=data;
	if (mouse_state_ptr>mouse_state_size)
	{
		mouse_state_ptr=0;
		i8042IrqOn();
		return;
	}
	mouse_state_ptr++;
	i8042IrqOn();
}

void i8042KbdInterrupt(void)//ok
{
	i8042IrqOff();
//	kprint("Keyb interrupt\n");
	kbd_state=i8042Read(DATA_REG);
//	hex(kbd_state);
	i8042IrqOn();
}

uchar i8042MouseTypeTest(uchar type)//ok
{
//changement sample rate
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, 0xF3);//changement sample rate
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge Error\n");}
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, 200);
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge Error\n");}

//changement sample rate
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, 0xF3);//changement sample rate
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge Error\n");}
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, type);
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge Error\n");}

//changement sample rate
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, 0xF3);//changement sample rate
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge Error\n");}
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, 80);
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge Error\n");}

//kprint("i8042: Get mouse type\n");
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, 0xF2);
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge Error\n");}

	return i8042Read(DATA_REG);
}

void i8042Init (void)//ok
{
	static unsigned char comp=0;

	i8042IrqOff();

kprint("i8042: Keyboard Init\n");
    IntAdd(1, (void*)i8042KbdInterrupt);//enregistre la routine d'irq

kprint("i8042: Reset\n");
	i8042Write(DATA_REG, 0xFF);//reset du clavier
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge error\n");}//ACK

	comp=i8042Read(DATA_REG);
//	hex(comp);
	if (comp==0xAA){kprint("i8042: Basic Assurance Test passed\n");}
	else if (comp==0xFC){kprint("i8042: Basic Assurance Test failled\n");}

kprint("i8042: Activating\n");
    i8042Write(DATA_REG, 0xF4);//active le clavier
	if (i8042Read(DATA_REG)!=0xFA){kprint("i8042: Acknowledge error\n");}//ACK

kprint("i8042: Mouse Init\n");
	IntAdd(12, (void*)i8042MouseInterrupt);

kprint("i8042: Activate controle\n");
	i8042Write(COMMAND_REG, 0xA8);

kprint("i8042: Reset mouse\n");
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, 0xFF);
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge Error\n");}

	comp=i8042Read(DATA_REG);
//	hex(comp);
	if (comp==0xAA){kprint("i8042: Basic Assurance Test passed\n");}
	else if (comp==0xFC){kprint("i8042: Basic Assurance Test failled\n");}
	//la souris renvoie toujours le type standard après reset donc l'octet suivant n'a pas de valeur
	if ((i8042Read(DATA_REG))!=0x00){kprint("i8042: Unattended value\n");}

kprint("i8042: Test for advanced mouse\n");
	//get device id 0=standard 3=intellimouse 3 axes 4=intellimouse 5 boutons
	if (i8042MouseTypeTest(100)==3)
	{
		kprint("i8042: Intellimouse 3 axis 5 buttons detected\n");
		mouse_state_size=2;//envoie 4 octets
	}
	else if (i8042MouseTypeTest(200)==4)
	{
		kprint("i8042: Intellimouse 5 buttons detected\n");
		mouse_state_size=2;//envoie 4 octets
	}
	else
	{
		kprint("i8042: Standard mouse detected\n");
		mouse_state_size=1;//envoie 3 octets
	}

kprint("i8042: Active repport\n");
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, 0xF4);
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Acknowledge Error\n");}

kprint("i8042: Enable interrupt\n");
	//set bit 2 to 1 to enable mouse interrupt
	i8042Write(COMMAND_REG, 0x20);//Write command "Read command byte"
	comp = i8042Read(DATA_REG);//Read Data Reg
	comp = comp | 0x2;//Activate int 12
	//binaire(comp);
	i8042Write(COMMAND_REG, 0x60);//Write command "Write command byte"
	i8042Write(DATA_REG, comp);

kprint("i8042: Init end\n");
	i8042IrqOn();
}

uchar i8042GetKeybState(void)//ok
{
	static uchar comp=0;
	comp=kbd_state;
	kbd_state=0;
	return comp;
}

/*
Conversion de l'état de la souris pour une utilisation dans l'application
*/
void i8042GetMouseState(struct mousestate *mstate)//ok
{
//binaire(mouse_state[0]);
	if (mouse_state[0]& 0x8)//bit 4 toujours  à 1 si paquet pour mouvement
	{
		mstate->sx = (mouse_state[0]&0x10)>>4;//signe x
		mstate->sy = (mouse_state[0]&0x20)>>5;//signe y
		mstate->b1 = (mouse_state[0]&0x1);//bouton 1
		mstate->b2 = (mouse_state[0]&0x2)>>2;//bouton 2
		mstate->b3 = (mouse_state[0]&0x4)>>3;//bouton 3
		mstate->x = mouse_state[1];
		mstate->y = mouse_state[2];

//binaire((state[0])>>4);
//binaire((state[0])>>5);
//binaire(mstate->x);
//binaire(mstate->y);
	}
    else
	{
		mstate->sx = 0;
		mstate->sy = 0;
		mstate->b1 = 0;//bouton 1
		mstate->b2 = 0;//bouton 2
		mstate->b3 = 0;//bouton 3
		mstate->x = 0;
		mstate->y = 0;
	}
	mouse_state[0]=0;
}

uchar i8042CheckMouseState()//ok
{
//	hex(mouse_state_ptr);
	if(mouse_state_ptr==0){return 1;}
	return 0;
}

uchar i8042CheckKeybState()//ok
{
	if(kbd_state){return 1;}
	return 0;
}
