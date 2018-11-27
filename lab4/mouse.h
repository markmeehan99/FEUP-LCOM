#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "i8042.h"

extern struct packet processed_packet;

int mouse_subscribe_int(int *bit_no);
int mouse_unsubscribe_int();
void (mouse_ih)();
void process_packet();
int write_cmd(unsigned long cmd);
void process_remote();

#endif
