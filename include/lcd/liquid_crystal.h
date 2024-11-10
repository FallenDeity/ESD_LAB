//
// Created by Triyan Mukherjee on 08-10-2024.
//

#ifndef ESD_LIQUID_CRYSTAL_H
#define ESD_LIQUID_CRYSTAL_H

#include "LPC17xx.h"
#include "utils/utility.h"

#include <stdbool.h>

#define RS_CTRL 27  // Register select, P0.27
#define EN_CTRL 28  // Enable, P0.28
#define DT_CTRL 23  // Data lines, P0.23 - P0.26

#define LCD_MODE LCD_4_BIT_MODE
#define LCD_2_ROWS 0x08
#define LCD_5x8_FONT 0x00
#define LCD_5x10_FONT 0x04

#define LCD_ROWS 2
#define LCD_COLUMNS 16

typedef enum {
    LEFT_SHIFT = 0,
    RIGHT_SHIFT = 1,
} ShiftDirection;

typedef enum {
    LCD_CLEAR_DISPLAY = 0x01,  // Clear display
    LCD_RETURN_HOME = 0x02,  // Return home, basically move cursor to 0,0
    LCD_DEC_CURSOR = 0x04,  // Make cursor move left after writing
    LCD_INC_CURSOR = 0x06,  // Make cursor move right after writing
    LCD_SHIFT_ROW_LEFT = 0x07,  // Shift row left
    LCD_SHIFT_ROW_RIGHT = 0x05,  // Shift row right
    LCD_DISPLAY_ON = 0x0C,  // Turn on display
    LCD_DISPLAY_OFF = 0x08,  // Turn off display
    LCD_CURSOR_ON = 0x0A,  // Turn on cursor
    LCD_CURSOR_OFF = 0x0C,  // Turn off cursor
    LCD_CURSOR_BLINK_ON = 0x0E,  // Turn on cursor blink
    LCD_CURSOR_BLINK_OFF = 0x0C,  // Turn off cursor blink
    LCD_SHIFT_CURSOR_LEFT = 0x10,  // Shift cursor left, i.e. backspace
    LCD_SHIFT_CURSOR_RIGHT = 0x14,  // Shift cursor right, i.e. space
    LCD_SHIFT_DISPLAY_LEFT = 0x18,  // Shift display left, i.e. scroll left
    LCD_SHIFT_DISPLAY_RIGHT = 0x1C,  // Shift display right, i.e. scroll right
    LCD_FORCE_CURSOR_1ST_LINE = 0x80,  // Force cursor to 1st line, 0th position
    LCD_FORCE_CURSOR_2ND_LINE = 0xC0,  // Force cursor to 2nd line, 0th position
    LCD_8_BIT_MODE = 0x30,  // 8 bit mode
    LCD_4_BIT_MODE = 0x20,  // 4 bit mode
} LCDCommand;

void lcd_init(void);

void lcd_write_command(LCDCommand command);

void lcd_write_data(uint8_t data);

void pulse_enable(void);

void send_8_bits_in_4_bit_mode(uint8_t data, bool command);

void send_4_bits_in_4_bit_mode(uint8_t data, bool command);

void send_8_bits_in_8_bit_mode(uint8_t data, bool command);

void check_busy_flag_4_bit_mode(void);

void check_busy_flag_8_bit_mode(void);

void lcd_left_to_right(void);

void lcd_right_to_left(void);

void lcd_return_home(void);

void lcd_clear_display(void);

void lcd_display_on(void);

void lcd_display_off(void);

void lcd_cursor_on(void);

void lcd_cursor_off(void);

void lcd_cursor_blink_on(void);

void lcd_cursor_blink_off(void);

void lcd_write_char(char c);

void lcd_write_string(char *str);

int lcd_set_cursor(uint8_t x, uint8_t y);

int lcd_shift_display(ShiftDirection direction);

int lcd_shift_cursor(ShiftDirection direction);

#endif //ESD_LIQUID_CRYSTAL_H
