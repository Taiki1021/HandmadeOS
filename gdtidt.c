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

void GDT_SET(struct gdtdata* gdt,uint Limit,uint BaseAddress,ushort Flags){
	GDT_Clear(gdt);
	GDT_SetBaseAddress(gdt,BaseAddress);
	GDT_SetLimit(gdt,Limit);
	GDT_SetFlags(gdt,GDT_GetFlags(gdt) | Flags);
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

void IDT_SET(struct idtdata* idt,ushort Segment,uint Handler,uchar Flags){
	IDT_Clear(idt);
	IDT_SetHandler(idt,Handler);
	IDT_SetHandlerSegment(idt,Segment);
	IDT_SetFlags(idt,IDT_GetFlags(idt) | Flags);
}

void ISR_IGNORE(struct trapframe* tf){
	if(tf->trapno<0x20){
		Printf("Interrupt!! %d\n",tf->trapno,tf->trapno);

		Printf("Current process:%d\n",CurrentProc);
	
		Printf("EDI:0x%X\n",tf->edi);
		Printf("ESI:0x%X\n",tf->esi);
		Printf("EBP:0x%X\n",tf->ebp);
		Printf("ESP:0x%X\n",tf->esp);
		Printf("EBX:0x%X\n",tf->ebx);
		Printf("EDX:0x%X\n",tf->edx);
		Printf("ECX:0x%X\n",tf->ecx);
		Printf("EAX:0x%X\n",tf->eax);

		Printf("EIP:0x%X\n",tf->eip);
		Printf("ESP:0x%X\n",tf->esp);

		Printf("gs:0x%X\n",tf->gs);
		Printf("fs:0x%X\n",tf->fs);
		Printf("es:0x%X\n",tf->es);
		Printf("ds:0x%X\n",tf->ds);
		Printf("cs:0x%X\n",tf->cs);
		Printf("ss:0x%X\n",tf->ss);

		Printf("trapno:%d\n",tf->trapno);
		Printf("err:0x%X\n",tf->err);
		Printf("eflags:0x%X\n",tf->eflags);

		while(1)Halt();
	}
	SolvePICLock();
	return ;
}

void IDT_Init(){
	int A;
	uchar idtr[6];

	for(A=0;A<256;A++){
		IDT_SET(&IDT[A],SysCodeSelecter,(uint)vectors[A], IDT_D | IDT_P);
	}

	for(A=0;A<256;A++){
		IntHandler[A]=ISR_IGNORE;
	}

	//TODO:全てのハンドラ定義をここに書く

	idtr[0]=(uchar)(IDTCOUNT*8-1);
	idtr[1]=(uchar)((IDTCOUNT*8-1)>>8);
	idtr[2]=(uchar)((uint)(IDT));
	idtr[3]=(uchar)((uint)(IDT)>>8);
	idtr[4]=(uchar)((uint)(IDT)>>16);
	idtr[5]=(uchar)((uint)(IDT)>>24);
	lidt(idtr); //IDTの登録
}

void GDT_Init(){
	int A;
	uchar gdtr[6];

	GDT_SET(&GDT[0],0,0,0);
	GDT_SET(&GDT[1],0x8200,0x10000,	GDT_P|GDT_S|GDT_D|      GDT_TYPE3|GDT_TYPE1);
	GDT_SET(&GDT[2],0xFFFFF,0x0,	GDT_P|GDT_S|GDT_D|GDT_G|          GDT_TYPE1);
	GDT_SET(&GDT[3],0x0,0x0,	GDT_P|GDT_S|GDT_D|      GDT_TYPE3|GDT_TYPE1|GDT_DPL1|GDT_DPL0);
	GDT_SET(&GDT[4],0x0,0x0,	GDT_P|GDT_S|GDT_D|GDT_G|          GDT_TYPE1|GDT_DPL1|GDT_DPL0);

	for(A=0;A<TSSCOUNT;A++){
		GDT_SET(&GDT[A+SEGMENTCOUNT],104,0x0,	GDT_P|GDT_TYPE3|GDT_TYPE0);
	}

	gdtr[0]=(uchar)(GDTCOUNT*8-1);
	gdtr[1]=(uchar)((GDTCOUNT*8-1)>>8);
	gdtr[2]=(uchar)((uint)(GDT));
	gdtr[3]=(uchar)((uint)(GDT)>>8);
	gdtr[4]=(uchar)((uint)(GDT)>>16);
	gdtr[5]=(uchar)((uint)(GDT)>>24);
	lgdt(gdtr); //GDTの登録
}


void GDTDUMP(int A){
	struct gdtdata* g;
	g=&GDT[A];
	Printf("%d Limit:0x%X Base:0x%X Flag:%X\n",A,GDT_GetLimit(g),GDT_GetBaseAddress(g),GDT_GetFlags(g));
}

void IDTDUMP(int A){
	struct idtdata* i;
	i=&IDT[A];
	Printf("%d Segment:%X Handler:%X Flags:%X\n",A,IDT_GetHandlerSegment(i),IDT_GetHandler(i),IDT_GetFlags(i));
}
