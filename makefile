all: kernel

kernel: kerasm.o kerc.o interrupt.o io.o keyboard.o idt.o pic.o link-script.ld
	ld -m elf_i386 -T link-script.ld -o kernel kerasm.o kerc.o interrupt.o io.o keyboard.o idt.o pic.o

kerasm.o: kernel.asm
	nasm -f elf32 kernel.asm -o kerasm.o

interrupt.o: interrupt.asm
	nasm -f elf32 interrupt.asm -o interrupt.o

io.o: io.asm
	nasm -f elf32 io.asm -o io.o

kerc.o: kernel.c
	gcc -m32 -fno-stack-protector -c kernel.c -o kerc.o

keyboard.o: keyboard.c
	gcc -m32 -fno-stack-protector -c keyboard.c -o keyboard.o

idt.o: idt.c
	gcc -m32 -fno-stack-protector -c idt.c -o idt.o

pic.o: pic.c
	gcc -m32 -fno-stack-protector -c pic.c -o pic.o

clean:
	rm *.o
