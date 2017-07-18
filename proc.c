#include"defs.h"

typedef struct{
	char p_stat;
	void* p_text;	//プロセステキストセグメント
	int p_textsize;
	void* p_data;	//プロセスデータセグメント
	int p_datasize;
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
			process[A].p_datasize=process[CurrentProcID].p_datasize
			process[A].p_pid=A;
			process[A].ppid=CurrentProcID;
			process[A].p_data=mem_alloc(process[A].p_datasize);
			blockcpy(&process[A].Context,&process[CurrentProcID].Context,sizeof(struct trapframe));
			process[A].p_data=malloc(process[A].p_datasize);
			blockcpy(process[A].p_data,process[CurrentProcID].p_data,process[A].p_datasize);
			return A;
		}
	}
	return -1;
}



void InitProc(){
	IntHandler[0x20]=ISR_TIMER;
}

void swtch(){
	do{
		CurrentProcID=(CurrentProcID+1)%PROCCOUNT;
	}while(!process[CurrentProcID].p_stat);
	return ;
}

void saveu(struct trapframe* tf){ //注意　割り込み中に呼ぶこと
	blockcpy(&process[CurrentProcID].Context,tf,sizeof(struct trapframe));
	return ;
}

void loadu(struct trapframe* tf){
	blockcpy(tf,&process[CurrentProcID].Context,sizeof(struct trapframe));
	return ;
}

void ISR_TIMER(struct trapframe* tf){
	saveu(tf);
	swtch();
	loadu(tf);
}
