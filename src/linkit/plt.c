#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include <hcos/sem.h>
#include <lwip/err.h>
#include <lwip/dns.h>

#include "wifi_api.h"
#include "hal_gpio.h"
#include "syslog.h"
#include "io_def.h"
#include "nvdm.h"
#include "nvram_handler.h"
#include "hal_trng.h"
#include "term.h"
#include "mt7637_cm4_hw_memmap.h"
#include "hal_cache.h"
#include "network_init.h"
#include "plt.h"
#include "heap-mem.h"
#include "uart.h"
#include "_soc.h"
#include "plt.h"

void hal_lp_handle_intr();

void netbuf_destport(void *a)
{
}

void netbuf_destaddr(void *a)
{
}

log_create_module(main, PRINT_LEVEL_ERROR);

LOG_CONTROL_BLOCK_DECLARE(main);
LOG_CONTROL_BLOCK_DECLARE(common);
LOG_CONTROL_BLOCK_DECLARE(hal);
LOG_CONTROL_BLOCK_DECLARE(lwip);
LOG_CONTROL_BLOCK_DECLARE(inband);

log_control_block_t *syslog_control_blocks[] = {
	&LOG_CONTROL_BLOCK_SYMBOL(main),
	&LOG_CONTROL_BLOCK_SYMBOL(common),
	&LOG_CONTROL_BLOCK_SYMBOL(hal),
	&LOG_CONTROL_BLOCK_SYMBOL(lwip),
	&LOG_CONTROL_BLOCK_SYMBOL(inband),
	NULL
};

static char syslog_filter_buf[SYSLOG_FILTER_LEN] = { 0 };

void syslog_config_save(const syslog_config_t * config)
{
	syslog_convert_filter_val2str((const log_control_block_t **)
				      config->filters, syslog_filter_buf);
	nvdm_write_data_item("common", "syslog_filters",
			     NVDM_DATA_ITEM_TYPE_STRING,
			     (const unsigned char *)syslog_filter_buf,
			     strlen(syslog_filter_buf));
}

uint32_t syslog_config_load(syslog_config_t * config)
{
	int sz = SYSLOG_FILTER_LEN;

	nvdm_read_data_item("common", "syslog_filters",
			    (unsigned char *)syslog_filter_buf,
			    (uint32_t *) & sz);
	syslog_convert_filter_str2val(config->filters, syslog_filter_buf);
	return 0;
}

const hal_cache_region_config_t region_cfg_tbl[] = {
	{CM4_EXEC_IN_PLACE_BASE, 0x1000000}
};

static int cache_enable(hal_cache_size_t cache_size)
{
	unsigned char region, region_number;

	region_number = sizeof(region_cfg_tbl) / sizeof(region_cfg_tbl[0]);
	if (region_number > HAL_CACHE_REGION_MAX) {
		return -1;
	}
	hal_cache_init();
	hal_cache_set_size(cache_size);
	for (region = 0; region < region_number; region++) {
		hal_cache_region_config(region, &region_cfg_tbl[region]);
		hal_cache_region_enable(region);
	}
	for (; region < HAL_CACHE_REGION_MAX; region++) {
		hal_cache_region_disable(region);
	}
	hal_cache_enable();
	return 0;
}

static int random_init(void)
{
	uint32_t seed;
	hal_trng_status_t s;
	s = hal_trng_init();
	if (s == HAL_TRNG_STATUS_OK)
		s = hal_trng_get_generated_random_number(&seed);
	if (s == HAL_TRNG_STATUS_OK)
		srand((unsigned int)seed);
	if (s != HAL_TRNG_STATUS_OK)
		return -1;
	return 0;
}

static mut_t tcm_mut;

heap_t plt_tcm;

void plt_init(void)
{
	mut_init(&tcm_mut);
	heap_init(&plt_tcm, (unsigned *)BASE_TCM, 0x10000, &tcm_mut);
	hal_lp_handle_intr();

	if (cache_enable(HAL_CACHE_SIZE_32KB) < 0)
		_printf("cache enable failed");

	// Set Pinmux to UART0
	hal_pinmux_set_function(0, 7);
	hal_pinmux_set_function(1, 7);
	hal_pinmux_set_function(2, 7);
	hal_pinmux_set_function(3, 7);

	// UART 1 init
	hal_pinmux_set_function(HAL_GPIO_36, 7);
	hal_pinmux_set_function(HAL_GPIO_37, 7);
	hal_pinmux_set_function(HAL_GPIO_38, 7);
	hal_pinmux_set_function(HAL_GPIO_39, 7);

	log_init(syslog_config_save, syslog_config_load, syslog_control_blocks);
	random_init();
}

static sem_t sem_dhcp, sem_wifi;

static void ip_change(const struct netif *netif)
{
	sem_post(&sem_dhcp);
}

static int32_t wifi_connected(wifi_event_t event, uint8_t * payload,
			      uint32_t length)
{
	sem_post(&sem_wifi);
	return 1;
}

static char *mac_addrs[MAC_MAX + 1];

void net_init(char *mac_addr, ...)
{
	int i;
	char *s;
	va_list vl;
	sem_init(&sem_dhcp, 0);
	sem_init(&sem_wifi, 0);
	if (mac_addr) {
		mac_addrs[0] = mac_addr;
		va_start(vl, mac_addr);
		for (i = 1; i < MAC_MAX && (s = va_arg(vl, char *)); i++)
			mac_addrs[i] = s;
		va_end(vl);
		network_init(mac_addrs);
	} else {
		network_init(0);
	}
	wifi_register_ip_ready_callback(ip_change);
	wifi_connection_register_event_notifier(WIFI_EVENT_IOT_CONNECTED,
						wifi_connected);
}

void network_dhcp_start(unsigned char opmode);

extern struct netif sta_if;

void ip_static(char *_ip, char *_msk, char *_gw, char *_dns)
{
	ip4_addr_t ip, msk, gw, dns;
	sem_get(&sem_wifi, WAIT);
	ip.addr = ipaddr_addr(_ip);
	msk.addr = ipaddr_addr(_msk);
	gw.addr = ipaddr_addr(_gw);
	dns.addr = ipaddr_addr(_dns);
	netif_set_addr(&sta_if, &ip, &msk, &gw);
	dns_setserver(0, &dns);
}

void ip_dhcp()
{
	network_dhcp_start(WIFI_MODE_STA_ONLY);
	sem_get(&sem_dhcp, WAIT);
}

void pinmux(unsigned p, unsigned f)
{
	unsigned r;
	unsigned addr = BASE_PINMUX + 0x20 + ((p >> 3) * 4);
	unsigned lsb = (p & 0x7) * 4;
	r = reg(addr);
	r &= ~(0x7 << lsb);
	r |= (f & 0x7) << lsb;
	reg(addr) = r;
}

static void _printu(int c, void *_uart)
{
	uart_t *u = (uart_t *) _uart;
	if (c == '\r')
		return;
	if (c == '\n')
		uart_put(u, '\r');
	uart_put(u, c);
}

int plt_print(const char *buf, int n)
{
	while (n--) {
		char c = *buf++;
		_printu(c, &u0);
	}
	return n;
}

int _vprintf(const char *fmt, va_list ap)
{
	return _print(fmt, ap, _printu, &u0);
}

int main(int argc, char **argv);

int _main(void)
{
	char *s, *d, *h;
	int i, l, argc = 0;
	char **argv = 0;

	h = (char *)(BASE_SRAM + (256 << 10) - 256);
	if (*(unsigned *)h == 0xBEEFBEEF) {
		h += 4;
		argc = 1;
		for (s = h; *s;) {
			l = strlen(s);
			d = core_alloc(l + 1, 0);
			strcpy(d, s);
			s += (l + 1);
			argc++;
		}
		argv = (char **)core_alloc(sizeof(char *) * argc, 2);
		i = 0;
		argv[i++] = "app";
		for (s = h; *s; s += strlen(s) + 1)
			argv[i++] = s;
		argv[i] = 0;
	}
	return main(argc, argv);
}
