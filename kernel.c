#define IRQ0_MAP 0x20
#define IRQ8_MAP 0x28
#define INT_KEYBOARD 0x1
#define INT_RTC 0x0
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
extern void rtc_handler_call();
extern void polling_keycode();
extern void polling_rtc();
extern void rtc_on();
extern void rtc_handler();

void clear_screen(char color);
void prints_at(char* str, char color, int pos);
void int_to_str(unsigned char number, char* str, unsigned char elen);

int cur_pos = 0; // cursor position in the video_mem
/*
 * Video memory can be accessed from 0xb8000
 * its size is 80 column * 25 lines, each 2 bytes
 * the first byte the character itself
 * the second is the color (text and background).
 * 0x0f is the color for white-on-black, the left
 * four bits are the background color and the right
 * ones are the font color.
 */
char* video_mem = (char*) 0xb8000; // video memory

/*
 * This function is called from the entry_point
 * function "start" in the kernel.asm file.
 * It's the main routine of the kernel.
 */
void kmain(void){
  char* message = "Hello Kernel Dev";

  clear_screen(0x07);
  prints_at(message, 0x02, 0);

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
  //add rtc handler to the IDT
  handler_adr = (unsigned long) rtc_handler_call;
  idt_add(IRQ8_MAP + INT_RTC, (handler_adr & 0xffff0000) >> 16, handler_adr & 0xffff, CS_SELECTOR, INT_GATE_PRESENT, 0);
  //turn on RTC interrupt
  rtc_on();
  //load the IDT
  _load_idt();
  //enable interrupts
  asm("sti");

  //unmask keyboard interrupt
  set_imr_pic1(0xfd);
  //unmask rtc interrupt
  set_imr_pic2(0xfe);

  //keep the cpu busy waiting for interrupts
  while(1);

  return;
}


/*
 * This function copy the string pointed
 * by str to the video memory space, it uses
 * pos as an offset in the video memory.
 * The color to be used is provided through
 * the color variable.
 */
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

/*
 * This function clear the screen by copying
 * the character ' ' into each cells and setting
 * the background color specified by the color
 * variable.
 */
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

/*
 * This function convert a number
 * into a string and write it in the specified
 * adr str.
 * elan is used to expend zero at the beginning.
 */
void int_to_str(unsigned char number, char* str, unsigned char elen){
  int len = 1;
  int max;
  if(elen <= 0){
    max = 1;
    while(max*10 <= number){
      len++;
      max *= 10;
    }
  }else{
    max = 1;
    len = elen;
    for(int i=1; i < elen; i++)
      max *= 10;
  }

  for(int i=0; i < len; i++){
    str[i] = 48 + ((number % (max*10)) / max);
    max /= 10;
  }
  return;
}
