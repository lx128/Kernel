OUTPUT=drivers
CC=gcc
CFLAGS=-c -Wall -Wstrict-prototypes -I ../include -nostdlib -fdiagnostics-color
LDFLAGS=-r
SRC=textmode.c cmos.c pic.c i8042.c vesa.c ata.c
OBJ= $(SRC:.c=.o)

x86:  $(OBJ)
#	$(CC) -m32 $(CFLAGS) $(SRC)
	ld -mi386pe $(LDFLAGS) $(OBJ) -o $(OUTPUT).ld

clean:
	rm -f *.o *.ld

%.o: %.c
	$(CC) -m32 $(CFLAGS) $<
