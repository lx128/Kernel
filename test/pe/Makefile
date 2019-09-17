CC=gcc
CFLAGS=-Wall -Wstrict-prototypes -I ../../include -fdiagnostics-color
CFLAGS2=-c -fPIC -nostdlib -Wall -Wstrict-prototypes -I ../../include -fdiagnostics-color
CFLAGSLIB=-shared -fPIC -Wall -fdiagnostics-color

prog:
#	ld --oformat binary -Ttext 20204 --omagic $^ -o $@
	$(CC) $(CFLAGS) pe.c
	# -ltest.dll

lib:
	$(CC) $(CFLAGS2) libtest.c
##	dlltool -mi386 -e exports.o -l dll.lib libtest.o
#	$(CC) $(CFLAGSLIB) -nostdlib libtest.o -o libtest.dll.o
#	ld -r libtest.o -o libtest.dll.o
	gcc -shared libtest.o -o libtest.dll
#gcc dll.c -c
#dlltool -e exports.o -l dll.lib dll.o
#gcc dll.o exports.o -o dll.dll -nostdlib

clean:
	rm -f *.o *.lib
