#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "LCD.h"
#include <stdio.h>



static uint8_t backlight_state = LCD_BACKLIGHT;
void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &val, 1, false);
}

void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    sleep_us(600);
    i2c_write_byte(val | 0x04); // Enable bit set
    sleep_us(600);
    i2c_write_byte(val & ~0x04); // Enable bit cleared
    sleep_us(600);
}

// Sed byte to LCD as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_send_command(uint8_t command) {
    lcd_send_byte(command, 0x00);
}

void lcd_send_data(uint8_t data_value) {
    lcd_send_byte(data_value, 0x01);
}

void lcd_backlight(bool state) {
    backlight_state = state ? LCD_BACKLIGHT : 0x00; // Activa o desactiva el bit de retroiluminación
    i2c_write_byte(0x00 | backlight_state); // Enviar comando de retroiluminación
}

void lcd_init() {
    // Initiaalize the LCD in 4-bit mode
    sleep_ms(50); // Wait for LCD to power up
    lcd_send_command(0x03);
    lcd_send_command(0x03);
    lcd_send_command(0x03);
    lcd_send_command(0x02);

    // Set LCD parameters
    lcd_send_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE);
    lcd_send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
    lcd_send_command(LCD_ENTRYMODESET | LCD_ENTRYLEFT);
    lcd_send_command(LCD_CLEARDISPLAY);
    sleep_ms(5);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x80 + col : 0xC0 + col;
    lcd_send_command(address);
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

void lcd_clear() {
    lcd_send_command(LCD_CLEARDISPLAY);
    sleep_ms(2);
}

void init_gpios(){
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}


