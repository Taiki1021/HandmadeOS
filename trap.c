#include"trap.h"

void (*IntHandler[256])(struct trapframe* tf);

void trap(struct trapframe* tf){
	IntHandler[tf->trapno](tf);
	return ;
}
