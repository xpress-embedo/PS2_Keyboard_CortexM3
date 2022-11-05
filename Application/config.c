/**
 * @file config.c
 * @author Embedded Laboratory
 * @date April 30, 2016
 * @brief Configuration File of the Project.
 *
 * This file controls the speed and various other important things associated 
 * with micro-controller.
 */

#include "config.h"

// Private Variable
static u32_t msTicks = 0;		// Stores milli-second Counter

/**
 * @brief SysTick Interrupt Service Routine.
 *
 * SysTick Interrupt is configured to give interrupt every 1msecond.
 * 
 */
void SysTick_Handler( void )
{
  msTicks++;
}

/**
 * @brief Initialize System.
 *
 * Intializes the Micro-controller Clock and configure the SysTick Interrupt, to
 * at 1 milli-second.
 */
void InitializeSystem( void )
{
  u32_t returnCode = 0;
  u32_t SystemFrequency = 72000000ul;
  SystemInit();
  returnCode = SysTick_Config(SystemFrequency/1000);	// 1 msec interrupt
  if( returnCode != 0)
  {
    while(1);
  }
}

/**
 * @brief Millis.
 *
 * Returns the number of milliseconds since the board began running the current 
 * program. This number will overflow (go back to zero), after approximately 
 * 50 days.This function returns time in milli-second
 * @return Number of milliseconds since the program started (#u32_t)
 */
u32_t millis( void )
{
  return msTicks;
}
