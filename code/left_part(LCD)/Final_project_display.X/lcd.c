#include "sensor.h"
static uint16_t last_pressure0 = 0;
static uint16_t last_pressure1 = 0;
 
int main(void) {
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    uint8_t last_temperature = 0;
    uint8_t last_humidity = 0;
    uint16_t retry_count = 0;
    uint8_t status;
    uint8_t last_status = DHT11_ERROR;
    uint8_t need_update = 1;
    
    ADC_init();
    lcd_init();
    LCD_setScreen(COLOR_BACKGROUND);
    
    DHT11_SetPinOutput();
    DHT11_PORT |= (1 << DHT11_PIN);
    _delay_ms(3000);
    
    LCD_drawString(5, 5, "Sensor Data:", COLOR_TEXT, COLOR_BACKGROUND);
    _delay_ms(1000);
    
    last_pressure0 = ADC_read_channel(0);
    last_pressure1 = ADC_read_channel(1);
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Box1:100%%");
    LCD_drawString(5, 75, buffer, COLOR_PRESSURE, COLOR_BACKGROUND);
    snprintf(buffer, sizeof(buffer), "Box2:100%%");
    LCD_drawString(5, 95, buffer, COLOR_PRESSURE, COLOR_BACKGROUND);
    
    while(1) {
        status = DHT11_Read(&temperature, &humidity);
        
        need_update = 0;
        
        if(status == DHT11_OK) {
            if(last_status != DHT11_OK) {
                need_update = 1;
            } else {
                if(temperature != last_temperature || humidity != last_humidity) {
                    need_update = 1;
                }
            }
            retry_count = 0;
        } else {
            if(last_status == DHT11_OK || retry_count == 0) {
                need_update = 1;
            } else if(retry_count % 10 == 0) {
                need_update = 1;
            }
            retry_count++;
        }
        
        display_all_sensors(temperature, humidity, status, need_update, retry_count);
        
        if(need_update) {
            last_temperature = temperature;
            last_humidity = humidity;
            last_status = status;
        }
        
        _delay_ms(2000);
    }
    
    return 0;
}