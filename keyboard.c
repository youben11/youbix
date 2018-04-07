#include "keycode.h"

#define KEYBOARD_DATA 0x60
#define KEYBOARD_STATUS 0x64

extern void prints_at(char* str, char color, int pos);
extern void clear_screen(char color);
extern char read_from_port(short port);
extern void write_to_port(short port, char value);
extern int cur_pos;

void keyboard_handler(){
  write_to_port(0x20,0x20);//EOI
  unsigned char status = read_from_port(KEYBOARD_STATUS);
  unsigned char buf[2];

  if(status & 1){
    buf[0] = keycode[read_from_port(KEYBOARD_DATA)-1];
    if(buf[0] == 0 || buf[0] >= 128)
      return;
    buf[1] = '\0';
    prints_at(buf, 0x07, cur_pos);
  }
  return;
}

void polling_keycode(){
    char* poll = "polling... keycode:    ";
    clear_screen(0x07);

    while(1){
        unsigned char status = read_from_port(KEYBOARD_STATUS);
        if(status & 1){
          unsigned char data = read_from_port(KEYBOARD_DATA);
          poll[20] = 48 + (data / 100);
          poll[21] = 48 + ((data%100) / 10);
          poll[22] = 48 + (data % 10);
        }

        prints_at(poll, 0x0f, 0);
    }
    return;
}
