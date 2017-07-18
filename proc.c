#include"defs.h"

typedef struct{
	char p_stat;
	void* p_text;	//プロセステキストセグメント
	int p_textsize;
	void* p_data;	//プロセスデータセグメント
	unsigned int p_datasize;
	struct trapframe Context;
	int  pid;
	int  ppid;
} proc;

proc process[PROCCOUNT];
int  CurrentProcID;

int newproc(){
	int A;
	for(A=0;A<PROCCOUNT;A++){
		if(!process[A].p_stat){
			process[A].p_stat=1;
			process[A].p_text=process[CurrentProcID].p_text;
			process[A].p_textsize=process[CurrentProcID].p_textsize;
			process[A].p_datasize=process[CurrentProcID].p_datasize;
			process[A].pid=A;
			process[A].ppid=CurrentProcID;
			process[A].p_data=mem_alloc(process[A].p_datasize);
			blockcpy(&process[A].Context,&process[CurrentProcID].Context,sizeof(struct trapframe));
			process[A].p_data=mem_alloc(process[A].p_datasize);
			blockcpy(process[A].p_data,process[CurrentProcID].p_data,process[A].p_datasize);
			return A;
		}
	}
	return -1;
}

void IdleProcess(){
	while(1){
		Halt();
	}
}

void ProcTest(){
	while(1){
		KBD_Check();
	}
}

void ProcTest2(){
	unsigned char A;
	while(1){
		A++;
		VRAM[40].c=A;
	}
}

void ISR_TIMER(struct trapframe* tf){
	saveu(tf);
	CurrentProcID=(CurrentProcID+1)%3;
//	swtch();
	loadu(tf);
	return ;
}

void Proc_Init(){
	int A;
	IntHandler[0x20]=ISR_TIMER;
	for(A=0;A<PROCCOUNT;A++)process[A].p_stat=0;
	process[0].p_stat=1;
	process[0].p_data=0;
	process[0].p_datasize=0xFFFFFFFF;
	process[0].p_text=IdleProcess;
	process[0].pid=0;
	process[0].ppid=0;

	process[1].p_stat=1;
	process[1].p_data=0;
	process[1].p_datasize=0xFFFFFFFF;
	process[1].p_text=ProcTest;
	process[1].pid=1;
	process[1].ppid=0;
	process[1].Context.eip=(unsigned int)process[1].p_text;
	process[1].Context.eflags=0x00000202;
	process[1].Context.eax=0;
	process[1].Context.ecx=0;
	process[1].Context.edx=0;
	process[1].Context.ebx=0;
	process[1].Context.esp=(unsigned int)mem_alloc(1000)+1000-5;
	process[1].Context.ebp=0;
	process[1].Context.esi=0;
	process[1].Context.edi=0;
	process[1].Context.es=SysDataSelecter;
	process[1].Context.cs=SysCodeSelecter;
	process[1].Context.ss=SysDataSelecter;
	process[1].Context.ds=SysDataSelecter;
	process[1].Context.fs=SysDataSelecter;
	process[1].Context.gs=SysDataSelecter;

	process[2].p_stat=1;
	process[2].p_data=0;
	process[2].p_datasize=0xFFFFFFFF;
	process[2].p_text=ProcTest2;
	process[2].pid=2;
	process[2].ppid=0;
	process[2].Context.eip=(int)process[2].p_text;
	process[2].Context.eflags=0x00000202;
	process[2].Context.eax=0;
	process[2].Context.ecx=0;
	process[2].Context.edx=0;
	process[2].Context.ebx=0;
	process[2].Context.esp=(unsigned int)mem_alloc(1000)+1000-5;
	process[2].Context.ebp=0;
	process[2].Context.esi=0;
	process[2].Context.edi=0;
	process[2].Context.es=SysDataSelecter;
	process[2].Context.cs=SysCodeSelecter;
	process[2].Context.ss=SysDataSelecter;
	process[2].Context.ds=SysDataSelecter;
	process[2].Context.fs=SysDataSelecter;
	process[2].Context.gs=SysDataSelecter;

	return ;
}

void swtch(){
	do{
		CurrentProcID=(CurrentProcID+1)%PROCCOUNT;
	}while(!process[CurrentProcID].p_stat);
	Printf("%d\n",CurrentProcID);
	return ;
}

void saveu(struct trapframe* tf){ 
	blockcpy(&process[CurrentProcID].Context,tf,sizeof(struct trapframe));
	return ;
}

void loadu(struct trapframe* tf){
	blockcpy(tf,&process[CurrentProcID].Context,sizeof(struct trapframe));
	return ;
}
