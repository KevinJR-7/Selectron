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
int price_1 = 500;
int price_2 = 1000;
int price_3 = 1500; 
int price_4 = 2000; 
int cont = 0;
int coin_val; 
char buffer[17];
int Product_engine; 
volatile uint16_t top;
int back_ms = 3000;
bool infra_resolve_variable;
flagsIRQ IRQflags;

int main()
{
    infra_resolve_variable = false;
    stdio_init_all();
    init_gpios();
    saldo = 0;
    Product_engine = 0;
    cambio = 0;
    alarm_id_t my_alarm_id;
    IRQflags.OutProduct = false; 
    IRQflags.Product_1 = false;
    IRQflags.Product_2 = false;
    IRQflags.Product_3 = false;
    IRQflags.Product_4 = false;
    IRQflags.inCoin = false; 
    IRQflags.Change_text_coin = false; 
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
    Button_init();
    motor_init();
    coin_machine_init();
    gpio_init(GPIO_COIN_REPLACE_TEST);
    gpio_set_dir(GPIO_COIN_REPLACE_TEST, GPIO_IN);
    gpio_pull_down(GPIO_COIN_REPLACE_TEST);
    gpio_set_irq_enabled_with_callback(GPIO_COIN_REPLACE_TEST, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);
    while (true) {
        if(IRQflags.inCoin == true){
            //cancel_alarm(my_alarm_id); //DD
            cancel_alarm(my_alarm_id);
            lcd_clear();
            IRQflags.inCoin = false;
            coin_val = coin_ident(C_500_PIN, C_1000_PIN, C_OUT_PIN);
            disable_all_irqs();
            coin_way(SERVO1_PIN, coin_val, top);
            printf("Valor Moneda: %d\n,", coin_val);
            if(coin_val <= 2){
                lcd_set_cursor(0, 0);
                lcd_print("La moneda "); 
                lcd_set_cursor(1, 0);
                lcd_print("No es valida $");
                disable_all_irqs();
                sleep_ms(3000);
                enable_all_irqs();
                lcd_clear();
                if(saldo == 0){
                    Insert_coin_text();               
                }
                else{
                    printf("saldo: %d", saldo);
                    sprintf(buffer, "saldo: %d", saldo);
                    lcd_print(buffer);
                }
                

            }
            else{
                //IRQflags.inCoin = false;
                saldo = saldo + (coin_val*100);
                //sleep_ms(500);
                printf("saldo: %d", saldo);
                sprintf(buffer, "saldo: %d", saldo);
                lcd_print(buffer);
            }
            enable_all_irqs();
            
            /* lcd_clear();
            cancel_alarm(my_alarm_id);
            IRQflags.inCoin = false;
            saldo = saldo + 500;
            sprintf(buffer, "saldo: %d", saldo);
            lcd_print(buffer);
            sleep_ms(46);
            printf("saldo: %d\n",saldo); */


        }
        if(IRQflags.OutProduct == true ){
            IRQflags.OutProduct = false;
            cancel_alarm(my_alarm_id);
            //if(infra_resolve_variable == true){
                infra_resolve_variable = false;
                printf("ingresa aqui?????????????22222222\n");
                set_duty_cycle(Product_engine,0);
                lcd_clear();
                lcd_print("Inserte Monedas"); 
                lcd_set_cursor(1, 0);
                lcd_print("500 o 1000");
                gpio_set_irq_enabled_with_callback(INTERRUPT_PIN,GPIO_IRQ_EDGE_FALL,false,gpio_callback1);
                enable_all_irqs();
            //}
        }
        if(IRQflags.Product_1 == true){
            disable_all_irqs();
            cancel_alarm(my_alarm_id);
            lcd_clear();
            //cancel_alarm(my_alarm_id);
            IRQflags.Product_1 = false;
            if(0 < saldo){
                
            change_or_not_function(price_1,MOTOR1_PIN);
            printf("saldo: %d", saldo );
            cancel_alarm(my_alarm_id);
            }
            else if(saldo == 0){
                lcd_set_cursor(0, 0);
                lcd_print("precio:"); 
                /* lcd_set_cursor(1, 0);
                lcd_print("500 $"); */
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
        if(IRQflags.Product_2 == true){
            disable_all_irqs();
            cancel_alarm(my_alarm_id);
            lcd_clear();
            //cancel_alarm(my_alarm_id);
            IRQflags.Product_2 = false;
            if(0 < saldo){
                change_or_not_function(price_2,MOTOR2_PIN);
                printf("saldo: %d", saldo );
                cancel_alarm(my_alarm_id);
            }
            else if(saldo == 0){
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
        if(IRQflags.Product_3 == true){
            disable_all_irqs();
            cancel_alarm(my_alarm_id);
            lcd_clear();
            //cancel_alarm(my_alarm_id);

            IRQflags.Product_3 = false;
            if(0 < saldo){
                change_or_not_function(price_3,MOTOR3_PIN);
                printf("saldo: %d", saldo );
                cancel_alarm(my_alarm_id);
            }
            else if(saldo == 0){
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
        if(IRQflags.Product_4 == true){
            disable_all_irqs();
            cancel_alarm(my_alarm_id);
            lcd_clear();
            //cancel_alarm(my_alarm_id);
            IRQflags.Product_4 = false;
            //printf("Saldo: %d", saldo);
            if(0 < saldo){
                change_or_not_function(price_4,MOTOR4_PIN);
                printf("saldo: %d", saldo );
                cancel_alarm(my_alarm_id);
            }
            else if(saldo == 0){
                //printf("saldo: %d", saldo);
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
        if(IRQflags.Change_text_coin == true){
            IRQflags.Change_text_coin = false;
            printf("entra acaa ?????\n");
            Insert_coin_text();
        
        }

    __wfi(); 
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
        //printf("GPIO_BUTTON_PRODUCT_2 activated. Setting IRQflags.Product_2 = true\n");
        IRQflags.Product_2 = true;
    }
    else if (gpio == GPIO_BUTTON_PRODUCT_3) {
        //printf("GPIO_BUTTON_PRODUCT_3 activated. Setting IRQflags.Product_3 = true\n");
        IRQflags.Product_3 = true;
    }
    else if (gpio == GPIO_BUTTON_PRODUCT_4) {
        //printf("GPIO_BUTTON_PRODUCT_4 activated. Setting IRQflags.Product_4 = true\n");
        IRQflags.Product_4 = true;
    }
    else if (gpio == INTERRUPT_PIN) {
       if (events & GPIO_IRQ_EDGE_FALL) { // Detectar el flanco de bajada
                                   // Función de manejo
            printf("Infrarrojo %d\n", gpio);
            IRQflags.OutProduct = true;
        } 
    }
    /* else if(gpio == GPIO_COIN_REPLACE_TEST){
        IRQflags.inCoin = true;
    } */
    else if (gpio == C_IN_PIN) {
        IRQflags.inCoin = true;
    }
    else {
        //printf("Unknown GPIO: %d\n", gpio);
    }
}


void motor_init(){
    setup_pwm(MOTOR1_PIN,MOTOR_PRODUCT_FRECUENCY, 0);
    setup_pwm(MOTOR2_PIN,MOTOR_PRODUCT_FRECUENCY, 0);
    setup_pwm(MOTOR3_PIN,MOTOR_PRODUCT_FRECUENCY, 0);
    setup_pwm(MOTOR4_PIN,MOTOR_PRODUCT_FRECUENCY, 0);
}

void user_buy_function(int gpio){
    gpio_set_irq_enabled_with_callback(INTERRUPT_PIN,GPIO_IRQ_EDGE_FALL,true,gpio_callback1);
    disable_all_irqs();
    saldo = 0; 
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Gracias Por"); 
    lcd_set_cursor(1, 0);
    lcd_print("Su Compra :)");
    set_duty_cycle(gpio,1.0);
    Product_engine = gpio;
    
}

void disable_all_irqs() {
    //Irq_disable();
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_1, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_2, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_3, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_BUTTON_PRODUCT_4, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(GPIO_COIN_REPLACE_TEST, GPIO_IRQ_EDGE_RISE, false);
    gpio_set_irq_enabled(C_IN_PIN, GPIO_IRQ_EDGE_FALL, false);
}

void enable_all_irqs() {
    //Irq_enable();
    gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_1, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);
    gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_2, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);
    gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_3, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);
    gpio_set_irq_enabled_with_callback(GPIO_BUTTON_PRODUCT_4, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);
    gpio_set_irq_enabled_with_callback(GPIO_COIN_REPLACE_TEST, GPIO_IRQ_EDGE_RISE, true, &gpio_callback1);
    gpio_set_irq_enabled_with_callback(C_IN_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback1);
}
void coin_machine_init(){
    gpio_init(SERVO1_PIN);
    gpio_init(SERVO2_PIN);
    gpio_init(C_IN_PIN);
    gpio_init(C_500_PIN);
    gpio_init(C_1000_PIN);
    gpio_init(C_OUT_PIN);
    // Set if GPIO is in(0) or out(1
    gpio_set_dir(SERVO1_PIN, 1);
    gpio_set_dir(SERVO2_PIN, 1);
    gpio_set_dir(C_IN_PIN, 0);
    gpio_set_dir(C_500_PIN, 0);
    gpio_set_dir(C_1000_PIN, 0);
    gpio_set_dir(C_OUT_PIN, 0);
    //Start leds okf
    gpio_set_pulls(C_IN_PIN, false, true);
    gpio_set_pulls(C_500_PIN, false, true);
    gpio_set_pulls(C_1000_PIN, false, true);
    gpio_set_pulls(C_OUT_PIN, false, true);

    top = servo_pwm_init(SERVO1_PIN);
    servo_pwm_init(SERVO2_PIN);
    //top = servo_pwm_init(SERVO2_PIN);
    coin_way(SERVO1_PIN, 0, top); //37 - 35
    coin_way(SERVO2_PIN, 0, top);
	// INfinite loop. This function shouldn't finish or return

    gpio_set_irq_enabled_with_callback(C_IN_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback1);
}
void change_or_not_function(int precio , int gpio ){
    //Irq_disable();
    precio = precio / 100;
    int8_t devueltiviris = coin_change(precio);

    printf("Devuelta %d\n :", devueltiviris);
    int saldo_cen = saldo / 100;
    if(devueltiviris == -1){
        printf("saldo %d",saldo_cen);
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
    else if (devueltiviris == -2){
            printf("saldo %d",saldo_cen);
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
    else if(devueltiviris == -3){
            printf("saldo %d",saldo_cen);
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
    else{
        printf("saldo: %d",saldo_cen);
        coin_change_deliver(devueltiviris,top);
        user_buy_function(gpio);
        infra_resolve_variable = true;
    }
    
}
long long int alarm_change_text(long int time, void *user_data){
    IRQflags.Change_text_coin = true;
    printf("alama\n");
    return 0;
}
void Insert_coin_text(){
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Inserte Monedas"); 
    lcd_set_cursor(1, 0);
    lcd_print("500 o 1000");
}
void all_IRQ_false(){
    IRQflags.OutProduct = false; 
    IRQflags.Product_1 = false;
    IRQflags.Product_2 = false;
    IRQflags.Product_3 = false;
    IRQflags.Product_4 = false;
    IRQflags.inCoin = false; 
    IRQflags.Change_text_coin = false;  
}

//// agrrgar funcion de todas  las benderas falsas con todos las irq falsas donde para los botones o ANTIRREBOTE
//// para que el precio no salga despues del saldo 
//// MONEDA INVALIDA CAMBIAR