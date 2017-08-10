#include"defs.h"

struct fifo* KbdFifo;
char InputBuf[KBDBUFFERSIZE];
char *InputBufHead;
char SHIFT;
char INPUTEND;

char KeyCodeU[0x7F]={
	0  ,0  ,'!','\"','#','$','%','&','\'','(',')',' ','=','~','\b','\t',
	'Q','W','E','R','T','Y','U','I','O','P','`','{','\n',0  ,'A','S',
	'D','F','G','H','J','K','L','+','*',0  ,0  ,'}','Z','X','C','V',
	'B','N','M','<','>','?','_',0 ,0 ,' ',0  ,0  ,0  ,0  ,0  ,0  ,0  ,0
};

char KeyCodeD[0x7F]={
	0  ,0  ,'1','2','3','4','5','6','7','8','9','0','-','^','\b','\t',
	'q','w','e','r','t','y','u','i','o','p','@','[','\n',0  ,'a','s',
	'd','f','g','h','j','k','l',';',':',0  ,0  ,']','z','x','c','v',
	'b','n','m',',','.','/' ,'\\',0  ,0,' '  ,0  ,0  ,0  ,0  ,0  ,0  ,0
};

void ISR_KBD(struct trapframe* tf){
	efifo(KbdFifo,inb(0x60));
	SolvePICLock();
	return ;
}

void KBD_Check(){
	unsigned char A;
	char B;
	while(!isfifoend(KbdFifo)){
		A=dfifo(KbdFifo);
		if(!INPUTEND){
		if(!(A & 0x80)){
			switch(A){
			case 0x36:
			case 0x2A:
				SHIFT=1;
				break;
			default:
				if(SHIFT){
					vputc(KeyCodeU[A]);
					if(KeyCodeU[A]!='\b'){
						*InputBufHead++=KeyCodeU[A];
					}else{
						InputBufHead--;
					}
					if(KeyCodeU[A]=='\n'){
						INPUTEND=1;
						*InputBufHead=0;
						InputBufHead=InputBuf;
						wakeup(&INPUTEND);
					}
				}else{	
					vputc(KeyCodeD[A]);
					if(KeyCodeD[A]!='\b'){
						*InputBufHead++=KeyCodeD[A];
					}else{
						InputBufHead--;
					}
					if(KeyCodeD[A]=='\n'){
						INPUTEND=1;
						*InputBufHead=0;
						InputBufHead=InputBuf;
						wakeup(&INPUTEND);
					}
				}
				break;
			}
		}else{
			switch(A^0x80){
			case 0x36:
			case 0x2A:
				SHIFT=0;
				break;
			}
		}
		}
	}
}

void KBD_Init(){
	KbdFifo=createfifo(KBDBUFFERSIZE);
	InputBufHead=InputBuf;
	IntHandler[0x21]=ISR_KBD;
	KeyCodeD[0x73]='\\';
	KeyCodeU[0x73]='_';
	KeyCodeD[0x7D]='\\';
	KeyCodeU[0x7D]='|';
	INPUTEND=1;
	SHIFT=0;
}

void vgets(char* dist){
	int A;
	INPUTEND=0;
	wait(&INPUTEND);
	for(A=0;InputBuf[A];A++)*dist++=InputBuf[A];
	*dist=0;
}
