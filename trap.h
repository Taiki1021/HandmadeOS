struct trapframe{
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int oesp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx; 
	unsigned int eax;

	unsigned short gs;
	unsigned short padding1;
	unsigned short fs;
	unsigned short padding2;
	unsigned short es;
	unsigned short padding3;
	unsigned short ds;
	unsigned short padding4;

	unsigned int trapno;

	unsigned int eip;
	unsigned short cs;
	unsigned short padding5;
	unsigned int eflags;

	unsigned int esp;
	unsigned short ss;
	unsigned short padding6;
};

extern void (*IntHandler[256])(struct trapframe*);
extern void* vectors[256];

void alltraps();

void trap(struct trapframe* ft);
