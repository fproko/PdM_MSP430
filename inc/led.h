/*=============================================================================
 * Author: Fernando Prokopiuk <fernandoprokopiuk@gmail.com>
 * Date: 2021/07/28
 * Version: v1.0
 *===========================================================================*/
#include <msp430.h>
#define LED1_PDIR P1DIR
#define LED1_POUT P1OUT
#define LED1 BIT0
#define LED2_PDIR P4DIR
#define LED2_POUT P4OUT
#define LED2 BIT7

/**
 * @brief Incializa GPIO para LEDs .
 *
 */
void ledInit(void);

/**
 * @brief Enciende el led que recibe como parámetro .
 *
 * @param  led      Led que se busca encender.

 */
int encenderLed(int led);

/**
 * @brief Apaga todos los Leds.
 *
 * @return int "1" -> Se completó el apagado del LEDX.
 */
int apagarLed(int led);
