//
// Created by Triyan Mukherjee on 10-10-2024.
//

#ifndef ESD_KEYMATRIX_H
#define ESD_KEYMATRIX_H

#include "LPC17xx.h"
#include "utils/utility.h"

#include <stdint.h>

#define ROWS 4
#define COLS 4
#define DefaultKey 'x'

#define ROW_LINES 10  // P2.10 - P2.13
#define COL_LINES 23  // P1.23 - P1.26

uint8_t keypad_get_key(void);

void keypad_init(void);


#endif //ESD_KEYMATRIX_H
