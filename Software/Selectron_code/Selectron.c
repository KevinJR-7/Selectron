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

int saldo; 
int cambio; 
int coin_val;
int price_1 = 10;
int price_2 = 5;
int price_3 = 25; 
int price_4 = 30; 
int coin_val; 
char buffer[17];
int Product_engine; 
volatile uint16_t top;

flagsIRQ IRQflags;

int main()
{
    stdio_init_all();
    init_gpios();
    saldo = 0;
    Product_engine = 0;
    cambio = 0;
    IRQflags.OutProduct = false; 
    IRQflags.Product_1 = false;
    IRQflags.Product_2 = false;
    IRQflags.Product_3 = false;
    IRQflags.Product_4 = false;
    IRQflags.inCoin = false; 
    // Initialize LCD
    lcd_init();
    // Set cursor to first row, first column
    lcd_backlight(true);
    lcd_set_cursor(0, 0);
    lcd_print("Bienvenido!!");

    // Set cursor to second row, first column
    lcd_set_cursor(1, 0);
    lcd_print("Estimado Usuario");
    sleep_ms(4000);
    IRQ_init_outproduct();
    lcd_clear();
    //sprintf(buffer, "saldo: %d", saldo);
    lcd_set_cursor(0, 0);
    //lcd_print(buffer); // Imprimir el contenido del buffer
    lcd_print("Inserte Monedas"); 
    lcd_set_cursor(1, 0);
    lcd_print("500 o 1000");
    //Button_init();
    //motor_init();
    gpio_init(GPIO_COIN_REPLACE_TEST);
    gpio_set_dir(GPIO_COIN_REPLACE_TEST, GPIO_IN);
    gpio_pull_down(GPIO_COIN_REPLACE_TEST);
    gpio_set_irq_enabled_with_callback(GPIO_COIN_REPLACE_TEST, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);

    coin_machine_init();
    //set_duty_cycle(MOTOR1_PIN,0.5);
    sleep_ms(4000);

    while (true) {
        if(IRQflags.inCoin == true){
            lcd_clear();
            IRQflags.inCoin = false;
            coin_val = coin_ident(C_500_PIN, C_1000_PIN, C_OUT_PIN);
            coin_way(SERVO1_PIN, coin_val, top);
            //IRQflags.inCoin = false;
            saldo = saldo + coin_val;
            sleep_ms(500);
            printf("saldo: %d", saldo);
            sprintf(buffer, "saldo: %d", saldo);
            lcd_print(buffer);

        }
        if(IRQflags.OutProduct == true ){
            //printf("Interrupción detectada en GPIO \n");
            IRQflags.OutProduct = false;
            sleep_ms(10);
            set_duty_cycle(Product_engine,0);
                lcd_clear();
                lcd_print("Inserte Monedas"); 
                lcd_set_cursor(1, 0);
                lcd_print("500 o 1000");
        }
        if(IRQflags.Product_1 == true){
            lcd_clear();
             //printf("entro papito \n");
            IRQflags.Product_1 = false;
            if(0 < saldo){
                
               change_or_not_function(price_1,MOTOR1_PIN);

            }
            else if(saldo == 0){
                lcd_set_cursor(0, 0);
                lcd_print("precio:"); 
                lcd_set_cursor(1, 0);
                lcd_print("500 $");
            }
            else{
                lcd_set_cursor(0, 0);
                lcd_print("Saldo    :c "); 
                lcd_set_cursor(1, 0);
                lcd_print("Insuficiente");
            }
        }
        if(IRQflags.Product_2 == true){
            lcd_clear();
             //printf("entro papito \n");
            IRQflags.Product_2 = false;
            if(0 < saldo){
                change_or_not_function(price_2,MOTOR2_PIN);
            }
            else if(saldo == 0){
                lcd_set_cursor(0, 0);
                lcd_print("precio:"); 
                lcd_set_cursor(1, 0);
                lcd_print("1000 $");
            }
            else{
                lcd_set_cursor(0, 0);
                lcd_print("Saldo    :c "); 
                lcd_set_cursor(1, 0);
                lcd_print("Insuficiente");
            }
        }
        if(IRQflags.Product_3 == true){
            lcd_clear();            
            IRQflags.Product_3 = false;
            if(0 < saldo){
                change_or_not_function(price_3,MOTOR3_PIN);
            }
            else if(saldo == 0){
                lcd_set_cursor(0, 0);
                lcd_print("precio:"); 
                lcd_set_cursor(1, 0);
                lcd_print("1500 $");
            }
            else{
                lcd_set_cursor(0, 0);
                lcd_print("Saldo    :c "); 
                lcd_set_cursor(1, 0);
                lcd_print("Insuficiente");
            }
        }
        if(IRQflags.Product_4 == true){
            lcd_clear();
            IRQflags.Product_4 = false;
            //printf("saldo: %d", saldo);
            if(0 < saldo){
                change_or_not_function(price_4,MOTOR4_PIN);
            }
            else if(saldo == 0){
                //printf("saldo: %d", saldo);
                lcd_set_cursor(0, 0);
                lcd_print("precio:"); 
                lcd_set_cursor(1, 0);
                lcd_print("2000 $");
            }
            else{
                lcd_set_cursor(0, 0);
                lcd_print("Saldo    :c "); 
                lcd_set_cursor(1, 0);
                lcd_print("Insuficiente");
            }
        }

    __wfi(); 

    //printf("Interrupción");
    }
}

void Button_init(){

    gpio_init(GPIO_BUTTON_PRODUCT_1);
    gpio_set_dir(GPIO_BUTTON_PRODUCT_1, GPIO_IN);
    gpio_pull_down(GPIO_BUTTON_PRODUCT_1);
    gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_1, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);

    gpio_init(GPIO_BUTTON_PRODUCT_2);
    gpio_set_dir(GPIO_BUTTON_PRODUCT_2, GPIO_IN);
    gpio_pull_down(GPIO_BUTTON_PRODUCT_2);
    gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_2, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);

    gpio_init(GPIO_BUTTON_PRODUCT_3);
    gpio_set_dir(GPIO_BUTTON_PRODUCT_3, GPIO_IN);
    gpio_pull_down(GPIO_BUTTON_PRODUCT_3);
    gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_3, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);

    gpio_init(GPIO_BUTTON_PRODUCT_4);
    gpio_set_dir(GPIO_BUTTON_PRODUCT_4, GPIO_IN);
    gpio_pull_down(GPIO_BUTTON_PRODUCT_4);
    gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_4, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);

}
void gpio_callback1(uint gpio, uint32_t events) {
    printf("GPIO callback triggered: gpio = %d, events = %u\n", gpio, events);

    if (gpio == GPIO_BUTTON_PRODUCT_1) {
        printf("GPIO_BUTTON_PRODUCT_1 activated. Setting IRQflags.Product_1 = true\n");
        IRQflags.Product_1 = true;
    }
    else if (gpio == GPIO_BUTTON_PRODUCT_2) {
        printf("GPIO_BUTTON_PRODUCT_2 activated. Setting IRQflags.Product_2 = true\n");
        IRQflags.Product_2 = true;
    }
    else if (gpio == GPIO_BUTTON_PRODUCT_3) {
        printf("GPIO_BUTTON_PRODUCT_3 activated. Setting IRQflags.Product_3 = true\n");
        IRQflags.Product_3 = true;
    }
    else if (gpio == GPIO_BUTTON_PRODUCT_4) {
        printf("GPIO_BUTTON_PRODUCT_4 activated. Setting IRQflags.Product_4 = true\n");
        IRQflags.Product_4 = true;
    }
    else if (gpio == INTERRUPT_PIN) {
        printf("INTERRUPT_PIN activated. Setting IRQflags.OutProduct = true\n");
        IRQflags.OutProduct = true;
    }
    else if (gpio == GPIO_COIN_REPLACE_TEST) {
        printf("GPIO_COIN_REPLACE_TEST activated. Setting IRQflags.inCoin = true\n");
        IRQflags.inCoin = true;
    }
    else if (gpio == C_IN_PIN) {
        printf("C_IN_PIN activated. Setting IRQflags.inCoin = true\n");
        IRQflags.inCoin = true;
    }
    else {
        printf("Unknown GPIO: %d\n", gpio);
    }
}


void motor_init(){
    setup_pwm(MOTOR1_PIN,MOTOR_PRODUCT_FRECUENCY, 0);
    setup_pwm(MOTOR2_PIN,MOTOR_PRODUCT_FRECUENCY, 0);
    setup_pwm(MOTOR3_PIN,MOTOR_PRODUCT_FRECUENCY, 0);
    setup_pwm(MOTOR4_PIN,MOTOR_PRODUCT_FRECUENCY, 0);
}

void user_buy_function(int gpio){
    saldo = 0; 
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Gracias Por"); 
    lcd_set_cursor(1, 0);
    lcd_print("Su Compra :)");
    set_duty_cycle(gpio,0.5);
    Product_engine = gpio;
}

void disable_all_irqs() {
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_1, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_2, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_3, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_4, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_COIN_REPLACE_TEST, GPIO_IRQ_EDGE_RISE, false);
    Irq_disable();
}

void enable_all_irqs() {
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_1, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_2, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_3, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_4, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_COIN_REPLACE_TEST, GPIO_IRQ_EDGE_RISE, false);
    Irq_enable();
}
void coin_machine_init(){
    gpio_init(SERVO1_PIN);
    gpio_init(SERVO2_PIN);
    gpio_init(C_IN_PIN);
    gpio_init(C_500_PIN);
    gpio_init(C_1000_PIN);
    gpio_init(C_OUT_PIN);
    // Set if GPIO is in(0) or out(1)
    gpio_set_dir(SERVO1_PIN, 1);
    gpio_set_dir(SERVO2_PIN, 1);
    gpio_set_dir(C_IN_PIN, 0);
    gpio_set_dir(C_500_PIN, 0);
    gpio_set_dir(C_1000_PIN, 0);
    gpio_set_dir(C_OUT_PIN, 0);
    //Start leds off
    gpio_set_pulls(C_IN_PIN, false, true);
    gpio_set_pulls(C_500_PIN, false, true);
    gpio_set_pulls(C_1000_PIN, false, true);
    gpio_set_pulls(C_OUT_PIN, false, true);

    top = servo_pwm_init(SERVO1_PIN);
    servo_pwm_init(SERVO2_PIN);
    //top = servo_pwm_init(SERVO2_PIN);
    coin_way(SERVO1_PIN, 0, top); //37 - 35
    coin_way(SERVO2_PIN, 0, top);
	// Infinite loop. This function shouldn't finish or return

    gpio_set_irq_enabled_with_callback(C_IN_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback1);
}
void change_or_not_function(int precio , int gpio ){
    uint8_t devueltiviris = coin_change(precio);
    if(devueltiviris == -2){
            printf("saldo %d",saldo);
            coin_change_deliver(saldo, top);
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("No hay Modenas"); 
            lcd_set_cursor(1, 0);
            lcd_print("suficientes");
            sleep_ms(3000);
    }
    else if(devueltiviris == -3){
            printf("saldo %d",saldo);
            coin_change_deliver(saldo, top);
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("Saldo    :c "); 
            lcd_set_cursor(1, 0);
            lcd_print("Insuficiente");
            sleep_ms(3000);
    }
    else{
        printf("saldo %d",saldo);
        coin_change_deliver(devueltiviris,top);
        user_buy_function(gpio);
        //return devueltiviris;
    }
}