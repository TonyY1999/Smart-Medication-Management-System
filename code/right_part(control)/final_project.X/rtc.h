#ifndef RTC_H
#define RTC_H

#include "config.h"
#include "uart.h"

// DS3231 Address
#define DS3231_ADDR 0x68

// TWI Parameters
#define TW_START        0x08
#define TW_REP_START    0x10
#define TW_MT_SLA_ACK   0x18
#define TW_MT_DATA_ACK  0x28
#define TW_MR_SLA_ACK   0x40
#define TW_MR_DATA_NACK 0x58

#define ALARM1_SECONDS  0x07
#define ALARM1_MINUTES  0x08
#define ALARM1_HOURS    0x09
#define ALARM1_DAY      0x0A
#define CONTROL_REG     0x0E
#define STATUS_REG      0x0F

// Function declarations
void TWI_Init();

void TWI_Start();

void TWI_Stop();

void TWI_Write(uint8_t data);

uint8_t TWI_Read(uint8_t ack);

void DS3231_Write(uint8_t reg, uint8_t data);

uint8_t DS3231_Read(uint8_t reg);

uint8_t BCD_to_DEC(uint8_t bcd);

uint8_t DEC_to_BCD(uint8_t dec);

void DS3231_SetTime(uint8_t hour, uint8_t min, uint8_t sec);

void DS3231_SetDate(uint8_t date, uint8_t month, uint8_t year);

void DS3231_GetTime(uint8_t *hour, uint8_t *min, uint8_t *sec);

void TWI_WriteWithCheck(uint8_t data);

void DS3231_SetAlarm1(uint8_t hour, uint8_t min);

uint8_t DS3231_CheckAlarm1(void);

void DS3231_ClearAlarm1Flag(void); 

void DS3231_EnableAlarm1(void); 

#endif
