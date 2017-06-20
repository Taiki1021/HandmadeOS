void PM_start();
void Halt();
void WriteChar(char,int);
void WriteStr(char*);
void WriteFar(unsigned short Selecter,int offset,void* src,int n);

extern char* HelloMSG;
