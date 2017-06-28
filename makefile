TARGET = test.img

TRAPOBJS   = trap.o trapasm.o vectors.o vectors.asm
MAINOBJS   = main.o segment.o video.o gdtidt.o

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

kernel.o: main.o segment.o video.o gdtidt.o trap.o trapasm.o vectors.o 
	ld  -o $@ -T kernel.ld $^

#↓カーネル用オブジェクトファイル↓

segment.o:segment.asm segment.h
	nasm -f elf32 $<

main.o:main.c segment.h video.h trap.h
	gcc  -m32 $< -c

video.o:video.c video.h segment.h
	gcc -m32 $< -c

gdtidt.o:gdtidt.c gdtidt.h segment.h selecter.h
	gcc -m32 $< -c

trap.o:trap.c trap.h
	gcc -m32 $< -c

trapasm.o:trapasm.asm trap.h
	nasm -f elf32 $<

vectors.o:vectors.asm trap.h
	nasm -f elf32 $<

vectors.asm:vectors.pl
	perl $< > $@


#↑カーネル用オブジェクトファイル↑

.PHONY: clean sector qemu debug
clean:
	rm -f $(TARGET) $(OBJS)

sector:
	objdump -h kernel.o

qemu:
	qemu-system-x86_64 -m 256 -fda test.img

debug:
	qemu-system-x86_64 -S -gdb tcp::1234 -m 256 -fda test.img
