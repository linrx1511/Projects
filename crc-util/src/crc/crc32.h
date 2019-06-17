/**
 ******************************************************************************
 * @file    crc32.h
 * @author  linrx
 * @version 1.0
 * @date    2017-9-21
 * @brief
 ******************************************************************************
 */

#ifndef __CRC32_H
#define __CRC32_H

/* ���ö���Ŀʽ */
#define CRC32_POLY       0x04C11DB7    /* x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1*/

/*0��ʾ�������㣬��0��ʾλ�������*/
#define CRC32_REFLECT    0

/*���crcʱ������*/
#define CRC32_XOR        0xffffffff

/*ֱ�Ӽ���*/
uint32_t crc32_calc(uint32_t crc_init, const uint8_t *data, uint16_t len);

/*������㣬�ٶ����*/
uint32_t crc32_calc_tbl_fast(uint32_t crc_init, const uint8_t *data, uint16_t len);

/*���ֽڲ��ռ�ÿռ���*/
uint32_t crc32_calc_tbl(uint32_t crc_init, const uint8_t *data, uint16_t len);

#endif /* __CRC32_H */
