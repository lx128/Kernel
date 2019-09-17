OUTPUT=hal
CC=gcc
CFLAGS=-c -Wall -Wstrict-prototypes -I ../include -nostdlib -fdiagnostics-color
LDFLAGS=-r
SRC=cpux86.c interrupt.c gdt.c idt.c bioscall.c syscall.c mmu.c
OBJ= $(SRC:.c=.o)

x86: $(OBJ)
	nasm bioscall.asm -o bioscall.ld
	nasm -f elf int.asm -o int.ld
#	$(CC) -m32 $(CFLAGS)
	ld -mi386pe $(LDFLAGS) $(OBJ) -o $(OUTPUT).ld

clean:
	rm -f *.o *.ld

%.o: %.c
	$(CC) -m32 $(CFLAGS) $<
