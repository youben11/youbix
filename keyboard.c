#include "keycode.h"

#define KEYBOARD_DATA 0x60
#define KEYBOARD_STATUS 0x64

extern void prints_at(char* str, char color, int pos);
extern void clear_screen(char color);
extern void int_to_str(unsigned char number, char* str, unsigned char elen);
extern char read_from_port(short port);
extern void write_to_port(short port, char value);
extern void pic1_eoi();
extern void pic2_eoi();
extern int cur_pos;

/*
 * This function is still in dev,
 * it's the fucntion that get called
 * when a keyboard interrupt occur,
 * it read the keycode and print it
 * to the screen.
 */
void keyboard_handler(){
  pic1_eoi();
  unsigned char status = read_from_port(KEYBOARD_STATUS);
  unsigned char buf[2];
  unsigned char data = read_from_port(KEYBOARD_DATA)-1;

  if (status & 0x80){
    // key released
    return;
  }

  if(status & 1 && data < 256){
    buf[0] = keycode[data];

    if (buf[0] == RET){ // Delete
      cur_pos -= 2;
      prints_at(" ", 0x07, cur_pos);
      cur_pos -= 2;
    }

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
          int_to_str(data, poll+20, 3);
        }

        prints_at(poll, 0x0f, 0);
    }
    return;
}
