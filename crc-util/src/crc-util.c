/*
 ============================================================================
 Name        : crc-util.c
 Author      : linrx
 Version     :
 Copyright   : 
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void test_crc4(void);
void test_crc8(void);
void test_crc16(void);
void test_crc32(void);

int main(void)
{
    printf("test crc4...\n");
    test_crc4();

    printf("\ntest crc8...\n");
    test_crc8();

    printf("\ntest crc16...\n");
    test_crc16();

    printf("\ntest crc32...\n");
    test_crc32();

    getchar();
    
    return EXIT_SUCCESS;
}
