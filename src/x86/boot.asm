section .multiboot
	dd 0x1badb002                 ; magic number for multiboot 1
	dd 0x00000000                 ; flags
	dd -(0x1badb002 + 0x00000000) ; checksum

section .text
global _start
extern kernel_main
_start:
	cli                  ; clear / block interrupts
	mov esp, stack_space ; set stack pointer
	call kernel_main
	hlt                  ; halt cpu

section .bss
resb 16384     ; stack size
stack_space:
