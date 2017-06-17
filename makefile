TARGET = test.img
OBJS = boot.bin boot2.bin
AS = nasm
FLAGS = -f bin
CAT=cat

SUFFIXES: .asm .bin

.asm .bin:
	$(AS) $(FLAGS) $< -o $@

$(TARGET):$(OBJS)
	$(CAT) $(OBJS) > $(TARGET)
