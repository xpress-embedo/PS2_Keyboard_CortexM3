/**
 * @file ps2_keyboard.c
 * @author Embedded Laboratory
 * @date June 10, 2016
 * @brief PS2 Keyboard, contains State Machine for decoding PS2 data coming from
 * PS2 keyboard.
 */

#include "ps2_keyboard.h"

/* Private Functions */
static u8_t Decode_PS2_Key( void );
static u8_t Delete_From_Queue ( void );
static u8_t Get_From_Queue ( void );
static boolean IS_Queue_Empty( void );
static boolean Insert_In_Queue( u8_t scan_code );

// http://www.computer-engineering.org/ps2keyboard/scancodes2.html
// PS2 keyboard codes (standard set #2)
const u8_t PS2_KeyCodes[128] = {
  0,    F9,     0,      F5,     F3,     F1,     F2,     F12,    //0x00
  0,    F10,    F8,     F6,     F4,     TAB,    '`',    0,      //0x08
  0,    0,      L_SHFT, 0,      L_CTRL, 'q',    '1',    0,      //0x10
  0,    0,      'z',    's',    'a',    'w',    '2',    0,      //0x18
  0,    'c',    'x',    'd',    'e',    '4',    '3',    0,      //0x20
  0,    ' ',    'v',    'f',    't',    'r',    '5',    0,      //0x28
  0,    'n',    'b',    'h',    'g',    'y',    '6',    0,      //0x30
  0,    0,      'm',    'j',    'u',    '7',    '8',    0,      //0x38
  0,    ',',    'k',    'i',    'o',    '0',    '9',    0,      //0x40
  0,    '.',    '/',    'l',    ';',    'p',    '-',    0,      //0x48
  0,    0,      '\'',   0,      '[',    '=',    0,      0,      //0x50
  CAPS, R_SHFT  ,ENTER, ']',    0,      0x5c,   0,      0,      //0x58
  0,    0,      0,      0,      0,      0,      BKSP,   0,      //0x60
  0,    '1',    0,      '4',    '7',    0,      0,      0,      //0x68
  0,    '.',    '2',    '5',    '6',    '8',    ESC,    NUM,    //0x70
  F11,  '+',    '3',    '-',    '*',    '9',    0,      0       //0x78
};  /**< PS2 Keyboard ASCII Value LookUp Table. */

const u8_t PS2_ShiftKeyCodes[128] = {
  0,    F9,     0,      F5,     F3,     F1,     F2,     F12,    //0x00
  0,    F10,    F8,     F6,     F4,     TAB,    '~',    0,      //0x08
  0,    0,      L_SHFT, 0,      L_CTRL, 'Q',    '!',    0,      //0x10
  0,    0,      'Z',    'S',    'A',    'W',    '@',    0,      //0x18
  0,    'C',    'X',    'D',    'E',    '$',    '#',    0,      //0x20
  0,    ' ',    'V',    'F',    'T',    'R',    '%',    0,      //0x28
  0,    'N',    'B',    'H',    'G',    'Y',    '^',    0,      //0x30
  0,    0,      'M',    'J',    'U',    '&',    '*',    0,      //0x38
  0,    '<',    'K',    'I',    'O',    ')',    '(',    0,      //0x40
  0,    '>',    '?',    'L',    ':',    'P',    '_',    0,      //0x48
  0,    0,      '\"',   0,      '{',    '+',    0,      0,      //0x50
  CAPS, R_SHFT, ENTER,  '}',    0,      '|',    0,      0,      //0x58
  0,    0,      0,      0,      0,      0,      BKSP,   0,      //0x60
  0,    '1',    0,      '4',    '7',    0,      0,      0,      //0x68
  0,    '.',    '2',    '5',    '6',    '8',    ESC,    NUM,    //0x70
  F11,  '+',    '3',    '-',    '*',    '9',    0,      0       //0x78
};  /**< PS2 Keyboard ASCII Value LookUp Table when Shift Key is Pressed. */

static Queue_s s_queue = {0,-1,{0}};
static PS2_State_e PS2_State = PS2_START; /**<Track PS2 State in StateMachine.*/
static PS2_Keyboard_s ps2 = {0, 0, 0, 0, 0, FALSE, FALSE, FALSE};

/**
 * @brief Initialize PS2 Keyboard.
 *
 * Initialize PS2 Keyboard pins as Input and Clock Pin is configured on falling
 * edge interrupt.
 */
void PS2_Keyboard_Init( void )
{
  // Enable PS2 Keyboard Clock Interrupt for Port-3
  NVIC_EnableIRQ(EINT3_IRQn);
  // Set Keyboard Clock and Data Pin as Input
  GPIO_SetDir( PS2_CLK_PORT, PS2_CLK_PIN, 0);
  GPIO_SetDir( PS2_DATA_PORT, PS2_DATA_PIN, 0);
  // Set External Interrupt Pin, single trigger, active high.
  GPIO_SetInterrupt( PS2_CLK_PORT, PS2_CLK_PIN, 0, 0, 0);
  // Enable Interrupt
  GPIO_IntEnable( PS2_CLK_PORT, PS2_CLK_PIN );
}

/**
 * @brief PS2 Port State.
 *
 * Returns the TRUE or FALSE depending on the bus situation, if the bus is busy
 * in some processing of data coming from PS2 keyboard then TRUE will be return.
 * @return True if Busy and False if Free.
 */
boolean IS_PS2_Busy( void )
{
  return ( ps2.PS2_Busy || IS_Queue_Empty() );
}

/**
 * @brief PS2 State Machine.
 *
 * The function automatically decodes the pulses coming from the PS2 Keyboard
 * and stores it in internal buffer. This is time critical function, so make 
 * sure to call this function on falling edge of PS2 Clock Pin. 
 * 
 * Call this function as follow:
 * @code
 * if( CLOCK PIN goes Low )
 * {
 *  PS2_State_Machine();
 * }
 * @endcode
 * @note Call this function in Clock Pin Falling Interrupt.
 */
void PS2_State_Machine( void )
{
  u32_t regVal = 0;
  switch (PS2_State)
  {
  default:
  case PS2_START:
    ps2.parity_value = 0;
    ps2.scan_code = 0;
    regVal = (GPIO_ReadValue(PS2_DATA_PORT) >> PS2_DATA_PIN) & 0x01;
    if( regVal == 0x00 )
    {
      // Start Bit Received
      PS2_State++;
      ps2.PS2_Busy = TRUE;
    }
    break;
  case PS2_DATA:
    regVal = (GPIO_ReadValue(PS2_DATA_PORT) >> PS2_DATA_PIN) & 0x01;
    ps2.parity_value = regVal ? (++ps2.parity_value):(ps2.parity_value);
    // In PS2 0 Level Means Logic 1 and 5V Level means Logic 0
    /* Following If Else Logic can be simpilfied. I think*/
    if( regVal )
    {
      SET_BIT(ps2.scan_code,ps2.bit_pos);
    }
    else
    {
      CLR_BIT(ps2.scan_code,ps2.bit_pos);
    }
    ps2.bit_pos++;
    if( ps2.bit_pos >= 8 )
    {
      ps2.bit_pos = 0;
      PS2_State++;
    }
    break;
  case PS2_PARITY:
    regVal = (GPIO_ReadValue(PS2_DATA_PORT) >> PS2_DATA_PIN) & 0x01;
    if( regVal != (ps2.parity_value%2) )
    {
      PS2_State++;
    }
    else
    {
      PS2_State = PS2_START;
    }
    break;
  case PS2_STOP:
    regVal = (GPIO_ReadValue(PS2_DATA_PORT) >> PS2_DATA_PIN) & 0x01;
    if( regVal )
    {
      if (ps2.last_scan_code != ps2.scan_code 
          || ps2.penultimate_scan_code != ps2.scan_code )
      {
        ps2.penultimate_scan_code = ps2.last_scan_code;
        ps2.last_scan_code = ps2.scan_code;
        Insert_In_Queue(ps2.scan_code);
      }
    }
    PS2_State = PS2_START;
    ps2.PS2_Busy = FALSE;
    break;
  }
}

/**
 * @brief Queue is Empty or Not.
 *
 * The function returns the PS2 Queue Status, FALSE if queue is not empty and 
 * TRUE if empty.
 * @return TRUE is PS2 Data Queue is Empty, otherwise FALSE.
 */
static boolean IS_Queue_Empty( void )
{
  boolean is_empty = FALSE;
  if( (s_queue.front == 0) && (s_queue.rear == -1) )
  {
    // Queue is Empty
    is_empty = TRUE;
  }
  return is_empty;
}

/**
 * @brief Insert In Queue.
 *
 * The function will insert data into queue.
 * @param scan_code Data to insert into Queue.
 * @return TRUE if insertion is successfull otherwise FALSE.
 */
static boolean Insert_In_Queue( u8_t scan_code )
{
  boolean inserted = FALSE;
  if ((s_queue.front == 0 && s_queue.rear == SCAN_CODE_MAX-1) ||
      (s_queue.front > 0 && s_queue.rear == s_queue.front-1))
  {
    // Queue is Full
    inserted = FALSE;
  }
  else
  {
    if (s_queue.rear == SCAN_CODE_MAX-1 && s_queue.front > 0)
    {
      s_queue.rear = 0;
      s_queue.scan_codes_buffer[s_queue.rear] = scan_code;
    }
    else
    {
      if ((s_queue.front == 0 && s_queue.rear == -1) 
          || (s_queue.rear != s_queue.front-1) )
      {
        s_queue.rear++;
        s_queue.scan_codes_buffer[s_queue.rear] = scan_code;
      }
      inserted = TRUE;
    }
  }
  return inserted;
}

/**
 * @brief Delete From Queue.
 *
 * The function will remove data from queue based on FIFO principle.
 * @return Data removed from Queue.
 */
static u8_t Delete_From_Queue ( void )
{
  u8_t data = 0;
  __disable_interrupt();
  if( (s_queue.front == 0) && (s_queue.rear == -1) )
  {
    // Queue is Empty
    data = 0;
  }
  if ( s_queue.front == s_queue.rear )
  {
    data = s_queue.scan_codes_buffer[s_queue.front];
    s_queue.scan_codes_buffer[s_queue.front] = 0;
    // Deleting Last Element from Queue and Reseting the Queue Parameters
    s_queue.rear = -1;
    s_queue.front = 0;
  }
  else if( s_queue.front == SCAN_CODE_MAX-1 )
  {
    data = s_queue.scan_codes_buffer[s_queue.front];
    s_queue.scan_codes_buffer[s_queue.front] = 0x00;
    s_queue.front = 0;
  }
  else 
  {
    data = s_queue.scan_codes_buffer[s_queue.front];
    s_queue.scan_codes_buffer[s_queue.front] = 0x00;    // Empty this location
    s_queue.front++;
  }
  __enable_interrupt();
  return data;
}

/**
 * @brief Get From Queue.
 *
 * The function will return data from queue based on FIFO principle, without 
 * deleting it.
 * @return Front Data in Queue.
 */
static u8_t Get_From_Queue ( void )
{
  u8_t data = 0;
  if( (s_queue.front == 0) && (s_queue.rear == -1) )
  {
    // Queue is Empty
    data = 0;
  }
  else
  {
    data = s_queue.scan_codes_buffer[s_queue.front];
  }
  return data;
}

/**
 * @brief Decode PS2 Scan Codes.
 *
 * The function will find the correct key entry based on the scan codes 
 * received from the PS2 Keyboard.
 * It handles the Shift Key and Caps Key Press.
 * @return ASCII Value of Key Pressed from PS2 Keyboard.
 * @note On Pressing Caps/Num/Scroll Keys, Keyboard LED will not glow.
 */
static u8_t Decode_PS2_Key( void )
{
  u8_t key_value = 0;
  u8_t key_scan_code = 0u;
  key_scan_code = Delete_From_Queue();
  switch(key_scan_code)
  {
  case 0xF0:
    // Discard Next Data, as this is already taken care
    key_scan_code = Delete_From_Queue();
    if( key_scan_code == L_SHFT || key_scan_code == R_SHFT )
    {
      if( ps2.ShiftKey )
        ps2.ShiftKey = FALSE;
    }
    break;
  default:
    if( key_scan_code == L_SHFT || key_scan_code == R_SHFT )
    {
      ps2.ShiftKey = TRUE;
      break;
    }
    if( key_scan_code == CAPS )
    {
      if( ps2.CapsLock )
        ps2.CapsLock = FALSE;
      else
        ps2.CapsLock = TRUE;
      break;
    }
    if( ps2.CapsLock )
      key_value = ps2.ShiftKey ? PS2_KeyCodes[key_scan_code]
        :PS2_ShiftKeyCodes[key_scan_code];
    else
      key_value = ps2.ShiftKey ? PS2_ShiftKeyCodes[key_scan_code]
        :PS2_KeyCodes[key_scan_code];
    break;
  }
  return key_value;
}

/**
 * @brief Get Pressed Key.
 *
 * This is a Public functions which returns the pressed Key ASCII Value
 * @return ASCII Value of Key Pressed from PS2 Keyboard.
 * @note On Pressing Caps/Num/Scroll Keys, Keyboard LED will not glow.
 */
u8_t getKey( void )
{
  u8_t key;
  key = Decode_PS2_Key();
  return key;
}
