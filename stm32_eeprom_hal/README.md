
# 基于STM32 HAL库的EEPROM模拟


### STM32的EEPROM  
&emsp;&emsp;STM32芯片并没有内置真正的EEPROM,所以储存数据只能通过flash来模拟库来完成，模拟库目的是避免频繁擦写falsh带来的问题，flash一般只有10万次寿命。

### 基于hal库的EEPROM库  
&emsp;&emsp;ST官方提供了一个简易的EEPROM模拟库，但这个库是基于STD外设库的，现在官方提供的方便的外设代码生成软件是基于HAL的，所以如果你用HAL外设库的话，必须对EEPROM库进行改写。
&emsp;&emsp;改写也非常简单，只需在eeprom.c增加一小段代码：

```
#define ENABLE_HAL 1

#if ENABLE_HAL

#define FLASH_COMPLETE 	HAL_OK
#define FLASH_Status	HAL_StatusTypeDef

FLASH_Status FLASH_ErasePage(uint32_t PageAddr)
{
	uint32_t PageError = 0;
	FLASH_EraseInitTypeDef EraseInitstruct =
	{
		.TypeErase = FLASH_TYPEERASE_PAGES,
		.NbPages = 1,
		.PageAddress = PAGE0_BASE_ADDRESS
	};
		
	EraseInitstruct.PageAddress = PageAddr;
	return HAL_FLASHEx_Erase(&EraseInitstruct, &PageError);
}

#define FLASH_ProgramHalfWord(PageAddr, Data) \
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (PageAddr), (Data))

#endif
```

[STM32F0系列官方EEPROM库下载链接](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-library-expansion/stsw-stm32117.html)

[基于hal改写的eeprom库](https://github.com/linrx1511/Projects/tree/master/stm32_eeprom_hal)