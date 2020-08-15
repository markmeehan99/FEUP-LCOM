// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/timer.h>
// Any header files included below this line should have been created by you
#include "video_gr.h"
#include "keyboard.h"
#include "game.h"
#include "bitmap.h"
#include "chicken.h"
#include <time.h>
#include <stdlib.h>

/** @defgroup PROJ PROJ
 * @{
 * Funcoes relacionadas com o projeto
 */
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/**
 * @brief Calls game function.
 */
int (proj_main_loop)(int UNUSED(argc), char *UNUSED(argv[])) {
  srand(time(NULL));
  game();

  return 1;
}

/**@}*/
