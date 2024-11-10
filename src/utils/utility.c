//
// Created by Triyan Mukherjee on 08-10-2024.
//

#include "utils/utility.h"

volatile uint32_t millis_counter = 0;

void DelayTimer_Init(void) {
    LPC_SC->PCONP |= (1 << 1);    // Power up Timer 0 (PCONP bit 1)
    LPC_SC->PCLKSEL0 &= ~(3 << 2);// Clear the PCLKSEL0 bits for Timer 0
    LPC_SC->PCLKSEL0 |= (1 << 2); // Set PCLK for Timer 0 to CCLK (96 MHz)

    LPC_TIM0->TCR = 0x02;// Reset Timer 0
    LPC_TIM0->PR = 95;   // Set prescaler to 95 (96 MHz / (95+1) = 1 MHz timer clock, 1 µs per tick)
    LPC_TIM0->MR0 = 999; // Match Register 0 set for 1000 ticks (1000 µs = 1 ms)

    LPC_TIM0->MCR |= (1 << 0);// Interrupt on MR0
    LPC_TIM0->MCR |= (1 << 1);// Reset Timer on MR0 match

    NVIC_EnableIRQ(TIMER0_IRQn);// Enable Timer 0 interrupt in NVIC

    LPC_TIM0->TCR = 0x01;// Start Timer 0
}

void TIMER0_IRQHandler(void) {
    if (LPC_TIM0->IR & 0x01) {
        LPC_TIM0->IR |= 0x01;// Clear MR0 interrupt flag
        millis_counter++;
    }
}

uint32_t millis() {
    return millis_counter;
}

void delay(uint32_t delay_ms) {
    uint32_t start_time = millis();
    while (millis() - start_time < delay_ms)
        ;
}

void delay_us(uint32_t delay_us) {
    volatile uint32_t i;
    uint32_t _us_tick = SystemCoreClock / 1000000;
    while (delay_us--) {
        for (i = 0; i < _us_tick; i++)
            ;
    }
}

void random_init() {
    // using noise from the ADC to seed the random number generator
    LPC_SC->PCONP |= (1 << 12);// enable power to ADC block
    LPC_ADC->ADCR = (1 << 4) | // select AD0.4 pin
                    (1 << 21); // enable ADC

    LPC_PINCON->PINSEL3 |= (3 << ((30 - 16) * 2));// set bits 29:28 to select ADC function

    LPC_ADC->ADCR = (1 << 4) | // select AD0.4 pin
                    (1 << 21) |// enable ADC
                    (1 << 24); // start conversion

    while (!(LPC_ADC->ADGDR & (1 << 31)))
        ;// wait for conversion to complete

    srand(LPC_ADC->ADGDR >> 4);// seed the random number generator with the ADC value
}

int randint(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

char *format_time(uint32_t time) {
    static char time_str[12];
    uint32_t seconds = time / 1000;
    uint32_t minutes = seconds / 60;
    seconds %= 60;
    sprintf(time_str, "%02lu:%02lu", minutes, seconds);
    return time_str;
}
