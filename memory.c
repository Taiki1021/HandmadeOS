#include"memory.h"
#include"video.h"

#define SEGMENTTABLESIZE 10

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


void mem_free(void* address,int size){
	int A,B;

	for(A=0;A<SEGMENTTABLESIZE-1;A++){
		if( ((int)FreeMem[A].addr < (int)address) && ((int)address <= (int)FreeMem[A+1].addr) ){
			if(FreeMem[A].addr+FreeMem[A].size == address){
				FreeMem[A].size += size;
				return ;
			}
			if((int)address+size == (int)FreeMem[A+1].addr){
				FreeMem[A].addr = address;
				FreeMem[A].size += size;
				if(FreeMem[A].addr+FreeMem[A].size == FreeMem[A+1].addr){
					FreeMem[A].size += FreeMem[A+1].size;
					for(B=A+1;B<SEGMENTTABLESIZE-1;B++){
						FreeMem[B].addr = FreeMem[B+1].addr;
						FreeMem[B].size = FreeMem[B+1].size;
					}
					FreeMem[B].addr=0;
					FreeMem[B].size=0;
				}
				return ;
			}
			for(B=SEGMENTTABLESIZE-1;B>A;B--){
				FreeMem[B].addr=FreeMem[B-1].addr;
				FreeMem[B].size=FreeMem[B-1].size;
			}
			FreeMem[A].addr=address;
			FreeMem[A].size=size;
			return ;
		}
	}
	return ;
}
