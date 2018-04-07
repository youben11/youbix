#define IDT_SIZE 256

extern void load_idt();

struct idt_entry{
  unsigned short int off_lower; //lower bits of ISR offset
  unsigned short int selector; //segment selector where to locate the ISR
  unsigned char not_used; //just keep it like it is
  unsigned char present_dpl; //define the if the handler is present and the DPL
  unsigned short int off_higher; //higher bits of ISR offset
};


struct idt_entry IDT[IDT_SIZE];

void idt_add(unsigned char interrupt,unsigned short off_higher, unsigned short off_lower, unsigned short selector, unsigned char present_dpl, unsigned char not_used)
{
    IDT[interrupt].off_higher = off_higher ;
    IDT[interrupt].off_lower = off_lower;
    IDT[interrupt].selector = selector;
    IDT[interrupt].present_dpl = present_dpl;
    IDT[interrupt].not_used = not_used;
}

void _load_idt(){ //wrapper to assembly function
    load_idt();
}
