/*OUTPUT_FORMAT("elf-i386","elf-i386","elf-i386")*/
OUTPUT_ARCH(i386)
ENTRY(main)

SECTIONS
{
	. = 0;
	.text : {
		*(.text)
	}
	"file offset of data" = .;
	. = 0x7E00;
	.data : AT("file offset of data")
	{
		*(.rodata)
		*(.data)
	}
	.bss :
	{
		*(.bss)
	}
	.eh_frame : {}
}
