#define IDT_SIZE 256

extern void load_idt();

/*
 * This is the structure of an entry
 * in the Interrupt Descriptor Table
 * that the processor uses to hanle
 * interrupts.
 */
struct idt_entry{
  unsigned short int off_lower; //lower bits of ISR offset
  unsigned short int selector; //segment selector where to locate the ISR
  unsigned char not_used; //just keep it like it is
  unsigned char present_dpl; //define the if the handler is present and the DPL
  unsigned short int off_higher; //higher bits of ISR offset
};

/*
 * This is the IDT
 * it's composed of 256 entry.
 */
struct idt_entry IDT[IDT_SIZE];


/*
 * This function modify the data
 * of an IDT entry at position
 * specified by the interrupt
 * variable.
 */
void idt_add(unsigned char interrupt,unsigned short off_higher, unsigned short off_lower, unsigned short selector, unsigned char present_dpl, unsigned char not_used)
{
    IDT[interrupt].off_higher = off_higher ;
    IDT[interrupt].off_lower = off_lower;
    IDT[interrupt].selector = selector;
    IDT[interrupt].present_dpl = present_dpl;
    IDT[interrupt].not_used = not_used;
}

/*
 * This function is just a wrapper
 * fot the load_idt() function located
 * in interrupt.asm
 */
void _load_idt(){
    load_idt();
}
