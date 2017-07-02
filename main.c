#include"segment.h"
#include"selecter.h"
#include"video.h"
#include"gdtidt.h"
#include"trap.h"


#define printf(FORMAT,...) sformat(buf,FORMAT,__VA_ARGS__); vputs(buf);

void GDTDUMP(int A);
void IDTDUMP(int A);
void GDTIDT_Init();
void ISR_IGNORE(struct trapframe* tf);

int main(){
	int A=0;
	char buf[64];
	clear();
	GDTIDT_Init();
	sti();
	GDTDUMP(1);
	Halt();
}

void GDTIDT_Init(){
	int A;

	GDT_Load(5);
	GDT_SetBaseAddress(0);
	GDT_SetLimit(256*8);
	GDT_Save(5);

	IDT_Clear();
	IDT_SetHandlerSegment(SysCodeSelecter);
	IDT_SetFlags(IDT_GetFlags() | IDT_D | IDT_P);

	for(A=0;A<256;A++){
		IDT_SetHandler((int)vectors[A]);
		IDT_Save(A);
	}

	for(A=0;A<256;A++){
		IntHandler[A]=ISR_IGNORE;
	}

	lidt();
}

void ISR_IGNORE(struct trapframe* tf){
	char buf[64];
	printf("Interrupt!! 0x%X %d\n",tf->trapno,tf->trapno);
	/*
	printf("EDI:%X\n",tf->edi);
	printf("ESI:%X\n",tf->esi);
	printf("EBP:%X\n",tf->ebp);
	printf("ESP:%X\n",tf->esp);
	printf("EBX:%X\n",tf->ebx);
	printf("EDX:%X\n",tf->edx);
	printf("ECX:%X\n",tf->ecx);
	printf("EAX:%X\n",tf->eax);

	printf("gs:%X\n",tf->gs);
	printf("fs:%X\n",tf->fs);
	printf("es:%X\n",tf->es);
	printf("ds:%X\n",tf->ds);

	printf("trapno:%X\n",tf->trapno);
	printf("err:%X\n",tf->err);
	printf("cs:%X\n",tf->cs);
	printf("eflags:%X\n",tf->eflags);

	printf("ESP:%X\n",tf->esp);
	printf("ss:%X\n",tf->ss);
	*/
	//Halt();
	if(tf->trapno==0x21){
		printf("Data:%d\n",inb(0x0060));
	}
	return ;
}

void GDTDUMP(int A){
	char Buf[64];
	GDT_Load(A);
	sformat(Buf,"GDTNo.%d\n",A);vputs(Buf);
	sformat(Buf,"BaseAddress:\t0x%X\n",GDT_GetBaseAddress());vputs(Buf);
	sformat(Buf,"Limit:\t%X\n",GDT_GetLimit());vputs(Buf);
	sformat(Buf,"Type:\t%d%d%d%d\n",!!(GDT_GetFlags() & GDT_TYPE3),!!(GDT_GetFlags() & GDT_TYPE2),!!(GDT_GetFlags() & GDT_TYPE1),!!(GDT_GetFlags() & GDT_TYPE0));vputs(Buf);
	sformat(Buf,"DPL:\t%d%d\n",!!(GDT_GetFlags() & GDT_DPL1),!!(GDT_GetFlags() & GDT_DPL0));vputs(Buf);
	sformat(Buf,"AVL:\t%d\n",!!(GDT_GetFlags() & GDT_AVL));vputs(Buf);
	sformat(Buf,"PSDG:\t%d%d%d%d\n",!!(GDT_GetFlags() & GDT_P),!!(GDT_GetFlags() & GDT_S),!!(GDT_GetFlags() & GDT_D),!!(GDT_GetFlags() & GDT_G));vputs(Buf);
}

void IDTDUMP(int A){
	char Buf[64];
	IDT_Load(A);
	sformat(Buf,"IDTNo.%d\n",A);vputs(Buf);
	sformat(Buf,"Handler:\t%X\n",IDT_GetHandler());vputs(Buf);
	sformat(Buf,"HandlerSegment:\t%X\n",IDT_GetHandlerSegment());vputs(Buf);
}
