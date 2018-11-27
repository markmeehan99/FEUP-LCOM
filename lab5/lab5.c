#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include "keyboard.h"
#include "video_gr.h"


int sys_count = 0;
int kbd_id = 1;
uint8_t scancode;

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay)
{
  

  if (vg_init(mode) == NULL)
  {
    printf("Error in vg_init");
    return -1;
  }

  //tickdelay(micros_to_ticks(delay * 1000000));
  sleep(delay);

  if (vg_exit() != 0)
  {
    printf("Error in vg_init");
    return 1;
  }

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
  message msg;
  int ipc_status;
  uint16_t r, irq_set;
  int bit_no = 0;
  bool two_bytes = false;
  uint8_t scan_bytes[2];

  if (keyboard_subscribe_int(&bit_no) != OK)
  {
    return 1;
  }

  if (vg_init(mode) == NULL)
  {
    printf("Error in vg_init");
    return 1;
  }

  vg_draw_rectangle(x,y,width,height,color);

  irq_set = bit_no;

  while (scancode != ESC_BREAK)
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:

        if (msg.m_notify.interrupts & irq_set)
        {
          kbc_ih();

          if (scancode == TWO_BYTE_CODE)
          {
            two_bytes = true;
            scan_bytes[0] = scancode;
          }
          else if (two_bytes)
          {
            two_bytes = false;
            scan_bytes[1] = scancode;
            //scan code complete
          }
          else
          {
            scan_bytes[0] = scancode;
            //scan code complete
          }
        }

        break;
      default:
        break;
      }
    }
  }

  if (keyboard_unsubscribe_int() != OK)
  {
    return 1;
  }

  if (vg_exit() != OK)
  {
    printf("Error in vg_init");
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step)
{
  message msg;
  int ipc_status;
  uint16_t r, irq_set;
  int bit_no = 0;
  bool two_bytes = false;
  uint8_t scan_bytes[2];

  if (keyboard_subscribe_int(&bit_no) != OK)
  {
    return 1;
  }

  if (vg_init(mode) == NULL)
  {
    printf("Error in vg_init");
    return 1;
  }
  if (vg_draw_pattern( no_rectangles, first, step)!= OK){
    return 1;
  }

  irq_set = bit_no;

  while (scancode != ESC_BREAK)
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:

        if (msg.m_notify.interrupts & irq_set)
        {
          kbc_ih();

          if (scancode == TWO_BYTE_CODE)
          {
            two_bytes = true;
            scan_bytes[0] = scancode;
          }
          else if (two_bytes)
          {
            two_bytes = false;
            scan_bytes[1] = scancode;
            //scan code complete
          }
          else
          {
            scan_bytes[0] = scancode;
            //scan code complete
          }
        }

        break;
      default:
        break;
      }
    }
  }

  if (keyboard_unsubscribe_int() != OK)
  {
    return 1;
  }

  if (vg_exit() != OK)
  {
    printf("Error in vg_init");
    return 1;
  }

  return 0;



  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__, mode, no_rectangles, first, step);

  return 1;
}


int(video_test_xpm)(const char *xpm[], uint16_t x, uint16_t y)
{

  vg_init(0x105);
  vg_display_xpm(xpm,x,y);

  message msg;
  int ipc_status;
  uint16_t r, irq_set;
  int bit_no = 0;
  bool two_bytes = false;
  uint8_t scan_bytes[2];

  
  if (keyboard_subscribe_int(&bit_no) != OK)
  {
    return 1;
  }

  irq_set = bit_no;

  while (scancode != ESC_BREAK)
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:

        if (msg.m_notify.interrupts & irq_set)
        {
          kbc_ih();

          if (scancode == TWO_BYTE_CODE)
          {
            two_bytes = true;
            scan_bytes[0] = scancode;
          }
          else if (two_bytes)
          {
            two_bytes = false;
            scan_bytes[1] = scancode;
            //scan code complete
          }
          else
          {
            scan_bytes[0] = scancode;
            //scan code complete
          }
        }

        break;
      default:
        break;
      }
    }
  }

  vg_exit();

  if (keyboard_unsubscribe_int() != OK)
  {
    return 1;
  }

  return 0;
}	

