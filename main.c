#include"defs.h"


int main(){
//システム初期化
	clear();
	mem_reset();
	IDT_Init();
	KBD_Init();
	sti();
//システム起動
	while(1){
		KBD_Check();
	}
}

