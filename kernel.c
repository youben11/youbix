void clear_screen(char color);
void prints_at(char* str, char color, int pos);

int cur_pos = 0; // cursor position
char* video_mem = (char*) 0xb8000; // video memory

void kmain(void){
  char* message = "Hello Kernel Dev";

  clear_screen(0x07);
  prints_at(message, 0x0f, 0);

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
