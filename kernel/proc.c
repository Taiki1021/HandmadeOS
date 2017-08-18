#include"defs.h"

proc process[PROCCOUNT];
int  CurrentProc;

void IdleProcess(){
	while(1){
		KBD_Check();
		Halt();
	}
}

void InitProcess(){
	while(1){
	}
}

void TestProcess1(){
	int A;
	char Buf[64];
	while(1){
		clear();
		for(A=0;A<PROCCOUNT;A++){
			switch(process[A].p_stat){
			case SRUN:Printf("%d:\tSRUN\t%d\n",A,process[A].CpuTime);break;
			case SWAIT:Printf("%d:\tSWAIT\t%d\n",A,process[A].CpuTime);break;
			}
		}
	}
}

void TestProcess2(){
	int A;
	char Buf[64];
	while(1){
		sformat(Buf,"This is TestProcess2 %d",A--);
		CopyFar(SysDataSelecter,(void*)((uint)VRAM+80*2),2,SysDataSelecter,(void*)Buf,1,64);
	}
}

void TinyShell(){
	char Buf[64];
	int A,B;
	Printf("Tiny shell version 1.00\n");
	while(1){
		vputs(">");
		vgets(Buf);
		chomp(Buf);
		if(0){
		}else if(!strdiff(Buf,"mem")){
			mem_dump(0,mem_num());
		}else if(!strdiff(Buf,"clear")){
			clear();
		}else if(!strdiff(Buf,"gdt")){
			vputs("gdtno>");vgets(Buf);
			A=strnum(chomp(Buf));
			GDTDUMP(A);
		}else if(!strdiff(Buf,"idt")){
			vputs("idtno>");vgets(Buf);
			A=strnum(chomp(Buf));
			IDTDUMP(A);
		}else if(!strdiff(Buf,"fsys")){
			FATDump();
		}else if(!strdiff(Buf,"ls")){
			tinyls();
		}else if(!strdiff(Buf,"type")){
			Printf("filename>");
			vgets(Buf);
			type(chomp(Buf));
		}else if(!strdiff(Buf,"help")){
			Printf("mem\n");
			Printf("clear\n");
			Printf("gdt\n");
			Printf("idt\n");
			Printf("fsys\n");
			Printf("ls\n");
			Printf("type\n");
			Printf("help\n");
			Printf("memdump\n");
			Printf("ps\n");
			Printf("btype\n");
			Printf("pload\n");
		}else if(!strdiff(Buf,"memdump")){
			vputs("start>");vgets(Buf);
			A=strnum(chomp(Buf));
			vputs("size>");vgets(Buf);
			B=strnum(chomp(Buf));
			memorydump((uchar*)A,B);
		}else if(!strdiff(Buf,"ps")){
			for(A=0;A<PROCCOUNT;A++){
				switch(process[A].p_stat){
				case SRUN:Printf("%d:\tSRUN\t%d\n",A,process[A].CpuTime);break;
				case SWAIT:Printf("%d:\tSWAIT\t%d\n",A,process[A].CpuTime);break;
				}
			}
		}else if(!strdiff(Buf,"pload")){
			Printf("filename>");
			vgets(Buf);
			pload(chomp(Buf));
		}else if(!strdiff(Buf,"btype")){
			Printf("filename>");
			vgets(Buf);
			bintype(chomp(Buf));
		}else{
			Printf("Bad Command\n");
		}
	}
}

void ISR_TIMER(struct trapframe* tf){
	process[CurrentProc].CpuTime++;
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
	process[A].text=text;
	process[A].textsize=0xFFFFFFFF;
	process[A].data=mem_alloc(datasize);
	process[A].datasize=datasize;
	process[A].Context.eip=(unsigned int)text;
	process[A].Context.eflags=0x00000202;
	process[A].Context.eax=0;
	process[A].Context.ecx=0;
	process[A].Context.edx=0;
	process[A].Context.ebx=0;
	process[A].Context.esp=(uint)process[A].data+datasize-5;
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
		GDT_SetBaseAddress(&PDT[A],(uint)&process[A].Context);
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
	process[1].Context.eip=(unsigned int)TinyShell;
	process[1].Context.eflags=0x00000202;
	process[1].Context.eax=0;
	process[1].Context.ecx=0;
	process[1].Context.edx=0;
	process[1].Context.ebx=0;
	process[1].Context.esp=(uint)mem_alloc(10000)+10000-5;
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

	process[2].p_stat=SDEAD;
	process[2].text=InitProcess;
	process[2].textsize=0xFFFF;
	process[2].data=(void*)0;
	process[2].datasize=0xFFFFFFFF;
	process[2].CpuTime=0;
	process[2].Context.eip=(unsigned int)TestProcess2;
	process[2].Context.eflags=0x00000202;
	process[2].Context.eax=0;
	process[2].Context.ecx=0;
	process[2].Context.edx=0;
	process[2].Context.ebx=0;
	process[2].Context.esp=(uint)mem_alloc(1000)+1000-5;
	process[2].Context.ebp=0;
	process[2].Context.esi=0;
	process[2].Context.edi=0;
	process[2].Context.cs=SysCodeSelecter;
	process[2].Context.es=SysDataSelecter;
	process[2].Context.ss=SysDataSelecter;
	process[2].Context.ds=SysDataSelecter;
	process[2].Context.fs=SysDataSelecter;
	process[2].Context.gs=SysDataSelecter;
	process[2].CpuTime=0; 

	process[3].p_stat=SDEAD;
	process[3].text=InitProcess;
	process[3].textsize=0xFFFF;
	process[3].data=(void*)0;
	process[3].datasize=0xFFFFFFFF;
	process[3].CpuTime=0;
	process[3].Context.eip=(unsigned int)TestProcess1;
	process[3].Context.eflags=0x00000202;
	process[3].Context.eax=0;
	process[3].Context.ecx=0;
	process[3].Context.edx=0;
	process[3].Context.ebx=0;
	process[3].Context.esp=(uint)mem_alloc(1000)+1000-5;
	process[3].Context.ebp=0;
	process[3].Context.esi=0;
	process[3].Context.edi=0;
	process[3].Context.cs=SysCodeSelecter;
	process[3].Context.es=SysDataSelecter;
	process[3].Context.ss=SysDataSelecter;
	process[3].Context.ds=SysDataSelecter;
	process[3].Context.fs=SysDataSelecter;
	process[3].Context.gs=SysDataSelecter;
	process[3].CpuTime=0; 

	CurrentProc=0;
	ltr(TssSelecter(0));

	return ;
}

void wait(int *wchan){
	process[CurrentProc].wchan=wchan;
	process[CurrentProc].p_stat=SWAIT;
	swtch();
}

void wakeup(int *wchan){
	int A;
	for(A=0;A<PROCCOUNT;A++){
		if(process[A].p_stat==SWAIT && process[A].wchan==wchan)process[A].p_stat=SRUN;
	}
}

void swtch(){
	int A,B,C;
	B=process[0].CpuTime;
	C=0;
	for(A=0;A<PROCCOUNT;A++){
		if(A!=CurrentProc)process[A].CpuTime++;
	}
	for(A=0;A<PROCCOUNT;A++){
		if((process[A].p_stat==SRUN) && (B<process[A].CpuTime)){
			B=process[A].CpuTime;
			C=A;
		}
	}
	process[C].CpuTime=0;

	if(CurrentProc!=C){
		CurrentProc=C;
		GDT_SetBaseAddress(&GDT[3],(uint)process[CurrentProc].text);
		GDT_SetLimit(&GDT[3],(unsigned int)process[CurrentProc].textsize-1);
		GDT_SetBaseAddress(&GDT[4],(uint)process[CurrentProc].data);
		GDT_SetLimit(&GDT[4],(uint)process[CurrentProc].datasize-1);
		farjmp(0,TssSelecter(CurrentProc));
	}
	return ;
}
