#ifndef WIFI0419
#define WIFI0419

typedef enum {
	WIFI_OPEN = 0,
	WIFI_SHARED,
	WIFI_AUTO_WEP,
	WIFI_WPA,
	WIFI_WPA_PSK,
	WIFI_WPA_None,
	WIFI_WPA2,
	WIFI_WPA2_PSK,
	WIFI_WPA_WPA2,
	WIFI_WPA_PSK_WPA2_PSK,
} wifi_auth_t;

void wifi_init(wifi_auth_t auth, char *id, char *passwd);

#endif
