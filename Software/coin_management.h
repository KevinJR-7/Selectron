/**
 * @file main.h
 * @brief This is a brief description of the main H file.
 *
 * Detailed description of the main C file.
 */
 
// Avoid duplication in code
#ifndef _MAIN_H_
#define _MAIN_H_

// Write your definitions and other macros here

uint8_t coin_ident(uint8_t, uint8_t, uint8_t);
void coin_way(uint8_t, uint8_t, uint16_t);
float angle_to_duty(float);
uint16_t servo_pwm_init(uint8_t);


#endif