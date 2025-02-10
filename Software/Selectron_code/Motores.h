/**
 * @file Motores.h
 * @brief Prototipos de funciones y macros para el control de motores de los productos y la configuración de interrupciones.
 * 
 * Este archivo define los pines que se utilizarán para los motores y la interrupción, y declara las funciones
 * necesarias para configurar el PWM, cambiar el ciclo de trabajo, inicializar interrupciones y gestionar la lógica de eventos.
 * 
 * @author Angel
 * @date Octubre 2024
 */

 #ifndef _MOTORES_H
 #define _MOTORES_H
 
 /**
  * @def MOTOR1_PIN
  * @brief GPIO asignado al Motor 1.
  */
 #define MOTOR1_PIN 2
 
 /**
  * @def MOTOR2_PIN
  * @brief GPIO asignado al Motor 2.
  */
 #define MOTOR2_PIN 3
 
 /**
  * @def MOTOR3_PIN
  * @brief GPIO asignado al Motor 3.
  */
 #define MOTOR3_PIN 4
 
 /**
  * @def MOTOR4_PIN
  * @brief GPIO asignado al Motor 4.
  */
 #define MOTOR4_PIN 5
 
 /**
  * @def INTERRUPT_PIN
  * @brief GPIO asignado a la interrupción (por ejemplo, para un sensor infrarrojo).
  */
 #define INTERRUPT_PIN 10
 
 /**
  * @brief Configura el PWM en un pin específico con una frecuencia y un ciclo de trabajo determinados.
  *
  * Configura el GPIO especificado para operar en modo PWM, estableciendo la frecuencia y el
  * ciclo de trabajo inicial.
  *
  * @param gpio Pin GPIO para el PWM.
  * @param frequency Frecuencia deseada en Hz.
  * @param duty_cycle Ciclo de trabajo (valor entre 0 y 1).
  */
 void setup_pwm(uint gpio, float frequency, float duty_cycle);
 
 /**
  * @brief Cambia dinámicamente el ciclo de trabajo de un pin PWM.
  *
  * Actualiza el nivel del canal PWM en función del nuevo ciclo de trabajo.
  *
  * @param gpio Pin GPIO configurado en modo PWM.
  * @param duty_cycle Nuevo ciclo de trabajo (valor entre 0 y 1).
  */
 void set_duty_cycle(uint gpio, float duty_cycle);
 
 /**
  * @brief Inicializa la interrupción en el pin predefinido.
  *
  * Configura el GPIO designado para la interrupción (por ejemplo, para un sensor) como entrada.
  */
 void IRQ_init_outproduct();
 
 /**
  * @brief Callback para manejar la interrupción de GPIO.
  *
  * Se invoca cuando se genera una interrupción en alguno de los pines configurados y gestiona el evento
  * correspondiente.
  *
  * @param gpio Número del GPIO que generó la interrupción.
  * @param events Tipo de evento (por ejemplo, flanco de bajada).
  */
 void gpio_callback(uint gpio, uint32_t events);
 
 #endif /* _MOTORES_H */
 