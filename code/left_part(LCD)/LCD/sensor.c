#include "sensor.h"

static uint16_t last_pressure0 = 0;
static uint16_t last_pressure1 = 0;

void ADC_init() {
    ADMUX = (1 << REFS0);  
    ADCSRA = (1 << ADEN) | (1 << ADPS2);
}

uint16_t ADC_read_channel(uint8_t channel) {
    ADMUX &= 0xF0;
    ADMUX |= (channel & 0x0F);
    _delay_us(10);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void DHT11_SetPinOutput(void) {
    DHT11_DDR |= (1 << DHT11_PIN);
}

void DHT11_SetPinInput(void) {
    DHT11_DDR &= ~(1 << DHT11_PIN);
    DHT11_PORT |= (1 << DHT11_PIN);  
}

uint8_t DHT11_ReadPin(void) {
    return (DHT11_PIN_IN & (1 << DHT11_PIN));
}

uint8_t DHT11_ReadByte(void) {
    uint8_t data = 0;
    for(uint8_t i = 0; i < 8; i++) {
        while(!DHT11_ReadPin());
        _delay_us(30); 
        
        if(DHT11_ReadPin()) {
            data |= (1 << (7-i));
            while(DHT11_ReadPin());
        }
    }
    return data;
}

uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity) {
    uint8_t data[5] = {0};
    uint16_t timeout;
    
    DHT11_SetPinOutput();
    DHT11_PORT &= ~(1 << DHT11_PIN);
    _delay_ms(20);
    DHT11_PORT |= (1 << DHT11_PIN);
    _delay_us(40);
    DHT11_SetPinInput();
    
    timeout = 0;
    while(DHT11_ReadPin() && timeout < 100) {
        _delay_us(1);
        timeout++;
    }
    if(timeout >= 100) return DHT11_ERROR;
    
    timeout = 0;
    while(!DHT11_ReadPin() && timeout < 100) {
        _delay_us(1);
        timeout++;
    }
    if(timeout >= 100) return DHT11_ERROR;
    
    while(DHT11_ReadPin());
    
    for(uint8_t i = 0; i < 5; i++) {
        data[i] = DHT11_ReadByte();
    }
    
    if(data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        return DHT11_ERROR;
    }
    
    *humidity = data[0];
    *temperature = data[2];
    
    return DHT11_OK;
}

void display_all_sensors(uint8_t temperature, uint8_t humidity, uint8_t status, 
                        uint8_t need_update_temp, uint16_t retry_count) {
    char buffer[32];
    
    uint16_t pressure0 = ADC_read_channel(0);
    uint16_t pressure1 = ADC_read_channel(1);
    
    int16_t diff0 = abs(pressure0 - last_pressure0);
    int16_t diff1 = abs(pressure1 - last_pressure1);
    
    if (status == DHT11_OK) {
        if (need_update_temp) {
            LCD_drawBlock(5, 25, LCD_WIDTH-1, 40, COLOR_BACKGROUND);
            snprintf(buffer, sizeof(buffer), "Temperature:%dC", temperature);
            LCD_drawString(5, 25, buffer, COLOR_TEMP, COLOR_BACKGROUND);
            
            LCD_drawBlock(5, 45, LCD_WIDTH-1, 60, COLOR_BACKGROUND);
            snprintf(buffer, sizeof(buffer), "Humidity:%d%%", humidity);
            LCD_drawString(5, 45, buffer, COLOR_HUMID, COLOR_BACKGROUND);
        }
    } else {
        if (need_update_temp) {
            LCD_drawBlock(5, 25, LCD_WIDTH-1, 60, COLOR_BACKGROUND);
            LCD_drawString(5, 35, "DHT11 Error!", COLOR_ERROR, COLOR_BACKGROUND);
            snprintf(buffer, sizeof(buffer), "Retry: %d", retry_count);
            LCD_drawString(5, 45, buffer, COLOR_ERROR, COLOR_BACKGROUND);
        }
    }
    
    if (diff0 > THRESHOLD) {  
        uint8_t percent0 = (pressure0 * 100) / 500;
        snprintf(buffer, sizeof(buffer), "Box1:%d%%", percent0);
        LCD_drawBlock(5, 75, LCD_WIDTH-1, 90, COLOR_BACKGROUND);
        LCD_drawString(5, 75, buffer, COLOR_PRESSURE, COLOR_BACKGROUND);
        last_pressure0 = pressure0;
    }
    
    if (diff1 > THRESHOLD) {  
        uint8_t percent1 = (pressure1 * 100) / 500;
        snprintf(buffer, sizeof(buffer), "Box2:%d%%", percent1);
        LCD_drawBlock(5, 95, LCD_WIDTH-1, 110, COLOR_BACKGROUND);
        LCD_drawString(5, 95, buffer, COLOR_PRESSURE, COLOR_BACKGROUND);
        last_pressure1 = pressure1;
    }
}