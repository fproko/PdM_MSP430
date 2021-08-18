#include "./inc/uart.h"
#include "./inc/led.h"
#include "./inc/teclas.h"

extern char dato_rx;
extern int flag;

//Variable de estado de tipo FSM_UART_State_t
FSM_UART_State_t FSM_UART_State;

//FSM UART Mode Init
void fsm_UART_Init(void)
{
  FSM_UART_State = WAIT_C; //Se inicializa FSM UART en modo INIT_FSM
}

// FSM UART Mode Update
void fsm_UART_Update(void)
{
  static char datos[7];
  static int k = 0;
  static char *p;
  switch (FSM_UART_State)
  {
  case SEND_S:
    UART_Tx_char('\n');
    UART_Tx_char('\r');
    UART_Tx_string(p, 4);
    UART_Tx_char('\n');
    UART_Tx_char('\r');
    encenderLed(LED2);
    FSM_UART_State = WAIT_C;
    break;
  case WAIT_C:

    if (flag == 1)
    {
      apagarLed(LED2);
      if (dato_rx != 0x0D) //si no es un enter
      {
        datos[k] = dato_rx;
        k = k + 1;
        flag = 0;
      }
      if (dato_rx == 0x0D)
      {
        k = 0;
        flag = 0;
        FSM_UART_State = PROCESS_S;
      }
    }
    break;
  case PROCESS_S:
    p = calculadora(datos);
    break;
  default:
    fsm_UART_Error();
    break;
  }
}

void fsm_UART_Error(void)
{
  FSM_UART_State = WAIT_C;
}

//******************************************************************************
// Funcion: UART_Init
// Configuracion Inicial del USCI_Ax como UART
//******************************************************************************
void UART_Init(void)
{

  /* USCI Ax Control Register 1 */
  UART_UCAxCTL1 |= (UCSWRST); //USCI Software Reset, se coloca en 1 para configurar

  /* USCI Ax Control Register 0 */
  UART_UCAxCTL0 = 0x00;
  //UCSYNC=0    modo asincrónico
  //UCMODEx=00  UART Mode
  //UCSPB=0     1 bit de stop
  //UC7BIT=0    8 bit de data
  //UCMSB=0     LSBIT first
  //UCPEN=0     sin paridad

  /* USCI Ax Control Register 1 */
  UART_UCAxCTL1 |= UCSSEL__SMCLK; //Se coloca 1 en el bit 8 para seleccionar el SMCLK_1048MHz para alimentar el BRCLK

  /* USCI Ax Baud Rate  */
  /* USCI Ax Modulation Control */
#if MSP430_UART_BAUD_RATE == BAUD_RATE_9600BPS // 9600bps
  UART_UCAxBR1 = 0x00;
  UART_UCAxBR0 = 109;
  UART_UCAxMCTL |= UCBRS_2 + UCBRF_0;
#endif
#if MSP430_UART_BAUD_RATE == BAUD_RATE_19200BPS // 19200bps
  UART_UCAxBR1 = 0x00;
  UART_UCAxBR0 = 54;
  UART_UCAxMCTL |= UCBRS_5 + UCBRF_0;
#endif
#if MSP430_UART_BAUD_RATE == BAUD_RATE_56000BPS // 56000bps
  UART_UCAxBR1 = 0x00;
  UART_UCAxBR0 = 18;
  UART_UCAxMCTL |= UCBRS_1 + UCBRF_0;
#endif
#if MSP430_UART_BAUD_RATE == BAUD_RATE_115200BPS // 115200bps
  UART_UCAxBR1 = 0x00;                           //USCI_Ax Baud Rate Control 1
  UART_UCAxBR0 = 9;                              //USCI_Ax Baud Rate Control 0
  UART_UCAxMCTL |= UCBRS_1 + UCBRF_0;            //USCI_Ax Modulation Control
#endif

  UART_PORTSEL |= (UART_PIN_TX + UART_PIN_RX); // Configura los pines como  TXD/RXD
  //sive para saber si el pin correspontiente va a estar conectado al puerto o periferico
  // para el UART es con un 1
  //UART_PORTDIR |= (UART_PIN_TX); //configura como salida el pin de transmicion
  //UART_PORTDIR &= ~(UART_PIN_RX);//configura como entrada el pin de recepcion

  UART_UCAxCTL1 &= ~(UCSWRST); //USCI Software Reset, se colaca 0 para liberar operación USCI

  UART_IFG &= ~UART_UCAxRXIFG; //baja la bandera para decir q no hay interrupcion de recepcion pendiente
  UART_IE |= (UART_UCAxRXIE);  //habilita la interrupcion para la recepcion
}

//*****************************************************************
// Funcion: UART_Tx_char();
//                escribe un char por el UART
//******************************************************************
void UART_Tx_char(char dato)
{
  UART_UCAxTXBUF = dato; //Una transmisión se inicia escribiendo datos en el buffer UCAxTXBUF
  while (!(UART_IFG & UART_UCAxTXIFG))
    ;   //Espero a que se vacie el buffer de transmisión
        //UCTXIFG is set when UCAxTXBUF empty
}

//*****************************************************************
// Funcion: UART_Tx_string();
// Descr.:    escribe un string por el UART
// Paramatros: cant = 0 (escribe hasta encontrar un null (0))
//             cant != 0 (imprime el numero de chars asignado)
//*****************************************************************
// Puede usarse como:
//   sprintf((char *)&buffer[0], "variable: %d", var);
//   UART_Tx_string(buffer);
//*****************************************************************
void UART_Tx_string(char *dato, char cant)
{
  char i;

  if (!cant)
  { //si paso un cero imprime hasta null (un string)
    while (*dato)
    {                           //mientras que no tenga un null
      UART_UCAxTXBUF = *dato++; //escribo un caracter e incremento el puntero
      while (!(UART_IFG & UART_UCAxTXIFG))
        ; ///Espero a que se vacie el buffer de transmisión
    }
  }
  else
  {
    for (i = 0; i <= cant; i++)
    {                             //pasando la cantidad a imprimir
      UART_UCAxTXBUF = *(dato++); //escribo un caracter e incremento el puntero
      while (!(UART_IFG & UART_UCAxTXIFG))
        ; //Espero a que se vacie el buffer de transmisión
    }
  }
}

//******************************************
//Funcion: Calculadora();
//******************************************

char *calculadora(char *datos)
{
  static char dato_tx[4];
  int rta, j, dato1, dato2;

  dato1 = ((int)datos[0] - 0x30) * 100 + ((int)datos[1] - 0x30) * 10 + ((int)datos[2] - 0x30);
  dato2 = ((int)datos[4] - 0x30) * 100 + ((int)datos[5] - 0x30) * 10 + ((int)datos[6] - 0x30);
  switch (datos[3])
  {
  case '+':
    rta = dato1 + dato2;
    break;
  case '-':
    rta = dato1 - dato2;
    break;
  }
  j = 3;
  for (j = 3; j >= 0; j--)
  {
    dato_tx[j] = (char)(rta % 10) + 0x30; //me da el resto de la division al dividir por 10
    rta = rta / 10;
  }
  return dato_tx;
}
