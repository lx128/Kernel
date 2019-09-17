#include <archx86.h>
#include <cpux86.h>
#include <i8042.h>
#include <pic.h>
#include <fonctions.h>

void IntAdd(char, void*);
void EventSend(char);//recoit un evennement. Send est du point de vue de l'utilisateur de la fontion
void EventReceive(void);
void EventSubscribe(int, int);//abonne une fonction à un evennement sélectionné.
void EventRemove(int);//retire un evennement à la liste sélectionné.
unsigned char i8042GetKeybState(void);
void i8042GetMouseState(struct mousestate *);
void CpuIdle(void);
void end(void);

void i8042KbdInterrupt(void);
void i8042MouseInterrupt(void);

static unsigned char mouse_state[4]={0};//sauvegarde de l'etat de la souris
static unsigned char kbd_state=0;//sauvegarde de l'etat de la souris

unsigned char i8042Read(char add)
{
	static unsigned int timeout=1000000;

    for(timeout=1000000; timeout>0;timeout--)
    {
		if(inb(STATUS_REG)&1)
        {
           return inb(add);
        }
    }
    kprint("i8042 Read Time Out\n");
    return 0;
}

void i8042Write(char port, char byte)
{
	static unsigned int timeout=1000000;

/*    if(port == COMMAND_REG)
    {
	       outb(port, byte);
	       return;
    }*/
    for(timeout=1000000; timeout>0;timeout--)
    {
//binaire(inb(STATUS_REG));
		if((inb(STATUS_REG)&2)==0)
        {
			outb(port, byte);
			return;
        }
    }
    kprint("i8042 Write Time Out\n");
}

/*static void i8042MouseRead(char octet)
{
  static unsigned timeout=100000;
  timeout=100000;

    while(timeout--)
    {
      if((inb(STATUS_REG) & 2)==0)
        return;
    }
}*/

void i8042Init (void)
{
	static char var=0;
kprint("KbdInit\n");

    IntAdd(1, i8042KbdInterrupt);//enregistre la routine d'irq
    PicIrqOff(1);//desactive l'irq 1 du PIC au cas ou.

    i8042Write(COMMAND_REG, 0xAA);//Write command "Controler Self test"
    if (i8042Read(DATA_REG) != 0x55)
    {
		kprint ("Keyboard Controler not ready\n");
		end();
    }

    i8042Write(COMMAND_REG, 0xAB);//Keyboard Self test
    if (i8042Read(DATA_REG)!= 0x0)
    {
		kprint ("Keyboard not ready\n");
		end();
    }

	i8042Write(COMMAND_REG, 0x20);//commande "Keyb read command byte"
    var = i8042Read(DATA_REG);//Read Data Reg
    var = var | 0x65;//Activate int 1 and keymap convertion.

	i8042Write(COMMAND_REG, 0x60);//commande "Keyb write command byte"
    i8042Write(DATA_REG, var);//configuration du controleur

	i8042Write(DATA_REG, 0xFF);//reset du clavier
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Erreur reset\n");}//ACK
	if ((i8042Read(DATA_REG))!=0xAA){kprint("i8042: Erreur reset\n");}//OK

	i8042Write(DATA_REG, 0xEE);//commande echo
	if ((i8042Read(DATA_REG))!=0xEE){kprint("i8042: Erreur echo\n");}

    i8042Write(DATA_REG, 0xF4);//active le clavier
    IntAdd(1, i8042KbdInterrupt);//enregistre la routine d'irq
    PicIrqOn(1);//active l'irq 1 du PIC, l'interrution clavier est opérationnelle maintenant
	EventSubscribe((int)&var, 1);//abonnement à l'évenement clavier 1
		while(var==0)
		{
			EventReceive();//récupération des messages sur les évennements. Normalement, fontion ordonnanceur
			if (var==1)//test au cas ou une autre interruption a débloqué la boucle.
			{
				if (i8042GetKeybState()!=0xFA)
				{
	hex(kbd_state);
					kprint ("i8042: Erreur Acknowledge\n");
					end();
				}
			}
			else
			{
				CpuIdle();//met en pause le cpu jusqu'au prochain evenement.
			}
		}
	EventRemove(1);//retire un evennement à la liste sélectionné.

//    i8042Write(COMMAND_REG, 0x20);//Write command "Read command byte"
//    reg = i8042Read(DATA_REG);          //Read Data Reg
/*
//Write command "Read command byte"
    i8042Write(DATA_REG, 0x60);
*/

/*Keyboard Disable*/
//    i8042WriteKb(0xF5);

/*Keyboard config default*/
//    i8042WriteKb(0xF6);

/*Keyb echo*/
//    i8042Write(DATA_REG, 0xEE);
//    reg = i8042Read(DATA_REG);        //Return 0xEE = Reset


/*Set typematic rate*/
//    i8042WriteKb(0xF3);      //Write to keyboard
//    i8042WriteKb(0x20);      //Write to keyboard

/*Set scan code*/
//    i8042WriteKb(0xF0);      //Write to keyboard
//    i8042WriteKb(0x1);      //Write to keyboard

/*Keyb on*/

kprint("KbdInit Exit\n");

kprint("Initialization de la souris\n");
    IntAdd(12, i8042MouseInterrupt);
    PicIrqOff(12);//desactive irq souris au cas ou
    PicIrqOff(1);//desactive irq clavier car interfere en déclenchant des irq qui lise les buffers de sortie

	i8042Write(COMMAND_REG, 0xA8);       //active la souris

    i8042Write(COMMAND_REG, 0x20);//Write command "Read command byte"
    var = i8042Read(DATA_REG);//Read Data Reg
    var = var | 0x2;//Activate int 12
	i8042Write(COMMAND_REG, 0x60);//Write command "Write command byte"
    i8042Write(DATA_REG, var);

	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
    i8042Write(DATA_REG, 0xF6);//configuration du controleur au valeurs par defaut
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Erreur config souris\n");}

kprint("test3: ");
	i8042Write(COMMAND_REG, 0xD4);//prochain octet pour la souris
kprint("test4: ");
    i8042Write(DATA_REG, 0xF4);//active l'envoie de données
kprint("test5: ");
	if ((i8042Read(DATA_REG))!=0xFA){kprint("i8042: Erreur reset souris\n");}
kprint("test6: ");
/*static char i8042WriteMouse(char byte)
{
	i8042Write(COMMAND_REG, 0xD4);//Write command "Write command byte"
	i8042Write(DATA_REG, byte);
	return inb(DATA_REG);
}
/*	i8042Write(COMMAND_REG, 0x20);       //Write command "Read command byte"
	reg = inb(DATA_REG);        //Read Data Reg
//binaire(reg);
    reg = reg | 0x2;              //Activate int 12 for PS2 mouse
//binaire(reg);

	i8042Write(COMMAND_REG, 0x60);      //Write command "Write command byte"
kprint("test3: ");
	i8042Write(DATA_REG, reg);          //Write To Data Register
kprint("test4: ");
	i8042Write(COMMAND_REG, 0xD4);        //"Write to mouse" command
kprint("test5: ");
	i8042Write(DATA_REG, 0xFF);            //Write To Data Reg - "reset"
kprint("test6: ");
    mouse_state[0] = mouse_state[0]& 0xF7;


      i8042Write(COMMAND_REG, 0xD4);        //"Write to mouse" command
kprint("test7: ");
      i8042Write(DATA_REG, 0xF4);            //Write To Data Reg - "Enable data reporting"
kprint("test8: ");

/*    while ((mouse_state[0]& 0x8) == 0)
    {
		kprint("Wait ");
	}
    mouse_state[0] = mouse_state[0]& 0xF7;*/
//kprint("test9: ");
//	i8042Write(COMMAND_REG, 0xA8);        //Activate PS2 mouse

    PicIrqOn(12);//desactive irq souris au cas ou
    PicIrqOn(1);//desactive irq clavier car interfere en déclenchant des irq qui lise les buffers de sortie
kprint("Fin Initialization de la souris\n");
}

void i8042MouseInterrupt(void)
{
	static unsigned char comp = 0, start=1;
	if (start==0){start = 1; return;}
    PicIrqOff(12);//desactive irq souris au cas ou
    PicIrqOff(1);//desactive irq clavier car interfere en déclenchant des irq qui lise les buffers de sortie
//kprint("Mouse interrupt");
	mouse_state[comp]=i8042Read(DATA_REG);
//hex(mouse_state[comp]);
//hex(comp);
	comp++;
	if (comp>2)
	{
		comp=0;
		EventSend(12);
	}
    PicIrqOn(12);//desactive irq souris au cas ou
    PicIrqOn(1);//desactive irq clavier car interfere en déclenchant des irq qui lise les buffers de sortie
}

void i8042KbdInterrupt(void)
{
	kbd_state=i8042Read(DATA_REG);
//	hex(kbd_state);
	EventSend(1);
}

unsigned char i8042GetKeybState(void)
{
	return kbd_state;
}


/*
Conversion de l'état de la souris pour une utilisation dans l'application
*/
void i8042GetMouseState(struct mousestate *mstate)
{
hex(mouse_state[0]);
	CpuIrqOff();
	if ((mouse_state[0]& 0x8) != 0)//bit 4 toujours  à 1 si paquet pour mouvement
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
//kprint("ooo");
		mstate->sx = 0;
		mstate->sy = 0;
		mstate->x = 0;
		mstate->y = 0;
	}
	CpuIrqOn();
}
