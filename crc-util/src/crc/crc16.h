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

/* ���ö���Ŀʽ */
#define CRC16_POLY       0x1021    /*x16+x12+x5+1, ccitt crc16*/

/*0��ʾ�������㣬��0��ʾλ�������*/
#define CRC16_REFLECT    0

/*���crcʱ������*/
#define CRC16_XOR        0x0000

/*ֱ�Ӽ���*/
uint16_t crc16_calc(uint16_t crc_init, const uint8_t *data, uint16_t len);

/*������㣬�ٶ����*/
uint16_t crc16_calc_tbl_fast(uint16_t crc_init, const uint8_t *data, uint16_t len);

/*���ֽڲ��ռ�ÿռ���*/
uint16_t crc16_calc_tbl(uint16_t crc_init, const uint8_t *data, uint16_t len);

#endif /* __CRC16_H */
