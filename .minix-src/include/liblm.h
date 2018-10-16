#ifndef LIBLM_H_
#define LIBLM_H_

#include <stdbool.h>

#include <minix/syslib.h>

#define PB2BASE(x) (((x) >> 4) & 0x0F000) // phys_bytes to segment base
#define PB2OFF(x) ((x) & (0x0FFFF))       // phys_bytes to segment offset

/*
 * Struct to keep physical/virtual address mapping
 */
typedef struct {
  phys_bytes phys; // physical address
  void *virtual;   // virtual address
  size_t size;     // size of mapped memory region
} mmap_t;

void *lm_init(bool enable_logging);

void *lm_alloc(size_t size, mmap_t *map);

void lm_free(mmap_t *map);

#endif // LIBLM_H_
