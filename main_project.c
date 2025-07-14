#include "stm32f4xx.h"
#include "oled_driver.h"
#include "adc_driver.h"
#include "speaker_control.h"

void delay_ms(unsigned int ms) {
    for (unsigned int i = 0; i < ms; i++)
        for (volatile int j = 0; j < 20000; j++);
}

int main(void) {
    OLED_Init();
    OLED_Clear();
    ADC_Init();
    Speaker_Init();

    while (1) {
        uint16_t pressure = Read_Pressure();
        OLED_Clear();
        OLED_SetCursor(0, 0);

        if (pressure < 100) OLED_WriteString("OFF");
        else if (pressure < 1500) OLED_WriteString("MEDIUM");
        else OLED_WriteString("HIGH");

        if (speaker_on)
            GPIOC->ODR |= (1 << 3);
        else
            GPIOC->ODR &= ~(1 << 3);

        delay_ms(300);
    }
}
