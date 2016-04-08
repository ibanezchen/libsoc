#ifndef UART0721
#define UART0721

typedef struct {
	unsigned base, irq;
} uart_t;

void uart_init(uart_t * o, unsigned base, unsigned irq);

void uart_baud(uart_t * o, unsigned clk, unsigned baud);

void uart_put(uart_t * o, char c);

int uart_get(uart_t * o);

static inline int uart_w(uart_t * o, const char *buf, int n)
{
	while (n--) {
		char c = *buf++;
		uart_put(o, c);
	}
	return 0;
}

#endif
