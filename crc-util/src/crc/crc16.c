/**
 ******************************************************************************
 * @file    crc16.c
 * @author  linrx
 * @version 1.0
 * @date    2017-9-17
 * @brief
 ******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "crc16.h"

#define DEBUG 1
#if DEBUG
#define PRINTF(...)     printf(__VA_ARGS__);fflush(stdout)
#else
#define PRINTF(...)
#endif

static uint16_t crc16_table[256];
static uint16_t crc16_table_tiny[16];

#define CRC16_POLY_REFLECT   (((CRC16_POLY & 0x8000) >> 15) | ((CRC16_POLY & 0x4000) >> 13)\
                            | ((CRC16_POLY & 0x2000) >> 11) | ((CRC16_POLY & 0x1000) >> 9)\
                            | ((CRC16_POLY & 0x0800) >> 7) | ((CRC16_POLY & 0x0400) >> 5)\
                            | ((CRC16_POLY & 0x0200) >> 3) | ((CRC16_POLY & 0x0100) >> 1)\
                            | ((CRC16_POLY & 0x0080) << 1) | ((CRC16_POLY & 0x0040) << 3)\
                            | ((CRC16_POLY & 0x0020) << 5) | ((CRC16_POLY & 0x0010) << 7)\
                            | ((CRC16_POLY & 0x0008) << 9) | ((CRC16_POLY & 0x0004) << 11)\
                            | ((CRC16_POLY & 0x0002) << 13) | ((CRC16_POLY & 0x0001) << 15))

#if CRC16_REFLECT == 0
static
void crc16_talbe_init(void)
{
    int i, j;

    for (i = 0; i < 256; i++)
    {
        crc16_table[i] = (uint16_t)i << 8;
        for (j = 0; j < 8; j++)
        {
            if (crc16_table[i] & 0x8000)
            {
                crc16_table[i] = (crc16_table[i] << 1) ^ CRC16_POLY;
            }
            else
            {
                crc16_table[i] <<= 1;
            }
        }
    }

    PRINTF("crc16_table[] =\n{\n    ");
    for (i = 0; i < 255;)
    {
        PRINTF("0x%04X,", crc16_table[i]);
        if ((++i % 16) == 0)
        {
            PRINTF("\n    ");
        }
    }
    PRINTF("0x%04X\n", crc16_table[i]);
    PRINTF("};\n");
}

static
void crc16_talbe_tiny_init(void)
{
    int i, j;

    for (i = 0; i < 16; i++)
    {
        crc16_table_tiny[i] = (uint16_t)i << 8;
        for (j = 0; j < 8; j++)
        {
            if (crc16_table_tiny[i] & 0x8000)
            {
                crc16_table_tiny[i] = (crc16_table_tiny[i] << 1) ^ CRC16_POLY;
            }
            else
            {
                crc16_table_tiny[i] <<= 1;
            }
        }
    }

    PRINTF("crc16_table_tiny[] =\n{\n    ");
    for (i = 0; i < 15; i++)
    {
        PRINTF("0x%04X,", crc16_table_tiny[i]);
    }
    PRINTF("0x%04X\n", crc16_table_tiny[i]);
    PRINTF("};\n");
}

uint16_t crc16_calc(uint16_t crc_init, const uint8_t *data, uint16_t len)
{
    uint16_t crc = crc_init;
    uint8_t i;

    while (len--)
    {
        crc ^= ((uint16_t)*data++ << 8);
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ CRC16_POLY;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc ^ CRC16_XOR;
}

uint16_t crc16_calc_tbl_fast(uint16_t crc_init, const uint8_t *data, uint16_t len)
{
    uint16_t crc = crc_init;

    while (len--)
    {
        crc = crc16_table[(uint8_t)(crc >> 8) ^ *data++] ^ (crc << 8);
    }

    return crc ^ CRC16_XOR;
}

uint16_t crc16_calc_tbl(uint16_t crc_init, const uint8_t *data, uint16_t len)
{
    uint16_t crc = crc_init;
    uint8_t c;

    while (len--)
    {
        c = *data++;
        crc = crc16_table_tiny[((c >> 4) ^ (crc >> 12))] ^ (crc << 4);
        crc = crc16_table_tiny[(c & 0x0f) ^ (crc >> 12)] ^ (crc << 4);
    }

    return crc ^ CRC16_XOR;
}
#else
static
void crc16_talbe_init(void)
{
    int i, j;

    for (i = 0; i < 256; i++)
    {
        crc16_table[i] = i;
        for (j = 0; j < 8; j++)
        {
            if (crc16_table[i] & 0x0001)
            {
                crc16_table[i] = (crc16_table[i] >> 1) ^ CRC16_POLY_REFLECT;
            }
            else
            {
                crc16_table[i] >>= 1;
            }
        }
    }

    PRINTF("crc16_table[] =\n{\n    ");
    for (i = 0; i < 255;)
    {
        PRINTF("0x%04X,", crc16_table[i]);
        if ((++i % 16) == 0)
        {
            PRINTF("\n    ");
        }
    }
    PRINTF("0x%04X\n", crc16_table[i]);
    PRINTF("};\n");
}

static
void crc16_talbe_tiny_init(void)
{
    int i, j;

    for (i = 0; i < 16; i++)
    {
        crc16_table_tiny[i] = i << 4;
        for (j = 0; j < 8; j++)
        {
            if (crc16_table_tiny[i] & 0x0001)
            {
                crc16_table_tiny[i] = (crc16_table_tiny[i] >> 1) ^ CRC16_POLY_REFLECT;
            }
            else
            {
                crc16_table_tiny[i] >>= 1;
            }
        }
    }

    PRINTF("crc16_table_tiny[] =\n{\n    ");
    for (i = 0; i < 15; i++)
    {
        PRINTF("0x%04X,", crc16_table_tiny[i]);
    }
    PRINTF("0x%04X\n", crc16_table_tiny[i]);
    PRINTF("};\n");
}

uint16_t crc16_calc(uint16_t crc_init, const uint8_t *data, uint16_t len)
{
    uint16_t crc = crc_init;
    uint8_t i;

    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x0001)
            {
                crc = (crc >> 1) ^ CRC16_POLY_REFLECT;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc ^ CRC16_XOR;
}

uint16_t crc16_calc_tbl_fast(uint16_t crc_init, const uint8_t *data, uint16_t len)
{
    uint16_t crc = crc_init;

    while (len--)
    {
        crc = crc16_table[(uint8_t)crc ^ *data++] ^ (crc >> 8);
    }

    return crc ^ CRC16_XOR;
}

uint16_t crc16_calc_tbl(uint16_t crc_init, const uint8_t *data, uint16_t len)
{
    uint16_t crc = crc_init;
    uint8_t c;

    while (len--)
    {
        c = *data++;
        crc = crc16_table_tiny[(c ^ crc) & 0x000f] ^ (crc >> 4);
        crc = crc16_table_tiny[((c >> 4) ^ crc) & 0x000f] ^ (crc >> 4);
    }

    return crc ^ CRC16_XOR;
}
#endif


#if DEBUG
/* 测试例程 ，表格可以计算后放到const中减少占用RAM */
void test_crc16(void)
{
    uint16_t crc16;
    const uint8_t test_data[8] =
            { 1, 2, 3, 4, 5, 6, 7, 8 };

    crc16_talbe_init();
    crc16_talbe_tiny_init();

    crc16 = crc16_calc(0x0000, test_data, 8);
    PRINTF("crc16_calc() = 0x%04X\n", crc16);
    crc16 = crc16_calc_tbl_fast(0x0000, test_data, 8);
    PRINTF("crc16_calc_tbl_fast() = 0x%04X\n", crc16);
    crc16 = crc16_calc_tbl(0x0000, test_data, 8);
    PRINTF("crc16_calc_tbl() = 0x%04X\n", crc16);
}
#endif

