/*
 * File:   lcd_humid.c
 * Author: jlx
 *
 * Created on November 15, 2024, 1:58 PM
 */


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD_GFX.h"
#include "ST7735.h"

// DHT11 GPIO define
#define DHT11_PIN PD5
#define DHT11_DDR DDRD
#define DHT11_PORT PORTD
#define DHT11_PIN_IN PIND

// DHT11 error code
#define DHT11_OK 0
#define DHT11_ERROR 1

// color
#define COLOR_BACKGROUND rgb565(0,0,0)      // Black
#define COLOR_TEXT       rgb565(255,255,255) // White
#define COLOR_TEMP       rgb565(255,0,0)     // Red
#define COLOR_HUMID      rgb565(0,0,255)     // Blue
#define COLOR_ERROR      rgb565(255,0,0)     // Red

// DHT11
static void DHT11_SetPinOutput(void) {
    DHT11_DDR |= (1 << DHT11_PIN);
}

static void DHT11_SetPinInput(void) {
    DHT11_DDR &= ~(1 << DHT11_PIN);
    DHT11_PORT |= (1 << DHT11_PIN);  
}

static uint8_t DHT11_ReadPin(void) {
    return (DHT11_PIN_IN & (1 << DHT11_PIN));
}

static uint8_t DHT11_ReadByte(void) {
    uint8_t data = 0;
    for(uint8_t i = 0; i < 8; i++) {
        // wait for 50us
        while(!DHT11_ReadPin());
        _delay_us(30); 
        
        // If remains high, 1
        if(DHT11_ReadPin()) {
            data |= (1 << (7-i));
            while(DHT11_ReadPin());  // wait for the end of 1 status
        }
    }
    return data;
}

uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity) {
    uint8_t data[5] = {0};
    uint16_t timeout;
    
    // send starting signal
    DHT11_SetPinOutput();
    DHT11_PORT &= ~(1 << DHT11_PIN);  // pull down
    _delay_ms(20);
    DHT11_PORT |= (1 << DHT11_PIN);   // pull up
    _delay_us(40);
    DHT11_SetPinInput();
    
    // Wait for response
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
    
    // Wait for the signal ending
    while(DHT11_ReadPin());
    
    // read data (5 Bytes 40 bits)
    for(uint8_t i = 0; i < 5; i++) {
        data[i] = DHT11_ReadByte();
    }
    
    // calculate the sum of data to check for error
    if(data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        return DHT11_ERROR;
    }
    
    *humidity = data[0];      // humidity
    *temperature = data[2];   // temperature
    
    return DHT11_OK;
}

// print on lcd
void display_readings(uint8_t temperature, uint8_t humidity, uint8_t status, uint16_t retry_count) {
    char buffer[32];
    
    // Initialize
    LCD_Clear(COLOR_BACKGROUND);
    
    if (status == DHT11_OK) {
        // Title
        LCD_drawString(5, 10, "DHT11 Readings:", COLOR_TEXT, COLOR_BACKGROUND);
        
        // Temperature
        snprintf(buffer, sizeof(buffer), "Temp: %d C", temperature);
        LCD_drawString(5, 40, buffer, COLOR_TEMP, COLOR_BACKGROUND);
        
        // Humidity
        snprintf(buffer, sizeof(buffer), "Humidity: %d%%", humidity);
        LCD_drawString(5, 70, buffer, COLOR_HUMID, COLOR_BACKGROUND);
        
        // Status
        snprintf(buffer, sizeof(buffer), "Status: OK");
        LCD_drawString(5, 100, buffer, COLOR_TEXT, COLOR_BACKGROUND);
        

    } else {
        // Print error information
        LCD_drawString(5, 30, "Sensor Error!", COLOR_ERROR, COLOR_BACKGROUND);
        snprintf(buffer, sizeof(buffer), "Retry: %d", retry_count);
        LCD_drawString(5, 60, buffer, COLOR_ERROR, COLOR_BACKGROUND);
        
        // status
        snprintf(buffer, sizeof(buffer), "Status: ERROR");
        LCD_drawString(5, 100, buffer, COLOR_ERROR, COLOR_BACKGROUND);
    }
}

int main(void) {
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    uint8_t last_temperature = 0;  // last time temperature
    uint8_t last_humidity = 0;     // last time humidity
    uint16_t retry_count = 0;
    uint8_t status;
    uint8_t last_status = DHT11_ERROR;  // last time status
    uint8_t need_update = 1;      // need update or not
    
    // initialize
    lcd_init();
    LCD_setScreen(COLOR_BACKGROUND);
    
    // wait for dht11 to stablize
    DHT11_SetPinOutput();
    DHT11_PORT |= (1 << DHT11_PIN);
    _delay_ms(3000);  
    
    // menu
    LCD_drawString(5, 60, "Starting...", COLOR_TEXT, COLOR_BACKGROUND);
    _delay_ms(1000);
    
    while(1) {
        // read from dht11
        status = DHT11_Read(&temperature, &humidity);
        
        // check if need update
        need_update = 0;  // do not need 
        
        if(status == DHT11_OK) {
            if(last_status != DHT11_OK) {
                // Need update if last time is error
                need_update = 1;
            } else {
                // check if data have changed
                if(temperature != last_temperature || humidity != last_humidity) {
                    need_update = 1;
                }
            }
            retry_count = 0;
        } else {
            if(last_status == DHT11_OK || retry_count == 0) {
                // from success to failure, need update
                need_update = 1;
            } else if(retry_count % 10 == 0) {
                // update once every retry 10 times
                need_update = 1;
            }
            retry_count++;
        }
        
        if(need_update) {
            display_readings(temperature, humidity, status, retry_count);
            
            // update
            last_temperature = temperature;
            last_humidity = humidity;
            last_status = status;
        }
        
        // wait for last sampling
        _delay_ms(2500);
    }
    
    return 0;
}