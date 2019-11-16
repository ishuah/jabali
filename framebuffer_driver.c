#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define FB_BLACK                0
#define FB_WHITE                15

/** fb_write_cell:
 *  Writes a char array with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c[], unsigned char fg, unsigned char bg)
{
    char key = c[i];
    if (key != 0) {
        char *fb = (char *) 0x000B8000;
        fb[i * 2] = key;
        fb[i * 2 + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
        i++;
        fb_write_cell(i, c, fg, bg); 
    }
}