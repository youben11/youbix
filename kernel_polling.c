
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
  prints_at(message, 0x0f, 0);

  //two new line
  cur_pos += 160 - (cur_pos % 160);
  cur_pos += 160;
  prints_at("You can type now! give it a try:", 0x0f, cur_pos);


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
