//
// Created by Triyan Mukherjee on 14-10-2024.
//

#include "leds/led.h"

bool mole_present(int id) {
    for (int i = 0; i < MAX_MOLES_AT_A_TIME; i++) {
        if (moles[i].id == id) {
            return true;
        }
    }
    return false;
}

void mole_init() {
    LPC_PINCON->PINSEL0 &= ~(0xFF << (LED_LINE * 2));
    LPC_GPIO0->FIODIR |= 0xFF << LED_LINE;
    LPC_GPIO0->FIOCLR = 0xFF << LED_LINE;

    for (int i = 0; i < MAX_MOLES_AT_A_TIME; i++) {
        reset_mole(&moles[i]);
    }
}

void reset_mole(Mole *mole) {
    mole->id = -1;
    mole->delay = 0;
    mole->start_time = 0;
    mole->on_display = false;
    mole->is_active = false;
    LPC_GPIO0->FIOCLR = 1 << (mole->id + LED_LINE);
}

void mole_show() {
    for (int i = 0; i < MAX_MOLES_AT_A_TIME; i++) {
        if (!moles[i].is_active) {
            int chance = randint(0, 100);
            if (chance > 30) {
                continue;
            }
            int id = randint(0, 7);
            if (mole_present(id)) {
                continue;
            }
            moles[i].id = id;
            moles[i].delay = randint(MIN_MOLE_DELAY, MAX_MOLE_DELAY);
            moles[i].start_time = millis();
            moles[i].on_display = false;
            moles[i].is_active = true;
        }
    }
}

void mole_hide() {
    for (int i = 0; i < MAX_MOLES_AT_A_TIME; i++) {
        if (moles[i].is_active && millis() - moles[i].start_time > moles[i].delay + MOLE_SHOW_TIME) {
            reset_mole(&moles[i]);
        }
    }
}

void mole_display() {
    int led = 0;
    for (int i = 0; i < MAX_MOLES_AT_A_TIME; i++) {
        if (moles[i].is_active && millis() - moles[i].start_time > moles[i].delay) {
            led |= 1 << moles[i].id;
            moles[i].on_display = true;
        }
    }
    LPC_GPIO0->FIOSET = led << 4;
    delay(5);
}

void mole_update() {
    mole_show();
    mole_display();
    mole_hide();
}

bool mole_active(int id) {
    for (int i = 0; i < MAX_MOLES_AT_A_TIME; i++) {
        if (moles[i].is_active && moles[i].id == id && moles[i].on_display) {
            return true;
        }
    }
    return false;
}

void hit_mole(int id) {
    for (int i = 0; i < MAX_MOLES_AT_A_TIME; i++) {
        if (moles[i].is_active && moles[i].id == id && moles[i].on_display) {
            reset_mole(&moles[i]);
        }
    }
}
