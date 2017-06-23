#include"segment.h"
#include"selecter.h"
#include"video.h"

int main(){
	int A=0;
	char Buf[64];
	vputs("Hello World!!\n");
	vputs("This is a pen.\n");
	vputs("This is it\nYes we can!!\n          I can do it!!\n");
	for(A=0;;A++){
		sformat(Buf,"Your Number is %d\n",A);
		vputs(Buf);
	}
	Halt();
}
