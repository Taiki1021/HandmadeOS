#include "defs.h"

void efifo(struct fifo* f,unsigned char data){
	f->buffer[f->enext]=data;
	f->enext=(f->enext+1)%f->size;
	return;
}

unsigned char dfifo(struct fifo* f){
	unsigned char r;
	r=f->buffer[f->dnext];
	f->dnext=(f->dnext+1)%f->size;
	return r;
}

char isfifoend(struct fifo* f){
	return (f->enext == f->dnext);
}

struct fifo* createfifo(int size){
	struct fifo* f;
	f=(struct fifo*)mem_alloc(sizeof(struct fifo)+size);
	f->size=size;
	f->enext=f->dnext=0;
	return f;
}

void freefifo(struct fifo* f){
	mem_free(f,f->size+sizeof(struct fifo));
	return ;
}
