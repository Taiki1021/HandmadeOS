#include"defs.h"

#define SEGMENTTABLESIZE 1024

struct segment{
	void* addr;
	unsigned int size;
};

struct segment FreeMem[SEGMENTTABLESIZE];

void mem_reset(){
	int A;
	for(A=0;A<SEGMENTTABLESIZE;A++){
		FreeMem[A].addr=0;
		FreeMem[A].size=0;
	}
	FreeMem[0].addr=(void*)0x100000;
	FreeMem[0].size=256*1024*1024-0x100000;
	return ;
}

void* mem_alloc(int size){
	int A,B;
	void* r;
	for(A=0;A<SEGMENTTABLESIZE;A++){
		if(FreeMem[A].size>=size){
			r=FreeMem[A].addr;
			FreeMem[A].addr+=size;
			FreeMem[A].size-=size;
			if(FreeMem[A].size==0){
				for(B=A;B<SEGMENTTABLESIZE-1;B++){
					FreeMem[B].addr=FreeMem[B+1].addr;
					FreeMem[B].size=FreeMem[B+1].size;
				}
				FreeMem[SEGMENTTABLESIZE-1].addr=0;
				FreeMem[SEGMENTTABLESIZE-1].size=0;
			}
			return r;
		}
	}
	return 0;
}

void mem_reserve(void* address,int size){
	int A,B;
	for(A=0;A<SEGMENTTABLESIZE;A++){
		if(FreeMem[A].addr <= address && address < FreeMem[A].addr+FreeMem[A].size){
			if(FreeMem[A].addr == address){
				FreeMem[A].addr += size;
				FreeMem[A].size -= size;
			}else if(FreeMem[A].addr+FreeMem[A].size ==  address + size){
				FreeMem[A].size -= size;
			}
			if(FreeMem[A].size == 0){
				for(B=A;B<SEGMENTTABLESIZE;B++){
					FreeMem[B].addr=FreeMem[B+1].addr;
					FreeMem[B].size=FreeMem[B+1].size;
				}
			}
			return ;
		}
	}
	return ;
}


void mem_dump(int start,int end){
	int A;
	for(A=start;A<=end;A++){
		Printf("%d\tSTART : %X\tEND : %X\tSIZE : %X\n",A,FreeMem[A].addr,FreeMem[A].addr+FreeMem[A].size-1,FreeMem[A].size);
	}
	return ;
}

int mem_num(){
	int A;
	for(A=0;A<SEGMENTTABLESIZE;A++){
		if(!FreeMem[A].size)break;
	}
	return A;
}


void conj(int A){
	int B;
	if((uint)FreeMem[A].addr+(uint)FreeMem[A].size==(uint)FreeMem[A+1].addr){
		FreeMem[A].size+=FreeMem[A+1].size;
		for(B=A+1;B<SEGMENTTABLESIZE-1-1;B++){
			FreeMem[B].addr=FreeMem[B+1].addr;
			FreeMem[B].size=FreeMem[B+1].size;
		}
	}
}

void mem_free(void* address,int size){
	int A,B;

	if((uint)address<(uint)FreeMem[0].addr){
		for(B=SEGMENTTABLESIZE-1;B>0;B--){
			FreeMem[B].addr=FreeMem[B-1].addr;
			FreeMem[B].size=FreeMem[B-1].size;
		}
		FreeMem[0].addr=address;
		FreeMem[0].size=size;
		conj(0);
		return;
	}

	for(A=0;A<SEGMENTTABLESIZE-1;A++){
		if( ((uint)FreeMem[A].addr + (uint)FreeMem[A].size <= (uint)address) && ((uint)address < (uint)FreeMem[A+1].addr) ){
			for(B=SEGMENTTABLESIZE-1;B>A+1;B--){
				FreeMem[B].addr=FreeMem[B-1].addr;
				FreeMem[B].size=FreeMem[B-1].size;
			}
			FreeMem[A+1].addr=address;
			FreeMem[A+1].size=size;
			conj(A+1);
			conj(A);
			return ;
		}
	}
	return ;
}
