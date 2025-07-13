#include "stm32f4xx.h"
#include "adc_driver.h"

void ADC_Init() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= (3 << (2 * 2));  // PC2 analog

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC1->SQR3 = 12;                 // Channel 12
    ADC1->SMPR1 |= (7 << (2 * 3));   // Max sample time
    ADC1->CR2 |= ADC_CR2_ADON;
}

uint16_t Read_Pressure() {
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC));
    return ADC1->DR;
}
