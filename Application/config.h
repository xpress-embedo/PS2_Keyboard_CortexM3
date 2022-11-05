/**
 * @file config.h
 * @author Embedded Laboratory
 * @date April 30, 2016
 * @brief Configuration File of the Project.
 *
 * This file controls the speed and various other important things associated 
 * with micro-controller.
 */

#ifndef _CONFIG_H
#define _CONFIG_H

#include "LPC13xx.h"
#include "lpc13xx_gpio.h"
#include "micro.h"
#include <stdio.h>

#define	GreenLED        10    /**< Green LED Pin Number. */
#define BlueLED         11    /**< Blue LED Pin Number. */
#define LED_PORT        1     /**< LED PORt Number. */

#define EXT_INT_PORT    0     /**< External Interrupt Port Number. */
#define EXT_INT_PIN     7     /**< External Interrupt Pin Number. */

/* Function Prototype */
void InitializeSystem( void );
u32_t millis( void );

#endif /* _CONFIG_H */
