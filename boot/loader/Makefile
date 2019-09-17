OBJ=loader
NASM=nasm -f bin

x86: loader1.asm
	$(NASM) -o $(OBJ) $^;

clean:
	rm -f $(OBJ)
