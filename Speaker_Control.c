#include "stm32f4xx.h"
#include "speaker_control.h"

volatile int speaker_on = 1;

void Speaker_Init(void) {
    // Speaker on PC3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER &= ~(3 << (3 * 2));
    GPIOC->MODER |=  (1 << (3 * 2));

    // Button interrupt on PA2
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    GPIOA->MODER &= ~(3 << (2 * 2)); // Input
    GPIOA->PUPDR &= ~(3 << (2 * 2));
    GPIOA->PUPDR |=  (1 << (2 * 2)); // Pull-up

    SYSCFG->EXTICR[0] &= ~(0xF << 8);
    SYSCFG->EXTICR[0] |=  (0x0 << 8);  // PA2

    EXTI->IMR  |= (1 << 2);
    EXTI->RTSR |= (1 << 2);

    NVIC_EnableIRQ(EXTI2_IRQn);
}

void Toggle_Speaker(void) {
    speaker_on = !speaker_on;
}

void EXTI2_IRQHandler(void) {
    if (EXTI->PR & (1 << 2)) {
        for (volatile int i = 0; i < 100000; i++); // 20ms delay
        Toggle_Speaker();
        EXTI->PR |= (1 << 2);
    }
}
