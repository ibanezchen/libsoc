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
#include <hcos/task.h>
#include <hcos/soc.h>

#include <string.h>

#if HAL

void network_dhcp_start(unsigned char opmode);

void system_init(void);

#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "wifi_inband.h"
#include "network_init.h"
#include "term.h"

#define SOCK_TCP_SRV_PORT        6500
#define SOCK_UDP_SRV_PORT        6600
#define TRX_PACKET_COUNT         3
#define LWIP_SOCKET_STACK_SIZE  (1000/sizeof(unsigned))
#define LWIP_SOCKET_TASK_PRIO   1

static void tcp_server(void* p)
{
    int s;
    int c;
    int ret;
    int rlen;
    struct sockaddr_in addr;
    char srv_buf[32] = {0};
    _printf("TCP_server_test start");

    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = lwip_htons(SOCK_TCP_SRV_PORT);
    addr.sin_addr.s_addr = lwip_htonl(IPADDR_ANY);

    /* create the socket */
    s = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        _printf("TCP server create fail");
        goto done;
    }

    ret = lwip_bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        _printf("TCP server bind fail");
        goto clean;
    }

    ret = lwip_listen(s, 0);
    if (ret < 0) {
        _printf("TCP server listen fail");
        goto clean;
    }

    while ( 1 ) {
        socklen_t sockaddr_len = sizeof(addr);
        c = lwip_accept(s, (struct sockaddr *)&addr, &sockaddr_len);
        if (c < 0) {
            _printf("TCP server accept error");
            break;   // or go on waiting for connect requestion?
        }

        _printf("TCP server waiting for data...");
        while ((rlen = lwip_read(c, srv_buf, sizeof(srv_buf) - 1))) {
            if (rlen < 0) {
                _printf("read error");
                break;
            }
            srv_buf[rlen] = 0; //for the next statement - _printf string.
            _printf("TCP server received data:%s", srv_buf);

            lwip_write(c, srv_buf, rlen);      // sonar server
        }

        lwip_close(c);
    }

clean:
    lwip_close(s);

done:
    _printf("TCP server test done");

}

void socket_test(const struct netif *netif)
{
	task_new("net_tst", tcp_server, 5, 2048, -1, 0);
}

void net_init(void *args)
{
	unsigned char opmode  = WIFI_MODE_STA_ONLY;
	unsigned char port = WIFI_PORT_STA;
	unsigned char *ssid = (unsigned char*)"Linaro";
	wifi_auth_mode_t auth = WIFI_AUTH_MODE_WPA_PSK_WPA2_PSK;
	wifi_encrypt_type_t encrypt = WIFI_ENCRYPT_TYPE_TKIP_AES_MIX;
	unsigned char *password = (unsigned char*)"linaromtk";
	unsigned char nv_opmode;

	//FIXME:
	printf("net_init()\r\n");

	system_init();
	printf("system_init()\r\n");
	
	wifi_register_ip_ready_callback(socket_test);
	network_init();
	printf("network_init()\r\n");    

	if (wifi_config_init() == 0) {
		wifi_config_get_opmode(&nv_opmode);
		if (nv_opmode != opmode) {
			wifi_config_set_opmode(opmode);
		}
		wifi_config_set_ssid(port, ssid ,strlen((char*)ssid));
		wifi_config_set_security_mode(port, auth, encrypt);
		wifi_config_set_wpa_psk_key(port, password, strlen((char*)password));
		wifi_config_reload_setting();
		network_dhcp_start(opmode);
	}
}

#if _EXE_

int printf(const char *str, ...)
{
	if (str == 0) {
		return 0;
	}
	va_list ap;
	va_start(ap, str);
	_print((char*)str, ap);
	va_end(ap);
	return 0;
}

int main(void)
{
	core_init();
	task_new("net_init", net_init, 2, 4096, -1, 0);
	core_start();
	return 0;
}

#endif
#endif
