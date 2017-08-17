#include"defs.h"

typedef struct{
	uchar	e_ident[16];
	ushort	e_type;
	ushort	e_machine;
	uint	e_version;
	uint	e_entry;
	uint	e_phoff;
	uint	e_shoff;
	uint	e_flags;
	ushort	e_ehsize;
	ushort	e_phentsize;
	ushort	e_phnum;
	ushort	e_shentsize;
	ushort	e_shnum;
	ushort	e_shstrndx;
}__attribute__((packed)) elfheader;

typedef struct{
	uint	sh_name;
	uint	sh_type;
	uint	sh_flags;
	uint	sh_addr;
	uint	sh_offset;
	uint	sh_size;
	uint	sh_link;
	uint	sh_info;
	uint	sh_addralign;
	uint	sh_entsize;
}__attribute__((packed)) sheader;

typedef struct{
	uint	p_type;
	uint	p_offset;
	uint	p_vaddr;
	uint	p_paddr;
	uint	p_filesz;
	uint	p_memsz;
	uint	p_flags;
	uint	p_align;
}__attribute__((packed)) pheader;


void pload(char* filename){
	char buf[32];
	int A;
	int NUM;

	file* fp;
	elfheader	head;
	pheader*	phead;
	uchar*		text;
	int P;

	fp=open(filename);
	read(fp,&head,sizeof(elfheader));

	if(head.e_phnum>0){
		phead=(pheader*)mem_alloc(head.e_phentsize*head.e_phnum);
		seek(fp,head.e_phoff);
		read(fp,phead,head.e_phentsize*head.e_phnum);
	}
/*
	for(NUM=0;NUM<=1;NUM++){
	Printf("%d\n",NUM);
	Printf("p_type%X\n",phead[NUM].p_type);
	Printf("p_offset%X\n",phead[NUM].p_offset);
	Printf("p_vaddr%X\n",phead[NUM].p_vaddr);
	Printf("p_paddr%X\n",phead[NUM].p_paddr);
	Printf("p_filesz%X\n",phead[NUM].p_filesz);
	Printf("p_memsz%X\n",phead[NUM].p_memsz);
	Printf("p_flags%X\n",phead[NUM].p_flags);
	Printf("p_align%d\n",phead[NUM].p_align);
	}

	Printf("e_entry:%x\n",head.e_entry);
	
*/
	text=(uchar*)mem_alloc(0x200);
	seek(fp,0x90);
	read(fp,text,0x200);
	
	P=Proc_Run(text,0x10000);
	
	seek(fp,phead[0].p_offset);
	read(fp,process[P].data,phead[0].p_filesz);

	process[P].textsize=0x200;
	process[P].CpuTime=500;

	process[P].Context.eip=0x94;

//	process[P].p_stat=SWAIT;
//	Printf("text:%X\n",text+process[P].Context.eip);
/*
	Printf("text:%X\n",process[P].text);
	blockcpy(buf,head.e_ident,16);
	buf[16]=0;
	Printf("e_ident:%s\n",buf);
	Printf("e_type:%d\n",head.e_type);
	Printf("e_machine:%d\n",head.e_machine);
	Printf("e_version:%d\n",head.e_version);
	Printf("e_entry:%x\n",head.e_entry);
	Printf("e_phoff:%d\n",head.e_phoff);
	Printf("e_shoff:%d\n",head.e_shoff);
	Printf("e_flags:%d\n",head.e_flags);
	Printf("e_ehsize:%d\n",head.e_ehsize);
	Printf("e_phentsize:%d\n",head.e_phentsize);
	Printf("e_phnum:%d\n",head.e_phnum);
	Printf("e_shentsize:%d\n",head.e_shentsize);
	Printf("e_shnum:%d\n",head.e_shnum);
	Printf("e_shstrndx:%d\n",head.e_shstrndx);
	
	Printf("p_offset:%X\n",phead[1].p_offset);
*/
}

