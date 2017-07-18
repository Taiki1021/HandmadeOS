#include"defs.h"


int main(){
//システム初期化
	clear();
	mem_reset();
	IDT_Init();
	KBD_Init();
	Proc_Init();
	sti();

//システム起動
	IdleProcess();
}

