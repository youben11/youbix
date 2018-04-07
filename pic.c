/* This code is only usefull with machines
 * that use 8259 PIC. (like i386)
 */
#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1
#define PIC_RESTART 0x11
#define PIC_DONE 0x01
#define PIC_EOI 0x20

extern char read_from_port(short port);
extern void write_to_port(short port, char value);

/*
 * This function follow the protocol
 * used by the 8259 PIC to initialize it,
 * configure master/slave and map IRQs
 * into Interrupts.
 * Here we will map IRQ0 with base_pic1
 * and IRQ8 with base_pic2.
 */
void pic_remap(char base_pic1, char base_pic2){
  //restarting
  write_to_port(PIC1_CMD, PIC_RESTART);
  write_to_port(PIC2_CMD, PIC_RESTART);
  //remapping
  write_to_port(PIC1_DATA, base_pic1);
  write_to_port(PIC2_DATA, base_pic2);
  //setup cascading
  write_to_port(PIC1_DATA, 0x04);
  write_to_port(PIC2_DATA, 0x02);
  //terminating
  write_to_port(PIC1_DATA, PIC_DONE);
  write_to_port(PIC2_DATA, PIC_DONE);
}

/*
 * This functions acknowledge the PICs
 * with an End Of Interrupt.
 */
 void pic1_eoi(){
   write_to_port(PIC1_CMD, PIC_EOI);
 }

 void pic2_eoi(){
   write_to_port(PIC2_CMD, PIC_EOI);
   //We have to acknowledge the master (pic1)
   //when we acknowledge the slave (pic2).
   pic1_eoi();
 }

/*
 * The functions bellow set and get
 * the interrupt mask from each PIC.
 */

void set_imr_pic1(char mask){
  write_to_port(PIC1_DATA, mask);
}
char get_imr_pic1(){
  return read_from_port(PIC1_DATA);
}

void set_imr_pic2(char mask){
  write_to_port(PIC2_DATA, mask);
}
char get_imr_pic2(){
  return read_from_port(PIC2_DATA);
}
