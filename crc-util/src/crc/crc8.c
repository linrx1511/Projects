/**
 ******************************************************************************
 * @file    crc8.c
 * @author  linrx
 * @version 1.0
 * @date    2017-9-16
 * @brief
 ******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "crc8.h"

#define DEBUG 1
#if DEBUG
#define PRINTF(...)     printf(__VA_ARGS__);fflush(stdout)
#else
#define PRINTF(...)
#endif

static uint8_t crc8_table[256];
static uint8_t crc8_table_tiny[16];

#define CRC8_POLY_REFLECT   (((CRC8_POLY & 0x80) >> 7) | ((CRC8_POLY & 0x40) >> 5)\
                            | ((CRC8_POLY & 0x20) >> 3) | ((CRC8_POLY & 0x10) >> 1)\
                            | ((CRC8_POLY & 0x08) << 1) | ((CRC8_POLY & 0x04) << 3)\
                            | ((CRC8_POLY & 0x02) << 5) | ((CRC8_POLY & 0x01) << 7))

#if CRC8_REFLECT == 0
static
void crc8_talbe_init(void)
{
    int i, j;

    for (i = 0; i < 256; i++)
    {
        crc8_table[i] = i;
        for (j = 0; j < 8; j++)
        {
            if (crc8_table[i] & 0x80)
            {
                crc8_table[i] = (crc8_table[i] << 1) ^ CRC8_POLY;
            }
            else
            {
                crc8_table[i] <<= 1;
            }
        }
    }

    PRINTF("crc8_table[] =\n{\n    ");
    for (i = 0; i < 255;)
    {
        PRINTF("0x%02X,", crc8_table[i]);
        if ((++i % 16) == 0)
        {
            PRINTF("\n    ");
        }
    }
    PRINTF("0x%02X\n", crc8_table[i]);
    PRINTF("};\n");
}

static
void crc8_talbe_tiny_init(void)
{
    int i, j;

    for (i = 0; i < 16; i++)
    {
        crc8_table_tiny[i] = i;
        for (j = 0; j < 8; j++)
        {
            if (crc8_table_tiny[i] & 0x80)
            {
                crc8_table_tiny[i] = (crc8_table_tiny[i] << 1) ^ CRC8_POLY;
            }
            else
            {
                crc8_table_tiny[i] <<= 1;
            }
        }
    }

    PRINTF("crc8_table_tiny[] =\n{\n    ");
    for (i = 0; i < 15; i++)
    {
        PRINTF("0x%02X,", crc8_table_tiny[i]);
    }
    PRINTF("0x%02X\n", crc8_table_tiny[i]);
    PRINTF("};\n");
}

uint8_t crc8_calc(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;
    uint8_t i;

    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ CRC8_POLY;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc ^ CRC8_XOR;
}

uint8_t crc8_calc_tbl_fast(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;

    while (len--)
    {
        crc = crc8_table[crc ^ *data++];
    }

    return crc ^ CRC8_XOR;
}

uint8_t crc8_calc_tbl(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;
    uint8_t c;

    while (len--)
    {
        c = *data++;
        crc = crc8_table_tiny[(c ^ crc) >> 4] ^ (crc << 4);
        crc = crc8_table_tiny[(c & 0x0f) ^ (crc >> 4)] ^ (crc << 4);
    }

    return crc ^ CRC8_XOR;
}
#else
static
void crc8_talbe_init(void)
{
    int i, j;

    for (i = 0; i < 256; i++)
    {
        crc8_table[i] = i;
        for (j = 0; j < 8; j++)
        {
            if (crc8_table[i] & 0x01)
            {
                crc8_table[i] = (crc8_table[i] >> 1) ^ CRC8_POLY_REFLECT;
            }
            else
            {
                crc8_table[i] >>= 1;
            }
        }
    }

    PRINTF("crc8_table[] =\n{\n    ");
    for (i = 0; i < 255;)
    {
        PRINTF("0x%02X,", crc8_table[i]);
        if ((++i % 16) == 0)
        {
            PRINTF("\n    ");
        }
    }
    PRINTF("0x%02X\n", crc8_table[i]);
    PRINTF("};\n");
}

static
void crc8_talbe_tiny_init(void)
{
    int i, j;

    for (i = 0; i < 16; i++)
    {
        crc8_table_tiny[i] = i << 4;
        for (j = 0; j < 8; j++)
        {
            if (crc8_table_tiny[i] & 0x01)
            {
                crc8_table_tiny[i] = (crc8_table_tiny[i] >> 1) ^ CRC8_POLY_REFLECT;
            }
            else
            {
                crc8_table_tiny[i] >>= 1;
            }
        }
    }

    PRINTF("crc8_table_tiny[] =\n{\n    ");
    for (i = 0; i < 15; i++)
    {
        PRINTF("0x%02X,", crc8_table_tiny[i]);
    }
    PRINTF("0x%02X\n", crc8_table_tiny[i]);
    PRINTF("};\n");
}

uint8_t crc8_calc(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;
    uint8_t i;

    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x01)
            {
                crc = (crc >> 1) ^ CRC8_POLY_REFLECT;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc ^ CRC8_XOR;
}

uint8_t crc8_calc_tbl_fast(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;

    while (len--)
    {
        crc = crc8_table[crc ^ *data++];
    }

    return crc ^ CRC8_XOR;
}

uint8_t crc8_calc_tbl(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;
    uint8_t c;

    while (len--)
    {
        c = *data++;
        crc = crc8_table_tiny[(c ^ crc) & 0x0f] ^ (crc >> 4);
        crc = crc8_table_tiny[((c >> 4) ^ crc) & 0x0f] ^ (crc >> 4);
    }

    return crc ^ CRC8_XOR;
}
#endif


#if DEBUG
/* 测试例程 ，表格可以计算后放到const中减少占用RAM */
void test_crc8(void)
{
    uint8_t crc8;
    const uint8_t test_data[8] =
            { 1, 2, 3, 4, 5, 6, 7, 8 };

    crc8_talbe_init();
    crc8_talbe_tiny_init();

    crc8 = crc8_calc(0x00, test_data, 8);
    PRINTF("crc8_calc() = 0x%02X\n", crc8);
    crc8 = crc8_calc_tbl_fast(0x00, test_data, 8);
    PRINTF("crc8_calc_tbl_fast() = 0x%02X\n", crc8);
    crc8 = crc8_calc_tbl(0x00, test_data, 8);
    PRINTF("crc8_calc_tbl() = 0x%02X\n", crc8);
}
#endif

