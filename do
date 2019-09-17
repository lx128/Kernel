#!/bin/bash

if [ -f hdd.img ];
then echo
#else dd if=/dev/zero of=floppy.dsk bs=1474560 count=1
else dd if=/dev/zero of=hdd.img bs=1548288 count=1
fi

dd if=boot/mbr/mbr of=hdd.img conv=notrunc
dd if=xpldr.bin of=hdd.img conv=notrunc ibs=512 seek=1
