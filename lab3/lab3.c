#include "i8042.h"
#include "keyboard.h"
#include <lcom/lab3.h>
#include <lcom/lcf.h>
#include <lcom/timer.h>

int sys_count = 0;
uint8_t scancode;
int kbd_id = 1;
uint8_t irq_set;
extern uint32_t counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)(bool assembly) {
  message msg;
  int ipc_status;
  uint16_t r;
  uint16_t irq_set;
  int bit_no = 0;
  bool two_bytes = false;
  uint8_t scan_bytes[2];

  if (keyboard_subscribe_int(&bit_no) != OK) {
    return 1;
  }

  irq_set = bit_no;

  while (scancode != ESC_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if (msg.m_notify.interrupts & irq_set) {
            if (assembly) {
              kbc_asm_ih();
            }
            else {
              kbc_ih();
            }

            if (scancode == TWO_BYTE_CODE) {
              two_bytes = true;
              scan_bytes[0] = scancode;
            }
            else if (two_bytes) {
              two_bytes = false;
              scan_bytes[1] = scancode;
              kbd_print_scancode(!(scancode & MAKE_CODE), 2, scan_bytes);
            }
            else {
              scan_bytes[0] = scancode;

              kbd_print_scancode(!(scancode & MAKE_CODE), 1, scan_bytes);
            }
          }

          break;
        default:
          break;
      }
    }
  }

  if (keyboard_unsubscribe_int() != OK)
    return -1;

  kbd_print_no_sysinb(sys_count);

  return 0;
}

int(kbd_test_poll)() {

  scancode = 0;
  bool two_bytes = false;
  uint8_t scan_bytes[2] = {0};

  while (scancode != ESC_BREAK) {

    if (kbc_ih_poll() == OK) {
      if (scancode == TWO_BYTE_CODE) {
        two_bytes = true;
        scan_bytes[0] = scancode;
      }
      else if (two_bytes) {
        two_bytes = false;
        scan_bytes[1] = scancode;
        kbd_print_scancode(!(scancode & MAKE_CODE), 2, scan_bytes);
      }
      else {
        scan_bytes[0] = scancode;
        kbd_print_scancode(!(scancode & MAKE_CODE), 1, scan_bytes);
      }
    }
    else
      continue;

    tickdelay(micros_to_ticks(DELAY_US));
  }
  
  sys_count = 0; //Reset counter

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  scancode = 0;
  message msg;
  uint16_t r, returnValue;
  int ipc_status;

  uint32_t irq_set_keyboard;

  if (keyboard_subscribe_int(&irq_set_keyboard) == 1 || timer_subscribe_int(&irq_set) == 1) {
    return 1;
  }

  while (scancode != ESC_BREAK && counter < (n * 60)) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            timer_int_handler();
          }
          if (msg.m_notify.interrupts & irq_set_keyboard) {
            kbc_ih();
            if ((scancode & BIT(7)) == 0) {
              returnValue = kbd_print_scancode(true, 1, &scancode);
            }
            else {
              returnValue = kbd_print_scancode(false, 1, &scancode);
            }
            if (returnValue != 0)
              return returnValue;
            counter = 0;
          }
          break;
        default:
          break;
      }
    }
  }
  timer_unsubscribe_int();
  keyboard_unsubscribe_int();
 

  return 0;
}
