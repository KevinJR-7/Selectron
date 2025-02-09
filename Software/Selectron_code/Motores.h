/**
 * @file Motores.h
 * @brief Prototipos de funciones y macros para control de motores de los productos y configuración de interrupciones.
 * @author Angel
 * @date Octubre 2024
 */

#ifndef _MOTORES_H
#define _MOTORES_H


// Definición de pines para motores
#define MOTOR1_PIN 2  // GPIO para el Motor 1
#define MOTOR2_PIN 3  // GPIO para el Motor 2
#define MOTOR3_PIN 4  // GPIO para el Motor 3
#define MOTOR4_PIN 5  // GPIO para el motor 4

// Definición de pin para la interrupción
#define INTERRUPT_PIN 10


/**
 * @brief Configura el PWM en un pin específico con frecuencia y ciclo de trabajo.
 *
 * @param gpio Pin GPIO para el PWM.
 * @param frequency Frecuencia deseada en Hz.
 * @param duty_cycle Ciclo de trabajo (valor entre 0 y 1).
 */
void setup_pwm(uint gpio, float frequency, float duty_cycle);

/**
 * @brief Cambia dinámicamente el ciclo de trabajo de un pin PWM.
 *
 * @param gpio Pin GPIO configurado con PWM.
 * @param duty_cycle Ciclo de trabajo (valor entre 0 y 1).
 */
void set_duty_cycle(uint gpio, float duty_cycle);

/**
 * @brief Inicializa la interrupción en el pin predefinido para flanco de bajada.
 */
void IRQ_init_outproduct();

/**
 * @brief Callback para manejar la interrupción de GPIO.
 *
 * @param gpio Pin GPIO que generó la interrupción.
 * @param events Tipo de evento de la interrupción (flanco de bajada).
 */
void gpio_callback(uint gpio, uint32_t events);

//void Irq_enable();
//void Irq_disable();
#endif /* _MOTORES_H */
