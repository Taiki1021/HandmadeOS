
KERNELOBJS = main.o wrapper.o video.o gdtidt.o trap.o trapasm.o vectors.o memory.o kbd.o fifo.o proc.o string.o

OTHEROBJS = vectors.asm kernel.o kernel_data.bin kernel_code.bin boot.bin os.img

OBJS =$(OTHEROBJS) $(KERNELOBJS)

.SUFFIXES: .c .asm .o
.c.o:
	gcc -m32 $< -c

.asm.o:
	nasm -f elf32 $<


#OSディスクイメージ
#|1         |2                6|7              50|
#|          |                  |                 |
#|          |                  |                 |
#| boot.bin |  kernel_data.bin | kernel.code.bin |
#|          |                  |                 |
#|          |                  |                 |
os.img:boot.bin kernel_code.bin kernel_data.bin
	dd if=/dev/zero of=$@ count=50
	dd if=boot.bin of=$@ conv=notrunc
	dd if=kernel_data.bin of=$@ seek=1 conv=notrunc
	dd if=kernel_code.bin of=$@ seek=6 conv=notrunc

#ブートローダー
boot.bin:boot.asm selecter.inc
	nasm -f bin boot.asm -o boot.bin

#カーネル
kernel_code.bin: kernel.o
	objcopy -R .note -R .comment -R .eh_frame -R .rodata -R .bss -S -O binary $< $@

kernel_data.bin: kernel.o
	objcopy -R .note -R .comment -R .eh_frame -R .text           -S -O binary $< $@

kernel.o: $(KERNELOBJS)
	ld  -o $@ -T kernel.ld $^

#↓カーネル用オブジェクトファイル↓

main.o	:main.c defs.h

video.o	:video.c defs.h

gdtidt.o:gdtidt.c defs.h

trap.o	:trap.c defs.h

memory.o:memory.c defs.h

string.o:string.c defs.h

kbd.o	:kbd.c defs.h

fifo.o	:fifo.c defs.h

proc.o	:proc.c defs.h

trapasm.o:trapasm.asm 

vectors.o:vectors.asm 

wrapper.o:wrapper.asm 

vectors.asm:vectors.pl
	perl $< > $@


#↑カーネル用オブジェクトファイル↑

.PHONY: clean sector qemu debug
clean:
	rm -f $(OBJS)

sector:
	objdump -h kernel.o

qemu:
	qemu-system-x86_64 -m 256 -fda test.img

vbox:
	sudo virtualbox /root/'VirtualBox VMs'/HandmadeOS/HandmadeOS.vbox

debug:
	qemu-system-x86_64 -S -gdb tcp::1234 -m 256 -fda test.img

kernel_disasm:
	ndisasm kernel_code.bin |less

