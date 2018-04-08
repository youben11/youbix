#define RTC_INDEX 0x70
#define RTC_DATA 0x71
#define NMI_DIS 0x80 // NMI DISABLE BIT
#define RTC_BON 0x40 // BIT ON at pos 6 (start count at 0)
#define RTC_SEC 0x00 //index for seconds
#define RTC_MIN 0x02 //index for minutes
#define RTC_HOU 0x04 //index for hours
#define RTC_DOM 0x07 //index for day of month
#define RTC_MON 0x08 //index for month
#define RTC_YEA 0x09 //index for year
#define RTC_SRA 0x0A //index for status register A
#define RTC_SRB 0x0B //index for status register B

extern void write_to_port(short port, char value);
extern char read_from_port(short port);
extern void prints_at(char* str, char color, int pos);
extern void clear_screen(char color);

char date[11];
char time[9];
char rtc_status_b;

/*
 * This function turns on the RTC
 * and save the status register B.
 */
void rtc_on(){
  //interrupts must be already disabled
  write_to_port(RTC_INDEX, NMI_DIS | RTC_SRB);
  char value = read_from_port(RTC_DATA);
  //save the status register B for future uses
  rtc_status_b = value | RTC_BON;
  write_to_port(RTC_INDEX, NMI_DIS | RTC_SRB);
  write_to_port(RTC_DATA, value | RTC_BON);
  //interrupts can now be enabled
}

/*
 * This function set the divider
 * of the frequencie so we can get
 * interrupts at a certain rate.
 */
void rtc_set_freq(char divider){
  //interrupts must be already disabled
  divider = divider & 0x0f;
  write_to_port(RTC_INDEX, NMI_DIS | RTC_SRA);
  char value = read_from_port(RTC_DATA);
  write_to_port(RTC_INDEX, NMI_DIS | RTC_SRA);
  write_to_port(RTC_DATA, (value & 0xf0) | divider);
  //interrupts can now be enabled
}

/*
 * This function simply return 1
 * if the format is binary and 0
 * if the format is bcd.
 */
char is_binary(){
  return ((rtc_status_b & 0x4) >> 2);
}
/*
 * This function simply return 1
 * if the clock format is 24hour
 * and 0 if it's 12hours.
 */
char is_format24(){
  return ((rtc_status_b & 0x2) >> 1);
}

/*
 * This function convert a bcd value
 * to the binary.
 */
unsigned char bcd_to_binary(unsigned char bcd){
  return ((bcd & 0x0f) + (bcd >> 4) * 10);
}

unsigned char rtc_get_seconds(){
  //interrupts must be already disabled
  write_to_port(RTC_INDEX, NMI_DIS | RTC_SEC);
  unsigned char sec = read_from_port(RTC_DATA);
  if(!is_binary())
    sec = bcd_to_binary(sec);
  return sec;
  //interrupts can now be enabled
}

unsigned char rtc_get_minutes(){
  //interrupts must be already disabled
  write_to_port(RTC_INDEX, NMI_DIS | RTC_MIN);
  unsigned char min = read_from_port(RTC_DATA);
  if(!is_binary())
    min = bcd_to_binary(min);
  return min;
  //interrupts can now be enabled
}

unsigned char rtc_get_hours(){
  //interrupts must be already disabled
  write_to_port(RTC_INDEX, NMI_DIS | RTC_HOU);
  unsigned char hour = read_from_port(RTC_DATA);
  if(!is_binary())
    hour = bcd_to_binary(hour);
  return hour;
  //interrupts can now be enabled
}

void int_to_str(unsigned char number, char* str){
  int len = 1;
  int max = 10;
  while(max <= number){
    len++;
    max *= 10;
  }
  for(int i=0; i < len; i++){
    str[i] = 48 + ((number % (max*10)) / max);
    max /= 10;
  }
  return;
}

void rtc_handler(){
  unsigned char value;
  //get and write hours
  value = rtc_get_hours();
  int_to_str(value, time);
  time[2] = ':',
  //get and write minutes
  value = rtc_get_minutes();
  int_to_str(value, time);
  time[5] = ':',
  //get and write seconds
  value = rtc_get_seconds();
  int_to_str(value, time);
  time[8] = '\0',
  prints_at(time, 0x07, 80*25*2 - 20);
}

/*
 * This function just keep reading
 * from the IO ports of the RTC,
 * the one that hold the seconds
 * and print it to the screen.
 * it doesn't matter if there is an
 * interrupt or not, it just keep reading.
 */
void polling_rtc(){
    char* poll = "polling... rtc:    ";
    clear_screen(0x07);

    while(1){
      write_to_port(RTC_INDEX, NMI_DIS | RTC_SRA);
      unsigned char data = read_from_port(RTC_DATA);
      poll[16] = 48 + (data / 100);
      poll[17] = 48 + ((data%100) / 10);
      poll[18] = 48 + (data % 10);

      prints_at(poll, 0x0f, 320);
    }
    return;
}
