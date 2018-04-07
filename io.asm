bits 32

section .text

global read_from_port
global write_to_port

read_from_port:
  mov dx, [esp+0x4]
  in ax, dx
  ret

write_to_port:
  mov edx, [esp+0x4]
  mov al, [esp+0x8]
  out dx, al
  ret
