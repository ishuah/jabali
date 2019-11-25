 #include "io.h" /* io.h is implement in the section "Moving the cursor" */

/* The I/O ports */

/* All the I/O ports are calculated relative to the data port. This is because
 * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
 * order, but they start at different values.
 */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

#define DEBUG_SERIAL SERIAL_COM1_BASE

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/** serial_configure:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *  It also configures the line of the given serial port and the modem.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure(unsigned short com, unsigned short divisor) {
	outb(SERIAL_DATA_PORT(com) + 1, 0x00);				// Disable all interrupts
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x80);			// Enable DLAB (set baud rate divisor)
	outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);			// Set divisor to (lo byte)
	outb(SERIAL_DATA_PORT(com) + 1, (divisor >> 8) & 0x00FF);	//                (hi byte)
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);			// 8 bits, no parity, one stop bit
	outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);			// Enable FIFO, clear them, with 14-byte threshold
	outb(SERIAL_MODEM_COMMAND_PORT(com), 0x0B);			// IRQs enabled, RTS/DSR set
}

int serial_is_transmit_fifo_empty(unsigned short com) {
	/* 0x20 = 0010 0000 */
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

int serial_received(unsigned short com) {
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 1;
}

int serial_write(unsigned short com, char *buf, unsigned int len) {
	unsigned int i;
	for(i=0; i<len; i++) {
		while(serial_is_transmit_fifo_empty(com) == 0);
		outb(com, buf[i]);
	}
	return len;
}
