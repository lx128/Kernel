MK=make -C
OBJ=kernel.ld
KERN=kernel.exe

all: x86
x86: boot_x86 drivers_x86 hal_x86 kernel_x86 size

	#ld -mi386pe hal/int.ld hal/hal.ld drivers/drivers.ld kernel/$(OBJ) -e __Z5startv -o $(KERN)
	ld -mi386pe hal/int.ld hal/hal.ld drivers/drivers.ld kernel/$(OBJ) -e _start -o $(KERN)

#Total file size
	printf "\x00\x00\x00\x00" > map

	./size.exe boot/loader/loader >> map
	./size.exe boot/loader2/loader2 >> map
	./size.exe hal/bioscall.ld >> map
	./size.exe kernel.exe >> map
	printf "\x00\x00\x00\x00" >> map
	printf "\x00\x00\x00\x00" >> map
	printf "\x00\x00\x00\x00" >> map
	printf "\x00\x00\x00\x00" >> map
	printf "\x00\x00\x00\x00" >> map
	printf "\x00\x00\x00\x00" >> map
	printf "\x00\x00\x00\x00" >> map

	cat map > xpldr.bin
#Loader
	cat boot/loader/loader >> xpldr.bin
	cat boot/loader2/loader2 >> xpldr.bin

#Int86
	cat hal/bioscall.ld >> xpldr.bin

#Kernel
	cat kernel.exe >> xpldr.bin

	./size.exe xpldr.bin > sizeldr
	dd if=sizeldr of=xpldr.bin conv=notrunc

boot_x86:
	$(MK) boot x86

hal_x86:
	$(MK) hal x86

drivers_x86:
	$(MK) drivers x86

kernel_x86:
	$(MK) kernel x86

test: x86
	./do

size:
	gcc size.c -o $@.exe -s

clean:
	rm -f *.o *.exe
	$(MK) boot clean
	$(MK) hal clean
	$(MK) kernel clean
	$(MK) drivers clean
	rm -f xpldr.bin
	rm -f map
	rm -f sizeldr
