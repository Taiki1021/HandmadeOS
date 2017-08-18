OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(main)

SECTIONS
{
	. = SIZEOF_HEADERS;
	.text : {
		*(.text)
	}
	"file offset of data" = .;
	. = 0;
	.data : /*AT("file offset of data")*/
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
