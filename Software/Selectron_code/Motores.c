#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "Motores.h"
#include "selectron.h"

// Variable global para almacenar el valor de wrap (si fuera necesario)
uint32_t wrap;

/**
 * @brief Callback para manejar la interrupción de GPIO.
 *
 * Detecta el flanco de bajada y, si se produce, activa la bandera de salida de producto.
 *
 * @param gpio Número del GPIO que generó la interrupción.
 * @param events Tipo de evento de la interrupción.
 */
void gpio_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) { // Detecta el flanco de bajada
        IRQflags.OutProduct = true;
    }
}

/**
 * @brief Configura el PWM en un pin específico.
 *
 * Configura el GPIO indicado para operar en modo PWM, estableciendo la frecuencia y el ciclo de trabajo.
 *
 * @param gpio Pin GPIO para el PWM.
 * @param frequency Frecuencia deseada en Hz.
 * @param duty_cycle Ciclo de trabajo (valor entre 0 y 1).
 */
void setup_pwm(uint gpio, float frequency, float duty_cycle) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint channel = pwm_gpio_to_channel(gpio);
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint32_t clock_freq = clock_get_hz(clk_sys);
    if (frequency <= 0) {
        return;
    }

    float clkdiv = 1.0;
    uint32_t wrap = clock_freq / frequency - 1;

    if (wrap > 65535) {
        clkdiv = (float)wrap / 65535.0 + 1.0;
        wrap = clock_freq / (frequency * clkdiv) - 1;
    }

    if (wrap > 65535) {
        return;
    }

    pwm_set_clkdiv(slice_num, clkdiv);
    pwm_set_wrap(slice_num, wrap);

    if (duty_cycle < 0 || duty_cycle > 1) {
        return;
    }

    uint32_t level = duty_cycle * wrap;
    pwm_set_chan_level(slice_num, channel, level);
    pwm_set_enabled(slice_num, true);
}

/**
 * @brief Cambia dinámicamente el ciclo de trabajo del PWM.
 *
 * Actualiza el nivel del canal PWM en función del nuevo ciclo de trabajo. En este ejemplo,
 * se utiliza un valor fijo para wrap.
 *
 * @param gpio Pin GPIO configurado en modo PWM.
 * @param duty_cycle Nuevo ciclo de trabajo (valor entre 0 y 1).
 */
void set_duty_cycle(uint gpio, float duty_cycle) {
    // Se utiliza un wrap fijo en este ejemplo; este valor puede ajustarse según la configuración.
    wrap = 12499;
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint channel = pwm_gpio_to_channel(gpio);
    uint32_t level = (uint32_t)(duty_cycle * wrap);
    pwm_set_chan_level(slice_num, channel, level);
}

/**
 * @brief Inicializa la interrupción en el pin designado para salida de producto.
 *
 * Configura el pin INTERRUPT_PIN como entrada para que pueda detectar interrupciones.
 */
void IRQ_init_outproduct() {
    gpio_init(INTERRUPT_PIN);
    gpio_set_dir(INTERRUPT_PIN, GPIO_IN);
}
