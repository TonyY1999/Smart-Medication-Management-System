#include "alarm.h"
#include "config.h"
#include "fingerprint.h"
#include "servo.h"
#include "uart.h"
#include "uart_1.h"
#include "wireless_control.h"
#include "rtc.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

volatile uint8_t alarm_flag = 0;

#define EEPROM_TIME_SET_ADDR 0
#define TIME_SET_MAGIC 0x55   

void init_timer1(void) {
    // 
    TCCR1B |= (1 << WGM12);
    
    // 
    TCCR1B |= (1 << CS11) | (1 << CS10);
    
    // 
    OCR1A = 24999;  // 
    
    // 
    TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
    if (DS3231_CheckAlarm1()) {          // 
        alarm_flag = 1;                   // 
        DS3231_ClearAlarm1Flag();        // 
    }
}

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
//  
//    // ?????????
//    if (!is_time_initialized()) {
//        
//        // ?????????
//        DS3231_SetTime(10, 59, 30);
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

// Initilize the whole system
void init_sys() {
    // Initialize the UART comminucation
    // UART_init(BAUD_PRESCALER);
    UART1_init(BAUD_PRESCALER);
    
    // Initialize alarm
    init_alarm();
    
    // Initialize PWM for servo
    init_servo();
    
    // Initialize wireless control
    init_control();
}

int main() {
    cli();
    
    init_sys();  // Initialize the system
    init_timer1();
    TWI_Init();
    
    // setup_rtc_time();
    
    DS3231_SetTime(10, 59, 40);
    DS3231_SetAlarm1(11, 0);
    DS3231_EnableAlarm1();
      
    sei();
    
    while(1) {
        
        // start_alarm();
        
        // Check INPUT1 and trigger response if active       
        if(PINB & (1 << INPUT1)) {
            response_V1();
            stop_alarm();
        }
        // Check INPUT2 and trigger response if active
        else if(PINB & (1 << INPUT2)) {
            response_V2();
            stop_alarm();
        }
        // Check INPUT3 and trigger response if active
        else if(PINB & (1 << INPUT3)) {
            response_V3();
            stop_alarm();
        }
        else {
            after_V1();
            after_V2();
            after_V3();
        }      
        
        
        if (alarm_flag) {   
           UART_putstring("Alarm triggered!\r\n");
           start_alarm();
            
           alarm_flag = 0;
        }
                          
        // Fingerprint detection
        // If a finger be detected
        if (gen_img() == 0 && img_to_tz1() == 0) {
            uint8_t user_id = fingerprint_search();  // Get the id if this fingerprint can be found in the library
            
            // If fingerprint match one in library
            if(user_id != 0xFE) {
                stop_alarm();  // Turn the alarm off when detected
                
                // If this is patient0
                if(user_id == 0)  {
                    response_V1();
                }
                // If this is patient1
                else if(user_id == 1) {
                    response_V2();
                }
                // If this is patient2
                else if(user_id == 2) {
                    response_V3();
                } 
            }
            
            // Close corresponding pillbox based on detected fingerprint
            while(1) {
                if(gen_img() == 0 && img_to_tz1() == 0) {
                    user_id = fingerprint_search();
                    
                    // If this is patient0
                    if(user_id == 0)  {
                        after_V1();
                        break;
                    }
                    // If this is patient1
                    else if(user_id == 1) {
                        after_V2();
                        break;
                    }
                    // If this is patient2
                    else if(user_id == 2) {
                        after_V3();
                        break;
                    }
                }
            }
        }
    
        _delay_ms(1000);   
    }

    return 0;
}
