#include"segment.h"
#include"selecter.h"
#include"video.h"
#include"gdtidt.h"
#include"trap.h"
#include"memory.h"


void GDTDUMP(int A);
void IDTDUMP(int A);
void GDTIDT_Init();
void ISR_IGNORE(struct trapframe* tf);

int main(){
	int A=0;
	clear();
	GDTDUMP(1);
	Printf("%X\n",mem_reset);
	mem_reset();
	GDTIDT_Init();
	sti();
//	mem_dump(0,1);
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
	//printf("Interrupt!! 0x%X %d\n",tf->trapno,tf->trapno);
/*	
	Printf("EDI:%X\n",tf->edi);
	Printf("ESI:%X\n",tf->esi);
	Printf("EBP:%X\n",tf->ebp);
	Printf("ESP:%X\n",tf->esp);
	Printf("EBX:%X\n",tf->ebx);
	Printf("EDX:%X\n",tf->edx);
	Printf("ECX:%X\n",tf->ecx);
	Printf("EAX:%X\n",tf->eax);

	Printf("gs:%X\n",tf->gs);
	Printf("fs:%X\n",tf->fs);
	Printf("es:%X\n",tf->es);
	Printf("ds:%X\n",tf->ds);

	Printf("trapno:%X\n",tf->trapno);
	Printf("err:%X\n",tf->err);
	Printf("cs:%X\n",tf->cs);
	Printf("eflags:%X\n",tf->eflags);

	Printf("ESP:%X\n",tf->esp);
	Printf("ss:%X\n",tf->ss);
	*/
	//Halt();
	if(tf->trapno==0x21){
		Printf("Data:%d\n",inb(0x0060));
	}
	return ;
}

void GDTDUMP(int A){
	GDT_Load(A);
	Printf("GDTNo.%d\n",A);
	Printf("BaseAddress:\t0x%X\n",GDT_GetBaseAddress());
	Printf("Limit:\t%X\n",GDT_GetLimit());
	Printf("Type:\t%d%d%d%d\n",!!(GDT_GetFlags() & GDT_TYPE3),!!(GDT_GetFlags() & GDT_TYPE2),!!(GDT_GetFlags() & GDT_TYPE1),!!(GDT_GetFlags() & GDT_TYPE0));
	Printf("DPL:\t%d%d\n",!!(GDT_GetFlags() & GDT_DPL1),!!(GDT_GetFlags() & GDT_DPL0));
	Printf("AVL:\t%d\n",!!(GDT_GetFlags() & GDT_AVL));
	Printf("PSDG:\t%d%d%d%d\n",!!(GDT_GetFlags() & GDT_P),!!(GDT_GetFlags() & GDT_S),!!(GDT_GetFlags() & GDT_D),!!(GDT_GetFlags() & GDT_G));
}

void IDTDUMP(int A){
	IDT_Load(A);
	Printf("IDTNo.%d\n",A);
	Printf("Handler:\t%X\n",IDT_GetHandler());
	Printf("HandlerSegment:\t%X\n",IDT_GetHandlerSegment());
}
