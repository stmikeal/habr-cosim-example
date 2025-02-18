#define UART0_BASE 0x10000000
#define REG(base, offset) ((*((volatile unsigned char *)(base + offset))))
#define UART0_DR    REG(UART0_BASE, 0x00)
#define UART0_FCR   REG(UART0_BASE, 0x02)
#define UART0_LSR   REG(UART0_BASE, 0x05)
																						
#define UARTFCR_FFENA 0x01                // UART FIFO Control Register enable bit
#define UARTLSR_THRE 0x20                 // UART Line Status Register Transmit Hold Register Empty bit
#define UART0_FF_THR_EMPTY (UART0_LSR & UARTLSR_THRE)

void uart_putc(char c) {
	while (!UART0_FF_THR_EMPTY);            // Wait until the FIFO holding register is empty
	UART0_DR = c;                           // Write character to transmitter register
}

void uart_puts(const char *str) {
	while (*str) {                          // Loop until value at string pointer is zero
		uart_putc(*str++);                    // Write the character and increment pointer
	}
}

void write_mem_32(int addr, int value) {
	volatile int *const __ptr = (volatile int *)(addr);
	*__ptr = value;
};

int read_mem_32(const int addr) {
	const volatile int *const __ptr = (const volatile int *)(addr);
	const int __value = *__ptr;
	return __value;
};


void main(){
    UART0_FCR = UARTFCR_FFENA;
    uart_puts("main start\n");
    const char *str = "Hello World!";
    while(*str) {
		uart_puts("Send char: "); uart_putc(*str); uart_putc('\n');
		write_mem_32(0x28000000, *str++);
    }
	write_mem_32(0x28000004, 1);
    while (1);
}
