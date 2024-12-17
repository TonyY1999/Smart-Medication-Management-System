//#include <avr/eeprom.h>
//
//#include "alarm.h"
//#include "config.h"
//#include "fingerprint.h"
//#include "servo.h"
//#include "uart.h"
//#include "uart_1.h"
//#include "wireless_control.h"
//#include "rtc.h"
//
//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
//
//// EEPROM????
//#define EEPROM_TIME_SET_ADDR 0
//#define TIME_SET_MAGIC 0x55   
//
//volatile uint8_t alarm_flag = 0;
//volatile uint8_t time_set_flag = 0;
//
//
//void init_timer1(void) {
//    // 
//    TCCR1B |= (1 << WGM12);
//    
//    // 
//    TCCR1B |= (1 << CS11) | (1 << CS10);
//    
//    // 
//    OCR1A = 24999;  // 
//    
//    // 
//    TIMSK1 |= (1 << OCIE1A);
//}
//
//ISR(TIMER1_COMPA_vect) {
//    if (DS3231_CheckAlarm1()) {          // 
//        alarm_flag = 1;                   // 
//        DS3231_ClearAlarm1Flag();        // 
//    }
//}
//
//void print_time(void) {
//    uint8_t hour, min, sec;
//    DS3231_GetTime(&hour, &min, &sec);
//    char time_str[20];
//    snprintf(time_str, sizeof(time_str), "Time: %02d:%02d:%02d\r\n", hour, min, sec);
//    UART_putstring(time_str);
//}
//
//// Initilize the whole system
//void init_sys() {
//    // Initialize the UART comminucation
//    // UART_init(BAUD_PRESCALER);
//    UART1_init(BAUD_PRESCALER);
//    
//    // Initialize alarm
//    init_alarm();
//    
//    // Initialize PWM for servo
//    init_servo();
//    
//    // Initialize wireless control
//    init_control();
//}
//
//// ?EEPROM?????????
//uint8_t is_time_initialized(void) {
//    return eeprom_read_byte((uint8_t*)EEPROM_TIME_SET_ADDR) == TIME_SET_MAGIC;
//}
//
//void mark_time_initialized(void) {
//    eeprom_write_byte((uint8_t*)EEPROM_TIME_SET_ADDR, TIME_SET_MAGIC);
//}
//
//// ??RTC????????
//void setup_rtc_time(void) {
//    uint8_t hour, min, sec;
//    
//    // ????RTC??
//    DS3231_GetTime(&hour, &min, &sec);
//    //UART_putstring("Current RTC time: ");
//    print_time();
//    
//    // ?????????
//    if (!is_time_initialized()) {
//        //UART_putstring("First time setup - Initializing RTC...\r\n");
//        
//        // ?????????
//        DS3231_SetTime(10, 59, 50);
//        DS3231_SetAlarm1(11, 0);
//        DS3231_EnableAlarm1();
//        
//        // ???????
//        mark_time_initialized();
//        
//        //UART_putstring("RTC initialization complete\r\n");
//    } else {
//        //UART_putstring("RTC already initialized\r\n");
//    }
//}
//
//int main() {
//    // 1. ??????????????????
//    // cli();
//    
//    // 2. ?????
//    init_sys();
//    init_timer1();
//    TWI_Init();
//    
//    // 3. RTC?????EEPROM?????
//    setup_rtc_time();
//    
//    // start_alarm();
//    
//    // 4. ??????
//    sei();
//    // UART_putstring("System initialization complete\r\n");
//    
//    // 5. ???
//    while(1) {
//        
//        // start_alarm();
////        // ????
////        print_time();
////        
//        // ??????
//        if (alarm_flag) {
//            cli();  
//            
//            UART_putstring("Alarm triggered!\r\n");
//            start_alarm();
//            alarm_flag = 0;
//            
//            sei();  
//        }
//        
//        _delay_ms(1000);
//    }
//
//    return 0;
//}
//
////#include <avr/io.h>
////#include <avr/interrupt.h>
////#include "alarm.h"
////#include "config.h"
////#include "fingerprint.h"
////#include "servo.h"
////#include "uart.h"
////#include "uart_1.h"
////#include "wireless_control.h"
////#include "rtc.h"
////
////void init_sys() {
////    // ???????
////    init_servo();
////    init_control();
////    UART_init(BAUD_PRESCALER);
////    UART1_init(BAUD_PRESCALER);
////    
////    // ?????????
////    uint8_t timer3_config = TCCR3A;
////    char debug_str[50];
////    snprintf(debug_str, sizeof(debug_str), 
////             "Initial Timer3 config: %02X\r\n", timer3_config);
////    UART_putstring(debug_str);
////    
////    // ??????????
////    if (TCCR3A != timer3_config) {
////        TCCR3A = timer3_config;  // ??????
////        snprintf(debug_str, sizeof(debug_str), 
////                "Restored Timer3 config to: %02X\r\n", TCCR3A);
////        UART_putstring(debug_str);
////    }
////    
////    // ????????
////    init_alarm();
////    
////    // ??????
////    snprintf(debug_str, sizeof(debug_str), 
////             "Final Timer3 config: %02X\r\n", TCCR3A);
////    UART_putstring(debug_str);
////}
////
////int main() {
////    
////    // ?????
//////    init_servo();
//////    init_control();
//////    //UART_init(BAUD_PRESCALER);
//////    UART1_init(BAUD_PRESCALER);
////    //init_alarm();
////    
////    init_sys();
////    
////    // ?????
////    
////    sei();
////    start_alarm();
////    
////    
////    while(1) {
////
////    }
////    
////    return 0;
////}