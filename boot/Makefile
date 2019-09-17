MK=make -C

x86: mbr_x86 loader_x86 loader2_x86

mbr_x86:
	$(MK) mbr x86
#	dd if=$@ of=/test conv=notrunc

loader_x86:
	$(MK) loader x86

loader2_x86:
	$(MK) loader2 x86

clean:
	rm -f *.o
	make -C mbr clean
	make -C loader clean
	make -C loader2 clean
	rm -f XPLDR.BIN
	rm -f sizeldr
