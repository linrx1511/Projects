/**
 ******************************************************************************
 * @file    crc4.h
 * @author  linrx
 * @version 1.0
 * @date    2017-9-16
 * @brief
 ******************************************************************************
 */

#ifndef __CRC4_H
#define __CRC4_H

/* 设置多项目式 */
#define CRC4_POLY       0x03    /*crc4 itu, x4+x+1, 1 0011*/

/*0表示正常计算，非0表示位倒序计算*/
#define CRC4_REFLECT    1

/*输出crc时异或的数*/
#define CRC4_XOR        0x00

/*直接计算*/
uint8_t crc4_calc(uint8_t crc_init, const uint8_t *data, uint16_t len);

/*查表法计算，速度最快*/
uint8_t crc4_calc_tbl_fast(uint8_t crc_init, const uint8_t *data, uint16_t len);

/*半字节查表，占用空间少*/
uint8_t crc4_calc_tbl(uint8_t crc_init, const uint8_t *data, uint16_t len);

#endif /* __CRC4_H */
