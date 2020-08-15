#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_
#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"

#ifndef LAB5
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

/** @defgroup Keyboard Keyboard
 * @{
 * Functions for manipulating keyboard
 */

int sys_inb_cnt(port_t port, uint32_t *byte);

int kbd_sys_inb(port_t port, uint8_t *byte);

/**
 * @brief Subscribes and enables Timer interrupts.
 * 
 * @return 0 upon success and non-zero otherwise.
 */
int keyboard_subscribe_int();

/**
 * @brief Unsubscribes Timer interrupts.
 * 
 * @return 0 upon success and non-zero otherwise 
 */
int keyboard_unsubscribe_int();

/**
 * @brief Keyboard interrupt handler for polling.
 * 
 * @return 0 upon success and non-zero otherwise 
 */
int (kbc_ih_poll)();

/**
 * @brief Keyboard interrupt handler for interrupts.
 * 
 * @return 0 upon success and non-zero otherwise 
 */
void (kbc_ih)();

/**@}*/

#endif /*_LCOM_KEYBOARD_H_*/
