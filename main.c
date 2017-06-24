#include"segment.h"
#include"selecter.h"
#include"video.h"
#include"gdtidt.h"



void GDTDUMP(int A);


int main(){
	int A=0;
	clear();
	GDTDUMP(0);
	GDTDUMP(1);
	GDTDUMP(2);
	GDTDUMP(3);
	GDTDUMP(4);

	Halt();
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
