global gdt_flush

gdt_flush:
	mov eax, [esp+4] ; GDT pointer from gdt.c
	lgdt [eax]       ; load GDT from given location by GDT pointer
	
	mov eax, 0x10   ; offset to GDT
	mov ds, ax      ; data segment
	mov es, ax      ; extra segment 1
	mov fs, ax      ; extra segment 2
	mov gs, ax      ; extra segment 3
	mov ss, ax      ; stack segment
	jmp 0x08:.flush ; for code segment
.flush:
	ret


global tss_flush

tss_flush:
	mov ax, 0x28 ; tell that TSS inside GDT is located at sixth entry (Each entry is 8 bytes. So, 5*8 = 40 bytes)
	ltr ax       ; load TSR (Task State Register)
	ret
