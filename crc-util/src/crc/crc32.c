/**
 ******************************************************************************
 * @file    crc32.c
 * @author  linrx
 * @version 1.0
 * @date    2017-9-21
 * @brief
 ******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "crc32.h"

#define DEBUG 1
#if DEBUG
#define PRINTF(...)     printf(__VA_ARGS__);fflush(stdout)
#else
#define PRINTF(...)
#endif

static uint32_t crc32_table[256];
static uint32_t crc32_table_tiny[16];

#define CRC32_POLY_REFLECT   (((CRC32_POLY & 0x80000000) >> 31) | ((CRC32_POLY & 0x40000000) >> 29)\
                            | ((CRC32_POLY & 0x20000000) >> 27) | ((CRC32_POLY & 0x10000000) >> 25)\
                            | ((CRC32_POLY & 0x08000000) >> 23) | ((CRC32_POLY & 0x04000000) >> 21)\
                            | ((CRC32_POLY & 0x02000000) >> 19) | ((CRC32_POLY & 0x01000000) >> 17)\
                            | ((CRC32_POLY & 0x00800000) >> 15) | ((CRC32_POLY & 0x00400000) >> 13)\
                            | ((CRC32_POLY & 0x00200000) >> 11) | ((CRC32_POLY & 0x00100000) >> 9)\
                            | ((CRC32_POLY & 0x00080000) >> 7)  | ((CRC32_POLY & 0x00040000) >> 5)\
                            | ((CRC32_POLY & 0x00020000) >> 3)  | ((CRC32_POLY & 0x00010000) >> 1)\
                            | ((CRC32_POLY & 0x00008000) << 1)  | ((CRC32_POLY & 0x00004000) << 3)\
                            | ((CRC32_POLY & 0x00002000) << 5)  | ((CRC32_POLY & 0x00001000) << 7)\
                            | ((CRC32_POLY & 0x00000800) << 9)  | ((CRC32_POLY & 0x00000400) << 11)\
                            | ((CRC32_POLY & 0x00000200) << 13) | ((CRC32_POLY & 0x00000100) << 15)\
                            | ((CRC32_POLY & 0x00000080) << 17) | ((CRC32_POLY & 0x00000040) << 19)\
                            | ((CRC32_POLY & 0x00000020) << 21) | ((CRC32_POLY & 0x00000010) << 23)\
                            | ((CRC32_POLY & 0x00000008) << 25) | ((CRC32_POLY & 0x00000004) << 27)\
                            | ((CRC32_POLY & 0x00000002) << 29) | ((CRC32_POLY & 0x00000001) << 31)\
)

#if CRC32_REFLECT == 0
static
void crc32_talbe_init(void)
{
    int i, j;

    for (i = 0; i < 256; i++)
    {
        crc32_table[i] = (uint32_t)i << 24;
        for (j = 0; j < 8; j++)
        {
            if (crc32_table[i] & 0x80000000)
            {
                crc32_table[i] = (crc32_table[i] << 1) ^ CRC32_POLY;
            }
            else
            {
                crc32_table[i] <<= 1;
            }
        }
    }

    PRINTF("crc32_table[] =\n{\n    ");
    for (i = 0; i < 255;)
    {
        PRINTF("0x%08X,", crc32_table[i]);
        if ((++i % 16) == 0)
        {
            PRINTF("\n    ");
        }
    }
    PRINTF("0x%08X\n", crc32_table[i]);
    PRINTF("};\n");
}

static
void crc32_talbe_tiny_init(void)
{
    int i, j;

    for (i = 0; i < 16; i++)
    {
        crc32_table_tiny[i] = (uint32_t)i << 24;
        for (j = 0; j < 8; j++)
        {
            if (crc32_table_tiny[i] & 0x80000000)
            {
                crc32_table_tiny[i] = (crc32_table_tiny[i] << 1) ^ CRC32_POLY;
            }
            else
            {
                crc32_table_tiny[i] <<= 1;
            }
        }
    }

    PRINTF("crc32_table_tiny[] =\n{\n    ");
    for (i = 0; i < 15; i++)
    {
        PRINTF("0x%08X,", crc32_table_tiny[i]);
    }
    PRINTF("0x%08X\n", crc32_table_tiny[i]);
    PRINTF("};\n");
}

uint32_t crc32_calc(uint32_t crc_init, const uint8_t *data, uint16_t len)
{
    uint32_t crc = crc_init;
    uint8_t i;

    while (len--)
    {
        crc ^= ((uint32_t)*data++ << 24);
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80000000)
            {
                crc = (crc << 1) ^ CRC32_POLY;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc ^ CRC32_XOR;
}

uint32_t crc32_calc_tbl_fast(uint32_t crc_init, const uint8_t *data, uint16_t len)
{
    uint32_t crc = crc_init;

    while (len--)
    {
        crc = crc32_table[(uint8_t)(crc >> 24) ^ *data++] ^ (crc << 8);
    }

    return crc ^ CRC32_XOR;
}

uint32_t crc32_calc_tbl(uint32_t crc_init, const uint8_t *data, uint16_t len)
{
    uint32_t crc = crc_init;
    uint8_t c;

    while (len--)
    {
        c = *data++;
        crc = crc32_table_tiny[((c >> 4) ^ (crc >> 28))] ^ (crc << 4);
        crc = crc32_table_tiny[(c & 0x0f) ^ (crc >> 28)] ^ (crc << 4);
    }

    return crc ^ CRC32_XOR;
}
#else
static
void crc32_talbe_init(void)
{
    int i, j;

    for (i = 0; i < 256; i++)
    {
        crc32_table[i] = i;
        for (j = 0; j < 8; j++)
        {
            if (crc32_table[i] & 0x00000001)
            {
                crc32_table[i] = (crc32_table[i] >> 1) ^ CRC32_POLY_REFLECT;
            }
            else
            {
                crc32_table[i] >>= 1;
            }
        }
    }

    PRINTF("crc32_table[] =\n{\n    ");
    for (i = 0; i < 255;)
    {
        PRINTF("0x%08X,", crc32_table[i]);
        if ((++i % 16) == 0)
        {
            PRINTF("\n    ");
        }
    }
    PRINTF("0x%08X\n", crc32_table[i]);
    PRINTF("};\n");
}

static
void crc32_talbe_tiny_init(void)
{
    int i, j;

    for (i = 0; i < 16; i++)
    {
        crc32_table_tiny[i] = i << 4;
        for (j = 0; j < 8; j++)
        {
            if (crc32_table_tiny[i] & 0x00000001)
            {
                crc32_table_tiny[i] = (crc32_table_tiny[i] >> 1) ^ CRC32_POLY_REFLECT;
            }
            else
            {
                crc32_table_tiny[i] >>= 1;
            }
        }
    }

    PRINTF("crc32_table_tiny[] =\n{\n    ");
    for (i = 0; i < 15; i++)
    {
        PRINTF("0x%08X,", crc32_table_tiny[i]);
    }
    PRINTF("0x%08X\n", crc32_table_tiny[i]);
    PRINTF("};\n");
}

uint32_t crc32_calc(uint32_t crc_init, const uint8_t *data, uint16_t len)
{
    uint32_t crc = crc_init;
    uint8_t i;

    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x00000001)
            {
                crc = (crc >> 1) ^ CRC32_POLY_REFLECT;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc ^ CRC32_XOR;
}

uint32_t crc32_calc_tbl_fast(uint32_t crc_init, const uint8_t *data, uint16_t len)
{
    uint32_t crc = crc_init;

    while (len--)
    {
        crc = crc32_table[(uint8_t)crc ^ *data++] ^ (crc >> 8);
    }

    return crc ^ CRC32_XOR;
}

uint32_t crc32_calc_tbl(uint32_t crc_init, const uint8_t *data, uint16_t len)
{
    uint32_t crc = crc_init;
    uint8_t c;

    while (len--)
    {
        c = *data++;
        crc = crc32_table_tiny[(c ^ crc) & 0x0000000f] ^ (crc >> 4);
        crc = crc32_table_tiny[((c >> 4) ^ crc) & 0x0000000f] ^ (crc >> 4);
    }

    return crc ^ CRC32_XOR;
}
#endif


#if DEBUG
/* 测试例程 ，表格可以计算后放到const中减少占用RAM */
void test_crc32(void)
{
    uint32_t crc32;
    const uint8_t test_data[8] =
            { 1, 2, 3, 4, 5, 6, 7, 8 };

    crc32_talbe_init();
    crc32_talbe_tiny_init();

    crc32 = crc32_calc(0xffffffff, test_data, 8);
    PRINTF("crc32_calc() = 0x%08X\n", crc32);
    crc32 = crc32_calc_tbl_fast(0xffffffff, test_data, 8);
    PRINTF("crc32_calc_tbl_fast() = 0x%08X\n", crc32);
    crc32 = crc32_calc_tbl(0xffffffff, test_data, 8);
    PRINTF("crc32_calc_tbl() = 0x%08X\n", crc32);
}
#endif

