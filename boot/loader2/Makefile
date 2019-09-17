OUTPUT=loader2
CC=gcc
CFLAGS=-c -Wall -Wstrict-prototypes -I ../../include -nostdlib -fdiagnostics-color
#LDFLAGS=-Ttext 30230 -e __Z5startv
LDFLAGS=-Ttext 30230 -e _start
SRC=kernel.c cmos.c cpux86.c exec.c consolepc.c
OBJ= $(SRC:.c=.o)

x86: $(OBJ)

#	$(CC) -m32 $(CFLAGS) $^
#	gcc -c -Wall -Wstrict-prototypes -I ../include -o kernel.o kernel.c cmos.c
#	ld -Ttext 30230 -omagic -e _start $@.o -o $@
	ld -mi386pe $(LDFLAGS) kernel.o cmos.o cpux86.o exec.o consolepc.o -o $(OUTPUT).ld
	objcopy -O binary $(OUTPUT).ld $(OUTPUT) -S

clean:
	rm -f $(OBJ) $(OUTPUT) *.ld

%.o: %.c
	$(CC) -m32 $(CFLAGS) $<
