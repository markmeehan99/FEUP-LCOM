#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <minix/syslib.h>

extern uint32_t counter;

int(timer_set_frequency)(uint8_t timer, uint32_t freq);

int(timer_subscribe_int)(uint8_t *bit_no);

int(timer_unsubscribe_int)(void);

void(timer_int_handler)(void);

int(timer_get_conf)(uint8_t timer, uint8_t *st);

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif /*_LCOM_KEYBOARD_H_*/
