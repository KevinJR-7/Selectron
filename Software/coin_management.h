/**
 * @file main.h
 * @brief This is a brief description of the main H file.
 *
 * Detailed description of the main C file.
 */
 
// Avoid duplication in code
#ifndef _COIN_H_
#define _COIN_H_

// Write your definitions and other macros here

#define SERVO1_PIN  15
#define SERVO2_PIN  16

#define C_IN_PIN    19
#define C_500_PIN   18
#define C_1000_PIN  20
#define C_OUT_PIN   21

uint8_t coin_ident(uint8_t, uint8_t, uint8_t);
void coin_way(uint8_t, uint8_t, float);
float angle_to_duty(float, float);
float servo_pwm_init(uint8_t);
int8_t coin_change(uint8_t);
void coin_change_deliver(uint8_t, float);
int64_t come_back_irq(alarm_id_t, void *);


#endif