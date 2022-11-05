/**
 * @file ps2_keyboard.h
 * @author Embedded Laboratory
 * @date June 10, 2016
 * @brief PS2 Keyvoard Header File, contains macros, function prototype related
 * to project.
 */

#ifndef PS2_KEYBOARD_H
#define	PS2_KEYBOARD_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Port and Pin Configuration */
#define PS2_CLK_PORT    3     /**< PS2 Clock PORT. */
#define PS2_CLK_PIN     3     /**< PS2 Clock Pin. */

#define PS2_DATA_PORT   3     /**< PS2 Data PORT. */
#define PS2_DATA_PIN    2     /**< PS2 Data Pin. */
  
/* Special Function Character */
#define TAB             0x09  /**< TAB Scan Code. */
#define BKSP            0x08  /**< Backspace Scan Code. */
#define ENTER           0x0D  /**< Enter Scan Code. */
#define ESC             0x1B  /**< Escape Scan Code. */
#define L_SHFT          0x12  /**< Left Shift Scan Code. */
#define R_SHFT          0x59  /**< Right Shift Scan Code. */
#define CAPS            0x58  /**< Caps Lock Scan Code. */
#define L_CTRL          0x0   /**< Left Ctrl Scan Code. */
#define NUM             0x0   /**< F0 Scan Code. */
#define F1              0x0   /**< F1 Scan Code. */
#define F2              0x0   /**< F2 Scan Code. */
#define F3              0x0   /**< F3 Scan Code. */
#define F4              0x0   /**< F4 Scan Code. */
#define F5              0x0   /**< F5 Scan Code. */
#define F6              0x0   /**< F6 Scan Code. */
#define F7              0x0   /**< F7 Scan Code. */
#define F8              0x0   /**< F8 Scan Code. */
#define F9              0x0   /**< F9 Scan Code. */
#define F10             0x0   /**< F10 Scan Code. */
#define F11             0x0   /**< F11 Scan Code. */
#define F12             0x0   /**< F12 Scan Code. */

#define SCAN_CODE_MAX   20u   /**< Scan Codes Buffer Size. */

/**
 * @brief PS2 Keyboard States
 *
 * Different States of PS2 Keyboard protocol used by PS2 State Machine.
 */
typedef enum _PS2_State_e
{
  PS2_START = 0,      /**< Default Start State. */
  PS2_DATA,           /**< PS2 Data State. */
  PS2_PARITY,         /**< Parity Bit State. */
  PS2_STOP            /**< Stop/End Data Complete State. */
} PS2_State_e;

/**
 * @brief Queue/FIFO
 *
 * Circular FIFO to store and retrieve data of PS2 Keyboard.
 */
typedef struct _Queue_s
{
  s8_t front;                             /**< Front Pointer of Queue. */
  s8_t rear;                              /**< Rear Pointer of Queue. */
  u8_t scan_codes_buffer[SCAN_CODE_MAX];  /**< Queue/FIFO Buffer. */
} Queue_s;

/**
 * @brief PS2 Keyboard Structure
 *
 * Handle PS2 Keyboard Data.
 */
typedef struct _PS2_Keyboard_s
{
  u8_t bit_pos;               /**< Bit Position of Data. */
  u8_t scan_code;             /**< Current Scan Code Received. */
  u8_t last_scan_code;        /**< Last Scan Code Received. */
  u8_t penultimate_scan_code; /**< Second Last Scan Code Received. */
  u8_t parity_value;          /**< Parity Bit Calculated. */
  boolean PS2_Busy:1,         /**< PS2 Bus State. */
          CapsLock:1,         /**< CAPS Lock Key State. */
          ShiftKey:1;         /**< Shift Key State. */
} PS2_Keyboard_s;

// Function Prototypes
void PS2_Keyboard_Init( void );
void PS2_State_Machine( void );
boolean IS_PS2_Busy( void );
u8_t getKey( void );

#ifdef	__cplusplus
}
#endif

#endif	/* PS2_KEYBOARD_H */
