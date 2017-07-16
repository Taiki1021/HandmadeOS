#include"trap.h"
#define KBDBUFFERSIZE 1024

void ISR_KBD(struct trapframe* tf);
char vgetc();
void KBD_Check();
void KBD_Init();
