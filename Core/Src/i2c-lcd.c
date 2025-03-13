/*
 * i2c-lcd.c
 *
 *  Created on: Jan 17, 2025
 *      Author: onzer
 */


#include "i2c-lcd.h"

static void lcd_send_cmd(uint8_t cmd);
static void lcd_send_data(uint8_t data);

void lcd_init(void)
{
    HAL_Delay(50); // Wait >40ms after power up

    lcd_send_cmd(0x30);
    HAL_Delay(5);
    lcd_send_cmd(0x30);
    HAL_Delay(1);
    lcd_send_cmd(0x30);
    HAL_Delay(10);
    lcd_send_cmd(0x20); // 4-bit mode
    HAL_Delay(10);

    // Configure display
    lcd_send_cmd(0x28); // Function set: 2 lines, 5x8 dots
    lcd_send_cmd(0x08); // Display OFF
    lcd_send_cmd(0x01); // Clear display
    HAL_Delay(2);
    lcd_send_cmd(0x06); // Entry mode set
    lcd_send_cmd(0x0C); // Display ON, cursor OFF
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01); // Clear display
    HAL_Delay(2);
}

void lcd_put_cur(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_send_cmd(addr);
}

void lcd_send_string(char *str)
{
    while (*str)
    {
        lcd_send_data((uint8_t)*str++);
    }
}

static void lcd_send_cmd(uint8_t cmd)
{
    uint8_t data_u = (cmd & 0xF0);
    uint8_t data_l = ((cmd << 4) & 0xF0);
    uint8_t data[4] = {
        data_u | 0x0C, data_u | 0x08,
        data_l | 0x0C, data_l | 0x08
    };
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, data, 4, HAL_MAX_DELAY);
}

static void lcd_send_data(uint8_t data)
{
    uint8_t data_u = (data & 0xF0);
    uint8_t data_l = ((data << 4) & 0xF0);
    uint8_t data_arr[4] = {
        data_u | 0x0D, data_u | 0x09,
        data_l | 0x0D, data_l | 0x09
    };
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, data_arr, 4, HAL_MAX_DELAY);
}
