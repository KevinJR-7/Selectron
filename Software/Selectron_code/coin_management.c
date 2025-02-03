/**
 * @file coin_management.c
 * @brief Este documento contiene las funciones necesarias para la gestión de las monedas en la máquina expendedora.
 */

// Standard libraries
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

// Include your own header files here
#include "coin_management.h"

volatile uint8_t total_coin = 0;
uint8_t coin_1000_num = 2;
uint8_t coin_500_num = 2;
float top_g;
uint8_t servo_g;

//funcion para detectar el valor o validez de la moneda entrante
//esta funcion debe ser llamada al detectar un flanco de bajada en el IR de la entrada
uint8_t coin_ident(uint8_t c_500_gpio, uint8_t c_1000_gpio, uint8_t c_out_gpio)
{
    uint32_t cnt = 0;
    uint32_t c_500 = gpio_get(c_500_gpio);
    uint32_t c_1000 = gpio_get(c_1000_gpio);
    uint32_t c_out = gpio_get(c_out_gpio);
    while(c_500 && c_out)
    { 
        cnt += cnt; 
        c_500 = gpio_get(c_500_gpio);
        c_out = gpio_get(c_out_gpio);
        //if((cnt%125000000000) == 0){ printf("500: %d \n", c_500); printf("out: %d \n", c_out); }
    }
    if(!c_out){ return 0; } //retorno de 0 implica moneda invalida
    else
    {
        cnt = 0;
        c_1000 = gpio_get(c_1000_gpio);
        c_out = gpio_get(c_out_gpio);
        while(c_1000 && c_out)
        { 
            cnt += cnt; 
            c_1000 = gpio_get(c_1000_gpio);
            c_out = gpio_get(c_out_gpio);
        }

        if(!c_1000)
        { 
            total_coin += 5; 
            coin_500_num +=1;
            return 5; 
        } //retorno de 5 implica moneda de 500
        else
        { 
            total_coin += 10; 
            coin_1000_num +=1;
            return 10; 
        }  //retorno de 10 implica moneda de 1000
    }
}

//funcion para calcular la devuelta
//price es 5-10-15-etc
int8_t coin_change(uint8_t price)
{
    int8_t change = -1;
    if(price <= total_coin)    //revisa que no falte plata del usuario
    {
        change = total_coin - price;
        if(change > ((coin_1000_num*10) + (coin_500_num*5)))     //revisa que no falte plata en la máquina
        {
            return -2;  //cambio insuficiente
        }
        else if(((change%10) > 0) && (coin_500_num < 1))
        {
            return -3;  //insuficientes monedas de 500 (cambio no exacto)
        }
    }
    return change;
}

//funcion para entregar la devuelta
//calcula la mejor forma de entregar la devuelta teniendo en cuenta las monedas disponibles
void coin_change_deliver(uint8_t change, float top)
{
    total_coin = 0;
    uint8_t change_10 = change/10;
    uint8_t change_5 = (change%10)/5;
    if(change_10 > coin_1000_num)   //revisa si hay suficientes de 1000
    {
        change_5 += (change_10-coin_1000_num)*2;
        change_10 = coin_1000_num;
    }
    printf("change_5: %d \n", change_5);
    printf("change_10: %d \n", change_10);
    while(change_10 > 0)
    {
        sleep_ms(2000);
        coin_way(SERVO2_PIN, 10, top);
        change_10 -= 1;
        coin_1000_num -= 1;
        printf("change_10: %d \n", change_10);
    }
    while(change_5 > 0)
    {
        sleep_ms(2000);
        coin_way(SERVO2_PIN, 5, top);
        change_5 -= 1;
        coin_500_num -= 1;
        printf("change_5: %d \n", change_5);
    }
}

//funcion para cambiar el camino por el que se dijiren las monedas
//inicialmente esta funcion tambien puede ser llamada para el motor de la devuelta
void coin_way(uint8_t servo_gpio, uint8_t coin_case, float top)
{
    top_g = top;
    float fix;
    int ang;
    uint32_t back_ms;
    if(servo_gpio == SERVO1_PIN)
    {
        fix = 35;
        ang = 50;
        back_ms = 400;
        servo_g = servo_gpio;
    }
    else if(servo_gpio == SERVO2_PIN)
    {
        fix = 37;
        ang = -55;
        back_ms = 1000;
        servo_g = servo_gpio;
    }               else{fix = 0; ang = 0;}
    float duty_cycle;
    switch(coin_case) 
    { 
        case 0: 
        duty_cycle = angle_to_duty(90, fix);
        break;

        case 5: 
        duty_cycle = angle_to_duty(90-ang, fix);//145
        add_alarm_in_ms(back_ms, come_back_irq, NULL, true);
        break;

        case 10: 
        printf("Si entra");
        duty_cycle = angle_to_duty(90+ang, fix);//35
        add_alarm_in_ms(back_ms, come_back_irq, NULL, true);
        break;

        default: 
        duty_cycle = angle_to_duty(90, fix);
        break;
    }
    pwm_set_gpio_level(servo_gpio, top * duty_cycle);
}

// Calcula el ciclo de trabajo para el ángulo deseado 
float angle_to_duty(float angle, float fix)
{ 
    float pulse_width_us = 1000.f + ((angle-fix) * 1000.f / 93.f); // 1000 us para 0 grados, 2000 us para 180 grados 
    float duty_cycle = (pulse_width_us / 20000.f) ; // Convertir a duty cycle de PWM 
    return duty_cycle;
}

//Inicializa el PWM para el gpio deseado
float servo_pwm_init(uint8_t servo_gpio)
{
    // Configura el pin GPIO para PWM 
    gpio_set_function(servo_gpio, GPIO_FUNC_PWM); 
    uint slice_num = pwm_gpio_to_slice_num(servo_gpio); 
    /* // Configura el valor de wrap para un período de 20ms 
    uint32_t freq = 50; // frecuencia
    uint32_t clock_freq = clock_get_hz(clk_sys); // Frecuencia del reloj del sistema
    uint16_t top = clock_freq / freq - 1;
    pwm_set_wrap(slice_num, top);
    // Habilita el PWM 
    pwm_set_enabled(slice_num, true); */

    

    float clockDiv = 64;
    float wrap = 39062; 
    pwm_config config = pwm_get_default_config();
    
    uint64_t clockspeed = clock_get_hz(5);

    while (clockspeed/clockDiv/50 > 65535 && clockDiv < 256) clockDiv += 64; 
    wrap = clockspeed/clockDiv/50;

    pwm_config_set_clkdiv(&config, clockDiv);
    pwm_config_set_wrap(&config, wrap);

    pwm_init(slice_num, &config, true);

    return wrap;
}

int64_t come_back_irq(alarm_id_t id, void *top1) 
{ 
    //flag_alarm = 1;
    /* float *top2 = (float *)top1;
    float top = *top2; */
    coin_way(servo_g, 0, top_g);
    return 0; // No se repite la alarma
}
