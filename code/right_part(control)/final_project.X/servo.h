#ifndef SERVO_H
#define SERVO_H

#include "config.h"

// Servo pins
#define SERVO1_PIN PD5  // Timer0
#define SERVO2_PIN PD3  // Timer2

// Function declarations
void init_servo();

void open_left_box();

void close_left_box();

void open_right_box();

void close_right_box();

#endif