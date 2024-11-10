//
// Created by Triyan Mukherjee on 14-10-2024.
//

#ifndef ESD_BUZZER_H
#define ESD_BUZZER_H

#include "LPC17xx.h"

#define BUZZER_PIN 15

void buzzer_init();

void buzzer_on();

void buzzer_off();

#endif //ESD_BUZZER_H
