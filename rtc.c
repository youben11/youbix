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
  write_to_port(RTC_DATA, value | 0x40);
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
  write_to_port(RTC_INDEX, NMI_DIS | 0xA);
  char value = read_from_port(RTC_DATA);
  write_to_port(RTC_INDEX, NMI_DIS | 0xA);
  write_to_port(RTC_DATA, (value & 0xf0) | divider);
  //interrupts can now be enabled
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
      write_to_port(RTC_INDEX, NMI_DIS | 0);
      unsigned char data = read_from_port(RTC_DATA);
      poll[16] = 48 + (data / 100);
      poll[17] = 48 + ((data%100) / 10);
      poll[18] = 48 + (data % 10);

      prints_at(poll, 0x0f, 320);
    }
    return;
}
