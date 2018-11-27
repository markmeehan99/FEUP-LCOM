// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "timer.h"
#include "mouse.h"
#include "i8042.h"
#include "i8254.h"

uint16_t byte_packet[3];
struct packet processed_packet;
uint32_t byte;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt)
{
  uint8_t counter = 0;
  unsigned long r;
  int mouse_id = 12;
  int ipc_status;
  message msg;
  static uint16_t count_index = 0;

  if (write_cmd(ENABLE) != 0)
  {
    printf("Error enabling mouse\n");
    return 1;
  }

  if (mouse_subscribe_int(&mouse_id) != 0)
  {
    printf("Error subscribing a mouse notification\n");
    return 1;
  }

  uint32_t irq_set = mouse_id;

  while (counter < cnt)
  {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set)
        { /* subscribed interrupt */
          mouse_ih();

          if (count_index == 0 && (byte & CHECK_BYTE1))
          {
            byte_packet[0] = byte;
            count_index++;
          }

          else if (count_index == 1)
          {
            byte_packet[1] = byte;
            count_index++;
          }

          else if (count_index == 2)
          {
            byte_packet[2] = byte;
            process_packet();
            mouse_print_packet(&processed_packet);
            count_index = 0;
            counter++;
          }
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    }
    else
    { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int() != 0)
  {
    printf("The program was unable to unsubscribe a mouse notification\n");
    return 1;
  }

  if (write_cmd(DISABLE) != 0)
  {
    printf("The program failed to disable the mouse data reporting\n");
    return 1;
  }

  return 0;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt)
{
  uint8_t counter = 0;

  while (counter < cnt)
  {
    write_cmd(READ);

    process_remote();
    counter++;

    tickdelay(micros_to_ticks(period * 1000));
  }

  write_cmd(SET_STREAM);
  write_cmd(DISABLE);

  uint8_t cmd_byte = minix_get_dflt_kbc_cmd_byte();

  sys_outb(CMD_REG, WRITE_CMD_BYTE); //Reset minix mouse to default
  sys_outb(BUF_IN, cmd_byte);

  return 0;
}

int(mouse_test_async)(uint8_t idle_time)
{
int irq_set_mouse, ipc_status;
  uint8_t irq_set_timer;
  message msg;
  bool proceed = true;
  static uint16_t count_index = 0;
  unsigned long r;

  if (write_cmd(ENABLE) != OK) {
    return 1;
  }
  
  if (mouse_subscribe_int(&irq_set_mouse) != 0 || timer_subscribe_int(&irq_set_timer) != 0) {
    return 1;
  }

  while (proceed) {
    // Get a request message1
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d \n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { // Received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // Hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            if (counter >= idle_time * 60) {
              proceed = false;
            }
          }

          if (msg.m_notify.interrupts & irq_set_mouse) {
            counter = 0;
            mouse_ih();

            if (count_index == 0 && (byte & CHECK_BYTE1)) {
              byte_packet[0] = byte;
              count_index++;
            }

            else if (count_index == 1) {
              byte_packet[1] = byte;
              count_index++;
            }

            else if (count_index == 2) {
              byte_packet[2] = byte;
              process_packet();
              mouse_print_packet(&processed_packet);
              count_index = 0;
              counter++;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  printf("Time is over!\n");

  if (timer_unsubscribe_int() != 0) {
    printf("Error subscribing timer\n");
    return 1;
  }

  if (mouse_unsubscribe_int() != 0) {
    printf("Error subscribing mouse\n");
    return 1;
  }
  
  if (write_cmd(DISABLE) != OK) {
    return -1;
  }
  
  return 0;
}

//int(mouse_test_gesture)()
//{
/* To be completed */
//printf("%s: under construction\n", __func__);
//return 1;
//}
