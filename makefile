TARGET = test.img
OBJS = boot.bin kernel.bin
AS = nasm
ASFLAGS = -f bin
CAT=cat
RM=rm -f

$(TARGET): $(OBJS)
	dd if=/dev/zero of=$@ count=2
	dd if=boot.bin of=$@ conv=notrunc
	dd if=kernel.bin of=$@ seek=1 conv=notrunc

SUFFIXES: .asm .bin 
.asm .bin:
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) kernel.o kernel 


kernel.bin: kernel.c
	gcc kernel.c -c
	ld -o kernel -Ttext 0x00 -e main kernel.o
	objcopy -R .note -R .comment -S -O binary kernel kernel.bin

boot.bin:boot.asm selecter.inc
	$(AS) $(ASFLAGS) $< -o $@
