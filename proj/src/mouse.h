#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "i8042.h"
#include "menu.h"
#include "crosshair.h"

/** @defgroup Mouse Mouse
 * @{
 * Functions for manipulating mouse
 */

extern struct packet processed_packet;

/**
 * @brief Subscribes and enables Mouse interrupts.
 * 
 * @return 0 upon success and non-zero otherwise.
 */
int mouse_subscribe_int(int *bit_no);

/**
 * @brief Unsubscribes Mouse interrupts.
 * 
 * @return 0 upon success and non-zero otherwise 
 */
int mouse_unsubscribe_int();

/**
 * @brief Reads and handles mouse packets.
 */
void (mouse_ih)();

/**
 * @brief Processes mouse packets for further reading.
 */
void process_packet();

/**
 * @brief Writes a giver command to different registers.
 * 
 * @param cmd Command to be written to the registes.
 */
int write_cmd(unsigned long cmd);

/**
 * @brief Processes mouse packets for further reading.
 */
void process_remote();

/**@}*/

#endif
