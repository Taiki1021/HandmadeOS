#include"segment.h"
#include"selecter.h"
#include"video.h"
#include"gdtidt.h"
#include"trap.h"



void GDTDUMP(int A);
void IDTDUMP(int A);
void GDTIDT_Init();
void ISR_IGNORE(struct trapframe* tf);

int main(){
	int A=0;
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
	
	lidt();
}

void ISR_IGNORE(struct trapframe* tf){
	char Buf[64];
	sformat(Buf,"Interrupt!! No.%d\n",tf->trapno);
	vputs(Buf);
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
