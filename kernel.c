#define IRQ0_MAP 0x20
#define IRQ8_MAP 0x28
#define INT_KEYBOARD 0x1
#define CS_SELECTOR 0x8
//PRESENT:1bit(set)|DPL:2bits(00:ring0)|STORAGE_SEG:1bit(not set)|TYPE:(4bits)(1110:interrupt_gate)
#define INT_GATE_PRESENT 0x8e // 10001110

extern char read_from_port(short port);
extern void write_to_port(short port, char value);
extern void pic_remap(char base_pic1, char base_pic2);
extern void set_imr_pic1(char mask);
extern void set_imr_pic2(char mask);
extern void _load_idt();
extern void idt_add(unsigned char interrupt,unsigned short off_higher, unsigned short off_lower, unsigned short selector, unsigned char present_dpl, unsigned char not_used);
extern void keyboard_handler_call();
extern void polling_keycode();

void clear_screen(char color);
void prints_at(char* str, char color, int pos);

int cur_pos = 0; // cursor position
char* video_mem = (char*) 0xb8000; // video memory

void kmain(void){
  char* message = "Hello Kernel Dev";

  clear_screen(0x07);
  prints_at(message, 0x0f, 0);

  //two new line
  cur_pos += 160 - (cur_pos % 160);
  cur_pos += 160;
  prints_at("You can type now! give it a try:", 0x0f, cur_pos);

  //remap interrupts
  pic_remap(IRQ0_MAP, IRQ8_MAP);
  //mask interrupts
  set_imr_pic1(0xff);
  set_imr_pic2(0xff);

  //add keyboard handler to the IDT
  unsigned long handler_adr = (unsigned long) keyboard_handler_call;
  idt_add(IRQ0_MAP + INT_KEYBOARD, (handler_adr & 0xffff0000) >> 16, handler_adr & 0xffff, CS_SELECTOR, INT_GATE_PRESENT, 0);

  //load the IDT
  _load_idt();

  //unmask keyboard interrupts
  set_imr_pic1(0xfd);

  //keep the cpu busy waiting for interrupts
  while(1);

  return;
}

void prints_at(char* str, char color, int pos){
  int screen_size = 80 * 25 * 2;
  int i = 0;

  while(str[i] != '\0' && pos < screen_size){
    video_mem[pos] = str[i];
    video_mem[pos+1] = color;
    i++;
    pos+=2;
    cur_pos+=2;
  }
  return;
}

void clear_screen(char color){
  int screen_size = 80 * 25;
  //char color = 0x07;

  for(int i=0; i< screen_size; i++){
      video_mem[i*2] = ' '; //empty
      video_mem[i*2+1] = color;
  }
  cur_pos = 0;
  return;
}
