#include"segment.h"
#include"selecter.h"
#include"video.h"
#include<stdarg.h>

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
//	outb(0x3D4,0x0F);
//	outb(0x3D5,Counter);
//	outb(0x3D4,0x0E);
//	outb(0x3D5,Counter >> 8);
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






void blockcpy(void* dist,void* src,int size){
	int A;
	for(A=0;A<size;A++){
		((char*)dist)[A]=((char*)src)[A];
	}
}

void blockswap(void* A,void* B,int blocksize){
	char C[32];
	blockcpy(C,A,blocksize);
	blockcpy(A,B,blocksize);
	blockcpy(B,C,blocksize);
}

void reverse(void* seq,int blocksize,int count){
	int A;
	for(A=0;A<=count/2-1;A++){
		blockswap(seq+blocksize*A,seq+blocksize*(count-A-1),blocksize);
	}
}

int strlen(char* str){
	int A;
	for(A=0;*str++;A++);
	return A;
}

void numstr(char* out,int num){
	char* head=out;
	if(num==0){
		*head='0';
		head++;
	}
	while(num>0){
		*head=num%10+'0';
		num/=10;
		head++;
	}
	*head=0;
	reverse(out,sizeof(char),strlen(out));
}

void sformat(char* dist,char* form,...){
	int A;
	char* head=dist;
	char* sub;
	va_list ap;

	va_start(ap,form);

	for(A=0;form[A];A++){
		if(form[A]=='%'){
			switch(form[A+1]){
			case 'd':
				numstr(head,va_arg(ap,int));
				head+=strlen(head);
				break;
			case 's':
				sub=va_arg(ap,char*);
				blockcpy(head,sub,sizeof(char)*strlen(sub));
				head+=strlen(sub);
				break;
			case 'c':
				*head=va_arg(ap,int);
				head++;
			case '%':
				*head='%';
				head++;
				break;
			}
			A+=1;
		}else{
			*head=form[A];
			head++;
		}
	}
	*head=0;
	va_end(ap);
}
