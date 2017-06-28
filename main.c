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
void ISR_ERROR(struct trapframe* tf);

int main(){
	int A=0;
	char buf[64];
	clear();
	GDTIDT_Init();
	sti();
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
	IntHandler[0]=ISR_ERROR;
	IntHandler[1]=ISR_ERROR;
	IntHandler[2]=ISR_ERROR;
	IntHandler[3]=ISR_ERROR;
	IntHandler[4]=ISR_ERROR;
	IntHandler[5]=ISR_ERROR;
	IntHandler[6]=ISR_ERROR;
	IntHandler[7]=ISR_ERROR;
	IntHandler[8]=ISR_ERROR;
	IntHandler[9]=ISR_ERROR;
	IntHandler[10]=ISR_ERROR;
	IntHandler[11]=ISR_ERROR;
	IntHandler[12]=ISR_ERROR;
	IntHandler[13]=ISR_ERROR;
	IntHandler[14]=ISR_ERROR;
	IntHandler[16]=ISR_ERROR;
	IntHandler[17]=ISR_ERROR;
	IntHandler[18]=ISR_ERROR;
	lidt();
}

void ISR_ERROR(struct trapframe* tf){
	char buf[64];
	char name[16];

	switch(tf->trapno){
	case 0: sformat(name,"Division by zero");break;
	case 1: sformat(name,"Debug fault");break;
	case 3: sformat(name,"Break point");break;
	case 4: sformat(name,"Overflow");break;
	case 5: sformat(name,"Out of BR BOUND");break;
	case 6: sformat(name,"Invalid opcode");break;
	case 7: sformat(name,"Prohibited device");break;
	case 8: sformat(name,"Double fault"); break;
	case 9: sformat(name,"Coreprocessor segment overrun");break;
	case 10: sformat(name,"Invalid TSS");break;
	case 11: sformat(name,"Segment not existing");break;
	case 12: sformat(name,"Stack segment fault");break;
	case 13: sformat(name,"Segmentation fault");break;
	case 14: sformat(name,"Page fault");break;
	case 16: sformat(name,"Calculation fault");break;
	case 17: sformat(name,"Alignment check");break;
	case 18: sformat(name,"Machine check");break;
	}
	printf("SYSTEM ERROR!!:%s\n", name);
	printf("CodeSegment:\t%X\n",tf->cs);
	printf("Code:\t%X\n",tf->eip);
	printf("DataSegment:\t%X\n",tf->ds);
	printf("ErrorCode:%X\n",tf->err);
	if(tf->trapno == 13)Halt();
}

void ISR_IGNORE(struct trapframe* tf){
	char buf[64];
	printf("Interrupt!! No.%d\n",tf->trapno);
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
	Halt();
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
