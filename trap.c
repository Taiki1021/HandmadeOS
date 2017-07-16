#include"trap.h"
#include"video.h"

void (*IntHandler[256])(struct trapframe* tf);

void trap(struct trapframe* tf){
	IntHandler[tf->trapno](tf);
	return ;
}
