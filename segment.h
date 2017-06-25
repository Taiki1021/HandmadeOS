void Halt();
void CopyFar(unsigned short DistSelecter,void* DistOffset,int DStep,unsigned short SrcSelecter,void* SrcOffset,int SStep,int n);
int outb(unsigned short port,unsigned char data);
int outw(unsigned short port,unsigned short data);
void sti();
void cli();
void lidt();
void isr_ignore();

