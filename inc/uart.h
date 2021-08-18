#include "msp430f5529.h"

#define UART_UCAxCTL0 UCA1CTL0   //Control 0
#define UART_UCAxCTL1 UCA1CTL1   //Control 1
#define UART_UCAxBR1 UCA1BR1     //Baud rate control 1
#define UART_UCAxBR0 UCA1BR0     //Baud rate control 0
#define UART_UCAxMCTL UCA1MCTL   //Modulation control
#define UART_UCAxTXBUF UCA1TXBUF //Transmit Buffer
#define UART_UCAxRXBUF UCA1RXBUF //Receive buffer
#define UART_IE UCA1IE           //Interrupt Enable
#define UART_IFG UCA1IFG         //Interrupt Flag

/*Flags */
#define UART_UCAxRXIE UCRXIE   //Habilita interrupción de RX (forma parte del registro UCAxIE)
#define UART_UCAxTXIE UCTXIE   //Habilita interrupción de TX (forma parte del registro UCAxIE)
#define UART_UCAxRXIFG UCRXIFG //Flag de RX Es 1 cuando se recibió un dato y forma parte del registro UCAxIFG)
#define UART_UCAxTXIFG UCTXIFG //Flag de TX (Es 1 cuando TXBUF está libre y forma parte del registro UCAxIFG)

/*#define UART_ISR_RX_VECTOR  USCIAB0RX_VECTOR
#define UART_ISR_TX_VECTOR  USCIAB0TX_VECTOR*/

#define UART_PORTSEL P4SEL //seleccion del puerto 4 donde esta el UART conectado al usb
#define UART_PORTDIR P4DIR
#define UART_PIN_TX BIT4
#define UART_PIN_RX BIT5 //para el F5529 es 4 y 3 para el UCA0c

#define BAUD_RATE_9600BPS 9600     // 9600bps
#define BAUD_RATE_19200BPS 19200   // 19200bps
#define BAUD_RATE_56000BPS 56000   // 5600bps
#define BAUD_RATE_115200BPS 115200 // 115200bps

#define MSP430_UART_BAUD_RATE BAUD_RATE_115200BPS // Baud Rate in [BPS]

//! Enumeración de FSM UART state names
typedef enum
{
   SEND_S,   //!<Envia string.
   WAIT_C,   //!<Espera un caracter.
   PROCESS_S //!<Procesa caracter.

} FSM_UART_State_t;

/**
 * @brief Inicializa FSM UART.
 *
 */
void fsm_UART_Init(void);

/**
 * @brief Actualiza FSM UART.
 *
 */
void fsm_UART_Update(void);
/**
 * @brief Realiza tratamiento de un error en FSM UART.
 *
 */
void fsm_UART_Error(void);

//******************************************************************************
// Funcion: UART_Init
// Configuracion Inicial del USCI A1 como UART
//******************************************************************************
void UART_Init(void);

//*****************************************************************
// Funcion: UART_Tx_char();
//escribe un char por el UART
//******************************************************************
void UART_Tx_char(char dato);

//*****************************************************************
// Funcion: UART_Tx_string(char *dato, char cant);
// Descr.:    escribe un string por el UART
// Paramatros: cant = 0 (escribe hasta encontrar un null (0))
//             cant != 0 (imprime el numero de chars asignado)
//*****************************************************************
// Puede usarse como:
//   sprintf((char *)&buffer[0], "variable: %d", var);
//   UART_Tx_string(buffer);
//*****************************************************************
void UART_Tx_string(char *dato, char cant);

////******************************************
////Funcion: Calculadora();
////******************************************
char *calculadora(char *datos);
