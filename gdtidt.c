#include"segment.h"
#include"gdtidt.h"

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
