/*=============================================================================
 * Author: Fernando Prokopiuk <fernandoprokopiuk@gmail.com>
 * Date: 2021/08/11
 * Version: v1.0
 *===========================================================================*/
#include <msp430.h>

#define SW1_PDIR P2DIR
#define SW1_PREN P2REN
#define SW1_POUT P2OUT
#define SW1_PIN P2IN
#define SW1 BIT1

#define SW2_PDIR P1DIR
#define SW2_PREN P1REN
#define SW2_POUT P1OUT
#define SW2_PIN P1IN
#define SW2 BIT1

//! Enumeración de FSM Antirebote
typedef enum
{
   SW_UP,      //!<Mientras el switch está liberado.
   SW_FALLING, //!<Mientras está ocurriendo el flanco descendente, hace anti_rebote
   SW_RISING,  //!<Mientras está ocurriendo el flanco ascendente, hace anti_rebote
   SW_DOWN     //!<Mientras el switch está presionado.
} SWState_t;

/**
 * @brief Inicializa FSM Anti-rebote.
 *
 */
void swDBInit(void);

/**
 * @brief Actualiza FSM Anti-rebote.
 *
 * @param sw   Switch al cual se aplica Anti-Rebote.
 */
void swDBUpdate(int sw);

/**
 * @brief Realiza tratamiento de un error en FSM Antirebote.
 *
 */
void swDBError(void);

/**
 * @brief Resuelva las acciones correspondientes al flanco descendente.
 *
 * @param sw Switch presionado.
 */
void swPressed(int sw);

/**
 * @brief Resuelva las acciones correspondientes al flanco descendente.
 *
 *
 * Cuando se libera la sw SW2 se llama a la función modeUpdate(), para indicar que el switch
 * fue presionado.
 * @param sw Switch soltado.
 */
void swReleased(int sw);

int leerSW(int sw);
