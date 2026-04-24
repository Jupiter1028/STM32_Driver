#include "in_flash.h"
#include "stm32f2xx_hal.h"

#define FLASH_TIMEOUT_MS  1000U  // 操作超时时间（单位：毫秒）

// 内部函数：等待上一次操作完成
static FlashStatus WaitForLastOperation(void);

// 初始化 Flash（解锁）
FlashStatus InternalFlash_Init(void) {
    HAL_FLASH_Unlock();
    return FLASH_OK;
}

// 擦除指定扇区（Sector 5）
FlashStatus InternalFlash_EraseSector(void) {
    FLASH_EraseInitTypeDef EraseInit;
    uint32_t SectorError;

    EraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInit.Sector       = USER_FLASH_SECTOR;
    EraseInit.NbSectors    = 1;
    EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&EraseInit, &SectorError) != HAL_OK) {
        return FLASH_ERR;
    }

    return WaitForLastOperation();
}

// 写入数据到 Flash
//FlashStatus InternalFlash_Write(uint32_t offset, const uint8_t *data, uint32_t len) {
//    if ((offset % 4) != 0 || (len % 4) != 0 || (offset + len) > USER_FLASH_SIZE) {
//        return FLASH_PARAM;
//    }

//    FlashStatus status = FLASH_OK;
//    uint32_t address = USER_FLASH_BASE_ADDR + offset;

//    HAL_FLASH_Unlock();

//    for (uint32_t i = 0; i < len; i += 4) {
//        // 构造一个 uint32_t，确保字节顺序一致（小端）
//        uint32_t word =  (uint32_t)data[i] |
//                        ((uint32_t)data[i+1] << 8) |
//                        ((uint32_t)data[i+2] << 16) |
//                        ((uint32_t)data[i+3] << 24);

//        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, word) != HAL_OK) {
//            status = FLASH_ERR;
//            break;
//        }

//        if ((status = WaitForLastOperation()) != FLASH_OK) {
//            break;
//        }

//        address += 4;
//    }

//    HAL_FLASH_Lock();
//    return status;
//}

FlashStatus InternalFlash_Write(uint32_t offset, const uint8_t *data, uint32_t len) {
    // offset 必须 4 字节对齐
    if ((offset % 4) != 0 || (offset + len) > USER_FLASH_SIZE) {
        // 增加打印，看看谁越界了
        //printf("Param Error: off:%d, len:%d, max:%d\r\n", offset, len, USER_FLASH_SIZE);
        return FLASH_PARAM;
    }

    FlashStatus status = FLASH_OK;
    uint32_t address = USER_FLASH_BASE_ADDR + offset;
    HAL_FLASH_Unlock();

    // 关键修正：i 依然按照 4 字节累加，但读取数据时要做边界保护
    for (uint32_t i = 0; i < len; i += 4) {
        uint32_t word = 0xFFFFFFFF; // 默认填充 0xFF
        
        // 只有剩余数据足够 4 字节时才正常取值，否则逐字节拷贝防止内存越界
        uint8_t temp[4] = {0xFF, 0xFF, 0xFF, 0xFF};
        uint32_t bytes_to_copy = ((len - i) >= 4) ? 4 : (len - i);
        
        for(uint32_t j = 0; j < bytes_to_copy; j++) {
            temp[j] = data[i + j];
        }
        
        word = (uint32_t)temp[0] | ((uint32_t)temp[1] << 8) | 
               ((uint32_t)temp[2] << 16) | ((uint32_t)temp[3] << 24);

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, word) != HAL_OK) {
            status = FLASH_ERR;
            break;
        }
        address += 4;
    }

    HAL_FLASH_Lock();
    return status;
}


// 读取数据从 Flash
void InternalFlash_Read(uint32_t offset, uint8_t *buffer, uint32_t len) {
    const uint8_t *src = (const uint8_t *)(USER_FLASH_BASE_ADDR + offset);
    for (uint32_t i = 0; i < len; i++) {
        buffer[i] = src[i];
    }
}


// 获取 Flash 的起始地址
uint32_t InternalFlash_GetBaseAddr(void) {
    return USER_FLASH_BASE_ADDR;
}

#define FLASH_FLAG_ALL_ERRORS_F2  (FLASH_FLAG_PGSERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_WRPERR | FLASH_FLAG_OPERR)



// 等待 Flash 操作完成
static FlashStatus WaitForLastOperation(void) {
    uint32_t tickstart = HAL_GetTick();

    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)) {
        if ((HAL_GetTick() - tickstart) > FLASH_TIMEOUT_MS) {
            return FLASH_TIMEOUT;
        }
    }

    // 检查错误标志位
    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_ALL_ERRORS_F2)) {
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_F2);
    return FLASH_ERR;
}

    return FLASH_OK;
}
