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

#include <lwip/netif.h>
#include <lwip/tcpip.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>

#include "term.h"
#include "_soc.h"
#include "plt.h"
#include "plt-wifi.h"

#define PORT	80
#define HN	"www.google.com"

unsigned buf[16], sz;

static void tcp_client()
{
	char *get = "GET index.html\r\n";
	struct sockaddr_in addr;
	int s, r;
	struct hostent *hp = gethostbyname(HN);

	if (!hp) {
		printf("err DNS\r\n");
		return;
	}
	printf("%s = %s\r\n",
	       hp->h_name, inet_ntoa(*(struct in_addr *)(hp->h_addr_list[0])));
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		printf("err socket\r\n");
		return;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr = *(struct in_addr *)hp->h_addr_list[0];
	//inet_pton(AF_INET, HOST, &addr.sin_addr);
	sz = strlen(get);
	memcpy(buf, get, sz);
	if (connect(s, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("err connect\r\n");
		return;
	}
	send(s, buf, sz, 0);
	while ((r = recv(s, buf, sizeof(buf) - 1, 0))) {
		if (r <= 0) {
			printf("err recv\r\n");
			break;
		}
		((char *)buf)[r] = 0;
		printf("data: %s\r\n", (char *)buf);
	}
	lwip_close(s);
}

#define xstr(s) str(s)
#define str(s) #s

static void main_thread(void *p)
{
	char *m0 = "00:0c:43:76:87:22";
	char *m1 = "00:0c:43:76:87:24";
	char *mac_addrs[] = { m0, m1, 0 };
	char *ssid = xstr(WIFI_SSID);
	char *pass = xstr(WIFI_PASSWD);
	plt_init();
	net_init(mac_addrs);
	_printf("wifi=%s %s\r\n", ssid, pass);
	wifi_init(WIFI_WPA_PSK_WPA2_PSK, ssid, pass);
#if !DHCP
	ip_static("192.168.1.29", "255.255.255.0", "192.168.1.1");
#else
	ip_dhcp();
#endif
	tcp_client();
}

int main(void)
{
	core_init();
	task_new("main", main_thread, 8, 4096, -1, 0);
	core_start();
	return 0;
}

#endif
