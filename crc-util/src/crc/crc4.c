/**
 ******************************************************************************
 * @file    crc4.c
 * @author  linrx
 * @version 1.0
 * @date    2017-9-16
 * @brief
 ******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "crc4.h"

#define DEBUG 1
#if DEBUG
#define PRINTF(...)     printf(__VA_ARGS__);fflush(stdout)
#else
#define PRINTF(...)
#endif

static uint8_t crc4_table[256];
static uint8_t crc4_table_tiny[16];

#define CRC4_POLY_REFLECT   (((CRC4_POLY & 0x08) >> 3) | ((CRC4_POLY & 0x04) >> 1)\
                            | ((CRC4_POLY & 0x02) << 1) | ((CRC4_POLY & 0x01) << 3))

#if CRC4_REFLECT == 0
static
void crc4_talbe_init(void)
{
    int i, j;

    for (i = 0; i < 256; i++)
    {
        crc4_table[i] = i;
        for (j = 0; j < 8; j++)
        {
            if (crc4_table[i] & 0x80)
            {
                crc4_table[i] = (crc4_table[i] << 1) ^ CRC4_POLY;
            }
            else
            {
                crc4_table[i] <<= 1;
            }
        }
    }

    PRINTF("crc4_table[] =\n{\n    ");
    for (i = 0; i < 255;)
    {
        PRINTF("0x%02X,", crc4_table[i]);
        if ((++i % 16) == 0)
        {
            PRINTF("\n    ");
        }
    }
    PRINTF("0x%02X\n", crc4_table[i]);
    PRINTF("};\n");
}

static
void crc4_talbe_tiny_init(void)
{
    int i, j;

    for (i = 0; i < 16; i++)
    {
        crc4_table_tiny[i] = i;
        for (j = 0; j < 8; j++)
        {
            if (crc4_table_tiny[i] & 0x80)
            {
                crc4_table_tiny[i] = (crc4_table_tiny[i] << 1) ^ CRC4_POLY;
            }
            else
            {
                crc4_table_tiny[i] <<= 1;
            }
        }
    }

    PRINTF("crc4_table_tiny[] =\n{\n    ");
    for (i = 0; i < 15; i++)
    {
        PRINTF("0x%02X,", crc4_table_tiny[i]);
    }
    PRINTF("0x%02X\n", crc4_table_tiny[i]);
    PRINTF("};\n");
}

uint8_t crc4_calc(uint8_t crc_init, const uint8_t *data, uint16_t len)
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
                crc = (crc << 1) ^ CRC4_POLY;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc ^ CRC4_XOR;
}

uint8_t crc4_calc_tbl_fast(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;

    while (len--)
    {
        crc = crc4_table[crc ^ *data++];
    }

    return crc ^ CRC4_XOR;
}

uint8_t crc4_calc_tbl(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;
    uint8_t c;

    while (len--)
    {
        c = *data++;
        crc = crc4_table_tiny[(c ^ (crc >> 4)] ^ (crc << 4);
        crc = crc4_table_tiny[(c & 0x0f) ^ (crc >> 4)] ^ (crc << 4);
    }

    return crc ^ CRC4_XOR;
}
#else
static
void crc4_talbe_init(void)
{
    int i, j;

    for (i = 0; i < 256; i++)
    {
        crc4_table[i] = i;
        for (j = 0; j < 8; j++)
        {
            if (crc4_table[i] & 0x01)
            {
                crc4_table[i] = (crc4_table[i] >> 1) ^ CRC4_POLY_REFLECT;
            }
            else
            {
                crc4_table[i] >>= 1;
            }
        }
    }

    PRINTF("crc4_table[] =\n{\n    ");
    for (i = 0; i < 255;)
    {
        PRINTF("0x%02X,", crc4_table[i]);
        if ((++i % 16) == 0)
        {
            PRINTF("\n    ");
        }
    }
    PRINTF("0x%02X\n", crc4_table[i]);
    PRINTF("};\n");
}

static
void crc4_talbe_tiny_init(void)
{
    int i, j;

    for (i = 0; i < 16; i++)
    {
        crc4_table_tiny[i] = i << 4;
        for (j = 0; j < 8; j++)
        {
            if (crc4_table_tiny[i] & 0x01)
            {
                crc4_table_tiny[i] = (crc4_table_tiny[i] >> 1) ^ CRC4_POLY_REFLECT;
            }
            else
            {
                crc4_table_tiny[i] >>= 1;
            }
        }
    }

    PRINTF("crc4_table_tiny[] =\n{\n    ");
    for (i = 0; i < 15; i++)
    {
        PRINTF("0x%02X,", crc4_table_tiny[i]);
    }
    PRINTF("0x%02X\n", crc4_table_tiny[i]);
    PRINTF("};\n");
}

uint8_t crc4_calc(uint8_t crc_init, const uint8_t *data, uint16_t len)
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
                crc = (crc >> 1) ^ CRC4_POLY_REFLECT;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc ^ CRC4_XOR;
}

uint8_t crc4_calc_tbl_fast(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;

    while (len--)
    {
        crc = crc4_table[crc ^ *data++];
    }

    return crc ^ CRC4_XOR;
}

uint8_t crc4_calc_tbl(uint8_t crc_init, const uint8_t *data, uint16_t len)
{
    uint8_t crc = crc_init;
    uint8_t c;

    while (len--)
    {
        c = *data++;
        crc = crc4_table_tiny[(c ^ crc) & 0x0f] ^ (crc >> 4);
        crc = crc4_table_tiny[((c >> 4) ^ crc) & 0x0f] ^ (crc >> 4);
    }

    return crc ^ CRC4_XOR;
}
#endif


#if DEBUG
/* 测试例程 ，表格可以计算后放到const中减少占用RAM */
void test_crc4(void)
{
    uint8_t crc4;
    const uint8_t test_data[8] =
            { 1, 2, 3, 4, 5, 6, 7, 8 };

    crc4_talbe_init();
    crc4_talbe_tiny_init();

    crc4 = crc4_calc(0x00, test_data, 8);
    PRINTF("crc4_calc() = 0x%02X\n", crc4);
    crc4 = crc4_calc_tbl_fast(0x00, test_data, 8);
    PRINTF("crc4_calc_tbl_fast() = 0x%02X\n", crc4);
    crc4 = crc4_calc_tbl(0x00, test_data, 8);
    PRINTF("crc4_calc_tbl() = 0x%02X\n", crc4);
}
#endif

