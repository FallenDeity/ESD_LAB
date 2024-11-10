//
// Created by Triyan Mukherjee on 14-10-2024.
//

#include "buzzer/buzzer.h"

void buzzer_init() {
    LPC_PINCON->PINSEL0 &= ~(3 << (BUZZER_PIN * 2));
    LPC_GPIO0->FIODIR |= 1 << BUZZER_PIN;
    LPC_GPIO0->FIOCLR = 1 << BUZZER_PIN;
}

void buzzer_on() {
    LPC_GPIO0->FIOSET = 1 << BUZZER_PIN;
}

void buzzer_off() {
    LPC_GPIO0->FIOCLR = 1 << BUZZER_PIN;
}
