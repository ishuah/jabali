#include "framebuffer_driver.c"
#include "serial.c"

char header[] = "Jabali";
char byline[] = "Be brave enough to suck at something new.";

int kmain() {
	serial_configure(DEBUG_SERIAL, 2);
	fb_write(header, sizeof(header));
	fb_write(byline, sizeof(byline));
	serial_write(DEBUG_SERIAL, byline, sizeof(byline));
	return 0;
}