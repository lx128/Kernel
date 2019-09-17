#ifndef I8042_H
#define I8042_H

#include<clr.h>

#define     DATA_REG    0x60        //R/W
#define     COMMAND_REG    0x64     //W
#define     STATUS_REG    0x64      //R

struct mousestate
{
		unsigned char x;
		unsigned char y;
		unsigned char z;
		unsigned char sx: 1;
		unsigned char sy: 1;
		unsigned char sz: 1;
		unsigned char b1: 1;
		unsigned char b2: 1;
		unsigned char b3: 1;
		unsigned char b4: 1;
		unsigned char b5: 1;
};

uchar i8042CheckKeybState(void);
uchar i8042CheckMouseState(void);
uchar i8042GetKeybState(void);
void i8042GetMouseState(struct mousestate *);
void i8042Init(void);

#endif
