#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t counter = 0;
int timerID = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq)
{

  if (timer < 0 || timer > 2) 
    return 1;

  uint8_t MSB;
  uint8_t LSB;

  uint32_t value = TIMER_FREQ / freq;

  util_get_MSB(value, &MSB);
  util_get_LSB(value, &LSB);

  uint8_t byte1 = 0;
  timer_get_conf(timer, &byte1);

  switch (timer)
  {
  case 0:
    byte1 = (byte1 & 0x0f) | TIMER_SEL0 | TIMER_LSB_MSB;
    sys_outb(TIMER_CTRL, byte1);
    sys_outb(TIMER_0, LSB);
    sys_outb(TIMER_0, MSB);

  case 1:
    byte1 = (byte1 & 0x0f) | TIMER_1 | TIMER_LSB_MSB;
    sys_outb(TIMER_CTRL, byte1);
    sys_outb(TIMER_1, LSB);
    sys_outb(TIMER_1, MSB);

  case 2:
    byte1 = (byte1 & 0x0f) | TIMER_2 | TIMER_LSB_MSB;
    sys_outb(TIMER_CTRL, byte1);
    sys_outb(TIMER_2, LSB);
    sys_outb(TIMER_2, MSB);
  }

  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no)
{
  (*bit_no) = BIT(timerID);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timerID) != 0)
  {
    printf("Error unsubscribing interrupt.");
    return 1;
  }

  return 0;
}

int(timer_unsubscribe_int)()
{

  if (sys_irqrmpolicy(&timerID) != 0)
  {
    printf("Error enabling interrupt.");
    return 1;
  }

  return 0;
}

void(timer_int_handler)()
{
  counter++;
  return;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st)
{

  if (timer < 0 || timer > 2)
  {
    printf("Please insert a valid timer");
    return 1;
  }

  unsigned long rbc = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
  unsigned int rbc_state;

  if (sys_outb(TIMER_CTRL, rbc) != 0)
  {
    printf("Error in sys_outb.");
    return 1;
  }

  if (sys_inb(TIMER_0 + timer, &rbc_state))
  {
    printf("Error in sys_inb.");
    return 1;
  }

  *st = (uint8_t)rbc_state;

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field)
{

  union timer_status_field_val field_union;

  if (field == all)
  {
    field_union.byte = st;
  }
  else if (field == initial)
  {
    field_union.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
  }
  else if (field == mode)
  {
    field_union.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;

    if (field_union.count_mode > 5)
    {
      field_union.count_mode = field_union.count_mode & (BIT(0) | BIT(1));
    }
  }
  else if (field == base)
  {
    field_union.bcd = st & BIT(0);
  }
  else
  {
      printf("Not a valid field."); //Adresses invalid fields
      return 1;
  }

  int error_message = timer_print_config(timer, field, field_union);

  if (error_message != 0)
  {
    printf("Error in timer_print_config.");
    return error_message;
  }

  return 0;
}
