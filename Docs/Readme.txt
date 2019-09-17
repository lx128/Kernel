Kernel v 0.1b

Rappel:

Chargement du noyau en 3 étape
1 MBR de 512 octets qui charge le XPLDR
2 XPLDR en ASM passe en 32 bits et charge l'image de XPLDR32
3 XPLDR32 recherche et charge le système.

2 MBR existe:

MBR floppy compatible FAT12 charge le XPLDR depuis une position déterminé

MBR floppy compatible FAT12 recherche et charge XPLDR en se servant du système de fichier FAT12.

Dump de la mémoire:
Real mode
Bios		0h to 4FFh
    Interrupt table	0h to 3FFh
    Buffer bios		400h to 4FFh

Free		500h to 9FFFFh
    New Stack		2000h to 3000h
    MBR			7C00h to 7E00h		512 bytes
    Stack		FFFEh to 0h (after POST)
    Loader		20000h to ...h

Bios		A0000h to FFFFFh
HMA		100000h to 10FFFEh
Major mem	10FFFFh to ...
************************************************************

Protected mode
Bios		0h to FFFh
	Interrupt table	0h to 3FFh
	Buffer bios	400h to 4FFh
	Free		500h to FFFh

Idt		1000h to 17FFh		2048 bytes
Gdt		1800h to 1FFFh		2048 bytes
16 bits Stack	2000h to 3FFFh
System config	4000h to 4FFFh
I386.asm	5000h to 5FFFh
Code32		6000h to 6FFFh
Free		7000h to A0000h

Bios		A0000h to 10FFFFh
    VGA			B8000h to B8FA0h	4000 bytes

Free		101000h to ...
	Loader		401000h to ...h

Protected mode II
Bios		0h to FFFh
	Interrupt table	0h to 3FFh
	Buffer bios	400h to 4FFh
	Free		500h to FFFh

Idt		             1000h to 17FFh		      2048 bytes
Gdt		             1800h to 1FFFh		      2048 bytes
16 bits Stack	     2000h to 7FFFh		      6 pages
System config & Buff 8000h to 8FFFh		      1
I386.asm	         9000h to 9FFFh		      1
Buffer for DMA	     A000h to 23FFFh	      16 pages
32 bits Stack	     24000h to 2BFFFh	      8 pages
Free		         2C000h to A0000h

Bios		         A0000h to 10FFFFh
    VGA			            B8000h to B8FA0h	4000 bytes

Free		         101000h to ...
	Loader		            401000h to ...h
