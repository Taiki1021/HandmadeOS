#include"segment.h"
#include"selecter.h"
#include"video.h"

int Counter;

void putchr(char c){
	int A;
	if(c=='\n'){
		if(Counter/80 == 24)schrool();
		Counter+=80-Counter%80;
	}else{
		if(Counter == 80*25-1)schrool();
		CopyFar(VideoSelecter,(char*)(Counter*2),1,SysDataSelecter,&c,1,1);
		Counter++;
	}

}

void putstr(char* str){
	while(*str){
		putchr(*str);
		str++;
	}
}

void clear(){
	for(Counter=0;Counter<80*25;Counter++){
		CopyFar(VideoSelecter,(char*)(Counter*2),1,SysDataSelecter," ",1,1);
	}
	Counter=0;
}

void schrool(){
	int A;
	for(A=0;A<24;A++){
		CopyFar(VideoSelecter,(char*)(A*80*2),1,VideoSelecter,(char*)((A+1)*80*2),1,80);
	}
	Counter-=80;
}
