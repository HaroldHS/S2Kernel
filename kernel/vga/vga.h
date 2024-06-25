#pragma once

#include "../stdint.h"
#include "../stdbool.h"

void vga_clear_screen(void);
void vga_scroll_up(void);

void vga_write(const char* data);
void vga_write_string(const char *data, uint64_t size);

void new_line(void);
