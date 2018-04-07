/* This code is only usefull with machines
 * that use 8259 PIC. (like i386)
 */
#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1
#define PIC_RESTART 0x11
#define PIC_DONE 0x01

extern char read_from_port(short port);
extern void write_to_port(short port, char value);

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
