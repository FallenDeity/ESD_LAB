//
// Created by Triyan Mukherjee on 14-10-2024.
//

#ifndef ESD_LED_H
#define ESD_LED_H

#include "LPC17xx.h"
#include "utils/utility.h"

#include <stdbool.h>
#include <stdint.h>

#define LED_LINE 4

#define MAX_MOLES_AT_A_TIME 3
#define MOLE_SHOW_TIME 2500
#define MIN_MOLE_DELAY 1000
#define MAX_MOLE_DELAY 2500

typedef struct {
    uint8_t id;
    uint32_t delay;
    uint32_t start_time;
    bool on_display;
    bool is_active;
} Mole;

Mole moles[MAX_MOLES_AT_A_TIME];

void mole_init();

void hit_mole(int id);

void reset_mole(Mole *mole);

void mole_update();

bool mole_active(int id);

#endif//ESD_LED_H
