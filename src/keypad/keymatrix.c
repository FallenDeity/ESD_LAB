//
// Created by Triyan Mukherjee on 10-10-2024.
//

#include "keypad/keymatrix.h"

const uint8_t keymap[ROWS][COLS] = {
        {'4', '5', '6', '7'},
        {'8', '9', 'A', 'B'},
        {'C', 'D', 'E', 'F'},
        {'0', '1', '2', '3'},
};

void keypad_init(void) {
    // Set the row & column lines as GPIO
    LPC_PINCON->PINSEL3 &= ~(0xFF << ((COL_LINES - 16) * 2));// Column lines
    LPC_PINCON->PINSEL4 &= ~(0xFF << (ROW_LINES * 2));       // Row lines

    // Set the row lines as output
    LPC_GPIO2->FIODIR |= (0x0F << ROW_LINES);
    LPC_GPIO2->FIOCLR = (0x0F << ROW_LINES);

    // Set the column lines as input
    LPC_GPIO1->FIODIR &= ~(0x0F << COL_LINES);
    LPC_GPIO1->FIOPIN = (0x0F << COL_LINES);
}

uint8_t keypad_get_key(void) {
    uint8_t row, col;
    uint8_t key = DefaultKey;

    for (row = 0; row < ROWS; row++) {
        LPC_GPIO2->FIOPIN = (1 << (row + ROW_LINES));
        int check = LPC_GPIO1->FIOPIN & (0x0F << COL_LINES);
        if (check) {
            for (col = 0; col < COLS; col++) {
                if (check & (1 << (col + COL_LINES))) {
                    key = keymap[row][col];
                    break;
                }
            }
        }
    }
    return key;
}
