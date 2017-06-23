TARGET = test.img
OBJS = boot.bin kernel.bin segment.o main.o kernel kernel.bin
AS = nasm
ASFLAGS = -f bin
CAT=cat
RM=rm -f

#ディスクイメージ
$(TARGET):boot.bin kernel.bin 				
	dd if=/dev/zero of=$@ count=3
	dd if=boot.bin of=$@ conv=notrunc
	dd if=kernel.bin of=$@ seek=1 conv=notrunc

#ブートローダー
boot.bin:boot.asm selecter.inc
	nasm -f bin boot.asm -o boot.bin

#カーネル
kernel.bin: main.o segment.o video.o
	ld -m elf_i386 -o kernel -Ttext 0x00 -e main main.o segment.o video.o
	objcopy -R .note -R .comment -S -O binary kernel kernel.bin


#↓カーネル用オブジェクトファイル↓

segment.o:segment.asm
	nasm -f elf32 segment.asm

main.o:main.c segment.h video.h
	gcc  -m32 main.c -c

video.o:video.c video.h
	gcc -m32 video.c -c

#↑カーネル用オブジェクトファイル↑

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)


