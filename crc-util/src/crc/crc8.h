/**
 ******************************************************************************
 * @file    crc8.h
 * @author  linrx
 * @version 1.0
 * @date    2017-9-16
 * @brief
 ******************************************************************************
 */

#ifndef __CRC8_H
#define __CRC8_H

/* 设置多项目式 */
#define CRC8_POLY       0x07    /*x8+x2+x+1, 1 0000 0111*/

/*0表示正常计算，非0表示位倒序计算*/
#define CRC8_REFLECT    0

/*输出crc时异或的数*/
#define CRC8_XOR        0x55

/*直接计算*/
uint8_t crc8_calc(uint8_t crc_init, const uint8_t *data, uint16_t len);

/*查表法计算，速度最快*/
uint8_t crc8_calc_tbl_fast(uint8_t crc_init, const uint8_t *data, uint16_t len);

/*半字节查表，占用空间少*/
uint8_t crc8_calc_tbl(uint8_t crc_init, const uint8_t *data, uint16_t len);

#endif /* __CRC8_H */
