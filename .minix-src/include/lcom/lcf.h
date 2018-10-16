#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <machine/int86.h>
#include <minix/const.h>
#include <minix/driver.h>
#include <minix/endpoint.h>
#include <minix/sef.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <minix/type.h>
#include <minix/vm.h>
#include <sys/time.h>
#include <sys/timespec.h>
#include <sys/types.h>

#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"
#include "types.h"

#ifdef __LCOM__

// undef all MINIX functions defined as macros

#  undef driver_receive
#  undef sys_inb
#  undef sys_irqdisable
#  undef sys_irqenable
#  undef sys_irqrmpolicy
#  undef sys_irqsetpolicy
#  undef sys_outb

/*
 * 
 * Macros used to replace the set of functions being wrapped / "intercepted".
 *
 */

// not a wrapper, just a way to solve pseudo-random number generation issue in Minix 3.4.0-rc6
#  define rand() rand_()

//
// Minix / C library functions
//

#  define driver_receive(src, m_ptr, status_ptr) \
    driver_receive_(src, m_ptr, status_ptr, CTX_INFO(#src "\0" #m_ptr "\0" #status_ptr))

#  define micros_to_ticks(micros) \
    micros_to_ticks_(micros, CTX_INFO(#micros))

#  define nanosleep(req, rem) \
    nanosleep_(req, rem, CTX_INFO(#req "\0" #rem))

// printf is a variadic function, therefore it is treated differently:
//   1) format argument is included within variadic argument list to avoid any warnings;
//   2) the code string is set as a single argument;
//   3) context information is the first argument, not the last one.
#  define printf(...) \
    printf_(CTX_INFO(#__VA_ARGS__), __VA_ARGS__)

#  define sleep(seconds) \
    sleep_(seconds, CTX_INFO(#seconds))

// #define sef_startup() \
//   sef_startup_(CTX_INFO(""))

#  define sys_enable_iop(proc_ep) \
    sys_enable_iop_(proc_ep, CTX_INFO(#proc_ep))

#  define sys_inb(port, value) \
    sys_inb_(port, value, CTX_INFO(#port "\0" #value))

#  define sys_int86(reg86p) \
    sys_int86_(reg86p, CTX_INFO(#reg86p))

#  define sys_irqdisable(irq_hook_id) \
    sys_irqdisable_(irq_hook_id, CTX_INFO(#irq_hook_id))

#  define sys_irqenable(irq_hook_id) \
    sys_irqenable_(irq_hook_id, CTX_INFO(#irq_hook_id))

#  define sys_irqrmpolicy(irq_hook_id) \
    sys_irqrmpolicy_(irq_hook_id, CTX_INFO(#irq_hook_id))

#  define sys_irqsetpolicy(irq_vec, policy, hook_id) sys_irqsetpolicy_( \
    irq_vec, policy, hook_id, CTX_INFO(#  irq_vec "\0" #  policy "\0" #  hook_id))

#  define sys_outb(port, value) \
    sys_outb_(port, value, CTX_INFO(#port "\0" #value))

#  define sys_privctl(proc_ep, req, p) \
    sys_privctl_(proc_ep, req, p, CTX_INFO(#proc_ep "\0" #req "\0" #p))

#  define tickdelay(ticks) \
    tickdelay_(ticks, CTX_INFO(#ticks))

#  define usleep(useconds) \
    usleep_(useconds, CTX_INFO(#useconds))

#  define vm_map_phys(who, physaddr, len) \
    vm_map_phys_(who, physaddr, len, CTX_INFO(#who "\0" #physaddr "\0" #len))

#  define vm_unmap_phys(who, vaddr, len) \
    vm_unmap_phys_(who, vaddr, len, CTX_INFO(#who "\0" #vaddr "\0" #len))

//
// LCOM Lab 2 functions
//

#  define timer_test_read_config(timer, field) \
    timer_test_read_config_(timer, field, CTX_INFO(#timer "\0" #field))

#  define timer_test_time_base(timer, freq) \
    timer_test_time_base_(timer, freq, CTX_INFO(#timer "\0" #freq))

#  define timer_test_int(time) \
    timer_test_int_(time, CTX_INFO(#time))

#  define util_get_LSB(val, lsb) \
    util_get_LSB_(val, lsb, CTX_INFO(#val "\0" #lsb))

#  define util_get_MSB(val, msb) \
    util_get_MSB_(val, msb, CTX_INFO(#val "\0" #msb))

#  define timer_set_frequency(timer, freq) \
    timer_set_frequency_(timer, freq, CTX_INFO(#timer "\0" #freq))

#  define timer_subscribe_int(bit_no) \
    timer_subscribe_int_(bit_no, CTX_INFO(#bit_no))

#  define timer_unsubscribe_int() \
    timer_unsubscribe_int_(CTX_INFO(""))

#  define timer_int_handler() \
    timer_int_handler_(CTX_INFO(""))

#  define timer_get_conf(timer, st) \
    timer_get_conf_(timer, st, CTX_INFO(#timer "\0" #st))

#  define timer_display_conf(timer, st, field) \
    timer_display_conf_(timer, st, field, CTX_INFO(#timer "\0" #st "\0" #field))

#  define timer_print_config(timer, field, val) \
    timer_print_config_(timer, field, val, CTX_INFO(#timer "\0" #field "\0" #val))

#  define timer_print_elapsed_time() \
    timer_print_elapsed_time_(CTX_INFO(""))

/*
 *
 * Wrappers entry point.
 * 
 */

// not a wrapper, used as a way to solve pseudo-random number generation in Minix 3.4.0-rc6
int rand_();

//
// Minix / C library functions
//

int driver_receive_(endpoint_t src, message *m_ptr, int *status_ptr, const ctx_t ctx);

u32_t micros_to_ticks_(u32_t micros, const ctx_t ctx);

int nanosleep_(const struct timespec *req, struct timespec *rem, const ctx_t ctx);

int printf_(const ctx_t ctx, const char *__restrict format, ...);

// void sef_startup_(const ctx_t ctx);

unsigned int sleep_(unsigned int seconds, const ctx_t ctx);

int sys_enable_iop_(endpoint_t proc_ep, const ctx_t ctx);

int sys_inb_(int port, u32_t *value, const ctx_t ctx);

#  if defined(__i386__)
int sys_int86_(struct reg86u *reg86p, const ctx_t ctx);
#  endif

int sys_irqdisable_(int *irq_hook_id, const ctx_t ctx);

int sys_irqenable_(int *irq_hook_id, const ctx_t ctx);

int sys_irqrmpolicy_(int *irq_hook_id, const ctx_t ctx);

int sys_irqsetpolicy_(int irq_vec, int policy, int *irq_hook_id, const ctx_t ctx);

int sys_outb_(int port, u32_t value, const ctx_t ctx);

int sys_privctl_(endpoint_t proc_ep, int req, void *p, const ctx_t ctx);

int tickdelay_(clock_t ticks, const ctx_t ctx);

int usleep_(useconds_t useconds, const ctx_t ctx);

void *vm_map_phys_(endpoint_t who, void *physaddr, size_t len, const ctx_t ctx);

int vm_unmap_phys_(endpoint_t who, void *vaddr, size_t len, const ctx_t ctx);

//
// LCOM Lab 2
//

int timer_test_read_config_(uint8_t timer, enum timer_status_field field, const ctx_t ctx);

int timer_test_time_base_(uint8_t timer, uint32_t freq, const ctx_t ctx);

int timer_test_int_(uint8_t time, const ctx_t ctx);

int util_get_LSB_(uint16_t val, uint8_t *lsb, const ctx_t ctx);

int util_get_MSB_(uint16_t val, uint8_t *msb, const ctx_t ctx);

int timer_set_frequency_(uint8_t timer, uint32_t freq, const ctx_t ctx);

int timer_subscribe_int_(uint8_t *bit_no, const ctx_t ctx);

int timer_unsubscribe_int_(const ctx_t ctx);

void timer_int_handler_(const ctx_t ctx);

int timer_get_conf_(uint8_t timer, uint8_t *st, const ctx_t ctx);

int timer_display_conf_(uint8_t timer, uint8_t st, enum timer_status_field field, const ctx_t ctx);

int timer_print_config_(uint8_t timer, enum timer_status_field field,
                        union timer_status_field_val val, const ctx_t ctx);

uint32_t timer_print_elapsed_time_(const ctx_t ctx);

//
// LCF library functions
//

#define lcf_start(argc, argv) \
  lcf_start_(argc, argv, CTX_INFO(#argc "\0" #argv))

#define lcf_cleanup() \
  lcf_cleanup_(CTX_INFO(""))

#define lcf_log_output(output) \
  lcf_log_output_(output, CTX_INFO(#output))

#define lcf_trace_calls(syslog) \
  lcf_trace_calls_(syslog, CTX_INFO(#syslog))

#define lcf_set_language(lang) \
  lcf_set_language_(lang, CTX_INFO(#lang))

// initialize LCF library by passing the command line arguments
// [sef_startup() is invoked internally]
int lcf_start_(int argc, char *argv[], const ctx_t ctx);

// LCF library clean up operations (invoked immediately before exiting)
int lcf_cleanup_(const ctx_t ctx);

// if set, creates a file to save all the output
bool lcf_log_output_(const char *output, const ctx_t ctx);

// if set, creates a file to save the invocations of all functions being wrapped / "intercepted"
bool lcf_trace_calls_(const char *syslog, const ctx_t ctx);

// enables to set the language for LCF messages
bool lcf_set_language_(const char *lang, const ctx_t ctx);

#endif // __LCOM__
