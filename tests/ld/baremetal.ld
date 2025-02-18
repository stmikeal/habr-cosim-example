DRAM_BASE = 0x80000000;
DRAM_SIZE = 0x20000000;

MEMORY {
	DRAM 	(rwx): ORIGIN = DRAM_BASE, LENGTH = DRAM_SIZE
}

SECTIONS
{
	. = DRAM_BASE;
  	.text : {
    	KEEP(*(.text._start));
		*(.text*);
  	}
	. = ALIGN (CONSTANT (COMMONPAGESIZE));
	.data : { 
		*(.data*)
	}
}