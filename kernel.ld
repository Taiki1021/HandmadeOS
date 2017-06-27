OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(main)

MEMORY{
	DATASEGMENT(WI) : ORIGIN = 0, LENGTH = 2560
	TEXTSEGMENT(XAI) : ORIGIN = 0, LENGTH = 6144
}

SECTIONS
{
	.text:
	{
		*(.text)
	} > TEXTSEGMENT
	.rodata :
	{
		*(.rodata)
	} > DATASEGMENT
	.data :
	{
		*(.data)
	} > DATASEGMENT
	.bss :
	{
		*(.bss)
	} > DATASEGMENT
	
}
