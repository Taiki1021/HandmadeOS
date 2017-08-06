#include"defs.h"


void ISR_SYSCALL(struct trapframe* tf){
	switch(tf->eax){
	case 0:break;
	}
}

void WriteVRAM(char* str){
	1:
}

void Syscall_Init(){
	IntHandler[0x80]=ISR_SYSCALL;
}

