#include "stdio.h"
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	return ch;
}

void PrintfData(uint8_t *data, uint8_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
        if(i > 0 && i % 10 == 0) printf("\r\n");
        printf("%02X ", data[i]);
    }
    printf("\r\n");
}

#define LOG(msg) printf("[%s %s] [%s:%d] %s: %s\n", \
                 __DATE__, __TIME__, __FILE__, __LINE__, __func__, msg)
// 调用：LOG("Connection failed");
// 输出：[Mar 27 2026 10:45:00] [main.c:42] main: Connection failed