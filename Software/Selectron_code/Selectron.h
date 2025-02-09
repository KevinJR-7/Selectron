#ifndef SELECTRON_H
#define SELECTRON_H

#include <stdbool.h>

#define GPIO_BUTTON_PRODUCT_1 6
#define GPIO_BUTTON_PRODUCT_2 7
#define GPIO_BUTTON_PRODUCT_3 8
#define GPIO_BUTTON_PRODUCT_4 9
#define GPIO_COIN_REPLACE_TEST 22
#define MOTOR_PRODUCT_FRECUENCY 10000
#define DUTY_MOTOR_PRODUCT 0.5


typedef struct {
    volatile bool Change_text_coin;
    volatile bool OutProduct;
    volatile bool inCoin; 
    volatile bool alarmCoin; 
    volatile bool Product_1; 
    volatile bool Product_2; 
    volatile bool Product_3; 
    volatile bool Product_4; 
} flagsIRQ;

extern flagsIRQ IRQflags;
void Button_init();
void gpio_callback1(uint gpio, uint32_t events);
void motor_init();
void change_or_not_function(int precio , int gpio );
long long int alarm_change_text(long int time, void *user_data);
void disable_all_irqs();
void enable_all_irqs();
void coin_machine_init();
void Insert_coin_text();
void all_IRQ_false();
#endif // SELECTRON_H