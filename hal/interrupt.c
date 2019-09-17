#include <archx86.h>
#include <fonctions.h>
#include <textmode.h>
#include <irq.h>
#include <interrupt.h>

/* Routine par defaut pour traiter les interruptions */

void IdtDefault(void);

static uint int_map[16] = {0};

void IntDefault(void) {
	TextModePutcar('.'); 
}

void IntInt0(void) {
	kprint("Exeption 0: Divide error");
	while (1);
}
void IntInt1(void) {
	kprint("Exeption 1: Debug");
	while (1);
}
void IntInt2(void) {
	kprint("Exeption 2: Nonmaskable interrupt");
	while (1);
}
void IntInt3(void) {
	kprint("Exeption 3: Breakpoint");
	while (1);
}
void IntInt4(void) {
	kprint("Exeption 4: Overflow");
	while (1);
}
void IntInt5(void) {
	kprint("Exeption 5: Bounds check");
	while (1);
}
void IntInt6(void) {
	kprint("Exeption 6: Invalid opcode");
	while (1);
}
void IntInt7(void) {
	kprint("Exeption 7: Coprocessor not available");
	while (1);
}
void IntInt8(void) {
	kprint("Exeption 8: Double Fault");
	while (1);
}
void IntInt9(void) {
	kprint("Exeption 9: Coprocessor segment overrun");
	while (1);
}
void IntInt10(void) {
	kprint("Exeption 10: Invalid TSS"); 
	while (1);
}
void IntInt11(void) {
	kprint("Exeption 11: Segment no present"); 
	while (1);
}
void IntInt12(void) {
	kprint("Exeption 12: Stack exception"); 
	while (1);
}
void IntInt13(void) {
	kprint("Exeption 13: General protection violation"); 
	while (1);
}
void IntInt14(void) {
	kprint("Exeption 14: Page fault"); 
	while (1);
}
void IntInt15(void) {
	kprint("Exeption 15: Reserved You are not able to do this exeption!"); 
	while (1);
}
void IntInt16(void) {
	kprint("Exeption 16: Coprocessor error");
	while (1);
}
void IntInt17(void) {
	kprint("Exeption 17: Alignement error");
	while (1);
}
void IntInt18(void) {
	kprint("Exeption 18: ??? I don't now what you do ???");
	while (1);
}

void IntIrq0(void) {        //Horloge   Priorité:0
//	TextModePutcar('0');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[0]));
}
void IntIrq1(void) {       //Keyb   Priorité:1
//	TextModePutcar('1');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[1]));
}
void IntIrq2(void) {       //Cascade PIC 2   Priorité:Aucune
//	TextModePutcar('2');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[2]));
}
void IntIrq3(void) {       //Com 2/4   Priorité:10
//	TextModePutcar('3');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[3]));
}
void IntIrq4(void) {       //Com 1/3   Priorité:11
//	TextModePutcar('4');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[4]));
}
void IntIrq5(void) {       //Libre   Priorité:12
//   TextModePutcar('5');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[5]));
}
void IntIrq6(void) {       //Floppy   Priorité:13
//	TextModePutcar('6');
//	int_status();
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[6]));
}
void IntIrq7(void) {       //Paralell Port   Priorité:14
//	TextModePutcar('7');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[7]));
}
void IntIrq8(void) {       //Real Time Clock   Priorité:2
//	TextModePutcar('8');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[8]));
}
void IntIrq9(void) {       //Libre   Priorité:3
//	TextModePutcar('9');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[9]));
}
void IntIrq10(void) {       //Libre   Priorité:4
//	TextModePutcar('A');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[10]));
}
void IntIrq11(void) {       //Libre   Priorité:5
//	TextModePutcar('B');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[11]));
}
void IntIrq12(void) {       //PS2 mouse / Free   Priorité:6
//	TextModePutcar('C');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[12]));
}
void IntIrq13(void) {       //Coprocesseur   Priorité:7
//	TextModePutcar('D');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[13]));
}
void IntIrq14(void) {       //IDE 1   Priorité:8
//	TextModePutcar('E');
//	ata_irq();
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[14]));
}
void IntIrq15(void) {       //IDE 2   Priorité:9
//	TextModePutcar('F');
	asm("	movl %0,%%eax\n\
		call *%%eax\n"::"a" (int_map[15]));
}

void IntAdd(uchar inter, void* fonction)
{
    if (inter<16)
    {
		int_map[(uint)inter] = (uint)fonction;
    }
}

void IntDel(uchar inter)
{
    if (inter<16)
    {
		int_map[(uint)inter] = (uint)IntDefault;
    }
}

void IntInit(void)
{
    uchar a;
    for (a=0;a<16;a++)
    {
		int_map[(uint)a] = (uint)IntDefault;
    }
}
