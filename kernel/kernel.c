#include "vga/vga.h"

void kernel_main(void) {
	vga_clear_screen();
	vga_write_string("Hello World!", 13);
}
