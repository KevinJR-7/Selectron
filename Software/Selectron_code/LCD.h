/**
 * @file LCD.h
 * @brief Prototipos de funciones y macros para control de LCD
 * @author Angel y Benjamon
 * @date Octubre 2024
 */

#ifndef _LCD_H
#define _LCD_H

#include <stdbool.h>  // Incluye bool para el estado backlight
#include <stdint.h>   // Incluyeuint8_t

#define I2C_PORT i2c0
#define LCD_ADDR 0x27
#define LCD_ROWS 2
#define LCD_COLS 16

// Define I2C pins for Raspberry Pi Pico
#define SDA_PIN 0
#define SCL_PIN 1

// Commands for LCD
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// Flags for display entry mode
#define LCD_ENTRYLEFT 0x02

// Flags for display and cursor control
#define LCD_DISPLAYON 0x04

// Flags for function set
#define LCD_2LINE 0x08
#define LCD_4BITMODE 0x00

// Backlight control
#define LCD_BACKLIGHT 0x08

// Prototipos de funciones
void i2c_write_byte(uint8_t val);
void lcd_toggle_enable(uint8_t val);
void lcd_send_byte(uint8_t val, int mode);
void lcd_send_command(uint8_t command);
void lcd_send_data(uint8_t data_value);
void lcd_backlight(bool state);
void lcd_init();
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print(const char *str);
void lcd_clear();
void init_gpios();

#endif /* _LCD_H */
