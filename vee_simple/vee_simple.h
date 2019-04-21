
/**
 ******************************************************************************
 * @file    vee_simple.h
 * @author  linrx
 * @version V1.0
 * @date    2017-10
 * @brief	简易版本模拟EEPROM，芯片: mc9s08aw16a/mc9s08ac16/mcs08aw60
 * 		
 * 1. 用户可以创建多个模拟器件，每个器件包括至少两页，程序会轮流擦写这些页面实现负载平衡，
 *    所以一个器件可存储的数据大小为page_size - 8 字节，如设置512bytes为一页，则可用空间
 *    为512-8 = 504bytes。
 * 2. 每一次调用write函数，都会擦写一整页flash，所以建议每次尽量把所有要写的数据一次写完。
 * 3. 如果写入时意外断电，最差的情况可以恢复上次数据，此次数据丢失。
 * 4. flash可擦写次数每页为100000次，所以如果设置为N页，则flash寿命为原来N倍。
 * 5. 注意页大小是指虚拟EEPROM的页大小，建议设置为flash一个扇区大小，如果空间不够用也可
 *    设置为多个扇区。
 * 6. 需要实现4个宏，WRITE/READ/ERASE/DATA_COPY，这是flash底层读写函数。
 *
 ******************************************************************************
 * V1.0-20171012 Linrx	Initial version.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VEE_SIMPLE_H
#define __VEE_SIMPLE_H

#include "vee_simple_bsp.h"

typedef enum
{
    VEES_SUCCESS = 0,
    VEES_PARA_ERR,
    VEES_IO_ERR,
    VEES_FORMAT_ERR,
} vees_result_t;

struct vees_descriptor
{
	uint16_t addr_start;	/* flash开始地址 */
	uint16_t page_size;		/* 模拟eeprom的页大小，建议设置为flash一个扇区大小 */
	uint8_t page_count;		/* 由多少页构成，必须大于等于2 */
};

struct vees_device
{
	struct vees_descriptor *desc;
	uint8_t cur_page;
};

typedef struct vees_device vees_device_t;

/*********************************************************
使用VEES_CREATE宏创建器件，创建后即可用库函数进行操作，
初始化，读写等。
**********************************************************/
#define VEES_CREATE(name, start_addr, page_size, page_num)\
	static const struct vees_descriptor CC_CONCAT(name, _desc) =\
	        {start_addr, page_size, page_num};\
	static struct vees_device name = {(struct vees_descriptor *)&(CC_CONCAT(name, _desc)), 0}


/**
  * @brief  器件初始化
  * @param  dev: device's pointer (&name)
  * @retval vees_result_t
  */
vees_result_t vees_init(vees_device_t *dev);

/**
  * @brief  器件格式化
  * @param  dev: device's pointer (&name)
  * @retval vees_result_t
  */
vees_result_t vees_format(vees_device_t *dev);

/**
  * @brief  器件写函数
  * @param  dev: device's pointer (&name)
  *			addr: 写入的地址
  *			data: 数据指针
  *			size: 数据大小
  * @retval vees_result_t
  */
vees_result_t vees_write(vees_device_t *dev, uint16_t addr, uint8_t *data, uint16_t size);

/**
  * @brief  器件读函数
  * @param  dev: device's pointer (&name)
  *			addr: 写入的地址
  *			data: 数据指针
  *			size: 数据大小
  * @retval vees_result_t
  */
vees_result_t vees_read(vees_device_t *dev, uint16_t addr, uint8_t *data, uint16_t size);

/**
  * @brief  获取器件空间大小
  * @param  dev: device's pointer (&name)
  * @retval 器件容量大小
  */
uint16_t vees_size(vees_device_t *dev);


#endif

