void I2C_Scan(void)
{
    printf("Scanning I2C bus...\r\n");

    HAL_StatusTypeDef result;
    uint8_t i;
    int found = 0;

    for (i = 1; i < 128; i++)
    {
        result = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i << 1), 1, 10);
        if (result == HAL_OK)
        {
            printf("I2C device found at address 0x%02X\r\n", i);
            found = 1;
        }
    }
		

    if (!found)
    {
        printf("No I2C devices found!\r\n");
    }
    else
    {
        printf("Scan finished.\r\n");
    }
}

#define _I2C_READY                      (0x01)
#define _I2C_ADDRESS                    (0x24 << 1)
#define _I2C_CHANNEL                    (1)
/**************************************************************************
 * I2C
 **************************************************************************/
int PN532_I2C_ReadData(uint8_t* data, uint16_t count) {
    uint8_t status = 0;
    // 轮询 READY
    if (HAL_I2C_Master_Receive(&hi2c1, _I2C_ADDRESS, &status, 1, 100) != HAL_OK)
        return PN532_STATUS_ERROR;
    if (status != _I2C_READY) return PN532_STATUS_ERROR;

    uint8_t frame[count + 1];
    if (HAL_I2C_Master_Receive(&hi2c1, _I2C_ADDRESS, frame, count + 1, 100) != HAL_OK)
        return PN532_STATUS_ERROR;

    for (uint16_t i = 0; i < count; i++) {
        data[i] = frame[i + 1];
    }
    return PN532_STATUS_OK;
}

int PN532_I2C_WriteData(uint8_t *data, uint16_t count) {
    // 数据帧前不需要加额外的控制字
    if (HAL_I2C_Master_Transmit(&hi2c1, _I2C_ADDRESS, data, count, 100) != HAL_OK)
        return PN532_STATUS_ERROR;
    return PN532_STATUS_OK;
}

bool PN532_I2C_WaitReady(uint32_t timeout) {
    uint32_t tickstart = HAL_GetTick();
    uint8_t status = 0;

    while ((HAL_GetTick() - tickstart) < timeout) {
        if (HAL_I2C_Master_Receive(&hi2c1, _I2C_ADDRESS, &status, 1, 10) == HAL_OK) {
            if (status == _I2C_READY)
                return true;
        }
        HAL_Delay(5);
    }
    return false;  // 超时
}

int PN532_I2C_Wakeup(void) {
    HAL_GPIO_WritePin(_REQ_PORT, _REQ_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(_REQ_PORT, _REQ_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(_REQ_PORT, _REQ_PIN, GPIO_PIN_SET);
    HAL_Delay(50);
    return PN532_STATUS_OK;
}

void PN532_I2C_Init(PN532* pn532) {
    // 初始化函数指针
    pn532->reset = PN532_Reset;
    pn532->read_data = PN532_I2C_ReadData;
    pn532->write_data = PN532_I2C_WriteData;
    pn532->wait_ready = PN532_I2C_WaitReady;
    pn532->wakeup = PN532_I2C_Wakeup;
    pn532->log = PN532_Log;

    // 硬件重置
    pn532->reset();
    // 唤醒
    pn532->wakeup();
}