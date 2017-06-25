TARGET = test.img

TRAPOBJS   = trap.o trapasm.o vectors.o vectors.asm
MAINOBJS   = main.o segment.o video.o gdtidt.o

KERNELOBJS = kernel.bin kernel $(MAINOBJS) $(TRAPOBJS)
BOOTOBJS   = boot.bin

OBJS =$(BOOTOBJS) $(KERNELOBJS)

#ディスクイメージ
$(TARGET):boot.bin kernel.bin 				
	dd if=/dev/zero of=$@ count=18
	dd if=boot.bin of=$@ conv=notrunc
	dd if=kernel.bin of=$@ seek=1 conv=notrunc

#ブートローダー
boot.bin:boot.asm selecter.inc
	nasm -f bin boot.asm -o boot.bin

#カーネル
kernel.bin: main.o segment.o video.o gdtidt.o trap.o trapasm.o vectors.o
	ld -m elf_i386 -o kernel -Ttext 0x00 -e main $^
	objcopy -R .note -R .comment -S -O binary kernel kernel.bin


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

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)


