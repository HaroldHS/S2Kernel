#include "gdt/gdt.h"
#include "vga/vga.h"

void kernel_main(void) {
	vga_clear_screen();

	gdt_init();
	vga_write_string("[+] GDT has been initialized\r\n", 31);
	vga_write_string("Hello World!", 13);
}
