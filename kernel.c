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
  polling_keycode();

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
