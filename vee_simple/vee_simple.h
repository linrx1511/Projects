
/**
 ******************************************************************************
 * @file    vee_simple.h
 * @author  linrx
 * @version V1.0
 * @date    2017-10
 * @brief	���װ汾ģ��EEPROM��оƬ: mc9s08aw16a/mc9s08ac16/mcs08aw60
 * 		
 * 1. �û����Դ������ģ��������ÿ����������������ҳ�������������д��Щҳ��ʵ�ָ���ƽ�⣬
 *    ����һ�������ɴ洢�����ݴ�СΪpage_size - 8 �ֽڣ�������512bytesΪһҳ������ÿռ�
 *    Ϊ512-8 = 504bytes��
 * 2. ÿһ�ε���write�����������дһ��ҳflash�����Խ���ÿ�ξ���������Ҫд������һ��д�ꡣ
 * 3. ���д��ʱ����ϵ磬����������Իָ��ϴ����ݣ��˴����ݶ�ʧ��
 * 4. flash�ɲ�д����ÿҳΪ100000�Σ������������ΪNҳ����flash����Ϊԭ��N����
 * 5. ע��ҳ��С��ָ����EEPROM��ҳ��С����������Ϊflashһ��������С������ռ䲻����Ҳ��
 *    ����Ϊ���������
 * 6. ��Ҫʵ��4���꣬WRITE/READ/ERASE/DATA_COPY������flash�ײ��д������
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
	uint16_t addr_start;	/* flash��ʼ��ַ */
	uint16_t page_size;		/* ģ��eeprom��ҳ��С����������Ϊflashһ��������С */
	uint8_t page_count;		/* �ɶ���ҳ���ɣ�������ڵ���2 */
};

struct vees_device
{
	struct vees_descriptor *desc;
	uint8_t cur_page;
};

typedef struct vees_device vees_device_t;

/*********************************************************
ʹ��VEES_CREATE�괴�������������󼴿��ÿ⺯�����в�����
��ʼ������д�ȡ�
**********************************************************/
#define VEES_CREATE(name, start_addr, page_size, page_num)\
	static const struct vees_descriptor CC_CONCAT(name, _desc) =\
	        {start_addr, page_size, page_num};\
	static struct vees_device name = {(struct vees_descriptor *)&(CC_CONCAT(name, _desc)), 0}


/**
  * @brief  ������ʼ��
  * @param  dev: device's pointer (&name)
  * @retval vees_result_t
  */
vees_result_t vees_init(vees_device_t *dev);

/**
  * @brief  ������ʽ��
  * @param  dev: device's pointer (&name)
  * @retval vees_result_t
  */
vees_result_t vees_format(vees_device_t *dev);

/**
  * @brief  ����д����
  * @param  dev: device's pointer (&name)
  *			addr: д��ĵ�ַ
  *			data: ����ָ��
  *			size: ���ݴ�С
  * @retval vees_result_t
  */
vees_result_t vees_write(vees_device_t *dev, uint16_t addr, uint8_t *data, uint16_t size);

/**
  * @brief  ����������
  * @param  dev: device's pointer (&name)
  *			addr: д��ĵ�ַ
  *			data: ����ָ��
  *			size: ���ݴ�С
  * @retval vees_result_t
  */
vees_result_t vees_read(vees_device_t *dev, uint16_t addr, uint8_t *data, uint16_t size);

/**
  * @brief  ��ȡ�����ռ��С
  * @param  dev: device's pointer (&name)
  * @retval ����������С
  */
uint16_t vees_size(vees_device_t *dev);


#endif

