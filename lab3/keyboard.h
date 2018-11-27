#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_
#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

int sys_inb_cnt(port_t port, uint32_t *byte);

int kbd_sys_inb(port_t port, uint8_t *byte);

int keyboard_subscribe_int();

int keyboard_unsubscribe_int();

int (kbc_ih_poll)();

void (kbc_ih)();

#endif /*_LCOM_KEYBOARD_H_*/
