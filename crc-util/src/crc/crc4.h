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

/* ���ö���Ŀʽ */
#define CRC4_POLY       0x03    /*crc4 itu, x4+x+1, 1 0011*/

/*0��ʾ�������㣬��0��ʾλ�������*/
#define CRC4_REFLECT    1

/*���crcʱ������*/
#define CRC4_XOR        0x00

/*ֱ�Ӽ���*/
uint8_t crc4_calc(uint8_t crc_init, const uint8_t *data, uint16_t len);

/*������㣬�ٶ����*/
uint8_t crc4_calc_tbl_fast(uint8_t crc_init, const uint8_t *data, uint16_t len);

/*���ֽڲ��ռ�ÿռ���*/
uint8_t crc4_calc_tbl(uint8_t crc_init, const uint8_t *data, uint16_t len);

#endif /* __CRC4_H */
