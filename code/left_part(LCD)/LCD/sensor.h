#ifndef SENSOR_CONTROL_H
#define SENSOR_CONTROL_H

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD_GFX.h"
#include "ST7735.h"
#include <stdlib.h>

// DHT11 GPIO define
#define DHT11_PIN PD5
#define DHT11_DDR DDRD
#define DHT11_PORT PORTD
#define DHT11_PIN_IN PIND

// DHT11 status define
#define DHT11_OK 0
#define DHT11_ERROR 1

#define THRESHOLD 5

// Color define
#define COLOR_BACKGROUND rgb565(0,0,0)
#define COLOR_TEXT       rgb565(255,255,255)
#define COLOR_TEMP       rgb565(255,0,0)
#define COLOR_HUMID      rgb565(0,0,255)
#define COLOR_PRESSURE   rgb565(0,255,0)
#define COLOR_ERROR      rgb565(255,0,0)

// Function declarations
void ADC_init(void);
uint16_t ADC_read_channel(uint8_t channel);
void DHT11_SetPinOutput(void);
void DHT11_SetPinInput(void);
uint8_t DHT11_ReadPin(void);
uint8_t DHT11_ReadByte(void);
uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity);
void display_all_sensors(uint8_t temperature, uint8_t humidity, uint8_t status, 
                        uint8_t need_update_temp, uint16_t retry_count);

#endif