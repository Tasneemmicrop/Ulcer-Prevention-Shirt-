
#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <stdint.h>

void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t page, uint8_t col);
void OLED_WriteChar(char c);
void OLED_WriteString(const char *str);

#endif
