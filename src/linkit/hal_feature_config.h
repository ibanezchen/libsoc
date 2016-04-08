/**
 * @file    hal_feature_config.h
 * @brief   Every module feature option, If the module is unuseful,customer can turn off the module enable option. But all of the sub features have dependency with hareware,please not to modify
 *
 ***************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2005
 *
 *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
 *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
 *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
 *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
 ****************************************************************************
 */


#ifndef __HAL_FEATURE_CONFIG_H__
#define __HAL_FEATURE_CONFIG_H__


#ifdef __cplusplus
    extern "C" {
#endif


/*****************************************************************************
* module ON or OFF feature option,only option in this temple
*****************************************************************************/

//#define HAL_ACCDET_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_AES_MODULE_ENABLED          
//#define HAL_AUDIO_MODULE_ENABLED     
#define HAL_CACHE_MODULE_ENABLED    
#define HAL_DES_MODULE_ENABLED      
#define HAL_EINT_MODULE_ENABLED        
#define HAL_FLASH_MODULE_ENABLED       
#define HAL_GDMA_MODULE_ENABLED         
#define HAL_GPC_MODULE_ENABLED        
#define HAL_GPIO_MODULE_ENABLED         
#define HAL_GPT_MODULE_ENABLED          
#define HAL_I2C_MASTER_MODULE_ENABLED 
#define HAL_I2S_MODULE_ENABLED  
#define HAL_IRRX_MODULE_ENABLED       
#define HAL_IRTX_MODULE_ENABLED
//#define HAL_ISINK_MODULE_ENABLED       
//#define HAL_KEYPAD_MODULE_ENABLED      
#define HAL_MD5_MODULE_ENABLED          
//#define HAL_MPU_MODULE_ENABLED  
//#define HAL_NVIC_MODULE_ENABLED   
#define HAL_PWM_MODULE_ENABLED         
#define HAL_RTC_MODULE_ENABLED         
//#define HAL_SD_MODULE_ENABLED           
//#define HAL_SDIO_MODULE_ENABLED         
#define HAL_SHA_MODULE_ENABLED         
#define HAL_SPI_MASTER_MODULE_ENABLED   
//#define HAL_SPI_SLAVE_MODULE_ENABLED
#define HAL_TRNG_MODULE_ENABLED 
#define HAL_UART_MODULE_ENABLED        
#define HAL_WDT_MODULE_ENABLED       


#ifdef __cplusplus
}
#endif

#endif /* __HAL_FEATURE_CONFIG_H__ */ 

