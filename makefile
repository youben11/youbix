all: kernel

kernel: kerasm.o kerc.o link-script.ld
	ld -m elf_i386 -T link-script.ld -o kernel kerasm.o kerc.o

kerasm.o: kernel.asm
	nasm -f elf32 kernel.asm -o kerasm.o

kerc.o: kernel.c
	gcc -m32 -fno-stack-protector -c kernel.c -o kerc.o
