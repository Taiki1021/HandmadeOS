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
		if(Counter == 80*25-1)schroll();
		VRAM[Counter].c=c;
		Counter++;
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
	for(A=0;A<24;A++){
		VRAM[A*80]=VRAM[(A+1)*80];
	}
	for(A=0;A<80;A++){
		VRAM[24*80+A].c=' ';
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
	char neg=0;
	if(num==0){
		*head='0';
		head++;
	}
	if(num<0){
		num=-num;
		neg=1;
	}
	while(num>0){
		*head=num%10+'0';
		num/=10;
		head++;
	}
	if(neg){
		*head='-';
		head++;
	}
	*head=0;
	reverse(out,sizeof(char),strlen(out));
}

void hexstr(char* out,int num,char upper){
	char* head=out;
	char neg=0;
	if(num==0){
		*head='0';
		head++;
	}
	if(num<0){
		num=-num;
		neg=1;
	}
	while(num>0){
		if(num%0x10<10){
			*head=num%0x10+'0';
		}else{
			if(upper){
				*head=num%0x10-10+'A';
			}else{
				*head=num%0x10-10+'a';
			}
		}
		num/=0x10;
		head++;
	}
	if(neg){
		*head='-';
		head++;
	}
	*head=0;
	reverse(out,sizeof(char),strlen(out));
}

void vsformat(char* dist,char* form,va_list ap){
	int A;
	char* head=dist;
	char* sub;

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
				break;
			case 'x':
				hexstr(head,va_arg(ap,int),0);
				head+=strlen(head);
				break;
			case 'X':
				hexstr(head,va_arg(ap,int),1);
				head+=strlen(head);
				break;
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
	return ;
}


void sformat(char* dist,char* form,...){
	va_list ap;

	va_start(ap,form);
	vsformat(dist,form,ap);
	va_end(ap);
	return ;
}

void Printf(char* form,...){
	char Buf[64];
	va_list ap;

	va_start(ap,form);
	vsformat(Buf,form,ap);
	vputs(Buf);
	va_end(ap);
}
