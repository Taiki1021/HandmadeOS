void Halt();
void CopyFar(unsigned short DistSelecter,char* DistOffset,int DStep,unsigned short SrcSelecter,char* SrcOffset,int SStep,int n);
int outb(unsigned short port,unsinged char data);
int outw(unsigned short port,unsinged short data);
void sti();
void cli();

