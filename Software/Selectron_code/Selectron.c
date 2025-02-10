/**
 * @file Selectron.c
 * @brief Implementación principal del proyecto Selectron.
 * @authors Angel, Kevin y Benjamin 
 * Este archivo contiene la función main y la implementación de las funciones definidas en
 * selectron.h, que manejan la lógica del proyecto, como la gestión de interrupciones, la
 * inicialización de motores y la administración de monedas.
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "LCD.h"
 #include "Motores.h"
 #include "hardware/irq.h"
 #include "hardware/timer.h"
 #include "hardware/clocks.h"
 #include "hardware/sync.h" 
 #include "selectron.h"
 #include "coin_management.h"
 
 // Variables globales
 int saldo; 
 int cambio; 
 int coin_val;
 int price_1 = 500;
 int price_2 = 1000;
 int price_3 = 1500; 
 int price_4 = 2000; 
 int cont = 0;
 char buffer[17];
 int Product_engine; 
 volatile uint16_t top;
 int back_ms = 3000;
 bool infra_resolve_variable;
 flagsIRQ IRQflags;
 
 /**
  * @brief Función principal.
  *
  * Inicializa el sistema, configura el LCD, los botones, los motores, la máquina de monedas
  * y gestiona el bucle principal en el que se responden las interrupciones y se procesa la lógica
  * de inserción de monedas y selección de productos.
  *
  * @return int Código de salida.
  */
 int main()
 {
     infra_resolve_variable = false;
     stdio_init_all();
     init_gpios();
     saldo = 0;
     Product_engine = 0;
     cambio = 0;
     alarm_id_t my_alarm_id;
     
     // Inicialización de las banderas de interrupción
     IRQflags.OutProduct = false; 
     IRQflags.Product_1 = false;
     IRQflags.Product_2 = false;
     IRQflags.Product_3 = false;
     IRQflags.Product_4 = false;
     IRQflags.inCoin = false; 
     IRQflags.Change_text_coin = false; 
 
     // Inicialización del LCD
     lcd_init();
     lcd_backlight(true);
     lcd_set_cursor(0, 0);
     lcd_print("Bienvenido!!");
     lcd_set_cursor(1, 0);
     lcd_print("Estimado Usuario");
     sleep_ms(4000);
     
     IRQ_init_outproduct();
     lcd_clear();
     lcd_set_cursor(0, 0);
     lcd_print("Inserte Monedas"); 
     lcd_set_cursor(1, 0);
     lcd_print("500 o 1000");
     
     // Inicialización de botones, motores y máquina de monedas
     Button_init();
     motor_init();
     coin_machine_init();
     
     gpio_init(GPIO_COIN_REPLACE_TEST);
     gpio_set_dir(GPIO_COIN_REPLACE_TEST, GPIO_IN);
     gpio_pull_down(GPIO_COIN_REPLACE_TEST);
     gpio_set_irq_enabled_with_callback(GPIO_COIN_REPLACE_TEST, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);
     
     while (true) {
         // Procesamiento de interrupciones y eventos
         
         if (IRQflags.inCoin == true) {
             cancel_alarm(my_alarm_id);
             lcd_clear();
             IRQflags.inCoin = false;
             coin_val = coin_ident(C_500_PIN, C_1000_PIN, C_OUT_PIN);
             disable_all_irqs();
             coin_way(SERVO1_PIN, coin_val, top);
             printf("Valor Moneda: %d\n", coin_val);
             if (coin_val <= 2) {
                 lcd_set_cursor(0, 0);
                 lcd_print("La moneda "); 
                 lcd_set_cursor(1, 0);
                 lcd_print("No es valida $");
                 disable_all_irqs();
                 sleep_ms(3000);
                 enable_all_irqs();
                 lcd_clear();
                 if (saldo == 0) {
                     Insert_coin_text();               
                 } else {
                     printf("saldo: %d", saldo);
                     sprintf(buffer, "saldo: %d", saldo);
                     lcd_print(buffer);
                 }
             } else {
                 saldo = saldo + (coin_val * 100);
                 printf("saldo: %d", saldo);
                 sprintf(buffer, "saldo: %d", saldo);
                 lcd_print(buffer);
             }
             enable_all_irqs();
         }
         
         if (IRQflags.OutProduct == true) {
             IRQflags.OutProduct = false;
             cancel_alarm(my_alarm_id);
             //coin_change_deliver(cambio, top);
             //sleep_ms(3000);
             infra_resolve_variable = false;
             printf("ingresa aqui?????????????22222222\n");
             set_duty_cycle(Product_engine, 0);
             lcd_clear();
             lcd_print("Inserte Monedas"); 
             lcd_set_cursor(1, 0);
             lcd_print("500 o 1000");
             gpio_set_irq_enabled_with_callback(INTERRUPT_PIN, GPIO_IRQ_EDGE_FALL, false, gpio_callback1);
             enable_all_irqs();
         }
         
         if (IRQflags.Product_1 == true) {
             disable_all_irqs();
             cancel_alarm(my_alarm_id);
             lcd_clear();
             IRQflags.Product_1 = false;
             if (0 < saldo) {
                 change_or_not_function(price_1, MOTOR1_PIN);
                 printf("saldo: %d", saldo);
                 cancel_alarm(my_alarm_id);
             } else if (saldo == 0) {
                 lcd_set_cursor(0, 0);
                 lcd_print("precio:");
                 lcd_set_cursor(1, 0);
                 sprintf(buffer, "$ %d", price_1);
                 lcd_print(buffer);
                 cancel_alarm(my_alarm_id);
                 my_alarm_id = add_alarm_in_ms(back_ms, alarm_change_text, NULL, true);
             }
             sleep_ms(900);
             all_IRQ_false();
             enable_all_irqs();
         }
         
         if (IRQflags.Product_2 == true) {
             disable_all_irqs();
             cancel_alarm(my_alarm_id);
             lcd_clear();
             IRQflags.Product_2 = false;
             if (0 < saldo) {
                 change_or_not_function(price_2, MOTOR2_PIN);
                 printf("saldo: %d", saldo);
                 cancel_alarm(my_alarm_id);
             } else if (saldo == 0) {
                 lcd_set_cursor(0, 0);
                 lcd_print("precio:");
                 lcd_set_cursor(1, 0);
                 sprintf(buffer, "$ %d", price_2);
                 lcd_print(buffer);
                 cancel_alarm(my_alarm_id);
                 my_alarm_id = add_alarm_in_ms(back_ms, alarm_change_text, NULL, true);             
             }
             sleep_ms(900);
             all_IRQ_false();
             enable_all_irqs();
         }
         
         if (IRQflags.Product_3 == true) {
             disable_all_irqs();
             cancel_alarm(my_alarm_id);
             lcd_clear();
             IRQflags.Product_3 = false;
             if (0 < saldo) {
                 change_or_not_function(price_3, MOTOR3_PIN);
                 printf("saldo: %d", saldo);
                 cancel_alarm(my_alarm_id);
             } else if (saldo == 0) {
                 lcd_set_cursor(0, 0);
                 lcd_print("precio:");
                 lcd_set_cursor(1, 0);
                 sprintf(buffer, "$ %d", price_3);
                 lcd_print(buffer);
                 cancel_alarm(my_alarm_id);
                 my_alarm_id = add_alarm_in_ms(back_ms, alarm_change_text, NULL, true);
             }
             sleep_ms(900);
             all_IRQ_false();
             enable_all_irqs();
         }
         
         if (IRQflags.Product_4 == true) {
             disable_all_irqs();
             cancel_alarm(my_alarm_id);
             lcd_clear();
             IRQflags.Product_4 = false;
             if (0 < saldo) {
                 change_or_not_function(price_4, MOTOR4_PIN);
                 printf("saldo: %d", saldo);
                 cancel_alarm(my_alarm_id);
             } else if (saldo == 0) {
                 lcd_set_cursor(0, 0);
                 lcd_print("precio:");
                 lcd_set_cursor(1, 0);
                 sprintf(buffer, "$ %d", price_4);
                 lcd_print(buffer);
                 cancel_alarm(my_alarm_id);
                 my_alarm_id = add_alarm_in_ms(back_ms, alarm_change_text, NULL, true);
             }
             sleep_ms(900);
             all_IRQ_false();
             enable_all_irqs();
         }
         
         if (IRQflags.Change_text_coin == true) {
             IRQflags.Change_text_coin = false;
             printf("entra acaa ?????\n");
             Insert_coin_text();
         }
 
         __wfi();  // Espera en bajo consumo hasta la siguiente interrupción
     }
     return 0;
 }
 
 /**
  * @brief Inicializa los pines de los botones para la selección de productos.
  *
  * Configura los GPIO correspondientes a los botones de producto como entradas con pull-down
  * y habilita sus interrupciones usando el callback global.
  */
 void Button_init(){
     gpio_init(GPIO_BUTTON_PRODUCT_1);
     gpio_set_dir(GPIO_BUTTON_PRODUCT_1, GPIO_IN);
     gpio_pull_down(GPIO_BUTTON_PRODUCT_1);
     gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_1, GPIO_IRQ_EDGE_RISE, true, gpio_callback1);
 
     gpio_init(GPIO_BUTTON_PRODUCT_2);
     gpio_set_dir(GPIO_BUTTON_PRODUCT_2, GPIO_IN);
     gpio_pull_down(GPIO_BUTTON_PRODUCT_2);
     gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_2, GPIO_IRQ_EDGE_RISE, true, gpio_callback1);
 
     gpio_init(GPIO_BUTTON_PRODUCT_3);
     gpio_set_dir(GPIO_BUTTON_PRODUCT_3, GPIO_IN);
     gpio_pull_down(GPIO_BUTTON_PRODUCT_3);
     gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_3, GPIO_IRQ_EDGE_RISE, true, gpio_callback1);
 
     gpio_init(GPIO_BUTTON_PRODUCT_4);
     gpio_set_dir(GPIO_BUTTON_PRODUCT_4, GPIO_IN);
     gpio_pull_down(GPIO_BUTTON_PRODUCT_4);
     gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_4, GPIO_IRQ_EDGE_RISE, true, gpio_callback1);
 }
 
 /**
  * @brief Callback para las interrupciones de GPIO.
  *
  * Se invoca cuando se dispara una interrupción en uno de los pines configurados. Establece
  * la bandera correspondiente en la estructura IRQflags según el pin que disparó la interrupción.
  *
  * @param gpio El número del GPIO que generó la interrupción.
  * @param events El tipo de evento que ocurrió.
  */
 void gpio_callback1(uint gpio, uint32_t events) {
    
 
     if (gpio == GPIO_BUTTON_PRODUCT_1) {
     }
     else if (gpio == GPIO_BUTTON_PRODUCT_2) {
         IRQflags.Product_2 = true;
     }
     else if (gpio == GPIO_BUTTON_PRODUCT_3) {
         IRQflags.Product_3 = true;
     }
     else if (gpio == GPIO_BUTTON_PRODUCT_4) {
         IRQflags.Product_4 = true;
     }
     else if (gpio == INTERRUPT_PIN) {
         if (events & GPIO_IRQ_EDGE_FALL) {
             IRQflags.OutProduct = true;
         }
     }
     else if (gpio == C_IN_PIN) {
         IRQflags.inCoin = true;
     }
 }
 
 /**
  * @brief Inicializa los motores configurando sus PWM.
  *
  * Llama a la función setup_pwm para cada motor, usando la frecuencia definida para los productos.
  */
 void motor_init(){
     setup_pwm(MOTOR1_PIN, MOTOR_PRODUCT_FRECUENCY, 0);
     setup_pwm(MOTOR2_PIN, MOTOR_PRODUCT_FRECUENCY, 0);
     setup_pwm(MOTOR3_PIN, MOTOR_PRODUCT_FRECUENCY, 0);
     setup_pwm(MOTOR4_PIN, MOTOR_PRODUCT_FRECUENCY, 0);
 }
 
 /**
  * @brief Función para procesar la compra del usuario.
  *
  * Reinicia el saldo, muestra un mensaje de agradecimiento, activa el PWM al 100% para el motor
  * correspondiente y almacena el GPIO del motor usado.
  *
  * @param gpio El GPIO asociado al motor del producto comprado.
  */
 void user_buy_function(int gpio){
     gpio_set_irq_enabled_with_callback(INTERRUPT_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback1);
     disable_all_irqs();
     saldo = 0; 
     lcd_clear();
     lcd_set_cursor(0, 0);
     lcd_print("Gracias Por"); 
     lcd_set_cursor(1, 0);
     lcd_print("Su Compra :)");
     set_duty_cycle(gpio, 1.0);
     Product_engine = gpio;
 }
 
 /**
  * @brief Deshabilita todas las interrupciones configuradas.
  *
  * Desactiva las interrupciones de los pines asociados a los botones y la detección de monedas.
  */
 void disable_all_irqs() {
     // Irq_disable();  // Si se utiliza la función global, se puede deshabilitar aquí
     gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_1, GPIO_IRQ_EDGE_RISE, false);
     gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_2, GPIO_IRQ_EDGE_RISE, false);
     gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_3, GPIO_IRQ_EDGE_RISE, false);
     gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_4, GPIO_IRQ_EDGE_RISE, false);
     gpio_set_irq_enabled(GPIO_COIN_REPLACE_TEST, GPIO_IRQ_EDGE_RISE, false);
     gpio_set_irq_enabled(C_IN_PIN, GPIO_IRQ_EDGE_FALL, false);
 }
 
 /**
  * @brief Habilita todas las interrupciones configuradas.
  *
  * Reactiva las interrupciones para los botones y la detección de monedas.
  */
 void enable_all_irqs() {
     // Irq_enable();  // Si se utiliza la función global, se puede habilitar aquí
     gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_1, GPIO_IRQ_EDGE_RISE, true, gpio_callback1);
     gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_2, GPIO_IRQ_EDGE_RISE, true, gpio_callback1);
     gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_3, GPIO_IRQ_EDGE_RISE, true, gpio_callback1);
     gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_4, GPIO_IRQ_EDGE_RISE, true, gpio_callback1);
     gpio_set_irq_enabled_with_callback(GPIO_COIN_REPLACE_TEST, GPIO_IRQ_EDGE_RISE, true, gpio_callback1);
     gpio_set_irq_enabled_with_callback(C_IN_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback1);
 }
 
 /**
  * @brief Inicializa la máquina de monedas.
  *
  * Configura los pines de los servos, sensores y salidas relacionados con el manejo de monedas,
  * y establece los PWM para los servos.
  */
 void coin_machine_init(){
     gpio_init(SERVO1_PIN);
     gpio_init(SERVO2_PIN);
     gpio_init(C_IN_PIN);
     gpio_init(C_500_PIN);
     gpio_init(C_1000_PIN);
     gpio_init(C_OUT_PIN);
     
     gpio_set_dir(SERVO1_PIN, 1);
     gpio_set_dir(SERVO2_PIN, 1);
     gpio_set_dir(C_IN_PIN, 0);
     gpio_set_dir(C_500_PIN, 0);
     gpio_set_dir(C_1000_PIN, 0);
     gpio_set_dir(C_OUT_PIN, 0);
     
     // Configurar resistencias pull en los pines de entrada para monedas
     gpio_set_pulls(C_IN_PIN, false, true);
     gpio_set_pulls(C_500_PIN, false, true);
     gpio_set_pulls(C_1000_PIN, false, true);
     gpio_set_pulls(C_OUT_PIN, false, true);
 
     top = servo_pwm_init(SERVO1_PIN);
     servo_pwm_init(SERVO2_PIN);
     coin_way(SERVO1_PIN, 0, top);
     coin_way(SERVO2_PIN, 0, top);
     
     // Habilitar la interrupción para el sensor de entrada de monedas
     gpio_set_irq_enabled_with_callback(C_IN_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback1);
 }
 
 /**
  * @brief Procesa la lógica de cambio o no, basado en el precio y el saldo.
  *
  * Evalúa el valor retornado por coin_change() y toma las acciones correspondientes,
  * mostrando mensajes en el LCD y llamando a la función user_buy_function() si corresponde.
  *
  * @param precio Precio del producto (en centavos).
  * @param gpio Pin GPIO asociado al motor del producto.
  */
 void change_or_not_function(int precio, int gpio) {
     precio = precio / 100;
     int8_t devueltiviris = coin_change(precio);
 
     printf("Devuelta %d\n", devueltiviris);
     int saldo_cen = saldo / 100;
     if (devueltiviris == -1) {
         printf("saldo %d\n", saldo_cen);
         coin_change_deliver(saldo_cen, top);
         lcd_clear();
         lcd_set_cursor(0, 0);
         lcd_print("Saldo    :c ");
         lcd_set_cursor(1, 0);
         lcd_print("Insuficiente");
         sleep_ms(3000);
         saldo = 0;
         Insert_coin_text();
     }
     else if (devueltiviris == -2) {
         printf("saldo %d\n", saldo_cen);
         coin_change_deliver(saldo_cen, top);
         lcd_clear();
         lcd_set_cursor(0, 0);
         lcd_print("Cambio ");
         lcd_set_cursor(1, 0);
         lcd_print("insuficiente");
         sleep_ms(3000);
         saldo = 0;
         Insert_coin_text();
     }
     else if (devueltiviris == -3) {
         printf("saldo %d\n", saldo_cen);
         coin_change_deliver(saldo_cen, top);
         lcd_clear();
         lcd_set_cursor(0, 0);
         lcd_print("insuficientes");
         lcd_set_cursor(1, 0);
         lcd_print("de 500");
         sleep_ms(3000);
         saldo = 0;
         Insert_coin_text();
     }
     else {
         printf("saldo: %d\n", saldo_cen);
         //cambio = devueltiviris;
         coin_change_deliver(devueltiviris, top);
         sleep_ms(3000);
         user_buy_function(gpio);
         infra_resolve_variable = true;
     }
 }
 
 /**
  * @brief Callback de alarma para el cambio de texto en el display.
  *
  * Se invoca cuando se dispara la alarma configurada para cambiar el texto del display.
  *
  * @param time Tiempo actual (en milisegundos o según se use en la alarma).
  * @param user_data Puntero a datos del usuario (no utilizado).
  * @return long long int Retorna 0.
  */
 long long int alarm_change_text(long int time, void *user_data) {
     IRQflags.Change_text_coin = true;
     printf(" alarma\n");
     return 0;
 }
 
 /**
  * @brief Muestra el mensaje "Inserte Monedas" en el LCD.
  *
  * Limpia el LCD y despliega el mensaje para invitar al usuario a insertar monedas.
  */
 void Insert_coin_text(){
     lcd_clear();
     lcd_set_cursor(0, 0);
     lcd_print("Inserte Monedas");
     lcd_set_cursor(1, 0);
     lcd_print("500 o 1000");
 }
 
 /**
  * @brief Resetea todas las banderas de interrupción.
  *
  * Pone todas las flags de IRQ en falso para reiniciar el estado de las interrupciones.
  */
 void all_IRQ_false(){
     IRQflags.OutProduct = false; 
     IRQflags.Product_1 = false;
     IRQflags.Product_2 = false;
     IRQflags.Product_3 = false;
     IRQflags.Product_4 = false;
     IRQflags.inCoin = false; 
     IRQflags.Change_text_coin = false;
 }
 