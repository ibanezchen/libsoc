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

#include "wifi_inband.h"
#include "wifi.h"
#include "wifi_api.h"
#include <string.h>

void network_dhcp_start(unsigned char opmode);

void wifi_init(wifi_auth_t auth, char *ssid, char *passwd)
{
	unsigned char opmode = WIFI_MODE_STA_ONLY;
	unsigned char port = WIFI_PORT_STA;
	wifi_encrypt_type_t encrypt = WIFI_ENCRYPT_TYPE_TKIP_AES_MIX;
	unsigned char nv_opmode;

	if (wifi_config_init() == 0) {
		wifi_config_get_opmode(&nv_opmode);
		if (nv_opmode != opmode) {
			wifi_config_set_opmode(opmode);
		}
		wifi_config_set_ssid(port, (unsigned char *)ssid, strlen(ssid));
		wifi_config_set_security_mode(port, auth, encrypt);
		wifi_config_set_wpa_psk_key(port, (unsigned char *)passwd,
					    strlen(passwd));
		wifi_config_reload_setting();
	}
}

void dhcp_init()
{
	network_dhcp_start(WIFI_MODE_STA_ONLY);
}
