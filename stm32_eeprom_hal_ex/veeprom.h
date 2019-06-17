/**
  ******************************************************************************
  * @file    EEPROM_Emulation/inc/eeprom.h
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file contains all the functions prototypes for the EEPROM
  *          emulation firmware library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/**
 ******************************************************************************
 * @file    veeprom.h
 * @author  linrx
 * @version V2.0
 * @date    2014-04
 * @brief   由STM32模拟EEPROM库更改，并做了优化(由RAM变量记录当前读写地址).
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VEEPROM_H
#define __VEEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Exported constants --------------------------------------------------------*/

/* Define the size of the sectors to be used */
#define PAGE_SIZE             ((uint32_t)0x0400)  /* Page size = 1KByte */

/* EEPROM start address in Flash */
//#define EEPROM_START_ADDRESS  ((uint32_t)0x08002000) /* EEPROM emulation start address: from sector2, after 8KByte of used Flash memory */
#define EEPROM_START_ADDRESS  ((uint32_t)0x0800e000)


/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + 0x000))
#define PAGE0_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + PAGE_SIZE))
#define PAGE1_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))

/* Used Flash pages for EEPROM emulation */
#define PAGE0                   ((uint16_t)0x0000)
#define PAGE1                   ((uint16_t)0x0001)

/* No valid page define */
#define NO_VALID_PAGE           ((uint16_t)0x00AB)

/* Page status definitions */
#define ERASED                  ((uint16_t)0xFFFF)     /* PAGE is empty */
#define RECEIVE_DATA            ((uint16_t)0xEEEE)     /* PAGE is marked to receive data */
#define VALID_PAGE              ((uint16_t)0x0000)     /* PAGE containing valid data */

/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE    ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE     ((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL               ((uint8_t)0x80)

/* Variables' number */
#define NB_OF_VAR               ((uint8_t)2)

/* Variables' size ,字节为单位，必须是2的倍数*/
#define SIZE_OF_VAR				((uint8_t)8)

/* 读写falsh错误 */
#define FLASH_ERROR				((uint16_t)1)

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* API 函数返回结果 */
#define EE_SUCCESS			0
#define EE_FLASH_ERROR		FLASH_ERROR
#define EE_PAGE_FULL		PAGE_FULL
#define EE_NO_VALID_PAGE	NO_VALID_PAGE

/* Exported functions ------------------------------------------------------- */
uint16_t EE_Init(void);
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t *Data);
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t *Data);

#endif /* __EEPROM_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

