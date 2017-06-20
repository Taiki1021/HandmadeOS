#include"segment.h"
#include"selecter.h"

int main(){
	char str[]={'H',0x06,'e',0x06,'l',0x06,'l',0x06,'o',0x06};
	WriteFar(VideoSelecter,0,str,10);
	Halt();
}
