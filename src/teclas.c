/*=============================================================================
 * Author: Fernando Prokopiuk <fernandoprokopiuk@gmail.com>
 * Date: 2021/08/11
 * Version: v1.0
 *===========================================================================*/
#include "./inc/teclas.h"
#include "./inc/timer.h"
#include "./inc/led.h"
#include "./inc/uart.h"
extern int delayDB;
extern int timer;
extern FSM_UART_State_t FSM_UART_State;

//Función leerSW

int leerSW(int sw)
{
   int estado_sw;
   if (sw == SW2)
   {
      estado_sw = SW2_PIN & SW2; //vale 0 si está presionada
   }
   return estado_sw;
}

// Variable de estado actual (global), de tipo SWState_t
static SWState_t SWState;

//FSM Anti-rebote Init
void swDBInit(void)
{
   SW1_PDIR &= ~SW1; //Input
   SW1_PREN |= SW1;  //Pullup or pulldown resistor enabled
   SW1_POUT |= SW1;  //Pullup Resistor
   SW2_PDIR &= ~SW2;
   SW2_PREN |= SW2;
   SW2_POUT |= SW2;

   SWState = SW_UP; //Se inicializa FSM de Anti-rebote
}

//FSM Anti-rebote Error
void swDBError(void)
{
   SWState = SW_UP;
}

void swPressed(int sw)
{
   static int sw2pressed = 0;
   if (sw == SW2)
   {
      sw2pressed++;
      FSM_UART_State = SEND_S;
   }
}

void swReleased(int sw)
{
   //   if (sw == TEC2)
   //   {
   //      modeUpdate(); //Se cambia de modo
   //   }
}

//FSM Anti-rebote Update
void swDBUpdate(int sw)
{
   //static delay_t delayDB; //declaración de estructura tipo delay_t
   switch (SWState)
   {
   case SW_UP:
      //Si está en estado UP y presionan el pulsador pasa al estado FALLING.
      if (!(leerSW(sw)))
      {
         SWState = SW_FALLING;
         delayDB = 4;     //setea el delay en 40 ms
         TA0CCTL0 = CCIE; //Habilita interrupción del Timer A0
         //delayWrite(4);
      }
      break;

   case SW_FALLING:
      //Resuelve las acciones correspondientes al flanco descendente.
      if (timer == delayDB) //si se cumple el delay seteado ingresa
      {
         timer = 0;
         if (!(leerSW(sw))) //Si sw sigue presionado
         {
            SWState = SW_DOWN; //Pasa al estado DOWN
            swPressed(sw);
         }
         else
         {
            SWState = SW_UP; //Vuelve al estado UP
         }
      }
      break;

   case SW_DOWN:
      //Si está en estado DOWN y liberan el botón pasa al estado RISING.
      if (leerSW(sw))
      {
         SWState = SW_RISING;
         delayDB = 4;
         TA0CCTL0 = CCIE; //Habilita interrupción del Timer A0
      }
      break;

   case SW_RISING:
      //Resuelve las acciones correspondientes al flanco ascendente.
      if (timer == delayDB) //si se cumple el delay seteado ingresa
      {
         timer = 0;
         if (leerSW(sw)) //Si la sw sigue no presionada
         {
            SWState = SW_UP; //Pasa al estado UP
            swReleased(sw);
         }
         else
         {
            SWState = SW_DOWN; //Vuelve al estado DOWN
         }
      }
      break;

   default:
      swDBError();
      break;
   }
}
