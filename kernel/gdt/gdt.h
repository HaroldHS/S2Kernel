#include "../stdint.h"

/* 32-bit mode GDT Descriptor */
struct gdt_segment_descriptor {
	uint16_t limit;
	uint16_t start_base;
	uint8_t middle_base;
	uint8_t access_byte;
	uint8_t flags_limit;
	uint8_t last_base;
} __attribute__((packed));

struct gdt_segment_pointer {
	uint16_t limit;
	unsigned int base;
} __attribute__((packed));


void gdt_init(void);
/* 32-bit base = 16-bit start_base + 8-bit middle_base + 8-bit last_base */
/* 32-bit limit = 16-bit limit + 4-bit for limit in flags_limit + 12-bit unused bits */
/* 8-bit gran = 4-bit for flags in flags_limit + 4-bit unused bits */
void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
