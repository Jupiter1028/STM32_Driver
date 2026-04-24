#ifndef IN_FLASH_H
#define IN_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define USER_FLASH_SECTOR      		FLASH_SECTOR_1
#define USER_FLASH_BASE_ADDR   			0x08004000
#define USER_FLASH_SIZE           (448 * 1024)  //用户空间大小

// Flash 操作返回状态
typedef enum {
    FLASH_OK = 0,
    FLASH_ERR,
    FLASH_BUSY,
    FLASH_TIMEOUT,
    FLASH_PARAM
} FlashStatus;

// 初始化 Flash（解锁）
FlashStatus InternalFlash_Init(void);

// 擦除用户指定的扇区
FlashStatus InternalFlash_EraseSector(void);

// 写入数据到 Flash（offset 相对 USER_FLASH_BASE_ADDR）
FlashStatus InternalFlash_Write(uint32_t offset, const uint8_t *data, uint32_t len);

// 从 Flash 读取数据（offset 相对 USER_FLASH_BASE_ADDR）
void InternalFlash_Read(uint32_t offset, uint8_t *buffer, uint32_t len);

// 获取 Flash 的基地址（USER_FLASH_BASE_ADDR）
uint32_t InternalFlash_GetBaseAddr(void);

#ifdef __cplusplus
}
#endif

#endif // IN_FLASH_H
