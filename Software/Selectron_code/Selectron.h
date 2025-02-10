/**
 * @file selectron.h
 * @brief Declaraciones y definiciones para el proyecto Selectron.
 * @authors Angel, kevin y benjamin 
 * Este archivo contiene las macros, la definición de la estructura de banderas de interrupción
 * (IRQflags) y los prototipos de las funciones utilizadas en el proyecto.
 */

 #ifndef SELECTRON_H
 #define SELECTRON_H
 
 #include <stdbool.h>
 
 /**
  * @def GPIO_BUTTON_PRODUCT_1
  * @brief Pin GPIO asignado al botón del Producto 1.
  */
 #define GPIO_BUTTON_PRODUCT_1 6
 
 /**
  * @def GPIO_BUTTON_PRODUCT_2
  * @brief Pin GPIO asignado al botón del Producto 2.
  */
 #define GPIO_BUTTON_PRODUCT_2 7
 
 /**
  * @def GPIO_BUTTON_PRODUCT_3
  * @brief Pin GPIO asignado al botón del Producto 3.
  */
 #define GPIO_BUTTON_PRODUCT_3 8
 
 /**
  * @def GPIO_BUTTON_PRODUCT_4
  * @brief Pin GPIO asignado al botón del Producto 4.
  */
 #define GPIO_BUTTON_PRODUCT_4 9
 
 /**
  * @def GPIO_COIN_REPLACE_TEST
  * @brief Pin GPIO usado para testear la detección/reemplazo de monedas.
  */
 #define GPIO_COIN_REPLACE_TEST 22
 
 /**
  * @def MOTOR_PRODUCT_FRECUENCY
  * @brief Frecuencia en Hz para el PWM de los motores de producto.
  */
 #define MOTOR_PRODUCT_FRECUENCY 10000
 
 /**
  * @def DUTY_MOTOR_PRODUCT
  * @brief Ciclo de trabajo (duty cycle) para el PWM de los motores de producto.
  */
 #define DUTY_MOTOR_PRODUCT 0.5
 
 /**
  * @struct flagsIRQ
  * @brief Estructura que almacena las banderas de interrupción.
  *
  * Esta estructura contiene flags para identificar diferentes eventos de interrupción,
  * como la inserción de moneda, la selección de productos y la solicitud de cambio de texto.
  */
 typedef struct {
     volatile bool Change_text_coin; /**< Flag para cambio de texto en el display. */
     volatile bool OutProduct;        /**< Flag que indica la salida de producto. */
     volatile bool inCoin;            /**< Flag que indica la inserción de moneda. */
     volatile bool alarmCoin;         /**< Flag para alarma de monedas. */
     volatile bool Product_1;         /**< Flag para el Producto 1. */
     volatile bool Product_2;         /**< Flag para el Producto 2. */
     volatile bool Product_3;         /**< Flag para el Producto 3. */
     volatile bool Product_4;         /**< Flag para el Producto 4. */
 } flagsIRQ;
 
 /**
  * @brief Variable global de banderas de interrupción.
  */
 extern flagsIRQ IRQflags;
 
 /**
  * @brief Inicializa los botones para la selección de productos.
  *
  * Configura los pines GPIO correspondientes a los botones de producto como entrada
  * con resistencia pull-down y establece sus interrupciones.
  */
 void Button_init();
 
 /**
  * @brief Callback global para la interrupción de GPIO.
  *
  * Se ejecuta cuando ocurre una interrupción en uno de los pines configurados y establece
  * la bandera correspondiente en la estructura IRQflags.
  *
  * @param gpio Número del GPIO que disparó la interrupción.
  * @param events Tipo de evento (por ejemplo, flanco ascendente o descendente).
  */
 void gpio_callback1(uint gpio, uint32_t events);
 
 /**
  * @brief Inicializa los motores (PWM).
  *
  * Configura los canales PWM asociados a los motores de producto.
  */
 void motor_init();
 
 /**
  * @brief Evalúa si se debe realizar un cambio o no, basado en el precio y la selección.
  *
  * Compara el saldo actual con el precio del producto y toma acciones, como entregar cambio
  * o activar la compra.
  *
  * @param precio Precio del producto.
  * @param gpio Pin GPIO asociado al motor del producto.
  */
 void change_or_not_function(int precio, int gpio);
 
 /**
  * @brief Callback de alarma para cambio de texto en el display.
  *
  * Se utiliza con el sistema de alarmas para activar el flag que indica que se debe cambiar
  * el texto en el display.
  *
  * @param time Tiempo actual (en microsegundos o según el sistema de alarmas).
  * @param user_data Puntero a datos de usuario (no utilizado).
  * @return long long int Siempre retorna 0.
  */
 long long int alarm_change_text(long int time, void *user_data);
 
 /**
  * @brief Deshabilita todas las interrupciones configuradas.
  *
  * Desactiva las interrupciones en los pines correspondientes a los botones y la detección de monedas.
  */
 void disable_all_irqs();
 
 /**
  * @brief Habilita todas las interrupciones configuradas.
  *
  * Activa las interrupciones para los pines correspondientes a los botones y la detección de monedas.
  */
 void enable_all_irqs();
 
 /**
  * @brief Inicializa la máquina de monedas.
  *
  * Configura los pines y PWM de los servos, así como la detección de monedas y otras entradas relacionadas.
  */
 void coin_machine_init();
 
 /**
  * @brief Muestra el mensaje de "Inserte Monedas" en el display.
  *
  * Limpia el display y muestra el texto invitando a insertar monedas.
  */
 void Insert_coin_text();
 
 /**
  * @brief Resetea todas las banderas de interrupción.
  *
  * Establece todas las banderas en la estructura IRQflags a false.
  */
 void all_IRQ_false();
 
 #endif // SELECTRON_H
 