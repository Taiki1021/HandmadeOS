#include"defs.h"

struct GDT{
	unsigned short Limit1;
	unsigned short BaseAddress1;
	unsigned char  BaseAddress2;
	unsigned char  Flags1;
	unsigned char Flags2Limit2;
	unsigned char BaseAddress3;
};

struct IDT{
	unsigned short Handler1;
	unsigned short HandlerSegment;
	unsigned char  Unused;
	unsigned char  Flags;
	unsigned short Handler2;
};


struct GDT CurrentGDT;
struct IDT CurrentIDT;

void GDT_Load(int no){
	CopyFar(SysDataSelecter,&CurrentGDT,1,GDTSelecter,(void*)(8*no),1,8);
}

void GDT_Save(int no){
	CopyFar(GDTSelecter,(void*)(8*no),1,SysDataSelecter,&CurrentGDT,1,8);
}

void GDT_Clear(){
	CurrentGDT.Limit1=0;
	CurrentGDT.BaseAddress1=0;
	CurrentGDT.BaseAddress2=0;
	CurrentGDT.Flags1=0;
	CurrentGDT.Flags2Limit2=0;
	CurrentGDT.BaseAddress3=0;
}

unsigned int GDT_GetBaseAddress(){
	return CurrentGDT.BaseAddress3<<(16+8) | CurrentGDT.BaseAddress2<<16 | CurrentGDT.BaseAddress1;
}

void GDT_SetBaseAddress(unsigned int BaseAddress){
	CurrentGDT.BaseAddress1=BaseAddress;
	CurrentGDT.BaseAddress2=BaseAddress >> 16;
	CurrentGDT.BaseAddress3=BaseAddress >>(16+8);
}

unsigned int GDT_GetLimit(){
	return (CurrentGDT.Flags2Limit2 & 0x0F)<<16 | CurrentGDT.Limit1;
}

void GDT_SetLimit(unsigned int Limit){
	CurrentGDT.Limit1 = Limit;
	CurrentGDT.Flags2Limit2 &= 0xF0;
	CurrentGDT.Flags2Limit2 |= (Limit >> 16) & 0x000F;
}

unsigned short GDT_GetFlags(){
	return (CurrentGDT.Flags2Limit2 & 0xF0)<<4 | CurrentGDT.Flags1;
}

void GDT_SetFlags(unsigned short Flags){
	CurrentGDT.Flags1 = Flags;
	CurrentGDT.Flags2Limit2 &= 0x0F;
	CurrentGDT.Flags2Limit2 |= ((Flags >> 8) & 0x0F) << 4; 
}


void IDT_Load(int no){
	CopyFar(SysDataSelecter,&CurrentIDT,1,IDTSelecter,(void*)(8*no),1,8);
}

void IDT_Save(int no){
	CopyFar(IDTSelecter,(void*)(8*no),1,SysDataSelecter,&CurrentIDT,1,8);
}



void IDT_Clear(){
	CurrentIDT.Handler1=0;
	CurrentIDT.HandlerSegment=0;
	CurrentIDT.Unused=0;
	CurrentIDT.Flags=0x6;
	CurrentIDT.Handler2=0;
}

unsigned int IDT_GetHandler(){
	return CurrentIDT.Handler2 << 16 | CurrentIDT.Handler1;
}

void IDT_SetHandler(unsigned int Handler){
	CurrentIDT.Handler1 = Handler;
	CurrentIDT.Handler2 = Handler >> 16;
}

unsigned short IDT_GetHandlerSegment(){
	return CurrentIDT.HandlerSegment;
}

void IDT_SetHandlerSegment(unsigned short HandlerSegment){
	CurrentIDT.HandlerSegment=HandlerSegment;
}

unsigned char IDT_GetFlags(){
	return CurrentIDT.Flags;
}

void IDT_SetFlags(unsigned char Flags){
	CurrentIDT.Flags=Flags;
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

void IDT_Init(){
	int A;
	
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
