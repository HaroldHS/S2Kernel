#include "gdt.h"
#include "../utils/memset.h"

struct gdt_segment_descriptor gdt_entries[6];
struct gdt_segment_pointer gdt_ptr;
struct ts_segment_descriptor tss_entry;

extern void tss_flush();
extern void gdt_flush(uint32_t);

void gdt_init(void) {
	gdt_ptr.limit = (sizeof(struct gdt_segment_descriptor) * 6) - 1;
	gdt_ptr.base = (uint32_t) &gdt_entries;

	set_gdt_gate(0, 0, 0, 0, 0); // Null Segment
	/*
	 * Kernel Code Segment
	 *
	 * Base / Starting address = 0x00000000
	 * Limit / Biggest address = 0xffffffff
	 * Access = 0x9A or 0b10011010 -> P = 1, DPL =  00, S = 1, E = 1, DC = 0, RW = 1, A = 0
	 *                             |-> P = always aset to 1
	 *                             |-> DPL = CPU privilege level (00 = highest privilege (kernel), 11 = lowest (user app))
	 *                             |-> S = define descriptor type (0 = define System Segment (e.g. TSS), 1 = define Code Segment / Data Segment)
	 *                             |-> E = define executable bit (0 = define data segment, 1 = define code segment which can be executed from)
	 *                             |-> DC = define direction bit (0 = segment grows up, 1 = otherwise)
	 *                             |-> RW = define readable/writeable bit (
	 *                             |          for code segment, 0 = read in code segment is not allowed, 1 = otherwise.
	 *                             |          for data segment, 0 = write in data segment is not allowed, 1 = otherwise.
	 *                             |
	 *                             |          NOTE: write in code segment is prohibited and read in data segment is always allowed
	 *                             |        )
	 *                             |-> A = define access (0 = GDT stored in read-only pages, 1 = triggered by CPU when page fault occured)
	 *
	 * Gran / Flags = 0xcf or 0b11001111 (remove the 0xf, so 0xb1100)-> G = 1, DB = 1, L = 0, Reserve = 0
	 *                                                               |-> G = page granulity (0 = limits is 1 byte blocks, 1 = 4 bytes block)
	 *                                                               |-> DB = size (0 = 16-bit protected mode segment, 1 = 32-bit)
	 *                                                               |-> L = long-mode flag (1 = defines 64-bit mode segment, 0 = otherwise)
	 *
	 */
	set_gdt_gate(1, 0, 0xffffffff, 0x9a, 0xcf);
	set_gdt_gate(2, 0, 0xffffffff, 0x92, 0xcf); // Kernel Data Segment
	set_gdt_gate(3, 0, 0xffffffff, 0xfa, 0xcf); // User Code Segment (DPL = 11)
	set_gdt_gate(4, 0, 0xffffffff, 0xf2, 0xcf); // User Data Segment (DPL = 11)
	write_tss(5, 0x10, 0x0);
	
	gdt_flush((uint32_t) &gdt_ptr);
	tss_flush();
}

void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt_entries[num].start_base = (base >> 16) & 0xffff;
	gdt_entries[num].middle_base = (base & 0x0000ff00) >> 8;
	gdt_entries[num].last_base = (base & 0xff);

	gdt_entries[num].limit = (limit >> 16) & 0xffff;
	gdt_entries[num].flags_limit = (limit & 0x0000ff00) >> 8;
	gdt_entries[num].flags_limit |= (gran & 0xf0);

	gdt_entries[num].access_byte = access;
}

void write_tss(uint32_t num, uint16_t ss0, uint32_t esp0) {
	uint32_t base = (uint32_t) &tss_entry;
	uint32_t limit = base + sizeof(tss_entry);

	/*
	 * Access = 0xe9 or 0b11101001 -> P = 1, DPL = 11, S = 0, E = 1, DC = 0, RW = 0, A = 1
	 *
	 * Gran / Flags = 0x00 or 0b00000000
	 *
	 */
	set_gdt_gate(num, base, limit, 0xe9, 0x00);
	memset(&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0 = ss0;
	tss_entry.esp0 = esp0;
	tss_entry.cs = 0x08 | 0x3; // Code Segment (0x08 = Kernel Mode, 0x3 = User Mode / Ring 3)
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10 | 0x3; // (0x10 = Stack Segment Entry, 0x3 = Ring 3)
}
