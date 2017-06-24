#include"segment.h"
#include"selecter.h"
#include"video.h"
#include"gdtidt.h"

int main(){
	int A=0;
	char Buf[64];
	clear();
	for(A=0;A<=4;A++){
		GDT_Load(A);
		sformat(Buf,"No.%X\nBaseAddress:%X\nLimit:%X\n",A,GDT_GetBaseAddress(),GDT_GetLimit());
		vputs(Buf);
	}
	Halt();
}
