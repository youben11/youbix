bits 32

global keyboard_handler_call
global load_idt
extern keyboard_handler
extern IDT

section .text

load_idt:                    ; load the interrupt descriptor table
  mov edx, idt_pointer
  lidt [edx]
  sti
  ret

keyboard_handler_call:       ;just a wrapper to the read handler
  call keyboard_handler
  iretd

section .data

;idt pointer which describe the location and size of IDT
idt_pointer:
  dw 0x800                   ;size of IDT (256 interrupt)
  dd IDT                     ;location of IDT
