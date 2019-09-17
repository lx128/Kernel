OBJ=mbr
NASM=nasm -f bin

x86: mbrhdd.asm
	$(NASM) -o $(OBJ) $^;

clean:
	rm -f $(OBJ)
