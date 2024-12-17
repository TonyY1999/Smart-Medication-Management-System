#include "servo.h"

// Initialize PWM for both servos
void init_servo() {
    // Set servo pins as output
    DDRD |= (1 << SERVO1_PIN) | (1 << SERVO2_PIN);
    
    // Setup Timer0 for the right Servo
    TCCR0A |= (1 << WGM00);          // PWM, Phase Correct mode
    TCCR0A |= (1 << COM0B1);         // Non-inverting mode
    TCCR0B |= (1 << CS00) | (1 << CS02);  // Set prescaler to 1024
    OCR0A = 156;  // Set the PWM frequency to 50Hz
    OCR0B = 12;  // 0 degrees
    
    // Setup Timer2 for Servo2
    // Reset Timer2 control registers
    TCCR2A = 0;
    TCCR2B = 0;
    
    // Configure Timer2
    TCCR2A |= (1 << WGM20);          // PWM, Phase Correct mode
    TCCR2A |= (1 << COM2B1);         // Clear OC2B on Compare Match
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);  // Prescaler 1024
    OCR2A = 156;    // Top value for 50Hz
    OCR2B = 4;      // Initial position (0 degree)
}

// Servo2 control functions (using Timer2)
void open_left_box() {
    OCR2B = 12;  // Set duty cycle for 90 degrees
}

void close_left_box() {
    OCR2B = 4;   // Set duty cycle for 0 degrees
}

// Servo1 control functions (using Timer0)
void open_right_box() {
    OCR0B = 4;  // 90 degrees
}

void close_right_box() {
    OCR0B = 12;   // 0 degrees
}