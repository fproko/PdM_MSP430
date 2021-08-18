/*=============================================================================
 * Author: Fernando Prokopiuk <fernandoprokopiuk@gmail.com>
 * Date: 2021/08/11
 * Version: v1.0
 *===========================================================================*/
#include "./inc/timer.h"

//Función Configuracion Inicial Timer A0
void timerInit(void)
{
    //Control Register
    TA0CTL = (TASSEL_2 + ID_3 + MC_1); // SMCLK (1048 kHz ;  divisor por 8 (131 kHz) ; cuenta hasta el valor TAxCCR0 en modo 'up'
    TA0CCR0 = 1310;                    //Valor hasta el que cuenta: 1310 10 ms, 131 1 ms
    //Capture/Compare Control n Register
    TA0CCTL0 = (CM_0 + CCIS_2); // Captura desabilitada, input signal GND, interrupción habilitada
    //TA0CCTL0 = (CM_0 + CCIS_2 + CCIE);    // Captura desabilitada, input signal GND, interrupción habilitada
}
