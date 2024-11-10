//
// Created by Triyan Mukherjee on 08-10-2024.
//

#ifndef ESD_UTILITY_H
#define ESD_UTILITY_H

#include "LPC17xx.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void delay(uint32_t delay_ms);

void delay_us(uint32_t delay_us);

void random_init();

int randint(int min, int max);

void DelayTimer_Init(void);

uint32_t millis();

char *format_time(uint32_t time);

#endif //ESD_UTILITY_H
