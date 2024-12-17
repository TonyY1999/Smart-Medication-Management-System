#include "pressure.h"

// Initialize ADC
void ADC_init() {
    ADMUX = (1 << REFS0);  
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, set prescaler to 128
}

// Function to read the ADC value
uint16_t ADC_read() {
    ADCSRA |= (1 << ADSC);  // Start the conversion
    while (ADCSRA & (1 << ADSC));  // Wait for the conversion to complete
    return ADC;
}