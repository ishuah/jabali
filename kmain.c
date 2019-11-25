#include "framebuffer_driver.c"
#include "serial.c"

int kmain() {
	char header[] = "Jabali";
	char byline[] = "Be brave enough to suck at something new.";
	fb_write(header, sizeof(header));
	fb_write(byline, sizeof(byline));
	serial_write(1, byline, sizeof(byline));
	return 0;
}