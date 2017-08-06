#include"defs.h"

void (*IntHandler[256])(struct trapframe* tf);

void trap(struct trapframe* tf){
	IntHandler[tf->trapno](tf);
	return ;
}

void SolvePICLock(){
	outb(0x20,0x20);
	outb(0xA0,0x20);
}
