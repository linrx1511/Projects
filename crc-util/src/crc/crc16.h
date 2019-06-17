/**
 ******************************************************************************
 * @file    crc16.h
 * @author  linrx
 * @version 1.0
 * @date    2017-9-16
 * @brief
 ******************************************************************************
 */

#ifndef __CRC16_H
#define __CRC16_H

/* 设置多项目式 */
#define CRC16_POLY       0x1021    /*x16+x12+x5+1, ccitt crc16*/

/*0表示正常计算，非0表示位倒序计算*/
#define CRC16_REFLECT    0

/*输出crc时异或的数*/
#define CRC16_XOR        0x0000

/*直接计算*/
uint16_t crc16_calc(uint16_t crc_init, const uint8_t *data, uint16_t len);

/*查表法计算，速度最快*/
uint16_t crc16_calc_tbl_fast(uint16_t crc_init, const uint8_t *data, uint16_t len);

/*半字节查表，占用空间少*/
uint16_t crc16_calc_tbl(uint16_t crc_init, const uint8_t *data, uint16_t len);

#endif /* __CRC16_H */
