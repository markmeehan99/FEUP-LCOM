#include <lcom/lcf.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <lcom/video_gr.h>
#include "video_gr.h"
#include <lcom/lab5.h>
#include <sys/mman.h>
#include <liblm.h>
#include "macros.h"


static unsigned x_res;
static unsigned y_res;
static unsigned bits_per_pixel;
static unsigned int vram_base;
static unsigned int vram_size;
static char *video_mem;
static char *drawing_buf;

void *(vg_init)(uint16_t mode)
{   
    struct reg86u r;

    memset(&r, 0, sizeof(r));

    r.u.b.intno = 0x10;
    r.u.w.ax = 0x4F02;         // VBE call, function 02 -- set VBE mode
    r.u.w.bx = 1 << 14 | mode; // set bit 14: linear framebuffer

    vbe_mode_info_t vbe;
    lm_init(true);
    vbe_get_mode_info(mode, &vbe);

    x_res = vbe.XResolution;
    y_res = vbe.YResolution;
    bits_per_pixel = vbe.BitsPerPixel;

    struct minix_mem_range mr; /* physical memory range */
    vram_base = (unsigned int)vbe.PhysBasePtr;
    vram_size = x_res * y_res * (bits_per_pixel / 8);


    /* Allow memory mapping */
    mr.mr_base = vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    int r2;

    if (OK != (r2 = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r2);

    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    drawing_buf = (char*) malloc(x_res*y_res*bits_per_pixel/8);
    if (video_mem == MAP_FAILED)
        panic("couldn’t map video memory");
        

    if (sys_int86(&r) != OK)
    {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }

    return video_mem;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{

    if (x < 0 || y < 0 || x > x_res || y > y_res || x + len > x_res)
    {
        printf("Out of boundaries.\n");
        return -1;
    }

    for (int i = 0; i < len; i++)
    {
        //video_mem[x_res * y + x+i] = color;
        memcpy(video_mem + x_res * y * (bits_per_pixel / 8) + (x + i) * bits_per_pixel / 8, &color, bits_per_pixel / 8);
    }

    return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{

    if (x < 0 || y < 0 || x > x_res || y > y_res || x + width > x_res || y + height > y_res)
    {
        printf("Out of boundaries\n");
        return -1;
    }

    for (int i = 0; i < height; i++)
    {
        vg_draw_hline(x, y + i, width, color);
    }

    return 0;
}

int vg_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step)
{
    uint8_t width = x_res / no_rectangles;
    uint8_t height = y_res / no_rectangles;

    for (int i = 0; i <= no_rectangles; i++)
    {
        for (int j = 0; j <= no_rectangles; j++)
        {
            uint32_t index = (first + (i * no_rectangles + j) * step) % (1 << bits_per_pixel);
            vg_draw_rectangle(0 + (j * width), (i * height), width, height, index);
        }
    }

    return 0;
}

int color_set(uint16_t x, uint16_t y, uint32_t color)
{
    memcpy(video_mem + x_res * y * (bits_per_pixel / 8) + x * bits_per_pixel / 8, &color, bits_per_pixel / 8);
    return 0;
    
}


int (vg_display_xpm)(const char *xpm[],  uint16_t x, uint16_t y)
{
 int alt;
 int wid;
 
    char *pixmap = read_xpm(xpm, &wid, &alt);
    for (int i = 0; i < alt; i++){
        for (int z = 0; z < wid; z++){
            uint32_t color = pixmap[z + i*alt];
            color_set(x+z, y+i, color);
        }
    }
    return 0;
}

int getHorResolution()
{
	return x_res;
}

int getVerResolution()
{
	return y_res;
}

char * getDrawingBuffer()
{
	return drawing_buf;
}

char * getGraphicsBuffer()
{
    return drawing_buf;
}

void copy_draw()
{
    memcpy(video_mem, drawing_buf, x_res*y_res*bits_per_pixel/8);
}
