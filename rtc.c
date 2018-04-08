#define RTC_INDEX 0x70
#define RTC_DATA 0x71
#define NMI_DIS 0x80 // NMI DISABLE BIT


/*
 * This function switch the RTC
 */
void rtc_on(){
  //interrupts must be already disabled
  write_to_port(RTC_INDEX, NMI_DIS | 0xB);
  char value = read_from_port(RTC_DATA);
  write_to_port(RTC_INDEX, NMI_DIS | 0xB);
  write_to_port(RTC_DATA, 0x40 | value);
  //interrupts can now be enabled
}


void polling_rtc(){
    char* poll = "polling... rtc:    ";
    clear_screen(0x07);

    while(1){
      write_to_port(RTC_INDEX, NMI_DIS | 0);
      unsigned char data = read_from_port(RTC_DATA);
      poll[16] = 48 + (data / 100);
      poll[17] = 48 + ((data%100) / 10);
      poll[18] = 48 + (data % 10);

      prints_at(poll, 0x0f, 320);
    }
    return;
}
