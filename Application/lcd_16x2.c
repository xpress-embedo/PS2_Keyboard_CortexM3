/**
 * @file lcd_16x2.c
 * @author Embedded Laboratory
 * @date May 12, 2016
 * @brief LCD Initialization and Functions Definitions.
 */

#include "lcd_16x2.h"

/* Private Function Prototype*/
static void lcd_delay_ms( u32_t ms );

/**
 * @brief Initialize 16x2 LCD Module.
 *
 * Initialize 16x2 LCD Module in 8-bit mode.
 * 
 */
void LCD_Init(void)
{
  // Set LCD Pins as Output Pins
  GPIO_SetDir( LCD_DATA_PORT, LCD_D0, 1);
  GPIO_SetDir( LCD_DATA_PORT, LCD_D1, 1);
  GPIO_SetDir( LCD_DATA_PORT, LCD_D2, 1);
  GPIO_SetDir( LCD_DATA_PORT, LCD_D3, 1);
  GPIO_SetDir( LCD_DATA_PORT, LCD_D4, 1);
  GPIO_SetDir( LCD_DATA_PORT, LCD_D5, 1);
  GPIO_SetDir( LCD_DATA_PORT, LCD_D6, 1);
  GPIO_SetDir( LCD_DATA_PORT, LCD_D7, 1);
  
  LPC_IOCON->R_PIO1_0 |= 0x1;
  LPC_IOCON->R_PIO1_1 |= 0x1;
  LPC_IOCON->R_PIO1_2 |= 0x1;
  GPIO_SetDir( LCD_RS_PORT, LCD_RS, 1);
  GPIO_SetDir( LCD_RW_PORT, LCD_RW, 1);
  GPIO_SetDir( LCD_EN_PORT, LCD_EN, 1);
  
  GPIO_SetDir( LCD_BACKLIT_PORT, LCD_BACKLIT_PIN, 1);
  
  // Clear Values
  GPIO_ClearValue( LCD_DATA_PORT, LCD_D0);
  GPIO_ClearValue( LCD_DATA_PORT, LCD_D1);
  GPIO_ClearValue( LCD_DATA_PORT, LCD_D2);
  GPIO_ClearValue( LCD_DATA_PORT, LCD_D3);
  GPIO_ClearValue( LCD_DATA_PORT, LCD_D4);
  GPIO_ClearValue( LCD_DATA_PORT, LCD_D5);
  GPIO_ClearValue( LCD_DATA_PORT, LCD_D6);
  GPIO_ClearValue( LCD_DATA_PORT, LCD_D7);
  GPIO_ClearValue( LCD_RS_PORT, LCD_RS);
  GPIO_ClearValue( LCD_RW_PORT, LCD_RW);
  GPIO_ClearValue( LCD_EN_PORT, LCD_EN);
  GPIO_ClearValue(LCD_BACKLIT_PORT,LCD_BACKLIT_PIN);
  LCD_Cmd(LCD_16x2_INIT);
  lcd_delay_ms(10u);
  lcd_delay_ms(10u);
  LCD_Cmd(LCD_DISP_ON_CUR_ON);
  lcd_delay_ms(10u);
  LCD_Cmd(LCD_DISP_ON_CUR_OFF);
  lcd_delay_ms(10u);
  LCD_Cmd(LCD_CLEAR);
  lcd_delay_ms(10u);
  LCD_Cmd(LCD_FIRST_ROW);
  lcd_delay_ms(20u);
}

/**
 * @brief Send Command to LCD.
 *
 * Send Command to LCD, use the following commands.
 * <b>LCD_16x2_INIT,LCD_DISP_ON_CUR_ON,LCD_DISP_ON_CUR_OFF,LCD_DISP_ON_CUR_BLNK,
 * LCD_FIRST_ROW,LCD_SECOND_ROW,LCD_CLEAR</b>.
 * @param command Command to Send to the LCD.
 */
void LCD_Cmd(u8_t command)
{
  CHECK_BIT(command, 0u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D0):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D0);
  CHECK_BIT(command, 1u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D1):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D1);
  CHECK_BIT(command, 2u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D2):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D2);
  CHECK_BIT(command, 3u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D3):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D3);
  CHECK_BIT(command, 4u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D4):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D4);
  CHECK_BIT(command, 5u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D5):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D5);
  CHECK_BIT(command, 6u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D6):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D6);
  CHECK_BIT(command, 7u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D7):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D7);
  GPIO_ClearValue( LCD_RS_PORT,LCD_RS);
  GPIO_ClearValue( LCD_RW_PORT,LCD_RW);
  GPIO_ClearValue( LCD_EN_PORT,LCD_EN);
  lcd_delay_ms(1);
  GPIO_SetValue( LCD_EN_PORT,LCD_EN);
}

/**
 * @brief Write Data on LCD.
 *
 * Write Data on LCD, specified as arguments.
 * @param Data Data to Write on LCD.
 */
void LCD_Write(u8_t Data)
{
  CHECK_BIT(Data, 0u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D0):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D0);
  CHECK_BIT(Data, 1u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D1):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D1);
  CHECK_BIT(Data, 2u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D2):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D2);
  CHECK_BIT(Data, 3u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D3):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D3);
  CHECK_BIT(Data, 4u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D4):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D4);
  CHECK_BIT(Data, 5u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D5):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D5);
  CHECK_BIT(Data, 6u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D6):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D6);
  CHECK_BIT(Data, 7u) ? GPIO_SetValue( LCD_DATA_PORT,LCD_D7):
    GPIO_ClearValue( LCD_DATA_PORT,LCD_D7);
  GPIO_SetValue( LCD_RS_PORT,LCD_RS);
  GPIO_ClearValue( LCD_RW_PORT,LCD_RW);
  GPIO_ClearValue( LCD_EN_PORT,LCD_EN);
  lcd_delay_ms(1);
  GPIO_SetValue( LCD_EN_PORT,LCD_EN);
}

/**
 * @brief Write String on LCD.
 *
 * Write String on LCD, specified as arguments.
 * @param *msg First Character Address of the String.
 * @note String Must be terminated by NULL Character.
 */
void LCD_Write_Text(u8_t *msg)
{
  while(*msg)
  {
    LCD_Write(*msg);
    msg++;
  }
}

/**
 * @brief Turn On Back Light.
 *
 * Turn's On the Back Light of LCD.
 * @note Power Consumption will be increased.
 */
void LCD_BackLight_On( void )
{
  GPIO_SetValue(LCD_BACKLIT_PORT,LCD_BACKLIT_PIN);
}

/**
 * @brief Turn Off Back Light.
 *
 * Turn's Off the Back Light of LCD.
 */
void LCD_BackLight_Off( void )
{
  GPIO_ClearValue(LCD_BACKLIT_PORT,LCD_BACKLIT_PIN);
}

/**
 * @brief Delay For LCD.
 *
 * @warning Note Accurate, approximate.
 */
static void lcd_delay_ms( u32_t ms )
{
  u32_t i,j = 0;
  for(i=0;i<=ms;i++)
  {
    for(j=0;j<6000u;j++)
      ;
  }
}
