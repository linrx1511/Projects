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

/* ���ö���Ŀʽ */
#define CRC8_POLY       0x07    /*x8+x2+x+1, 1 0000 0111*/

/*0��ʾ�������㣬��0��ʾλ�������*/
#define CRC8_REFLECT    0

/*���crcʱ������*/
#define CRC8_XOR        0x55

/*ֱ�Ӽ���*/
uint8_t crc8_calc(uint8_t crc_init, const uint8_t *data, uint16_t len);

/*������㣬�ٶ����*/
uint8_t crc8_calc_tbl_fast(uint8_t crc_init, const uint8_t *data, uint16_t len);

/*���ֽڲ��ռ�ÿռ���*/
uint8_t crc8_calc_tbl(uint8_t crc_init, const uint8_t *data, uint16_t len);

#endif /* __CRC8_H */
