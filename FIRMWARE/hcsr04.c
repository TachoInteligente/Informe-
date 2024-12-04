#include "hcsr04.h"

// Timeout en microsegundos para que vuelva el pulso
static int timeout_us = 26100;

/**
 * @brief Configura los GPIO para el HC-SR04
 * @param trigger_gpio numero de GPIO para el Trigger
 * @param echo_gpio numero de GPIO para el echo
*/
void hcsr04_init(uint8_t trigger_gpio, uint8_t echo_gpio) {
    // Habilito GPIOs
    gpio_init(trigger_gpio);
    gpio_init(echo_gpio);
    // Configuro como entrada/salida
    gpio_set_dir(trigger_gpio, GPIO_OUT);
    gpio_set_dir(echo_gpio, GPIO_IN);
}

/**
 * @brief Funcion interna que dispara el pulso y espera el retorno
 * @param trigger_gpio numero de GPIO para el Trigger
 * @param echo_gpio numero de GPIO para el echo
 * @return cantidad de microsegundos que transcurrieron entre que se envio
 * y recibio el pulso
*/
static uint64_t hcsr04_get_pulse(uint8_t trigger_gpio, uint8_t echo_gpio) {
    // Envio pulso de 10us
    gpio_put(trigger_gpio, 1);
    sleep_us(10);
    gpio_put(trigger_gpio, 0);

    uint64_t width = 0;
    // Espero mientras que no este el pulso
    while (!gpio_get(echo_gpio));
    // Guardo el tiempo actual desde que llego
    absolute_time_t start_time = get_absolute_time();
    // Espero a que el pulso baje
    while (gpio_get(echo_gpio)) {
        // Cuanto cantidad de microsegundos que se espera
        width++;
        sleep_us(1);
        // Si se excedio del timeout, termino la funcion
        if (width > timeout_us) { return 0; }
    }
    // Guardo el tiempo a
    absolute_time_t end_time = get_absolute_time();
    // Devuelvo diferencia de tiempo
    return absolute_time_diff_us(start_time, end_time);
}

/**
 * @brief Obtiene la distancia medida en cm
 * @param trigger_gpio numero de GPIO para el Trigger
 * @param echo_gpio numero de GPIO para el echo
 * @return distancia en cm
*/
float hcsr04_get_distance_cm(uint8_t trigger_gpio, uint8_t echo_gpio) {
    // Obtengo el largo del pulso en us
    uint64_t pulse_length = hcsr04_get_pulse(trigger_gpio, echo_gpio);
    // Calculo y devuelvo la distancia en cm
    return pulse_length / 58.0;
}