OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS
{
	. = 0x7E00;

	.text :
	{
		*(.text)
	}

	.data : ALIGN(4K)
	{
		*(.data)
	}

	end = .;
	
}
