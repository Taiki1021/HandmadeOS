#include"defs.h"

void GDT_Clear(struct gdtdata* gdt){
	gdt->Limit1=0;
	gdt->BaseAddress1=0;
	gdt->BaseAddress2=0;
	gdt->Flags1=0;
	gdt->Flags2Limit2=0;
	gdt->BaseAddress3=0;
}

unsigned int GDT_GetBaseAddress(struct gdtdata* gdt){
	return gdt->BaseAddress3<<(16+8) | gdt->BaseAddress2<<16 | gdt->BaseAddress1;
}

void GDT_SetBaseAddress(struct gdtdata* gdt,unsigned int BaseAddress){
	gdt->BaseAddress1=BaseAddress;
	gdt->BaseAddress2=BaseAddress >> 16;
	gdt->BaseAddress3=BaseAddress >>(16+8);
}

unsigned int GDT_GetLimit(struct gdtdata* gdt){
	return (gdt->Flags2Limit2 & 0x0F)<<16 | gdt->Limit1;
}

void GDT_SetLimit(struct gdtdata* gdt,unsigned int Limit){
	gdt->Limit1 = Limit;
	gdt->Flags2Limit2 &= 0xF0;
	gdt->Flags2Limit2 |= (Limit >> 16) & 0x000F;
}

unsigned short GDT_GetFlags(struct gdtdata* gdt){
	return (gdt->Flags2Limit2 & 0xF0)<<4 | gdt->Flags1;
}

void GDT_SetFlags(struct gdtdata* gdt,unsigned short Flags){
	gdt->Flags1 = Flags;
	gdt->Flags2Limit2 &= 0x0F;
	gdt->Flags2Limit2 |= ((Flags >> 8) & 0x0F) << 4; 
}


void IDT_Clear(struct idtdata* idt){
	idt->Handler1=0;
	idt->HandlerSegment=0;
	idt->Unused=0;
	idt->Flags=0x6;
	idt->Handler2=0;
}

unsigned int IDT_GetHandler(struct idtdata* idt){
	return idt->Handler2 << 16 | idt->Handler1;
}

void IDT_SetHandler(struct idtdata* idt,unsigned int Handler){
	idt->Handler1 = Handler;
	idt->Handler2 = Handler >> 16;
}

unsigned short IDT_GetHandlerSegment(struct idtdata* idt){
	return idt->HandlerSegment;
}

void IDT_SetHandlerSegment(struct idtdata* idt,unsigned short HandlerSegment){
	idt->HandlerSegment=HandlerSegment;
}

unsigned char IDT_GetFlags(struct idtdata* idt){
	return idt->Flags;
}

void IDT_SetFlags(struct idtdata* idt,unsigned char Flags){
	idt->Flags=Flags;
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
		Printf("Data:%X\n",inb(0x0060));
	}
	return ;
}

void IDT_Init(){
	int A;
	for(A=0;A<256;A++){
		IDT_Clear(&IDT[A]);
		IDT_SetHandlerSegment(&IDT[A],SysCodeSelecter);
		IDT_SetFlags(&IDT[A],IDT_GetFlags(&IDT[A]) | IDT_D | IDT_P);
		IDT_SetHandler(&IDT[A],(int)vectors[A]);
	}

	for(A=0;A<256;A++){
		IntHandler[A]=ISR_IGNORE;
	}
}


void GDTDUMP(int A){
	struct gdtdata* g;
	g=&GDT[A];
	Printf("GDTNo.%d\n",A);
	Printf("BaseAddress:\t0x%X\n",GDT_GetBaseAddress(g));
	Printf("Limit:\t%X\n",GDT_GetLimit(g));
	Printf("Type:\t%d%d%d%d\n",!!(GDT_GetFlags(g) & GDT_TYPE3),!!(GDT_GetFlags(g) & GDT_TYPE2),!!(GDT_GetFlags(g) & GDT_TYPE1),!!(GDT_GetFlags(g) & GDT_TYPE0));
	Printf("DPL:\t%d%d\n",!!(GDT_GetFlags(g) & GDT_DPL1),!!(GDT_GetFlags(g) & GDT_DPL0));
	Printf("AVL:\t%d\n",!!(GDT_GetFlags(g) & GDT_AVL));
	Printf("PSDG:\t%d%d%d%d\n",!!(GDT_GetFlags(g) & GDT_P),!!(GDT_GetFlags(g) & GDT_S),!!(GDT_GetFlags(g) & GDT_D),!!(GDT_GetFlags(g) & GDT_G));
}

void IDTDUMP(int A){
	struct idtdata* i;
	i=&IDT[A];
	Printf("IDTNo.%d\n",A);
	Printf("Handler:\t%X\n",IDT_GetHandler(i));
	Printf("HandlerSegment:\t%X\n",IDT_GetHandlerSegment(i));
}
