#include <fonctions.h>
#include <cpux86.h>

//char cmos_read(char);

char CmosRead(char offset)
{
    offset = offset + 0x10;
    outb(0x70, offset);			//Byte to read
    return inb (0x71);			//Read it
}
