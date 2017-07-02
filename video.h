void setcursor(unsigned short pos);
void vputc(char c);
void vputs(char* str);
void clear();
void schroll();

void blockcpy(void* dist,void* src,int size);
void blockswap(void* A,void* B,int blocksize);
void reverse(void* seq,int blocksize,int count);
int strlen(char* str);
void numstr(char* out,int num);
void sformat(char* dist,char* form,...);
