/*OUTPUT_FORMAT("elf-i386","elf-i386","elf-i386")*/
OUTPUT_ARCH(i386)
ENTRY(main)

SECTIONS
{
	.text : {
		*(.text)
	}
	"file offset of data" = .;
	. = 0;
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
