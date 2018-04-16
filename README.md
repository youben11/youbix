# Youbix


Youbix is a simple kernel developed (and still in dev) for learning purposes. If you are new to kernel development then you may find it useful as the code is well documented.

### Why Youbix ?
Since all powerful kernels end this way ;) youbix was a good choice.

### Architecture

The Youbix kernel support the x86 architecture but depend mostly on the [8259 PIC](https://en.wikipedia.org/wiki/Intel_8259). It should run without problem on an i386 machine.

Because of the PIC dependency, we have introduced a version which deal with the hardware not via interrupt but via polling. It should run properly on modern machines that use [APIC](https://wiki.osdev.org/APIC) and don't emulate the 8259 PIC. This version was tested on an x86_64 machine.

### Building the Youbix kernel
#### Requirements
The majority of the tools needed are part of any basic Linux distribution and are under a GPL License (except for nasm : BSD).
- gcc
- ld
- nasm
- make

#### Build
The building process is automated using a makefile. It start by compiling and assembling then linking the object files.

This is an example of building the kernel on an x86-64 machine.

```bash
$ cd youbix
$ make
nasm -f elf32 kernel.asm -o kerasm.o
gcc -m32 -fno-stack-protector -c kernel.c -o kerc.o
nasm -f elf32 interrupt.asm -o interrupt.o
nasm -f elf32 io.asm -o io.o
gcc -m32 -fno-stack-protector -c keyboard.c -o keyboard.o
gcc -m32 -fno-stack-protector -c idt.c -o idt.o
gcc -m32 -fno-stack-protector -c pic.c -o pic.o
ld -m elf_i386 -T link-script.ld -o kernel kerasm.o kerc.o interrupt.o io.o keyboard.o idt.o pic.o
$ file kernel
kernel: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), statically linked, not stripped
$ file kernel_polling
kernel_polling: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), statically linked, not stripped
```
As you should see, there is two kernels. The first one deal with the hardware via interrupt and should work fine on an i386 machines. The second one deal with the hardware via polling and should work on all x86 machine.

We will be using the word kernel in the next sections to refer to the two versions.

### Installation
Youbix kernel is [multiboot compliant](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html) and can be added as an entry in the GRUB to boot from it. It can also be emulated using [Qemu](https://www.qemu.org/).

#### GRUB
Keep in mind that Youbix is intended to be run on a specific hardware. See the Architecture section for more information.

You can still emulate it if you don't have the adequate hardware. See the section below.

I will assume that you are running a Linux distro and have GRUB installed.

After building the kernel move it to the /boot directory under the name kernel-<version> (kernel-1)
```bash
$ mv kernel /boot/kernel-1
```
or
```bash
$ mv kernel_polling /boot/kernel-1
```
then open the /etc/grub.d/40_custom file and add the following its end
```vi
menuentry "youbix" {
  set root=(hd0, 3)
  multiboot /boot/kernel-1 ro
}
```
you should replace hd0 by the name of your drive and 3 by the partition number that hold the file.

last step, run the following command in your terminal
```bash
$ sudo update-grub
```

You can now reboot your computer and try Youbix :D


#### Qemu
If you don't have the adequate hardware or don't want to reboot your computer, you can still emulate it in userspace using Qemu.

I will assume that you are running a Linux distro and have Qemu installed.

Open a terminal and go to the youbix directory then run the following command
```bash
$ cd youbix
$ qemu-system-i386 -kernel kernel
```
a window should appear with Youbix booting on it.


### Debugging
We will use Qemu and [gdb-peda](https://github.com/longld/peda) to debug our kernel.

The first step is to run Qemu with debugging options:
```bash
$ qemu-system-i386 -S -gdb tcp::1234 -kernel kernel
```
The -S option freeze CPU at startup. -gdb tcp::1234 tell Qemu to listen on port 1234/tcp.

Then in another terminal, we will run our debugger:
```bash
$ gdb
gdb-peda$ set architecture i386
#The target architecture is assumed to be i386
gdb-peda$ target remote localhost:1234
#Remote debugging using localhost:1234
#warning: No executable has been specified and target #does not support
#determining executable automatically.  Try using the "file" command.
#Warning: not running or target is remote
#0x0000fff0 in ?? ()
gdb-peda$
```
Output lines are commented. You can now run gdb commands as usual. Enjoy!

### More

#### Protected mode
If you are running Youbix from GRUB then keep in mind that you are in the protected mode and the GDT is set up with full access to 4GB of memory-space (0x00000000-0xffffffff). You can read more about protected mode [here](https://wiki.osdev.org/Protected_Mode).
