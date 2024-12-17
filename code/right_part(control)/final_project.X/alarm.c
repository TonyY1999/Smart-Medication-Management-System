#include <avr/interrupt.h>
#include "alarm.h"

// Global variables
volatile uint8_t alarm_active = 0;        // Flag to track if alarm is active
volatile uint16_t buzzer_counter = 0;     // Counter for melody timing
volatile uint8_t current_note = 0;        // Current note index in melody
volatile uint8_t prev_rtc_state = 0;      // Previous RTC button state
volatile uint8_t prev_fp_state = 0;       // Previous FP button state

// Buzzer frequency array for alarm melody (Hz)
const uint16_t alarm_notes[] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_E4};
const uint8_t num_notes = 4;

/**
 * Initialize alarm system
 */
void init_alarm() {
    // Configure outputs
    DDRD |= (1 << BUZZER_PIN);           // Set buzzer pin as output
    DDRB |= (1 << ALARM_LED);            // Set LED pin as output
   
    
    // Initialize Timer3 for Buzzer PWM
    TCCR3A = 0;
    TCCR3B = 0;
    
    // Configure Timer3 for Fast PWM mode
    TCCR3A |= (1 << COM3A1) | (1 << WGM31);  // Fast PWM, non-inverting mode
    TCCR3B |= (1 << WGM32) | (1 << WGM33);   // Fast PWM mode
    TCCR3B |= (1 << CS31);                    // Prescaler = 8
    
    // Initialize to default state
    stop_alarm();
}

/**
 * Start the alarm
 * Activates LED and buzzer with melody
 */
void start_alarm() {
    if (!alarm_active) {
        alarm_active = 1;
        PORTB |= (1 << ALARM_LED);       // Turn on LED
        
        // Configure initial buzzer frequency
        uint16_t icr_value = F_CPU / (8 * alarm_notes[0]) - 1;
        ICR3 = icr_value;
        OCR3A = icr_value / 2;           // 50% duty cycle
        
        // Enable Timer3 PWM output
        TCCR3A |= (1 << COM3A1);         // Connect PWM to output pin
        
        // Enable Timer3 compare match interrupt
        TIMSK3 |= (1 << OCIE3A);
        
        // Reset counters
        buzzer_counter = 0;
        current_note = 0;
    }
}

/**
 * Stop the alarm
 * Deactivates LED and buzzer
 */
void stop_alarm() {
    alarm_active = 0;
    PORTB &= ~(1 << ALARM_LED);          // Turn off LED
    TCCR3A &= ~(1 << COM3A1);            // Disconnect PWM
    TIMSK3 &= ~(1 << OCIE3A);            // Disable interrupt
}  

/**
 * Timer3 Compare Match A interrupt handler
 * Generates melody by changing notes periodically
 */
ISR(TIMER3_COMPA_vect) {
    if (alarm_active) {
        buzzer_counter++;
        
        // Change note every 500ms
        if (buzzer_counter >= 500) {
            buzzer_counter = 0;
            current_note = (current_note + 1) % num_notes;
            
            // Update frequency
            uint16_t icr_value = F_CPU / (8 * alarm_notes[current_note]) - 1;
            ICR3 = icr_value;
            OCR3A = icr_value / 2;        // Maintain 50% duty cycle
        }
    }
}
