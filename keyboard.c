#define KEYBOARD_DATA 0x60
#define KEYBOARD_STATUS 0x64

extern void prints_at(char* str, char color, int pos);
extern void clear_screen(char color);
extern char read_from_port(short port);
extern void write_to_port(short port, char value);

void keyboard_handler(){
  //write_to_port(0x20,0x20);//EOI

  clear_screen(0x07);
  prints_at("Key Pressed", 0x07, 0);
  
  return;
}
