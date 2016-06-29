#include "wifi_inband.h"
#include "wifi.h"
#include "wifi_api.h"
#include "plt-wifi.h"
#include <string.h>

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
