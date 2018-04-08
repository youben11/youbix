all: kernel kernel_polling

kernel: kerasm.o kerc.o interrupt.o io.o keyboard.o idt.o pic.o rtc.o link-script.ld
	ld -m elf_i386 -T link-script.ld -o kernel kerasm.o kerc.o interrupt.o io.o keyboard.o idt.o pic.o rtc.o

kernel_polling: kerasm.o kerc_polling.o interrupt.o io.o keyboard.o idt.o pic.o rtc.o link-script.ld
	ld -m elf_i386 -T link-script.ld -o kernel_polling kerasm.o kerc_polling.o interrupt.o io.o keyboard.o idt.o pic.o rtc.o

kerasm.o: kernel.asm
	nasm -f elf32 kernel.asm -o kerasm.o

interrupt.o: interrupt.asm
	nasm -f elf32 interrupt.asm -o interrupt.o

io.o: io.asm
	nasm -f elf32 io.asm -o io.o

kerc.o: kernel.c
	gcc -m32 -fno-stack-protector -c kernel.c -o kerc.o

kerc_polling.o: kernel_polling.c
		gcc -m32 -fno-stack-protector -c kernel_polling.c -o kerc_polling.o

keyboard.o: keyboard.c
	gcc -m32 -fno-stack-protector -c keyboard.c -o keyboard.o

idt.o: idt.c
	gcc -m32 -fno-stack-protector -c idt.c -o idt.o

pic.o: pic.c
	gcc -m32 -fno-stack-protector -c pic.c -o pic.o

rtc.o: rtc.c
	gcc -m32 -fno-stack-protector -c rtc.c -o rtc.o

clean:
	rm *.o
