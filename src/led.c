/*=============================================================================
 * Author: Fernando Prokopiuk <fernandoprokopiuk@gmail.com>
 * Date: 2021/07/28
 * Version: v1.0
 *===========================================================================*/
#include "./inc/led.h"

//Función ledInit
void ledInit(void)
{
    LED1_PDIR |= LED1;  //Se define como salida P1.1
    LED1_POUT &= ~LED1; //Se apaga el led
    LED2_PDIR |= LED2;  //Se define como salida P2.1
    LED2_POUT &= ~LED2; //Se apaga el led
}
//Función encenderLed
int encenderLed(int led)
{
    if (led == LED1)
    {
        LED1_POUT |= LED1;
    }
    if (led == LED2)
    {
        LED2_POUT |= LED2;
    }
    return 1;
}

//Función apagarLeds
int apagarLed(int led)
{
    if (led == LED1)
    {
        LED1_POUT &= ~LED1;
    }
    if (led == LED2)
    {
        LED2_POUT &= ~LED2;
    }
    return 1;
}
