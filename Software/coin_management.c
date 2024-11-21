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

//funcion para detectar el valor o validez de la moneda entrante
//esta funcion debe ser llamada al detectar un flanco de bajada en el IR de la entrada
uint8_t coin_ident(uint8_t c_500_gpio, uint8_t c_1000_gpio, uint8_t c_out_gpio)
{
    uint32_t cnt = 0;
    while(gpio_get(c_500_gpio) && gpio_get(c_out_gpio)){ cnt += cnt; }

    if(gpio_get(c_out_gpio)){ return 0; } //retorno de 0 implica moneda invalida
    else
    {
        cnt = 0;
        while(gpio_get(c_1000_gpio) && gpio_get(c_out_gpio)){ cnt += cnt; }

        if(gpio_get(c_out_gpio)){ return 5; } //retorno de 5 implica moneda de 500
        else{ return 10; }  //retorno de 10 implica moneda de 1000
    }
}

//funcion para cambiar el camino por el que se dijiren las monedas
void coin_way(uint8_t servo_gpio, uint8_t coin_case, uint16_t top)
{
    float duty_cycle;
    switch(coin_case) 
    { 
        case 0: 
        duty_cycle = angle_to_duty(90);
        break;

        case 5: 
        duty_cycle = angle_to_duty(180);
        break;

        case 10: 
        duty_cycle = angle_to_duty(0);
        break;

        default: 
        duty_cycle = angle_to_duty(90);
        break;
    }
    pwm_set_gpio_level(servo_gpio, top * duty_cycle);
}

// Calcula el ciclo de trabajo para el ángulo deseado 
float angle_to_duty(float angle)
{ 
    float pulse_width_us = 1000 + (angle * 1000 / 180); // 1000 us para 0 grados, 2000 us para 180 grados 
    float duty_cycle = pulse_width_us / 20000; // Convertir a duty cycle de PWM 
    return duty_cycle;
}

//Inicializa el PWM para el gpio deseado
uint16_t servo_pwm_init(uint8_t servo_gpio)
{
    // Configura el pin GPIO para PWM 
    gpio_set_function(servo_gpio, GPIO_FUNC_PWM); 
    uint slice_num = pwm_gpio_to_slice_num(servo_gpio); 
    // Configura el valor de wrap para un período de 20ms 
    uint32_t freq = 50; // frecuencia
    uint32_t clock_freq = clock_get_hz(clk_sys); // Frecuencia del reloj del sistema
    uint16_t top = clock_freq / freq - 1;
    pwm_set_wrap(slice_num, top);
    // Habilita el PWM 
    pwm_set_enabled(slice_num, true);
    return top;
}