#include "rtc.h"

// TWI Setup
void TWI_Init() {
    // 100kHz (F_CPU = 16MHz)
    TWSR0 &= ~((1 << TWPS1) | (1 << TWPS0));
    TWBR0 = 72; 
    TWCR0 = (1<<TWEN);
}

void TWI_Start() {
    TWCR0 = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR0 & (1<<TWINT)));
}

void TWI_Stop() {
    TWCR0 = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    while (TWCR0 & (1<<TWSTO));
}

void TWI_Write(uint8_t data) {
    TWDR0 = data;
    TWCR0 = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR0 & (1<<TWINT)));
}

uint8_t TWI_Read(uint8_t ack) {
    TWCR0 = (1<<TWINT)|(1<<TWEN)|(ack<<TWEA);
    while (!(TWCR0 & (1<<TWINT)));
    return TWDR0;
}

// DS3231write
void DS3231_Write(uint8_t reg, uint8_t data) {
    TWI_Start();
    TWI_Write(DS3231_ADDR << 1);
    TWI_Write(reg);
    TWI_Write(data);
    TWI_Stop();
}

uint8_t DS3231_Read(uint8_t reg) {
    uint8_t data;
    TWI_Start();
    TWI_Write(DS3231_ADDR << 1);
    TWI_Write(reg);
    TWI_Start();
    TWI_Write((DS3231_ADDR << 1) | 1);
    data = TWI_Read(0);
    TWI_Stop();
    return data;
}

// BCD coverting to DEC
uint8_t BCD_to_DEC(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// DEC converting to BCD
uint8_t DEC_to_BCD(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

// Set time
void DS3231_SetTime(uint8_t hour, uint8_t min, uint8_t sec) {
    DS3231_Write(0x00, DEC_to_BCD(sec));
    DS3231_Write(0x01, DEC_to_BCD(min));
    DS3231_Write(0x02, DEC_to_BCD(hour));
}

// Set Date
void DS3231_SetDate(uint8_t date, uint8_t month, uint8_t year) {
    DS3231_Write(0x04, DEC_to_BCD(date));
    DS3231_Write(0x05, DEC_to_BCD(month));
    DS3231_Write(0x06, DEC_to_BCD(year));
}

//Read time
void DS3231_GetTime(uint8_t *hour, uint8_t *min, uint8_t *sec) {
    *sec = BCD_to_DEC(DS3231_Read(0x00));
    *min = BCD_to_DEC(DS3231_Read(0x01));
    *hour = BCD_to_DEC(DS3231_Read(0x02));
}

void TWI_WriteWithCheck(uint8_t data) {
    TWDR0 = data;
    TWCR0 = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR0 & (1 << TWINT)));
    if ((TWSR0 & 0xF8) != TW_MT_DATA_ACK) {
        UART_putstring("Data write failed!\r\n");
    }
}

void DS3231_SetAlarm1(uint8_t hour, uint8_t min) {
    // Set Alarm Time
    DS3231_Write(ALARM1_HOURS, DEC_to_BCD(hour) & 0x7F);     
    DS3231_Write(ALARM1_MINUTES, DEC_to_BCD(min) & 0x7F);    
    DS3231_Write(ALARM1_SECONDS, 0x00);                       
    
    DS3231_Write(ALARM1_DAY, 0x80); 
}

void DS3231_EnableAlarm1(void) {
    uint8_t control = DS3231_Read(CONTROL_REG);
    control |= 0x05;    // INTCN = 1, A1IE = 1
    DS3231_Write(CONTROL_REG, control);
}

uint8_t DS3231_CheckAlarm1(void) {
    uint8_t status = DS3231_Read(STATUS_REG);
    return (status & 0x01);    
}

void DS3231_ClearAlarm1Flag(void) {
    uint8_t status = DS3231_Read(STATUS_REG);
    status &= ~0x01;    
    DS3231_Write(STATUS_REG, status);
}
