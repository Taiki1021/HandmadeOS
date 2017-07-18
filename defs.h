#define GDT_TYPE0	1
#define GDT_TYPE1	2
#define GDT_TYPE2	4
#define GDT_TYPE3	8
#define GDT_S		16
#define GDT_DPL0	32
#define GDT_DPL1	64
#define GDT_P		128
#define GDT_AVL		256
#define GDT_D		1024
#define GDT_G		2048

#define IDT_D		8
#define IDT_DPL0	32
#define IDT_DPL1	64
#define IDT_P		128

#define SysCodeSelecter  0x08
#define SysDataSelecter  0x10
#define VideoSelecter    0x18
#define GDTSelecter	 0x20
#define IDTSelecter	 0x28
#define UsrCodeSelecter	 0x30
#define UsrDataSelecter	 0x38
#define BootSelecter	 0x40
#define MallocSelecter	 0x48

#define PROCCOUNT 64

#define KBDBUFFERSIZE 1024

#define VRAM ((vramdata*)(0xB8000))

#define IDT ((struct idtdata*)(0x0))

#define GDT ((struct gdtdata*)(0x7C00+0x150))



struct fifo{
	int enext;
	int dnext;
	int size;
	unsigned char buffer[];
};


struct gdtdata{
	unsigned short Limit1;
	unsigned short BaseAddress1;
	unsigned char  BaseAddress2;
	unsigned char  Flags1;
	unsigned char Flags2Limit2;
	unsigned char BaseAddress3;
};

struct idtdata{
	unsigned short Handler1;
	unsigned short HandlerSegment;
	unsigned char  Unused;
	unsigned char  Flags;
	unsigned short Handler2;
};


typedef struct{ 
	char c;
	unsigned char color;
} vramdata;

struct trapframe{
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int oesp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx; 
	unsigned int eax;

	unsigned short gs;
	unsigned short padding1;
	unsigned short fs;
	unsigned short padding2;
	unsigned short es;
	unsigned short padding3;
	unsigned short ds;
	unsigned short padding4;

	unsigned int trapno;

	unsigned int err;
	unsigned int eip;
	unsigned short cs;
	unsigned short padding5;
	unsigned int eflags;

	unsigned int esp;
	unsigned short ss;
	unsigned short padding6;
};

//gdtidt.c
void GDT_Clear(struct gdtdata* gdt);
unsigned int GDT_GetBaseAddress(struct gdtdata* gdt);
void GDT_SetBaseAddress(struct gdtdata* gdt,unsigned int BaseAddress);
unsigned int GDT_GetLimit(struct gdtdata* gdt);
void GDT_SetLimit(struct gdtdata* gdt,unsigned int Limit);
unsigned short GDT_GetFlags(struct gdtdata* gdt);
void GDT_SetFlags(struct gdtdata* gdt,unsigned short Flags);
void IDT_Clear(struct idtdata* idt);
unsigned int IDT_GetHandler(struct idtdata* idt);
void IDT_SetHandler(struct idtdata* idt,unsigned int Handler);
unsigned short IDT_GetHandlerSegment(struct idtdata* idt);
void IDT_SetHandlerSegment(struct idtdata* idt,unsigned short HandlerSegment);
unsigned char IDT_GetFlags(struct idtdata* idt);
void IDT_SetFlags(struct idtdata* idt,unsigned char Flags);
void IDT_Init();
void GDTDUMP(int A);
void IDTDUMP(int A);


//kbd.c
void ISR_KBD(struct trapframe* tf);
char vgetc();
void KBD_Check();
void KBD_Init();

//memory.c
void mem_reset();
void* mem_alloc(int size);
void mem_free(void* address,int size);
void mem_reserve(void* address,int size);
void mem_dump(int start,int end);
int mem_num();

//trap.c
extern void (*IntHandler[256])(struct trapframe*);
extern void* vectors[256];
void trap(struct trapframe* ft);
//trapasm.asm
void alltraps();

//video.c
void setcursor(unsigned short pos);
void vputc(char c);
void vputs(char* str);
void clear();
void schroll();
void blockcpy(void* dist,void* src,int size);
void blockswap(void* A,void* B,int blocksize);
void reverse(void* seq,int blocksize,int count);
int strlen(char* str);
void numstr(char* out,int num);
void sformat(char* dist,char* form,...);
void Printf(char* form,...);

//wrapper.asm
void Halt();
void CopyFar(unsigned short DistSelecter,void* DistOffset,int DStep,unsigned short SrcSelecter,void* SrcOffset,int SStep,int n);
int outb(unsigned short port,unsigned char data);
int outw(unsigned short port,unsigned short data);
unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);
void sti();
void cli();

//fifo.c
void efifo(struct fifo* f,unsigned char data);
unsigned char dfifo(struct fifo* f);
struct fifo* createfifo(int size);
void freefifo(struct fifo* f);
char isfifoend(struct fifo* f);
