#include"defs.h"
#include<stdarg.h>

short Counter;

void setcursor(unsigned short pos){
	outb(0x3D4,0x0F);
	outb(0x3D5,Counter);
	outb(0x3D4,0x0E);
	outb(0x3D5,Counter >> 8);
}

void vputc(char c){
	int A;
	char B,C;
	switch(c){
	case '\t':
		Counter=(Counter/8)*8+8;
		break;
	case '\n':
		//B=0;
		if(Counter/80 == 24)schroll();
		VRAM[Counter].c=0;
		Counter++;
		Counter+=80-Counter%80;
		break;
	case '\b':
		if(!(Counter%80) && Counter>0){
			Counter--;
			B=VRAM[Counter].c;
			VRAM[Counter].c=' ';
			if(B==' '){
				while(Counter>0 && B!=0){
					Counter--;
					B=VRAM[Counter].c;
					VRAM[Counter].c=' ';
				}
			}
			if(Counter<0)Counter=0;
		}else{
			Counter--;
			if(Counter<0)Counter=0;
			VRAM[Counter].c=' ';
		}
		break;
	default:
		if(0x20 <= c && c<=0x7e){
			if(Counter == 80*25-1)schroll();
			VRAM[Counter].c=c;
			Counter++;
		}else{
			if(Counter == 80*25-1)schroll();
			VRAM[Counter].c=' ';
			Counter++;
		}
	}
	setcursor(Counter);
}

void vputs(char* str){
	while(*str){
		vputc(*str);
		str++;
	}
}

void clear(){
	for(Counter=0;Counter<80*25;Counter++){
		VRAM[Counter].c=' ';
	}
	Counter=0;
	setcursor(Counter);
}

void schroll(){
	int A;
	int B;
	for(A=0;A<24;A++){
		blockcpy(&VRAM[A*80],&VRAM[(A+1)*80],80*2);
	}
	for(A=0;A<80;A++){
		VRAM[24*80+A].c=' ';
	}
	Counter-=80;
}

void Printf(char* form,...){
	char Buf[64];
	va_list ap;

	va_start(ap,form);
	vsformat(Buf,form,ap);
	vputs(Buf);
	va_end(ap);
}
