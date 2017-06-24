void GDT_Load(int no);
void GDT_Save(int no);
void GDT_Clear();
unsigned int GDT_GetBaseAddress();
void GDT_SetBaseAddress(unsigned int BaseAddress);
unsigned int GDT_GetLimit();
void GDT_SetLimit(unsigned int Limit);
unsigned short GDT_GetFlags();
void GDT_SetFlags(unsigned short Flags);
void IDT_Clear();
unsigned int IDT_GetHandler();
void IDT_SetHandler(unsigned int Handler);
unsigned short IDT_GetHandlerSegment();
void IDT_SetHandlerSegment(unsigned short HandlerSegment);
unsigned char IDT_GetFlags();
void IDT_SetFlags(unsigned char Flags);



#define GDT_TYPE0	1
#define GDT_TYPE1	2
#define GDT_TYPE2	4
#define GDT_TYPE3	8
#define GDT_S		16
#define GDT_DPL0	32
#define GDT_DPL1	64
#define GDT_P		128
#define GDT_AVL		256
#define GDT_D		1024
#define GDT_G		2048

#define IDT_D		8
#define IDT_DPL0	32
#define IDT_DPL1	64
#define IDT_P		128
