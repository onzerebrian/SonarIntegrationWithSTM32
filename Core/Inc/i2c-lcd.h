/*
 * i2c-lcd.h
 *
 *  Created on: Jan 17, 2025
 *      Author: onzer
 */

#ifndef INC_I2C_LCD_H_
#define INC_I2C_LCD_H_


#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

#define SLAVE_ADDRESS_LCD 0x7E // Adjust based on your LCD module

void lcd_init(void);
void lcd_clear(void);
void lcd_put_cur(uint8_t row, uint8_t col);
void lcd_send_string(char *str);


#endif /* INC_I2C_LCD_H_ */
