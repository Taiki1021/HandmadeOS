#include"wrapper.h"
#include"video.h"
#include"gdtidt.h"
#include"memory.h"

#include"selecter.h"


int main(){
//システム初期化
	clear();
	mem_reset();
	IDT_Init();
	sti();
//システム起動
	Halt();
}

