#include "LPC17xx.h"
#include "buzzer/buzzer.h"
#include "keypad/keymatrix.h"
#include "lcd/liquid_crystal.h"
#include "leds/led.h"
#include "utils/utility.h"

#include <stdlib.h>

#define LEVEL_DURATION (60 * 1000)// 60 seconds

/*
 * Occupied pins:
 * P0.23 - P0.26: LCD Data lines
 * P0.27: LCD RS
 * P0.28: LCD EN
 *
 * P1.23 - P1.26: Keypad Column
 * P2.10 - P2.13: Keypad Row
 *
 * P0.4 - P0.11: LED Linear Array
 *
 * P0.15: Buzzer
 *
 * P1.28 - P1.29: ADC AD0.4 Random Seed
 *
 */

char num_str[8];
char key = DefaultKey;
int score = 0;
uint32_t start_time = 0;
bool game_over = false;

void display_score();

void display_time();

int main() {
    SystemInit();
    random_init();

    SystemCoreClockUpdate();

    DelayTimer_Init();
    lcd_init();
    keypad_init();

    mole_init();
    buzzer_init();

    lcd_write_command(LCD_FORCE_CURSOR_1ST_LINE);
    lcd_write_string("Time: ");

    lcd_write_command(LCD_FORCE_CURSOR_2ND_LINE);
    lcd_write_string("Score: ");

    start_time = millis();

    while (1) {
        if (millis() - start_time > LEVEL_DURATION || game_over) {
            game_over = true;
            lcd_clear_display();
            lcd_write_string("Game Over!");
            lcd_set_cursor(0, 1);
            lcd_write_string("Score: ");
            itoa(score, num_str, 10);
            lcd_write_string(num_str);
            buzzer_on();
            delay(1000);
            buzzer_off();
            break;
        }

        key = keypad_get_key();
        if (key != DefaultKey && key >= '0' && key <= '7') {
            if (mole_active(key - '0')) {
                score++;
                hit_mole(key - '0');
            }
        }
        mole_update();
        display_time();
        display_score();
    }
    return 0;
}

void display_score() {
    lcd_set_cursor(8, 1);
    itoa(score, num_str, 10);
    lcd_write_string(num_str);
}

void display_time() {
    char *time_str = format_time(LEVEL_DURATION - (millis() - start_time));
    lcd_set_cursor(7, 0);
    lcd_write_string(time_str);
}
