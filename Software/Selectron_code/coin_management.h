/**
 * @file main.h
 * @brief Este archivo contiene las definiciones y prototipos para la gestión de monedas en la máquina expendedora.
 *
 * En este archivo se definen macros para los pines que se utilizarán en la gestión de monedas, y se declaran
 * los prototipos de las funciones encargadas de identificar monedas, calcular el cambio, y controlar los servos.
 */

 #ifndef _COIN_H_
 #define _COIN_H_
 
 /** 
  * @def SERVO1_PIN
  * @brief Pin GPIO utilizado para el Servomotor 1.
  */
 #define SERVO1_PIN  15
 
 /** 
  * @def SERVO2_PIN
  * @brief Pin GPIO utilizado para el Servomotor 2.
  */
 #define SERVO2_PIN  16
 
 /**
  * @def C_IN_PIN
  * @brief Pin GPIO utilizado como entrada del sensor de monedas.
  */
 #define C_IN_PIN    19
 
 /**
  * @def C_500_PIN
  * @brief Pin GPIO para la detección de monedas de 500.
  */
 #define C_500_PIN   18
 
 /**
  * @def C_1000_PIN
  * @brief Pin GPIO para la detección de monedas de 1000.
  */
 #define C_1000_PIN  20
 
 /**
  * @def C_OUT_PIN
  * @brief Pin GPIO utilizado para la detección de salida de monedas.
  */
 #define C_OUT_PIN   21
 
 /**
  * @brief Identifica la moneda insertada basándose en las lecturas de los sensores.
  *
  * Esta función lee el estado de los sensores de monedas (500, 1000 y salida) y determina si la
  * moneda es válida. Retorna 0 si la moneda es inválida, 5 si es de 500, o 10 si es de 1000.
  *
  * @param c_500_gpio Pin GPIO del sensor de monedas de 500.
  * @param c_1000_gpio Pin GPIO del sensor de monedas de 1000.
  * @param c_out_gpio Pin GPIO de la detección de salida de monedas.
  * @return uint8_t Valor que identifica la moneda.
  */
 uint8_t coin_ident(uint8_t c_500_gpio, uint8_t c_1000_gpio, uint8_t c_out_gpio);
 
 /**
  * @brief Controla el servomotor para entregar monedas.
  *
  * Activa el servomotor indicado para dispensar una moneda, dependiendo del valor (caso) de la moneda.
  *
  * @param servo_gpio Pin GPIO que controla el servomotor.
  * @param coin_case Indica el caso de la moneda: 0 para reposicionar, 5 para una moneda de 500, o 10 para una moneda de 1000.
  * @param top Valor de wrap (top) para la configuración del PWM.
  */
 void coin_way(uint8_t servo_gpio, uint8_t coin_case, float top);
 
 /**
  * @brief Convierte un ángulo en un ciclo de trabajo PWM.
  *
  * Calcula el ancho de pulso correspondiente al ángulo dado (aplicando una corrección) y lo convierte
  * en un ciclo de trabajo para un período de 20 ms.
  *
  * @param angle Ángulo deseado en grados.
  * @param fix Factor de corrección.
  * @return float Ciclo de trabajo resultante.
  */
 float angle_to_duty(float angle, float fix);
 
 /**
  * @brief Inicializa el PWM para un servomotor.
  *
  * Configura el pin GPIO para funcionar en modo PWM y ajusta los parámetros necesarios para controlar
  * un servomotor. Retorna el valor de wrap (top) obtenido.
  *
  * @param servo_gpio Pin GPIO para el servomotor.
  * @return float Valor de wrap (top) para el PWM.
  */
 float servo_pwm_init(uint8_t servo_gpio);
 
 /**
  * @brief Calcula el cambio de moneda a entregar.
  *
  * Compara el precio del producto con el total de monedas insertado y calcula la cantidad de cambio.
  * Retorna un valor negativo (-2 o -3) si no se dispone del cambio suficiente, o el cambio calculado en caso contrario.
  *
  * @param price Precio del producto (en centavos).
  * @return int8_t Cambio calculado o código de error.
  */
 int8_t coin_change(uint8_t price);
 
 /**
  * @brief Dispensa el cambio de moneda utilizando el servomotor.
  *
  * Calcula la mejor forma de entregar el cambio teniendo en cuenta las monedas disponibles y activa el
  * servomotor para dispensar el cambio.
  *
  * @param change Monto del cambio a entregar.
  * @param top Valor de wrap (top) para el PWM.
  */
 void coin_change_deliver(uint8_t change, float top);
 
 /**
  * @brief Callback para la alarma de reposición del servomotor.
  *
  * Se invoca cuando se dispara la alarma configurada para que el servomotor vuelva a su posición de reposo.
  *
  * @param id Identificador de la alarma.
  * @param user_data Puntero a datos del usuario (no utilizado).
  * @return int64_t Retorna 0.
  */
 int64_t come_back_irq(alarm_id_t id, void *user_data);
 
 #endif // _COIN_H_
 