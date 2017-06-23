#include"segment.h"
#include"selecter.h"
#include"video.h"

unsigned short Counter;

void vputc(char c){
	int A;
	if(c=='\n'){
		if(Counter/80 == 24)schroll();
		Counter+=80-Counter%80;
	}else{
		if(Counter == 80*25-1)schroll();
		CopyFar(VideoSelecter,(char*)(Counter*2),1,SysDataSelecter,&c,1,1);
		Counter++;
	}
	outb(0x3D4,0x0F);
	outb(0x3D5,Counter);
	outb(0x3D4,0x0E);
	outb(0x3D5,Counter >> 8);
}

void vputs(char* str){
	while(*str){
		vputc(*str);
		str++;
	}
}

void clear(){
	for(Counter=0;Counter<80*25;Counter++){
		CopyFar(VideoSelecter,(char*)(Counter*2),1,SysDataSelecter," ",1,1);
	}
	Counter=0;
}

void schroll(){
	int A;
	for(A=0;A<24;A++){
		CopyFar(VideoSelecter,(char*)(A*80*2),1,VideoSelecter,(char*)((A+1)*80*2),1,80);
	}
	Counter-=80;
}

