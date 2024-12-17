#ifndef WIRELESS_CONTROL_H
#define WIRELESS_CONTROL_H

#include "config.h"

// Servo pins
#define SERVO1_PIN PD5  // Timer0
#define SERVO2_PIN PD3  // Timer2

// ESP32 communication pin
#define INPUT1 PB0
#define INPUT2 PB1 
#define INPUT3 PB2   

// Pin Definitions
#define LED1 PC1  // The right pillbox inidcator
#define LED2 PC0  // The left pillbox inidcator

// Function declarations
void init_control();

void response_V1();

void after_V1();

void response_V2();

void after_V2();

void response_V3();

void after_V3();

#endif