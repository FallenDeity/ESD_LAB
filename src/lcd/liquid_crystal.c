//
// Created by Triyan Mukherjee on 08-10-2024.
//

#include "lcd/liquid_crystal.h"


// +---------------------------+
// |         Power on          |
// | Wait for more than 15 ms  |   // 15 ms wait
// | after VCC rises to 4.5 V  |
// +---------------------------+
//              |
// +---------------------------+
// |  RS R/W DB7 DB6 DB5 DB4   |
// |   0   0   0   0   1   1   |   // Initial sequence 0x30
// | Wait for more than 4.1 ms |   // 4.1 ms us writing DATA into DDRAM or CGRAM
// +---------------------------+
//              |
// +---------------------------+
// |  RS R/W DB7 DB6 DB5 DB4   |
// |   0   0   0   0   1   1   |   // Initial sequence 0x30
// | Wait for more than 0.1 ms |   // 100 us writing DATA into DDRAM or CGRAM
// +---------------------------+
//              |
// +---------------------------+
// |  RS R/W DB7 DB6 DB5 DB4   |   // Initial sequence 0x30
// |   0   0   0   0   1   1   |   // 37 us writing DATA into DDRAM or CGRAM 4 us tadd - time after busy flag disappeared
// | Wait for more than 45 us  |   // 37 us + 4 us = 41 us * (270/250) = 45us
// +---------------------------+
//              |
// +---------------------------+   // 4bit mode 0x20 !!! MUST BE SET TIME, BF CHECK DOESN'T WORK CORRECTLY !!!
// |  RS R/W DB7 DB6 DB5 DB4   |   //
// |   0   0   0   0   1   0   |   // 37 us writing DATA into DDRAM or CGRAM 4 us tadd - time after busy flag disappeared
// | Wait for more than 45 us  |   // !!! MUST BE SET DELAY TIME, BUSY FLAG CHECK DOESN'T WORK CORRECTLY !!!
// +---------------------------+
//              |
// +---------------------------+
// |  RS R/W DB7 DB6 DB5 DB4   |   // Display off 0x08
// |   0   0   0   0   1   0   |   //
// |   0   0   1   0   0   0   |   //
// |    Wait for BF Cleared    |   // Wait for BF Cleared
// +---------------------------+
//              |
// +---------------------------+
// |  RS R/W DB7 DB6 DB5 DB4   |   // Display clear 0x01
// |   0   0   0   0   0   0   |   //
// |   0   0   0   0   0   1   |   //
// |    Wait for BF Cleared    |   // Wait for BF Cleared
// +---------------------------+
//              |
// +---------------------------+
// |  RS R/W DB7 DB6 DB5 DB4   |   // Entry mode set 0x06
// |   0   0   0   0   0   0   |   //
// |   0   0   0   1   1   0   |   // shift cursor to the left, without text shifting
// |    Wait for BF Cleared    |   // Wait for BF Cleared
// +---------------------------+

void lcd_return_home(void) {
    lcd_write_command(LCD_RETURN_HOME);
}

void lcd_clear_display(void) {
    lcd_write_command(LCD_CLEAR_DISPLAY);
}

void lcd_display_on(void) {
    lcd_write_command(LCD_DISPLAY_ON);
}

void lcd_display_off(void) {
    lcd_write_command(LCD_DISPLAY_OFF);
}

void lcd_cursor_on(void) {
    lcd_write_command(LCD_CURSOR_ON);
}

void lcd_cursor_off(void) {
    lcd_write_command(LCD_CURSOR_OFF);
}

void lcd_cursor_blink_on(void) {
    lcd_write_command(LCD_CURSOR_BLINK_ON);
}

void lcd_cursor_blink_off(void) {
    lcd_write_command(LCD_CURSOR_BLINK_OFF);
}

void lcd_write_char(char c) {
    lcd_write_data(c);
}

void lcd_write_string(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        lcd_write_data(str[i++]);
    }
}

int lcd_set_cursor(uint8_t x, uint8_t y) {
    if (x >= LCD_COLUMNS || y >= LCD_ROWS) {
        return -1;
    }
    if (y == 0) {
        lcd_write_command(LCD_FORCE_CURSOR_1ST_LINE + x);
    } else {
        lcd_write_command(LCD_FORCE_CURSOR_2ND_LINE + x);
    }
    return 0;
}

int lcd_shift_display(ShiftDirection direction) {
    if (direction == LEFT_SHIFT) {
        lcd_write_command(LCD_SHIFT_DISPLAY_LEFT);
    } else {
        lcd_write_command(LCD_SHIFT_DISPLAY_RIGHT);
    }
    return 0;
}

int lcd_shift_cursor(ShiftDirection direction) {
    if (direction == LEFT_SHIFT) {
        lcd_write_command(LCD_SHIFT_CURSOR_LEFT);
    } else {
        lcd_write_command(LCD_SHIFT_CURSOR_RIGHT);
    }
    return 0;
}

void lcd_left_to_right(void) {
    lcd_write_command(LCD_INC_CURSOR);
}

void lcd_right_to_left(void) {
    lcd_write_command(LCD_DEC_CURSOR);
}

void lcd_init(void) {
    // set RS_CTRL, P0.27, as GPIO Output
    LPC_PINCON->PINSEL1 &= ~(3 << ((RS_CTRL - 16) * 2));
    LPC_GPIO0->FIODIR |= (1 << RS_CTRL);
    LPC_GPIO0->FIOCLR = (1 << RS_CTRL);

    // set EN_CTRL, P0.28, as GPIO Output
    LPC_PINCON->PINSEL1 &= ~(3 << ((EN_CTRL - 16) * 2));
    LPC_GPIO0->FIODIR |= (1 << EN_CTRL);
    LPC_GPIO0->FIOCLR = (1 << EN_CTRL);

    // set DT_CTRL, P0.23 - P0.26, as GPIO Output
    LPC_PINCON->PINSEL1 &= ~(0xFF << ((DT_CTRL - 16) * 2));
    LPC_GPIO0->FIODIR |= (0x0F << DT_CTRL);
    LPC_GPIO0->FIOCLR = (0x0F << DT_CTRL);

    delay(16);

    // set 8 bit mode
    lcd_write_command(LCD_8_BIT_MODE);
    delay(5);

    // set 8 bit mode
    lcd_write_command(LCD_8_BIT_MODE);
    delay_us(110);

    // set 8 bit mode
    lcd_write_command(LCD_8_BIT_MODE);
    delay_us(50);

    // 4 bit mode
    lcd_write_command(LCD_4_BIT_MODE);
    delay_us(50);

    // 4 bit mode, 2 line, 5x8 font
    lcd_write_command(LCD_4_BIT_MODE | LCD_2_ROWS | LCD_5x8_FONT);

    // display on
    lcd_write_command(LCD_DISPLAY_ON);

    // lcd increment cursor
    lcd_write_command(LCD_INC_CURSOR);

    // clear display
    lcd_write_command(LCD_CLEAR_DISPLAY);
}

void pulse_enable(void) {
    LPC_GPIO0->FIOSET = (1 << EN_CTRL);
    delay_us(10);
    LPC_GPIO0->FIOCLR = (1 << EN_CTRL);
    delay_us(10);
}

void lcd_write_command(LCDCommand command) {
    if (LCD_MODE == LCD_4_BIT_MODE) {
        if (command == LCD_8_BIT_MODE || command == LCD_4_BIT_MODE) {
            send_4_bits_in_4_bit_mode(command, true);
        } else {
            send_8_bits_in_4_bit_mode(command, true);
        }
        delay(2);
    } else {
        send_8_bits_in_8_bit_mode(command, true);
        delay(2);
    }
}

void lcd_write_data(uint8_t data) {
    if (LCD_MODE == LCD_4_BIT_MODE) {
        send_8_bits_in_4_bit_mode(data, false);
        delay_us(50);
    } else {
        send_8_bits_in_8_bit_mode(data, false);
        delay_us(50);
    }
}

void send_4_bits_in_4_bit_mode(uint8_t data, bool command) {
    // send higher nibble
    LPC_GPIO0->FIOPIN = (data & 0xF0) << (DT_CTRL - 4);
    if (command) {
        LPC_GPIO0->FIOCLR = (1 << RS_CTRL);
    } else {
        LPC_GPIO0->FIOSET = (1 << RS_CTRL);
    }
    pulse_enable();
}

void send_8_bits_in_4_bit_mode(uint8_t data, bool command) {
    // send higher nibble
    send_4_bits_in_4_bit_mode(data, command);
    // send lower nibble
    send_4_bits_in_4_bit_mode(data << 4, command);
}

void send_8_bits_in_8_bit_mode(uint8_t data, bool command) {
    // since we are in 8 bit mode, we can directly send the data
    LPC_GPIO0->FIOPIN = data << DT_CTRL;
    if (command) {
        LPC_GPIO0->FIOCLR = (1 << RS_CTRL);
    } else {
        LPC_GPIO0->FIOSET = (1 << RS_CTRL);
    }
    pulse_enable();
}
