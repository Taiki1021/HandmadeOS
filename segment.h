void Halt();
void CopyFar(unsigned short DistSelecter,char* DistOffset,int DStep,unsigned short SrcSelecter,char* SrcOffset,int SStep,int n);
int outb(unsigned short port,unsigned char data);
int outw(unsigned short port,unsigned short data);
void sti();
void cli();

