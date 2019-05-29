#include "framebuffer_driver.c"

int kmain() {
	char s[] = "Welcome to Jabali!";
	fb_write_cell(0, s, FB_BLACK, FB_WHITE);
	return 0;
}