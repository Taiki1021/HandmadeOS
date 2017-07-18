TARGET = test.img

TRAPOBJS   = trap.o trapasm.o vectors.o vectors.asm
MAINOBJS   = main.o wrapper.o video.o gdtidt.o memory.o kbd.o fifo.o proc.o

KERNELOBJS = kernel.o kernel_code.bin kernel_data.bin $(MAINOBJS) $(TRAPOBJS)
BOOTOBJS   = boot.bin

OBJS =$(BOOTOBJS) $(KERNELOBJS)

#ディスクイメージ
#|1         |2                6|7              18|
#|          |                  |                 |
#|          |                  |                 |
#| boot.bin |  kernel_data.bin | kernel.code.bin |
#|          |                  |                 |
#|          |                  |                 |
$(TARGET):boot.bin kernel_code.bin kernel_data.bin
	dd if=/dev/zero of=$@ count=32
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

kernel.o: main.o wrapper.o video.o gdtidt.o trap.o trapasm.o vectors.o memory.o kbd.o fifo.o proc.o
	ld  -o $@ -T kernel.ld $^

#↓カーネル用オブジェクトファイル↓

wrapper.o:wrapper.asm 
	nasm -f elf32 $<

main.o:main.c defs.h
	gcc  -m32 $< -c

video.o:video.c defs.h
	gcc -m32 $< -c

gdtidt.o:gdtidt.c defs.h
	gcc -m32 $< -c

trap.o:trap.c defs.h
	gcc -m32 $< -c

trapasm.o:trapasm.asm 
	nasm -f elf32 $<

vectors.o:vectors.asm 
	nasm -f elf32 $<

vectors.asm:vectors.pl
	perl $< > $@

memory.o: memory.c defs.h
	gcc -m32 $< -c

kbd.o:kbd.c defs.h
	gcc -m32 $< -c

fifo.o:fifo.c defs.h
	gcc -m32 $< -c

proc.o:proc.c defs.h
	gcc -m32 $< -c


#↑カーネル用オブジェクトファイル↑

.PHONY: clean sector qemu debug
clean:
	rm -f $(TARGET) $(OBJS)

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

