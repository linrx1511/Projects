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

/* 设置多项目式 */
#define CRC32_POLY       0x04C11DB7    /* x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1*/

/*0表示正常计算，非0表示位倒序计算*/
#define CRC32_REFLECT    0

/*输出crc时异或的数*/
#define CRC32_XOR        0xffffffff

/*直接计算*/
uint32_t crc32_calc(uint32_t crc_init, const uint8_t *data, uint16_t len);

/*查表法计算，速度最快*/
uint32_t crc32_calc_tbl_fast(uint32_t crc_init, const uint8_t *data, uint16_t len);

/*半字节查表，占用空间少*/
uint32_t crc32_calc_tbl(uint32_t crc_init, const uint8_t *data, uint16_t len);

#endif /* __CRC32_H */
