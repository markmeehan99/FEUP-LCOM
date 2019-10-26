#include "keyboard.h"

extern uint8_t scancode;
extern int sys_count;
uint8_t byte1;
extern int kbd_id;

int sys_inb_cnt(port_t port, uint32_t *byte) {
  sys_count++;
  return sys_inb(port, byte);
}

int kbd_sys_inb(port_t port, uint8_t *byte) { //Calls sysinb with a 8 byte variable
  uint32_t tmp;

  if (sys_inb_cnt(port, &tmp) != OK) {
    return -1;
  }

  (*byte) = (uint8_t) tmp;

  return 0;
}

int keyboard_subscribe_int(int *bit_no) {
  (*bit_no) |= BIT(kbd_id);

  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_id) != 0) {
    printf("Failed to subscribe the KBC interrupts.\n");
    return 1;
  }

  return 0;
}

int keyboard_unsubscribe_int() {
  if (sys_irqrmpolicy(&kbd_id) != 0) {
    printf("Failed to unsubscribe the interrupt of the associated hook.\n");
    return 1;
  }
  printf("Copiem menos\n");
  printf("Praxe por github vale? :(\n");
  return 0;
}

void(kbc_ih)() {

  uint32_t status_r;

  if (sys_inb_cnt(STAT_REG, &status_r) != OK) { // Gets status
    printf("Unable to read the keyboard status\n");
    return;
  }

  if (status_r & OB_FULL) { // Checks if Out_buffer is full. It needs to be in order to read data from it
    if (kbd_sys_inb(BUF_OUT, &scancode) != OK) { // Retrieve data from output buffer
      printf("Unable to read the scancode\n");
      return;
    }

    if ((status_r & (PRT_ERROR | TIMOUT_ERROR)) == OK) {
      return;
    }
    else {
      printf("IH - Parity or Timeout Error");
      return;
    }
  }

  return;
}

int(kbc_ih_poll)() {

  uint32_t status_r;

  if (sys_inb_cnt(STAT_REG, &status_r) != OK) { // Gets status
    printf("Unable to read the keyboard status\n");
    return 1;
  }

  if (status_r & OB_FULL) { // Checks if Out_buffer is full. It needs to be in order to read data from it
    if (kbd_sys_inb(BUF_OUT, &scancode) != OK) { // Retrieves data from output buffer
      printf("Unable to read the scancode\n");
      return 1;
    }

    if ((status_r & (PRT_ERROR | TIMOUT_ERROR | BIT(5))) == OK) {
      return 0;
    }
    else {
      printf("IH - Parity or Timeout Error");
      return 1;
    }
  }

  return 1;
}
