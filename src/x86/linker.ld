ENTRY(_start)

SECTIONS
{
	. = 2M;

	.text : {
		. = ALIGN(8);
		KEEP(*( .multiboot ))
		*(.text)
	}

	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
	}
}
