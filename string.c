#include"defs.h"
#include<stdarg.h>

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

