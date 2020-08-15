#include "mouse.h"
#include <lcom/lcf.h>
#include "i8042.h"
#include <stdint.h>
#include <stdio.h>

extern uint16_t byte_packet[3];
extern uint32_t byte;
extern struct packet processed_packet;
int hookID = 12;

int mouse_subscribe_int(int *bit_no)
{
  (*bit_no) = BIT(hookID);

  if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) != 0)
  {
    printf("Failed to subscribe the KBC interrupts.\n");
    return 1;
  }

  return 0;
}

int mouse_unsubscribe_int()
{
  if (sys_irqrmpolicy(&hookID) != 0)
  {
    printf("Failed to unsubscribe the interrupt of the associated hook.\n");
    return 1;
  }
  return 0;
}

void(mouse_ih)(void)
{
  uint32_t status;

  if (sys_inb(STAT_REG, &status) != OK)
    return; // verify the status of the buffer

  if (status & OB_FULL)
  {
    if ((status & (TIMOUT_ERROR | PRT_ERROR)) == 0)
    {
      if (sys_inb(BUF_OUT, &byte) != OK) //Reads the output buffer
      {
        printf("Error reading output buffer");
        return;
      }
    }
  }
}

void process_remote()
{
  uint32_t status;

  if (sys_inb(STAT_REG, &status) != OK)
    return; // verify the status of the buffer

  if (status & OB_FULL)
  {
    if ((status & (BIT(7) | BIT(6))) == 0)
    {
      for (int i = 0; i < 3; i++)
      {
        if (sys_inb(BUF_OUT, &byte) != OK) //Reads the output buffer
        {
          printf("Error reading output buffer");
          return;
        }

        if (i == 0)
          if (byte & CHECK_BYTE1)
            byte_packet[0] = byte;

        if (i == 1)
          byte_packet[1] = byte;

        if (i == 2)
          byte_packet[2] = byte;
      }

      process_packet(); //write the data packet
      //mouse_print_packet(&processed_packet);
    }
  }
}

void process_packet()
{
  //Sorts LB
  if (byte_packet[0] & LB)
    processed_packet.lb = true;
  else
    processed_packet.lb = false;

  //Sorts RB
  if (byte_packet[0] & RB)
    processed_packet.rb = true;
  else
    processed_packet.rb = false;

  //Sorts MB
  if (byte_packet[0] & MB)
    processed_packet.mb = true;
  else
    processed_packet.mb = false;

  //Sorts x_ov
  if (byte_packet[0] & X_OV)
    processed_packet.x_ov = true;
  else
    processed_packet.x_ov = false;

  //Sorts y_ov
  if (byte_packet[0] & Y_OV)
    processed_packet.y_ov = true;
  else
    processed_packet.y_ov = false;

  // If the x value is negative
  if (byte_packet[0] & X_NEG)
    processed_packet.delta_x = byte_packet[1] - 256;
  else
    processed_packet.delta_x = byte_packet[1];

  // If the x value is negative
  if (byte_packet[0] & Y_NEG)
    processed_packet.delta_y = byte_packet[2] - 256;
  else
    processed_packet.delta_y = byte_packet[2];

  //Sets byte array
  //processed_packet.bytes
  processed_packet.bytes[0] = byte_packet[0];
  processed_packet.bytes[1] = byte_packet[1];
  processed_packet.bytes[2] = byte_packet[2];
}

int write_cmd(unsigned long cmd)
{
  uint32_t status, byt;

  do
  {
    if (sys_inb(STAT_REG, &status) != OK)
      return -1;

    if ((status & IBF_FULL) == 0)
    {

      if (sys_outb(CMD_REG, CMD_INIT) != OK)
        return -1;
    }
    else
      continue;

    if (sys_inb(STAT_REG, &status) != OK)
      return -1;

    if ((status & IBF_FULL) == 0)
    {
      if (sys_outb(BUF_OUT, cmd) != OK)
        return -1;
    }
    else
      continue;

    tickdelay(micros_to_ticks(DELAY_US));

    if (sys_inb(BUF_OUT, &byt) != OK)
      return -1;

  } while (byt != ACK);

  return 0;
}

