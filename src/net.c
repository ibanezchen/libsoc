/*-****************************************************************************/
/*-                                                                           */
/*-            Copyright (c) 2013 by SOCware Inc.                             */
/*-                                                                           */
/*-  This software is copyrighted by and is the sole property of SOCware, Inc.*/
/*-  All rights, title, ownership, or other interests in the software remain  */
/*-  the property of SOCware, Inc. The source code is for FREE short-term     */
/*-  evaluation, educational or non-commercial research only. Any commercial  */
/*-  application may only be used in accordance with the corresponding license*/
/*-  agreement. Any unauthorized use, duplication, transmission, distribution,*/
/*-  or disclosure of this software is expressly forbidden.                   */
/*-                                                                           */
/*-  Knowledge of the source code may NOT be used to develop a similar product*/
/*-                                                                           */
/*-  This Copyright notice may not be removed or modified without prior       */
/*-  written consent of SOCware, Inc.                                         */
/*-                                                                           */
/*-  SOCWare, Inc. reserves the right to modify this software                 */
/*-  without notice.                                                          */
/*-                                                                           */
/*-             socware.help@gmail.com                                        */
/*-             http://socware.net                                            */
/*-                                                                           */
/*-****************************************************************************/

#if _EXE_

#include <hcos/task.h>
#include <hcos/soc.h>
#include <string.h>

#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "term.h"
#include "_soc.h"
#include "plt.h"
#include "plt-wifi.h"
#include "_malloc.h"

#define SOCK_TCP_SRV_PORT        6500
#define SOCK_UDP_SRV_PORT        6600
#define TRX_PACKET_COUNT         3
#define LWIP_SOCKET_STACK_SIZE  (1000/sizeof(unsigned))
#define LWIP_SOCKET_TASK_PRIO   1

static void tcp_server()
{
	int s;
	int c;
	int ret;
	int rlen;
	struct sockaddr_in addr;
	char srv_buf[32] = { 0 };
	_printf("TCP_server_test start\r\n");

	memset(&addr, 0, sizeof(addr));
	addr.sin_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = lwip_htons(SOCK_TCP_SRV_PORT);
	addr.sin_addr.s_addr = lwip_htonl(IPADDR_ANY);

	/* create the socket */
	s = lwip_socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		_printf("TCP server create fail\r\n");
		goto done;
	}

	ret = lwip_bind(s, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		_printf("TCP server bind fail\r\n");
		goto clean;
	}

	ret = lwip_listen(s, 0);
	if (ret < 0) {
		_printf("TCP server listen fail\r\n");
		goto clean;
	}

	while (1) {
		socklen_t sockaddr_len = sizeof(addr);
		c = lwip_accept(s, (struct sockaddr *)&addr, &sockaddr_len);
		if (c < 0) {
			_printf("TCP server accept error\r\n");
			break;	// or go on waiting for connect requestion?
		}

		_printf("TCP server waiting for data...\r\n");
		while ((rlen = lwip_read(c, srv_buf, sizeof(srv_buf) - 1))) {
			if (rlen < 0) {
				_printf("read error\r\n");
				break;
			}
			srv_buf[rlen] = 0;	//for the next statement - _printf string.
			_printf("TCP server received data:%s\r\n", srv_buf);

			lwip_write(c, srv_buf, rlen);	// sonar server
		}

		lwip_close(c);
	}

clean:
	lwip_close(s);

done:
	_printf("TCP server test done\r\n");

}

#define xstr(s) str(s)
#define str(s) #s

static void main_thread(void *p)
{
	char *ssid = xstr(WIFI_SSID);
	char *pass = xstr(WIFI_PASSWD);
	malloc_init();
	plt_init();
	net_init();
	_printf("wifi=%s %s\r\n", ssid, pass);
	wifi_init(WIFI_WPA_PSK_WPA2_PSK, ssid, pass);
	ip_dhcp();
	tcp_server();
}

int main(void)
{
	core_init();
	task_new("main", main_thread, 8, 4096, -1, 0);
	core_start();
	return 0;
}

#endif
