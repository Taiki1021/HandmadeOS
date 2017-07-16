#include"wrapper.h"
#include"video.h"
#include"gdtidt.h"
#include"memory.h"
#include"trap.h"
#include"kbd.h"


int main(){
//システム初期化
	clear();
	mem_reset();
	KBD_Init();
	IDT_Init();
	IntHandler[0x21]=ISR_KBD;
	sti();
//システム起動
	while(1){
		KBD_Check();
	}
}

