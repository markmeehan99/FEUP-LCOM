#include "i8042.h"

.global scancode

scancode:
.byte 0


.global kbc_asm_ih

kbc_asm_ih:
  xor %al, %al
  inb $BUF_OUT, %al
  movb %al, scancode
  ret
