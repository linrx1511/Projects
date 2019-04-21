/**
 ******************************************************************************
 * @file    vee_simple.c
 * @author  linrx
 * @version V1.0
 * @date    2017-10
 * @brief
 ******************************************************************************
 */

#include "common.h"
#include "vee_simple.h"

#define DEBUG 0
#if DEBUG
#include "ext_printf.h"
#define DBG(code)	(void)code
#else
#define DBG(code)
#endif

/* Page status definitions */
#define UNKOWN_PAGE           	0xFFFFU		/* 未知状态 */
#define ERASED_PAGE             0xEEFFU     /* 已经擦除状态 */
#define READY_PAGE            	0xEEEEU     /* 新页刚刚写完成状态 */
#define VALID_PAGE              0xCCCCU     /* 当前有效页 */
#define DISCARD_PAGE            0x8888U     /* 废弃的页 */

#define PAGE_HEAD_SIZE          8U			/* 页状态长度 */	
#define ADDR_START				(dev->desc->addr_start)
#define PAGE_SIZE				(dev->desc->page_size)
#define PAGE_COUNT				(dev->desc->page_count)
#define CURRENT_PAGE			(dev->cur_page)
#define CURRENT_PAGE_ADDR		(ADDR_START + CURRENT_PAGE * PAGE_SIZE)
#define CURRENT_PAGE_DATA_ADDR	(CURRENT_PAGE_ADDR + PAGE_HEAD_SIZE)
#define DEVICE_SIZE				(PAGE_SIZE - PAGE_HEAD_SIZE)


/************************************************************************/
/* static functions                                                     */
/************************************************************************/


/************************************************************************/
/* extern functions                                                     */
/************************************************************************/
/**
 * @brief
 * @param  dev: device pointer
 * @retval Success or error status:
 *           vees_result_t
 */
vees_result_t vees_format(vees_device_t *dev)
{
	uint16_t page_sta;
	uint8_t i;
	uint16_t page_addr;

	page_sta = ERASED_PAGE;
	page_addr = ADDR_START;
	for (i = 0; i < PAGE_COUNT; i++)
	{
		if (0 != ERASE(page_addr))
		{
			return VEES_IO_ERR;
		}

		if (0 != WRITE(page_addr, (uint8_t *)&page_sta, 2U))
		{
			return VEES_IO_ERR;
		}
		page_addr += PAGE_SIZE;
	}

	page_sta = VALID_PAGE;
	page_addr = ADDR_START;
	if (0 != WRITE(page_addr, (uint8_t *)&page_sta, 2U))
	{
		return VEES_IO_ERR;
	}

	dev->cur_page = 0;

	return VEES_SUCCESS;
}

vees_result_t vees_init(vees_device_t *dev)
{
	uint8_t valid_page;
	uint8_t ready_page;
	uint8_t valid_cnt = 0;
	uint8_t ready_cnt = 0;
	uint8_t receive_cnt = 0;
	uint8_t i;
	uint16_t page_sta;
	uint16_t addr;

	if ((dev == NULL) || (dev->desc == NULL) || (PAGE_COUNT < 2U))
	{
		return VEES_PARA_ERR;
	}

	addr = ADDR_START;
	for (i = 0; i < PAGE_COUNT; i++)
	{
		if (0 != READ(addr, (uint8_t *)&page_sta, 2U))
		{
			return VEES_IO_ERR;
		}

		if ((ERASED_PAGE == page_sta) || (DISCARD_PAGE == page_sta)
		        || (UNKOWN_PAGE == page_sta))
		{
			;
		}
		else if (VALID_PAGE == page_sta)
		{
			valid_page = i;
			valid_cnt++;
		}
		else if (READY_PAGE == page_sta)
		{
			ready_page = i;
			ready_cnt++;
		}
		else
		{
			/* 页状态格式有错误 */
			return VEES_FORMAT_ERR;
		}

		addr += PAGE_SIZE;
	}


	if(valid_cnt == 0)
	{
		if(ready_cnt == 1U)
		{
			/* 上次刚写完数据且差最后转换状态一步未完成 */
			page_sta = VALID_PAGE;
			if (WRITE(ADDR_START + PAGE_SIZE * ready_page, (uint8_t *)&page_sta, 2U) != 0)
			{
				return VEES_IO_ERR;
			}
			dev->cur_page = ready_page;
		}
		else
		{
			/* 页状态格式有错误 */
			return VEES_FORMAT_ERR;
		}
	}
	else if(valid_cnt == 1U)
	{
		if(ready_cnt == 0U)
		{
			dev->cur_page = valid_page;
		}
		else if(ready_cnt == 1U)
		{
			/* 上次刚写完数据且差最后转换状态两步未完成 */
			page_sta = DISCARD_PAGE;
			if (WRITE(ADDR_START + PAGE_SIZE * valid_page, (uint8_t *)&page_sta, 2U) != 0)
			{
				return VEES_IO_ERR;
			}
			page_sta = VALID_PAGE;
			if (WRITE(ADDR_START + PAGE_SIZE * ready_page, (uint8_t *)&page_sta, 2U) != 0)
			{
				return VEES_IO_ERR;
			}
			dev->cur_page = ready_page;
		}
		else
		{
			/* 页状态格式有错误 */
			return VEES_FORMAT_ERR;
		}
	}
	else
	{
		/* 页状态格式有错误 */
		return VEES_FORMAT_ERR;
	}

	return VEES_SUCCESS;
}

vees_result_t vees_read(vees_device_t *dev, uint16_t addr, uint8_t *data, uint16_t size)
{
	if ((addr + size) > DEVICE_SIZE)
	{
		return VEES_PARA_ERR;
	}

	if (READ(CURRENT_PAGE_DATA_ADDR + addr, data, size) != 0)
	{
		return VEES_IO_ERR;
	}

	return VEES_SUCCESS;
}

vees_result_t vees_write(vees_device_t *dev, uint16_t addr, uint8_t *data, uint16_t size)
{
	uint16_t addr_cur_page, addr_new_page;
	uint16_t i;
	vees_result_t res = VEES_SUCCESS;
	uint16_t page_sta;

	if ((addr + size) > DEVICE_SIZE)
	{
		return VEES_PARA_ERR;
	}

	addr_new_page = ADDR_START + PAGE_SIZE * ((CURRENT_PAGE + 1) % PAGE_COUNT);
	if (ERASE(addr_new_page) != 0)
	{
		return VEES_IO_ERR;
	}

	addr_new_page += PAGE_HEAD_SIZE;
	if (WRITE(addr_new_page + addr, data, size) != 0)
	{
		return VEES_IO_ERR;
	}

	addr_cur_page = CURRENT_PAGE_DATA_ADDR;

	if(addr >  0)
	{
		res = DATA_COPY(addr_new_page, addr_cur_page, addr);
		if (res != VEES_SUCCESS)
		{
			return res;
		}
	}

	i = addr + size;
	if(i < DEVICE_SIZE)
	{
		res = DATA_COPY(addr_new_page + i, addr_cur_page + i,  DEVICE_SIZE - i);
		if (res != VEES_SUCCESS)
		{
			return res;
		}
	}
	
	addr_cur_page -= PAGE_HEAD_SIZE;	/* 当前页开始地址 */
	addr_new_page -= PAGE_HEAD_SIZE;	/* 新页开始地址 */
	page_sta = READY_PAGE;
	if (WRITE(addr_new_page, (uint8_t *)&page_sta, 2U) != 0)
	{
		return VEES_IO_ERR;
	}
	page_sta = DISCARD_PAGE;
	if (WRITE(addr_cur_page, (uint8_t *)&page_sta, 2U) != 0)
	{
		return VEES_IO_ERR;
	}
	page_sta = VALID_PAGE;
	if (WRITE(addr_new_page, (uint8_t *)&page_sta, 2U) != 0)
	{
		return VEES_IO_ERR;
	}

	dev->cur_page++;
	if(dev->cur_page >= PAGE_COUNT)
	{
		dev->cur_page = 0;
	}
	/* dev->cur_page = (CURRENT_PAGE + 1U) % PAGE_COUNT; */

	return VEES_SUCCESS;
}

uint16_t vees_size(vees_device_t *dev)
{
	return DEVICE_SIZE;
}


