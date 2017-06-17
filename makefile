TARGET = test.img
OBJS = boot.bin boot2.bin
AS = nasm
ASFLAGS = -f bin
CAT=cat
RM=rm -f

$(TARGET): $(OBJS)
	$(CAT) $(OBJS) > $(TARGET)

SUFFIXES: .asm .bin
.asm .bin:
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

boot.bin:boot.asm
	$(AS) $(ASFLAGS) $< -o $@

boot2.bin:boot2.asm
	$(AS) $(ASFLAGS) $< -o $@

