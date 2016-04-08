/* Copyright Statement:
 *
 * @2015 MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek Inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE.
 */
#if HAL
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hal_gpio.h"
#include "syslog.h"
/* Kernel includes. */
#define CFG_FPGA 0


#include "io_def.h"

#include <nvdm.h>
#include <nvram_handler.h>

#include <hal_trng.h>


log_create_module(main, PRINT_LEVEL_ERROR);

LOG_CONTROL_BLOCK_DECLARE(main);
LOG_CONTROL_BLOCK_DECLARE(common);
LOG_CONTROL_BLOCK_DECLARE(hal);
LOG_CONTROL_BLOCK_DECLARE(lwip);
LOG_CONTROL_BLOCK_DECLARE(minisupp);
LOG_CONTROL_BLOCK_DECLARE(inband);

log_control_block_t *syslog_control_blocks[] =
{
   &LOG_CONTROL_BLOCK_SYMBOL(main),
   &LOG_CONTROL_BLOCK_SYMBOL(common),
   &LOG_CONTROL_BLOCK_SYMBOL(hal),
   &LOG_CONTROL_BLOCK_SYMBOL(lwip),
   &LOG_CONTROL_BLOCK_SYMBOL(minisupp),
   &LOG_CONTROL_BLOCK_SYMBOL(inband),
   NULL
};

void top_xtal_init(void);

static void SystemClock_Config(void)
{
    top_xtal_init();
}

/* CACHE Init */
#include "mt7637_cm4_hw_memmap.h"
#include "hal_cache.h"
const hal_cache_region_config_t region_cfg_tbl[] = {
    /* cache_region_address, cache_region_size */
    { CM4_EXEC_IN_PLACE_BASE, 0x1000000}
    /* add cache regions below if you have any */
};

int32_t cache_enable(hal_cache_size_t cache_size)
{
    uint8_t region, region_number;

    region_number = sizeof(region_cfg_tbl) / sizeof(region_cfg_tbl[0]);
    if (region_number > HAL_CACHE_REGION_MAX) {
        return -1;
    }
    /* If cache is enabled, flush and invalidate cache */
    hal_cache_init();
    hal_cache_set_size(cache_size);
    for (region = 0; region < region_number; region++) {
        hal_cache_region_config(region, &region_cfg_tbl[region]);
        hal_cache_region_enable(region);
    }
    for ( ; region < HAL_CACHE_REGION_MAX; region++) {
        hal_cache_region_disable(region);
    }
    hal_cache_enable();
    return 0;
}

void cmnCpuClkConfigureTo192M(void);

void cmnSerialFlashClkConfTo64M(void);

static void prvSetupHardware( void )
{
// FIXME:
//    if (cache_enable(HAL_CACHE_SIZE_32KB) < 0) {
//        LOG_E(common, "cache enable failed");
//    }

    /* Enable MCU clock to 192MHz */
    cmnCpuClkConfigureTo192M();

    /*Enable flash clock to 64MHz*/
    cmnSerialFlashClkConfTo64M();

    /* Set Pinmux to UART0  */
    hal_pinmux_set_function(0, 7);
    hal_pinmux_set_function(1, 7);
    hal_pinmux_set_function(2, 7);
    hal_pinmux_set_function(3, 7);

    /* UART 1 init. */
    hal_pinmux_set_function(HAL_GPIO_36, 7);
    hal_pinmux_set_function(HAL_GPIO_37, 7);
    hal_pinmux_set_function(HAL_GPIO_38, 7);
    hal_pinmux_set_function(HAL_GPIO_39, 7);

//    io_def_uart_init();
}

static char syslog_filter_buf[SYSLOG_FILTER_LEN] = {0};

void syslog_config_save(const syslog_config_t *config)
{
    syslog_convert_filter_val2str((const log_control_block_t **)config->filters, syslog_filter_buf);
    nvdm_write_data_item("common", "syslog_filters",\
            NVDM_DATA_ITEM_TYPE_STRING,(const uint8_t *)syslog_filter_buf, strlen(syslog_filter_buf));
}

uint32_t syslog_config_load(syslog_config_t *config)
{
    int sz = SYSLOG_FILTER_LEN;

    nvdm_read_data_item("common", "syslog_filters", (uint8_t *)syslog_filter_buf, (uint32_t *)&sz);
    syslog_convert_filter_str2val(config->filters, syslog_filter_buf);

    return 0;
}

/**
 * Initialize C library random function using HAL TRNG.
 */
static void _main_sys_random_init(void)
{
    uint32_t            seed;
    hal_trng_status_t   s;

    s = hal_trng_init();

    if (s == HAL_TRNG_STATUS_OK) {
        s = hal_trng_get_generated_random_number(&seed);
        printf("seed %lx\n", seed);
    }

    if (s == HAL_TRNG_STATUS_OK) {
        srand((unsigned int)seed);
    }

    if (s != HAL_TRNG_STATUS_OK) {
        printf("trng init failed\n");
    } else {
        printf("random %d\n", rand());
    }
}

void hal_lp_handle_intr();

void system_init(void)
{
    time_t      t       = 12345;

    /* SystemClock Config */
    SystemClock_Config();
    /* Handle low power interrupt */
    hal_lp_handle_intr();

    /* Configure the hardware ready to run the test. */
    prvSetupHardware();

//    log_init(syslog_config_save, syslog_config_load, syslog_control_blocks);
    /* workaround for NSTP */
    ctime(&t);

    _main_sys_random_init();

}

void netbuf_destport(void* a){}
void netbuf_destaddr(void* a){}
#endif
