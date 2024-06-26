#include "../stdint.h"

/* 32-bit mode GDT (Global Descriptor Table) */
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

/* 32-bit protected mode TSM (Task State Management) */
struct ts_segment_descriptor {
	uint32_t link; // previous task link field (16-bit unused / reserved + 16-bit segment selector)
	uint32_t esp0;
	uint32_t ss0;  // 16-bit unused/reserved + 16-bit segment selector
	uint32_t esp1;
	uint32_t ss1;  // 16-bit unused/reserved + 16-bit segment selector
	uint32_t esp2;
	uint32_t ss2;  // 16-bit unused/reserved + 16-bit segment selector
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;   // 16-bit unused/reserved + extra segment
	uint32_t cs;   // 16-bit unused/reserved + code segment
	uint32_t ss;   // 16-bit unused/reserved + stack segment
	uint32_t ds;   // 16-bit unused/reserved + data segment
	uint32_t fs;   // 16-bit unused/reserved + general purpose segment
	uint32_t gs;   // 16-bit unused/reserved + general purpose segment
	uint32_t ldtr; // 16-bit unused/reserved + 16-bit location of Load Task Register
	uint32_t iopb; // 16-bit IOPB (I/O Map Base address) + 16-bit unused/reserved
	uint32_t ssp;  // shadow stack pointer
} __attribute__((packed));

void gdt_init(void);
/* 32-bit base = 16-bit start_base + 8-bit middle_base + 8-bit last_base */
/* 32-bit limit = 16-bit limit + 4-bit for limit in flags_limit + 12-bit unused bits */
/* 8-bit gran = 4-bit for flags in flags_limit + 4-bit unused bits */
void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void write_tss(uint32_t num, uint16_t ss0, uint32_t esp0);
