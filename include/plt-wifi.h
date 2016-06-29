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
