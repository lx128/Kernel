OUTPUT=kernel
CC=gcc
CFLAGS=-c -Wall -Wstrict-prototypes -I ../include -nostdlib -fdiagnostics-color
LDFLAGS=-r
SRC=kernel.c fonctions.c exec.c exemple.c syscallc.c
OBJ= $(SRC:.c=.o)

x86: $(OBJ)
#	ld --oformat binary -Ttext 20204 --omagic $^ -o $@
#	$(CC) $^ -m32 $(CFLAGS)
	ld -mi386pe $(LDFLAGS) $(OBJ) -o $(OUTPUT).ld

clean:
	rm -f *.o *.ld

%.o: %.c
	$(CC) -m32 $(CFLAGS) $<
