/**
 * @file main.c
 * @author Embedded Laboratory
 * @date June 10, 2016
 * @brief This is the main file of the project.
 * 
 * \mainpage Description
 * This project demonstates how to use PS2 keyboard to take inputs from user.
 * PS2 Keyboard has two important pins one is clock and another one is data.
 * Data Changes only on falling edges of Clock Signal, hence clock pin is
 * configured to interrupt on falling edges and then data line is read and the
 * data read is stored in buffers. This data is then displayed on 16x2 LCD.
 * 
 * \section library_sec Libraries Used in this Project
 * \subsection library1 PS2 Keyboard Library
 * This library handles decoding PS2 Keyboard Data.
 * \subsection library2 16x2 LCD Library
 * This library controls and display characters on LCD.
 */

#include "config.h"
#include "ps2_keyboard.h"
#include "lcd_16x2.h"

static boolean int_led_state = FALSE;

/**
 * Main Program.
 */
int main()
{
  u32_t timestamp = 0, keyboard_timestamp = 0, lcd_backlit_timestamp = 0;;
  u8_t keypress = 0, lcd_count = 0;;
  boolean led_state = TRUE, first_keypress = FALSE;
  InitializeSystem();
  // Enable External Interrupt for Port-0
  NVIC_EnableIRQ(EINT0_IRQn);
  // Set Direction as Output
  GPIO_SetDir(LED_PORT, GreenLED, 1);
  GPIO_SetDir(LED_PORT, BlueLED,  1);
  // Set External Interrupt Pin as Input
  GPIO_SetDir( EXT_INT_PORT, EXT_INT_PIN, 0);
  // Set External Interrupt Pin, single trigger, active high.
  GPIO_SetInterrupt( EXT_INT_PORT, EXT_INT_PIN, 0, 0, 0);
  // Enable Interrupt
  GPIO_IntEnable( EXT_INT_PORT, EXT_INT_PIN );
  GPIO_Init();
  PS2_Keyboard_Init();
  LCD_Init();
  timestamp = millis();
  LCD_BackLight_On();
  LCD_Write_Text("PS2 Board Exmple");
  LCD_Cmd(LCD_FIRST_ROW);
  while(1)
  {
    if (millis() - keyboard_timestamp > 50u )
    {
      keyboard_timestamp = millis();
      if( !(IS_PS2_Busy()) )
      {
        u8_t temp = getKey();
        if( temp )
        {
          keypress = temp;
          lcd_count++;
          if(lcd_count > 15u || !first_keypress)
          {
            lcd_count = 0;
            LCD_Cmd(LCD_CLEAR);
            LCD_Cmd(LCD_FIRST_ROW);
          }
          first_keypress = TRUE;
          LCD_Write(keypress);
          LCD_BackLight_On();
          lcd_backlit_timestamp = millis();
        }
      }
    }
    
    if( millis() - lcd_backlit_timestamp > 10000u)
    {
      lcd_backlit_timestamp = millis();
      LCD_BackLight_Off();
    }
    
    if( millis() - timestamp > 1000ul )
    {
      timestamp = millis();
      if( led_state )
      {
        led_state = FALSE;
        // Turn-Off LED's
        GPIO_SetValue(LED_PORT, GreenLED);
      }
      else
      {
        led_state = TRUE;
        // Turn-On LED's
        GPIO_ClearValue(LED_PORT, GreenLED);
      }
    }
  }
}

/**
 * @brief External Interrupt PORT0.
 */
void PIOINT0_IRQHandler(void)
{
  uint32_t regVal;
  regVal = GPIO_IntStatus( EXT_INT_PORT, EXT_INT_PIN);
  if ( regVal )
  {
    GPIO_IntClear( EXT_INT_PORT, EXT_INT_PIN);
    if( int_led_state )
    {
      int_led_state = FALSE;
      GPIO_SetValue(LED_PORT, BlueLED);
    }
    else
    {
      int_led_state = TRUE;
      GPIO_ClearValue(LED_PORT, BlueLED);
    }
  }
  return;
}

/**
 * @brief External Interrupt Port3
 *
 * PS2 State Machine is Handled in this Interrupt.
 * 
 */
void PIOINT3_IRQHandler(void)
{
  uint32_t regVal;
  regVal = GPIO_IntStatus( PS2_CLK_PORT, PS2_CLK_PIN);
  if ( regVal )
  {
    GPIO_IntClear( PS2_CLK_PORT, PS2_CLK_PIN);
    PS2_State_Machine();
  }
  return;
}
