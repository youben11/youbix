#include "keycode.h"

#define KEYBOARD_DATA 0x60
#define KEYBOARD_STATUS 0x64

extern void prints_at(char* str, char color, int pos);
extern void clear_screen(char color);
extern char read_from_port(short port);
extern void write_to_port(short port, char value);
extern int cur_pos;

/*
 * This function is still in dev,
 * it's the fucntion that get called
 * when a keyboard interrupt occur,
 * it read the keycode and print it
 * to the screen.
 */
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

/*
 * This function just keep reading
 * from the IO ports of the keyboard
 * if it signal that there is data
 * to read (lsb(status) == 1) then
 * it read it and format the keycode
 * into a number and print it to the
 * screen.
 * it doesn't matter if there is an
 * interrupt or not, it just keep reading.
 */
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
