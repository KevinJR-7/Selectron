#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "stdio.h"
#include "Motores.h"
#include "selectron.h"
//include "Selectron.c"
// Rutina de manejo de la interrupci0n
void gpio_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) { // Detectar el flanco de bajada
                                   // Función de manejo
        //printf("Interrupción detectada en GPI %d\n", gpio);
        IRQflags.OutProduct = true;
    }
}


//extern flagsIRQ IRQflags; 

uint32_t wrap;

// Función para configurar el PWM con frecuencia y ciclo de trabajo
void setup_pwm(uint gpio, float frequency, float duty_cycle) {
    printf("Configuring PWM for GPIO %d with frequency %.2f Hz and duty cycle %.2f%%\n", gpio, frequency, duty_cycle * 100);

    uint slice_num = pwm_gpio_to_slice_num(gpio);
    printf("Slice Number: %d\n", slice_num);

    uint channel = pwm_gpio_to_channel(gpio);
    printf("Channel: %d\n", channel);

    gpio_set_function(gpio, GPIO_FUNC_PWM);
    printf("GPIO %d set to PWM function\n", gpio);

    uint32_t clock_freq = clock_get_hz(clk_sys);
    printf("Clock frequency: %d Hz\n", clock_freq);

    if (frequency <= 0) {
        printf("Error: Frequency must be greater than 0\n");
        return;
    }

    float clkdiv = 1.0;
    uint32_t wrap = clock_freq / frequency - 1;

    // Si wrap es demasiado grande, reducimos el clock con clkdiv
    if (wrap > 65535) {
        clkdiv = (float)wrap / 65535.0 + 1.0;  // Ajustar el divisor para reducir wrap
        wrap = clock_freq / (frequency * clkdiv) - 1;
    }

    printf("Calculated wrap value: %d (after clkdiv = %.2f)\n", wrap, clkdiv);

    if (wrap > 65535) {
        printf("Error: Wrap value still exceeds maximum allowed (65535)\n");
        return;
    }

    pwm_set_clkdiv(slice_num, clkdiv);  // Ajustar el divisor de frecuencia del PWM
    pwm_set_wrap(slice_num, wrap);

    if (duty_cycle < 0 || duty_cycle > 1) {
        printf("Error: Duty cycle must be between 0 and 1\n");
        return;
    }

    uint32_t level = duty_cycle * wrap;
    printf("Calculated duty cycle level: %d\n", level);

    pwm_set_chan_level(slice_num, channel, level);
    printf("Duty cycle level set to: %d\n", level);

    pwm_set_enabled(slice_num, true);
    printf("PWM enabled for slice %d\n", slice_num);
}

// Función para cambiar el ciclo de trabajo dinámicamente
void set_duty_cycle(uint gpio, float duty_cycle) {
    printf("\n--- set_duty_cycle() ---\n");
    printf("GPIO: %d\n", gpio);
    printf("Duty cycle recibido: %.2f%%\n", duty_cycle * 100);
    wrap = 12499;
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint channel = pwm_gpio_to_channel(gpio);
    printf("Slice asignado: %d\n", slice_num);
    printf("Canal asignado: %d\n", channel);

    printf("Valor actual de wrap: %d\n", wrap);  // Imprimir el valor de wrap

    uint32_t level = (uint32_t)(duty_cycle * wrap);  // Convertir duty_cycle a nivel PWM
    printf("Nivel PWM calculado: %d\n", level);

    // Configurar el nivel PWM
    pwm_set_chan_level(slice_num, channel, level);
    printf("PWM actualizado en GPIO %d\n", gpio);
}

void IRQ_init_outproduct() {
    gpio_init(INTERRUPT_PIN);                     // Inicializar el GPIO
    gpio_set_dir(INTERRUPT_PIN, GPIO_IN);         // Configurarlo como entrada
    gpio_pull_down(INTERRUPT_PIN);                  // Activar resistencia pull-up

    // Configurar la interrupción en el GPIO
    gpio_set_irq_enabled_with_callback(
        INTERRUPT_PIN,                            // GPIO a configurar
        GPIO_IRQ_EDGE_FALL,                       // Interrupción por flanco de bajada
        true,                                     // Habilitar interrupción
        gpio_callback                             // Función de manejo
    );
}

void Irq_enable(){
    gpio_set_irq_enabled_with_callback(
        INTERRUPT_PIN,                            // GPIO a configurar
        GPIO_IRQ_EDGE_FALL,                       // Interrupción por flanco de bajada
        true,                                     // Habilitar interrupción
        gpio_callback                             // Función de manejo
    );
}

void Irq_disable(){
    gpio_set_irq_enabled_with_callback(
        INTERRUPT_PIN,                            // GPIO a configurar
        GPIO_IRQ_EDGE_FALL,                       // Interrupción por flanco de bajada
        false,                                     // Habilitar interrupción
        gpio_callback                             // Función de manejo
    );
}

