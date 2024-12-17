#include "wireless_control.h"
#include "servo.h"

// Initialize the control system
void init_control() {
    // Configure LED pins as outputs
    DDRC |= (1 << LED1) | (1 << LED2);
    // Initialize LEDs to off state
    PORTC &= ~((1 << LED1) | (1 << LED2));
    
    // Configure input pins and disable their pull-up resistors
    DDRB &= ~((1 << INPUT1) | (1 << INPUT2) | (1 << INPUT3));
    PORTB &= ~((1 << INPUT1) | (1 << INPUT2) | (1 << INPUT3));
    
    // Set servo to initial closed position
    close_left_box();
    close_right_box();
}

// Response handler functions
void response_V1() {
    PORTC |= (1 << LED1);  
    open_right_box(); 
}

// Response handler functions
void after_V1() {
    PORTC &= ~(1 << LED1);  
    close_right_box();  
}

void response_V2() {
    PORTC |= (1 << LED2);
    open_left_box();
}

void after_V2() {
    PORTC &= ~(1 << LED2);
    close_left_box();
}

void response_V3() {
    PORTC |= ((1 << LED1) | (1 << LED2));
    open_right_box();
    open_left_box();
}

void after_V3() {
    PORTC &= ~((1 << LED1) | (1 << LED2));
    close_right_box();
    close_left_box();
}
