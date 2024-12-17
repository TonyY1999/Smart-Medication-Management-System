#ifndef ALARM_H
#define ALARM_H

#include "config.h"

// Pin Definitions
#define BUZZER_PIN      PD0     // Timer3 OC3A for buzzer
#define ALARM_LED       PB5     // Onboard LED

// Musical notes frequencies
#define NOTE_C4  262
#define NOTE_E4  330
#define NOTE_G4  392

// Time definitions
#define DEBOUNCE_DELAY 20      // 20ms for debouncing

// Function declarations
void init_alarm();

void start_alarm();

void stop_alarm();

#endif
