bits 32

global start
extern kmain

section .text

;multiboot specefication
align 4
dd 0x1BADB002               ;magic
dd 0x00                     ;flags
dd - (0x1BADB002 + 0x00)    ;checksum

start:
	cli                        ; no interrupt
	mov esp, stack_start
	call kmain
	hlt                        ; no interrupt + hlt = cpu sleep

section .data

section .bss

stack_end:
resb 8192
stack_start:
