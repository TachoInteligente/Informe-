#include <stdio.h> 
#include "pico/stdlib.h" 
#include "hcsr04.h" 
#include "hardware/pwm.h"

#define TRIG_GPIO	8
#define ECHO_GPIO	7
#define SERV_GPIO	22

/*
* @brief Programa principal 
*/
int main(void) { 
    // Inicializacion del USB 
    stdio_init_all();
	sleep_ms(3000);
	
 	// Inicializacion del ultrasonico 
    hcsr04_init(TRIG_GPIO, ECHO_GPIO);

    //configuro el GPIO como PWM
    gpio_set_function(SERV_GPIO, GPIO_FUNC_PWM);

    //obtengo el slice
    uint8_t slice = pwm_gpio_to_slice_num(SERV_GPIO);
 
    //configuracion por defecto de PWM
	pwm_config config = pwm_get_default_config();
	
	//divido el clock de 125Mhz por 125 para tener 1Mhz
	pwm_config_set_clkdiv(&config, 125.0);
	
	//Wrap de 50000 para una frecuencia de 50Hz (20ms)
	pwm_config_set_wrap(&config, 20000);

	//inicializo PWM con la configuracion
	pwm_init(slice, &config, true);

	// Inicializo la variable de medición
	float d = 0;
	
	while(1) {

		printf("Vivo\n");
		// Hago una lectura de distancia 
		d = hcsr04_get_distance_cm(TRIG_GPIO, ECHO_GPIO);

		sleep_ms(250);
		printf("Post distancia\n");
		
		// Verifico distancia si hay contacto
		if(1<= d && d< 10) {
			// Muestro por consola
			printf("Distancia: %2.2f cm",  d);

			// Abrir el tacho(activar servo)
			
			//ajuste de nivel de PWM para activar
			pwm_set_gpio_level(SERV_GPIO, 2500); 

			// Espero
			sleep_ms(2000);

			//ajuste de nivel de PWM para desactivar
			pwm_set_gpio_level(SERV_GPIO, 500);
		}	

		// Muestro la distancia cuando hay algo cerca pero no lo suficiente
		if(10 < d && d <=20) {
			printf("Todavía estas muy lejos... Distancia: %2.2f cm",  d);

		}
	}
}