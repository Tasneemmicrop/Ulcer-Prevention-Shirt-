#include "stm32f4xx.h"
#include "oled_driver.h"

// Delay
static void delay_us(unsigned int us) {
    for (volatile int i = 0; i < us * 20; i++);
}

static void I2C_Delay() { delay_us(2); }

// Pin Initialization
#define OLED_SCL_H GPIOC->ODR |= (1 << 0)
#define OLED_SCL_L GPIOC->ODR &= ~(1 << 0)
#define OLED_SDA_H GPIOC->ODR |= (1 << 1)
#define OLED_SDA_L GPIOC->ODR &= ~(1 << 1)

static void I2C_Start() {
    OLED_SDA_H; OLED_SCL_H; I2C_Delay();
    OLED_SDA_L; I2C_Delay(); OLED_SCL_L;
}

static void I2C_Stop() {
    OLED_SCL_L; OLED_SDA_L; I2C_Delay();
    OLED_SCL_H; OLED_SDA_H;
}

static void I2C_WriteByte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        (byte & 0x80) ? OLED_SDA_H : OLED_SDA_L;
        I2C_Delay();
        OLED_SCL_H; I2C_Delay();
        OLED_SCL_L; I2C_Delay();
        byte <<= 1;
    }
    OLED_SDA_H; OLED_SCL_H; I2C_Delay(); OLED_SCL_L;
}

static void OLED_WriteCommand(uint8_t cmd) {
    I2C_Start(); I2C_WriteByte(0x78); I2C_WriteByte(0x00); I2C_WriteByte(cmd); I2C_Stop();
}

static void OLED_WriteData(uint8_t data) {
    I2C_Start(); I2C_WriteByte(0x78); I2C_WriteByte(0x40); I2C_WriteByte(data); I2C_Stop();
}

// Font data
static const uint8_t font[][5] = {
    ['A'-'A'] = {0x7C, 0x12, 0x12, 0x7C, 0x00},
    ['D'-'A'] = {0x7E, 0x52, 0x52, 0x2C, 0x00},
    ['E'-'A'] = {0x7E, 0x4A, 0x4A, 0x42, 0x00},
    ['F'-'A'] = {0x7E, 0x48, 0x48, 0x40, 0x00},
    ['G'-'A'] = {0x3C, 0x42, 0x52, 0x34, 0x00},
    ['H'-'A'] = {0x7E, 0x08, 0x08, 0x7E, 0x00},
    ['I'-'A'] = {0x42, 0x7E, 0x42, 0x00, 0x00},
    ['M'-'A'] = {0x7E, 0x04, 0x08, 0x04, 0x7E},
    ['O'-'A'] = {0x3C, 0x42, 0x42, 0x3C, 0x00},
    ['U'-'A'] = {0x3E, 0x40, 0x40, 0x3E, 0x00}
};

void OLED_Init() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= (1 << (0 * 2)) | (1 << (1 * 2));
    OLED_SCL_H; OLED_SDA_H;

    OLED_WriteCommand(0xAE); OLED_WriteCommand(0x20); OLED_WriteCommand(0x10);
    OLED_WriteCommand(0xB0); OLED_WriteCommand(0xC8); OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x10); OLED_WriteCommand(0x40); OLED_WriteCommand(0x81);
    OLED_WriteCommand(0xFF); OLED_WriteCommand(0xA1); OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0xA8); OLED_WriteCommand(0x3F); OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xD3); OLED_WriteCommand(0x00); OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0xF0); OLED_WriteCommand(0xD9); OLED_WriteCommand(0x22);
    OLED_WriteCommand(0xDA); OLED_WriteCommand(0x12); OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x20); OLED_WriteCommand(0x8D); OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);
}

void OLED_Clear() {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WriteCommand(0xB0 + i);
        OLED_WriteCommand(0x00);
        OLED_WriteCommand(0x10);
        for (uint8_t j = 0; j < 128; j++) {
            OLED_WriteData(0x00);
        }
    }
}

void OLED_SetCursor(uint8_t page, uint8_t col) {
    OLED_WriteCommand(0xB0 + page);
    OLED_WriteCommand(0x00 + (col & 0x0F));
    OLED_WriteCommand(0x10 + ((col >> 4) & 0x0F));
}

void OLED_WriteChar(char c) {
    if (c >= 'A' && c <= 'Z') {
        const uint8_t *data = font[c - 'A'];
        for (int i = 0; i < 5; i++) OLED_WriteData(data[i]);
        OLED_WriteData(0x00);
    }
}

void OLED_WriteString(const char *str) {
    while (*str) OLED_WriteChar(*str++);
}
