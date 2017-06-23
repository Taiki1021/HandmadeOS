#include"segment.h"
#include"selecter.h"
#include"video.h"

int main(){
	int A=0;
	putstr("Hello World!!\n");
	putstr("This is a pen.\n");
	putstr("This is it\nYes we can!!\n          I can do it!!");
	while(1){
		putchr(A%10+'0');
		putchr('\n');
		A++;
	}
	Halt();
}
