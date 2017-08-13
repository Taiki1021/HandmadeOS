#include<stdarg.h>

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

#define SysCodeSelecter  8*1
#define SysDataSelecter  8*2
#define UsrCodeSelecter	 8*3
#define UsrDataSelecter	 8*4

#define BootSelecter	 8*3

#define PROCCOUNT 16

#define KBDBUFFERSIZE 1024

#define VRAM ((vramdata*)(0xB8000))
#define DISK ((void*)(0x16000))


#define IDTCOUNT	256
#define TSSCOUNT	(PROCCOUNT)
#define SEGMENTCOUNT	5
#define GDTCOUNT	(SEGMENTCOUNT+TSSCOUNT)

#define IDT ((struct idtdata*)(0))
#define GDT ((struct gdtdata*)(256*8))
#define PDT ((struct gdtdata*)((uint)GDT+SEGMENTCOUNT*8))



#define TssSelecter(no) (8*(SEGMENTCOUNT+no))

#define SDEAD	0
#define SRUN	1
#define SWAIT	2


typedef unsigned short	ushort;
typedef unsigned int 	uint;
typedef unsigned char	uchar;




struct TSS{
	uint backlink,esp0,ss0,esp1,ss1,esp2,ss2,cr3;
	uint eip,eflags,eax,ecx,edx,ebx,esp,ebp,esi,edi;
	uint es,cs,ss,ds,fs,gs;
	uint ldtr,iomap;
};

typedef struct{
	char p_stat;
	struct TSS Context;
	void* text;
	uint textsize;
	void* data;
	uint datasize;
	uint CpuTime;
	int* wchan;
} proc;

struct fifo{
	int enext;
	int dnext;
	int size;
	uchar buffer[];
};


struct gdtdata{
	ushort Limit1;
	ushort BaseAddress1;
	uchar  BaseAddress2;
	uchar  Flags1;
	uchar Flags2Limit2;
	uchar BaseAddress3;
};

struct idtdata{
	ushort Handler1;
	ushort HandlerSegment;
	uchar  Unused;
	uchar  Flags;
	ushort Handler2;
};

typedef struct{ 
	char c;
	uchar color;
} vramdata;

struct trapframe{
	uint edi;
	uint esi;
	uint ebp;
	uint oesp;
	uint ebx;
	uint edx;
	uint ecx; 
	uint eax;

	ushort gs;
	ushort padding1;
	ushort fs;
	ushort padding2;
	ushort es;
	ushort padding3;
	ushort ds;
	ushort padding4;

	uint trapno;

	uint err;
	uint eip;
	ushort cs;
	ushort padding5;
	uint eflags;

	uint esp;
	ushort ss;
	ushort padding6;
};

extern proc process[PROCCOUNT];
extern int CurrentProc;

//gdtidt.c
void	GDT_Clear(struct gdtdata* gdt);
uint	GDT_GetBaseAddress(struct gdtdata* gdt);
void	GDT_SetBaseAddress(struct gdtdata* gdt,uint BaseAddress);
uint 	GDT_GetLimit(struct gdtdata* gdt);
void 	GDT_SetLimit(struct gdtdata* gdt,uint Limit);
ushort	GDT_GetFlags(struct gdtdata* gdt);
void 	GDT_SetFlags(struct gdtdata* gdt,ushort Flags);
void 	GDT_SET(struct gdtdata* gdt,uint Limit,uint BaseAddress,ushort Flags);
void 	GDT_Init();
void 	GDTDUMP(int A);
void 	IDT_Clear(struct idtdata* idt);
uint 	IDT_GetHandler(struct idtdata* idt);
void 	IDT_SetHandler(struct idtdata* idt,uint Handler);
ushort 	IDT_GetHandlerSegment(struct idtdata* idt);
void 	IDT_SetHandlerSegment(struct idtdata* idt,ushort HandlerSegment);
uchar 	IDT_GetFlags(struct idtdata* idt);
void 	IDT_SetFlags(struct idtdata* idt,uchar Flags);
void 	IDT_SET(struct idtdata* idt,ushort Segment,uint Handler,uchar Flags);
void 	IDT_Init();
void 	IDTDUMP(int A);


//kbd.c
void ISR_KBD(struct trapframe* tf);
void vgets(char* dist);
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
void SolvePICLock();
//trapasm.asm
void alltraps();

//video.c
void setcursor(ushort pos);
void vputc(char c);
void vputs(char* str);
void clear();
void schroll();
void Printf(char* form,...);

//string.c
void blockcpy(void* dist,void* src,int size);
void blockswap(void* A,void* B,int blocksize);
void reverse(void* seq,int blocksize,int count);
int strlen(char* str);
void numstr(char* out,int num);
void sformat(char* dist,char* form,...);
int strnum(char* str);
char* chomp(char* str);
int strdiff(char* s1,char* s2);
void bufdump(uchar* str,int size);
void vsformat(char* dist,char* form,va_list ap);
void memdump(uchar* start,int size);

//wrapper.asm
void Halt();
void CopyFar(ushort DistSelecter,void* DistOffset,int DStep,ushort SrcSelecter,void* SrcOffset,int SStep,int n);
int outb(ushort port,uchar data);
int outw(ushort port,ushort data);
unsigned char inb(ushort port);
unsigned short inw(ushort port);
void sti();
void cli();
void syscall(int a,int b,int c,int d,int e,int f);
void farjmp(int eip,int cs);
void lgdt(void* gdtr);
void lidt(void* idtr);
void ltr(ushort selecter);

//fifo.c
void efifo(struct fifo* f,uchar data);
uchar dfifo(struct fifo* f);
struct fifo* createfifo(int size);
void freefifo(struct fifo* f);
char isfifoend(struct fifo* f);

//proc.c
void Proc_Init();
void swtch();
void IdleProcess();
void wait(int *wchan);
void wakeup(int *wchan);

//FAT.c
typedef struct{
	uint size;
	uint pos;
	uchar* buf;
} file;

void tinyls();
void type(char* filename);

file* open(char* filename);
void read(file* fp,char* buf,int size);
void close(file* fp);
int feof(file* fp);

void FATDump();
