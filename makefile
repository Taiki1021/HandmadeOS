TARGET = test.img
OBJS = boot.bin boot2.bin
AS = nasm
ASFLAGS = -f bin
CAT=cat
RM=rm -f

$(TARGET): $(OBJS)
	dd if=/dev/zero of=$@ count=1000
	dd if=boot.bin of=$@ conv=notrunc
	dd if=boot2.bin of=$@ seek=1 conv=notrunc

SUFFIXES: .asm .bin
.asm .bin:
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

boot.bin:boot.asm selector.inc
	$(AS) $(ASFLAGS) $< -o $@

boot2.bin:boot2.asm selector.inc
	$(AS) $(ASFLAGS) $< -o $@

