#include"defs.h"

proc process[PROCCOUNT];
int  CurrentProcID;

void IdleProcess(){
	while(1){
		Halt();
	}
}

void InitProcess(){
	while(1){
		KBD_Check();
	}
}

void ISR_TIMER(struct trapframe* tf){
	process[CurrentProcID].CpuTime++;
	SolvePICLock();
	swtch();
	return ;
}


int  Proc_Run(void* text,int datasize){
	int A;
	for(A=0;A<PROCCOUNT;A++){
		if(process[A].p_stat==0)break;
	}
	process[A].p_stat=1;
	process[A].text=0x10000;
	process[A].textsize=0xFFFFFFFF;
	process[A].data=mem_alloc(datasize);
	process[A].datasize=datasize;
	process[A].Context.eip=(unsigned int)text;
	process[A].Context.eflags=0x00000202;
	process[A].Context.eax=0;
	process[A].Context.ecx=0;
	process[A].Context.edx=0;
	process[A].Context.ebx=0;
	process[A].Context.esp=process[A].data+datasize-5;
	process[A].Context.ebp=0;
	process[A].Context.esi=0;
	process[A].Context.edi=0;
	process[A].Context.cs=UsrCodeSelecter;
	process[A].Context.es=UsrDataSelecter;
	process[A].Context.ss=UsrDataSelecter;
	process[A].Context.ds=UsrDataSelecter;
	process[A].Context.fs=UsrDataSelecter;
	process[A].Context.gs=UsrDataSelecter;
	process[A].CpuTime=0;  //TODO:親プロセス用に修正
	return A;
}

void Proc_Init(){
	int A;
	IntHandler[0x20]=ISR_TIMER;
	for(A=0;A<PROCCOUNT;A++)process[A].p_stat=SDEAD;
	for(A=0;A<PROCCOUNT;A++){
		GDT_SetBaseAddress(&PDT[A],&process[A].Context);
	}

	process[0].p_stat=SRUN;
	process[0].text=IdleProcess;
	process[0].textsize=0xFFFF;
	process[0].data=(void*)0;
	process[0].datasize=0xFFFFFFFF;
	process[0].CpuTime=0;

	process[1].p_stat=SRUN;
	process[1].text=InitProcess;
	process[1].textsize=0xFFFF;
	process[1].data=(void*)0;
	process[1].datasize=0xFFFFFFFF;
	process[1].CpuTime=0;
	process[1].Context.eip=(unsigned int)InitProcess;
	process[1].Context.eflags=0x00000202;
	process[1].Context.eax=0;
	process[1].Context.ecx=0;
	process[1].Context.edx=0;
	process[1].Context.ebx=0;
	process[1].Context.esp=0x7E00-5;
	process[1].Context.ebp=0;
	process[1].Context.esi=0;
	process[1].Context.edi=0;
	process[1].Context.cs=SysCodeSelecter;
	process[1].Context.es=SysDataSelecter;
	process[1].Context.ss=SysDataSelecter;
	process[1].Context.ds=SysDataSelecter;
	process[1].Context.fs=SysDataSelecter;
	process[1].Context.gs=SysDataSelecter;
	process[1].CpuTime=0; 

	CurrentProcID=0;
	ltr(TssSelecter(0));

	return ;
}

void swtch(){
	int A,B,C;
	int PostProcID=CurrentProcID;
	B=process[0].CpuTime;
	C=0;
	for(A=0;A<PROCCOUNT;A++){
		if((process[A].p_stat==SRUN) && (B>process[A].CpuTime)){
			B=process[A].CpuTime;
			C=A;
		}
	}
	CurrentProcID=C;
	GDT_SetBaseAddress(&GDT[3],(unsigned int)process[CurrentProcID].text);
	GDT_SetLimit(&GDT[3],(unsigned int)process[CurrentProcID].textsize);
	GDT_SetBaseAddress(&GDT[4],(unsigned int)process[CurrentProcID].data);
	GDT_SetLimit(&GDT[4],(unsigned int)process[CurrentProcID].datasize);
	if(CurrentProcID!=PostProcID){
		farjmp(0,TssSelecter(CurrentProcID));
	}
	return ;
}
