#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define FB_BLACK                0
#define FB_WHITE                15

unsigned short fb_cursor_pos = 0x00000000;
char *fb = (char *) 0x000B8000;

/** fb_write_cell:
 *  Writes a char with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param key  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char key, unsigned char fg, unsigned char bg)
{
    fb[i * 2] = key;
    fb[i * 2 + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

 /** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);

    fb_cursor_pos = pos;
}

void fb_write(char *buf, unsigned int len)
{
    unsigned int i;
    for(i=0;i<len;i++){
        fb_write_cell(fb_cursor_pos, buf[i], FB_BLACK, FB_WHITE);
        fb_move_cursor(fb_cursor_pos + 1);
    }
}