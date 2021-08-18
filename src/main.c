/*=============================================================================
 * Author: Fernando Prokopiuk <fernandoprokopiuk@gmail.com>
 * Date: 2021/08/11
 * Version: v1.0
 *===========================================================================*/
#include <msp430.h>
#include "./inc/led.h"
#include "./inc/timer.h"
#include "./inc/teclas.h"
#include "./inc/uart.h"
int timer = 0;
int delayDB = 0;
char dato_rx;
int flag = 0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    __enable_interrupt();
    ledInit();
    timerInit();
    swDBInit();
    UART_Init();
    fsm_UART_Init();

    while (1)
    {
        swDBUpdate(SW2);
        fsm_UART_Update();
    }
}

//******************************************************************************
// Function: TimerA0_ISR_CCR0
// ISR de CCR0 del Timer A0
//******************************************************************************
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA_ISR_CCR0(void)
{
    timer++; // por cada timerled son 10ms
    if (timer == delayDB)
    {
        TA0CCTL0 &= ~CCIE; //Deshabilita interrupción del Timer A0
    }
}

//******************************************************************************
// Function: USCI_A1_ISR
// ISR de USCI A1
//******************************************************************************
#pragma vector = USCI_A1_VECTOR //Se utiliza un único vector para TX y RX
__interrupt void ISR_UartRx(void)
{
    if (UART_IFG & UART_UCAxRXIFG) // UCRXIFG es 1 cuando se recibió un dato
    {
        flag = 1;
        dato_rx = UART_UCAxRXBUF; //Se guarda el contenido del RXBUF
    }
}
