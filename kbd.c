#include"kbd.h"
#include"video.h"
#include"wrapper.h"

unsigned char KbdEvBuffer[KBDBUFFERSIZE];
char InputBuffer[KBDBUFFERSIZE];
int KBDRHead;
int KBDWHead;
int InputRHead;
int InputWHead;
char SHIFT;


char KeyCodeU[0x7F]={
	0  ,0  ,'!','\"','#','$','%','&','\'','(',')',' ','=','~','\b','\t',
	'Q','W','E','R','T','Y','U','I','O','P','`','{','\n',0  ,'A','S',
	'D','F','G','H','J','K','L','+','*',0  ,0  ,'}','Z','X','C','V',
	'B','N','M','<','>','?','_',0  ,' ',0  ,0  ,0  ,0  ,0  ,0  ,0  ,0
};

char KeyCodeD[0x7F]={
	0  ,0  ,'1','2','3','4','5','6','7','8','9','0','-','^','\b','\t',
	'q','w','e','r','t','y','u','i','o','p','@','[','\n',0  ,'a','s',
	'd','f','g','h','j','k','l',';',':',0  ,0  ,']','z','x','c','v',
	'b','n','m',',','.','/' ,'\\',0  ,' ',0  ,0  ,0  ,0  ,0  ,0  ,0  ,0
};

void ISR_KBD(struct trapframe* tf){
	KbdEvBuffer[KBDWHead]=inb(0x60);
	KBDWHead=(KBDWHead+1)%KBDBUFFERSIZE;
	return ;
}

void KBD_Check(){
	unsigned char A;
	char B;
	while(KBDRHead!=KBDWHead){
		A=KbdEvBuffer[KBDRHead];
		KBDRHead=(KBDRHead+1)%KBDBUFFERSIZE;
		if(!(A & 0x80)){
			switch(A){
			case 0x36:
			case 0x2A:
				SHIFT=1;
				break;
			default:
				if(SHIFT){
					B=KeyCodeU[A];
				}else{	
					B=KeyCodeD[A];
				}
				vputc(B);
				InputBuffer[InputWHead]=B;
				InputWHead=(InputWHead+1)%KBDBUFFERSIZE;
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

void KBD_Init(){
	KBDRHead=KBDWHead=0;
	InputRHead=InputWHead=0;
	SHIFT=0;
}

char vgetc(){
	char R;
	if(InputRHead!=InputWHead){
		R=InputBuffer[InputRHead];
		InputRHead=(InputRHead+1)%KBDBUFFERSIZE;
		return R;
	}else{
		return 0;
	}
}
