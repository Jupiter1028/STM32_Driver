#include "0d96lcd.h"
#include "lcdfont.h"

/*==================================================================
 * 0.96寸 TFT LCD (ST7735) 驱动 - 源文件 (HAL 版)
 *==================================================================*/

/*============================ 初始化 / 底层 ============================*/

/**
  * @brief  SPI 发送一个字节
  * @param  dat 要发送的数据
  * @retval 无
  */
void LCD_Writ_Bus(uint8_t dat)
{
    LCD_CS_Clr();
    HAL_SPI_Transmit(LCD_SPI_HANDLE, &dat, 1, HAL_MAX_DELAY);
    LCD_CS_Set();
}

/**
  * @brief  LCD 写一个字节数据
  * @param  dat 要写入的数据
  * @retval 无
  */
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_Writ_Bus(dat);
}

/**
  * @brief  LCD 写两个字节数据（16位颜色）
  * @param  dat 要写入的数据
  * @retval 无
  */
void LCD_WR_DATA(uint16_t dat)
{
    uint8_t buf[2];
    buf[0] = (uint8_t)(dat >> 8);
    buf[1] = (uint8_t)(dat & 0xFF);
    LCD_CS_Clr();
    HAL_SPI_Transmit(LCD_SPI_HANDLE, buf, 2, HAL_MAX_DELAY);
    LCD_CS_Set();
}

/**
  * @brief  LCD 写一个命令
  * @param  dat 要写入的命令
  * @retval 无
  */
void LCD_WR_REG(uint8_t dat)
{
    LCD_DC_Clr();          /* 写命令 */
    LCD_Writ_Bus(dat);
    LCD_DC_Set();          /* 切回写数据 */
}

/**
  * @brief  设置显示区域起止地址
  * @param  x1,x2 列起止坐标
  * @param  y1,y2 行起止坐标
  * @retval 无
  */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (USE_HORIZONTAL == 0)
    {
        LCD_WR_REG(0x2a); LCD_WR_DATA(x1 + 26); LCD_WR_DATA(x2 + 26);
        LCD_WR_REG(0x2b); LCD_WR_DATA(y1 +  1); LCD_WR_DATA(y2 +  1);
        LCD_WR_REG(0x2c);
    }
    else if (USE_HORIZONTAL == 1)
    {
        LCD_WR_REG(0x2a); LCD_WR_DATA(x1 + 26); LCD_WR_DATA(x2 + 26);
        LCD_WR_REG(0x2b); LCD_WR_DATA(y1 +  1); LCD_WR_DATA(y2 +  1);
        LCD_WR_REG(0x2c);
    }
    else if (USE_HORIZONTAL == 2)
    {
        LCD_WR_REG(0x2a); LCD_WR_DATA(x1 +  1); LCD_WR_DATA(x2 +  1);
        LCD_WR_REG(0x2b); LCD_WR_DATA(y1 + 26); LCD_WR_DATA(y2 + 26);
        LCD_WR_REG(0x2c);
    }
    else
    {
        LCD_WR_REG(0x2a); LCD_WR_DATA(x1 +  1); LCD_WR_DATA(x2 +  1);
        LCD_WR_REG(0x2b); LCD_WR_DATA(y1 + 26); LCD_WR_DATA(y2 + 26);
        LCD_WR_REG(0x2c);
    }
}

/**
  * @brief  LCD 初始化（ST7735 上电序列）
  * @param  无
  * @retval 无
  * @note   GPIO/SPI 由外部初始化，这里只做屏复位与配置
  */
void LCD_Init(void)
{
    /* 硬件复位 */
    LCD_RES_Clr();
    HAL_Delay(100);
    LCD_RES_Set();
    HAL_Delay(100);

    /* 打开背光 */
    LCD_BLK_Set();
    HAL_Delay(100);

    LCD_WR_REG(0x11);     /* Sleep out */
    HAL_Delay(120);

    LCD_WR_REG(0xB1);     /* Normal mode */
    LCD_WR_DATA8(0x05); LCD_WR_DATA8(0x3C); LCD_WR_DATA8(0x3C);
    LCD_WR_REG(0xB2);     /* Idle mode */
    LCD_WR_DATA8(0x05); LCD_WR_DATA8(0x3C); LCD_WR_DATA8(0x3C);
    LCD_WR_REG(0xB3);     /* Partial mode */
    LCD_WR_DATA8(0x05); LCD_WR_DATA8(0x3C); LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x05); LCD_WR_DATA8(0x3C); LCD_WR_DATA8(0x3C);
    LCD_WR_REG(0xB4);     /* Dot inversion */
    LCD_WR_DATA8(0x03);
    LCD_WR_REG(0xC0);     /* AVDD GVDD */
    LCD_WR_DATA8(0xAB); LCD_WR_DATA8(0x0B); LCD_WR_DATA8(0x04);
    LCD_WR_REG(0xC1);     /* VGH VGL */
    LCD_WR_DATA8(0xC5);
    LCD_WR_REG(0xC2);     /* Normal Mode */
    LCD_WR_DATA8(0x0D); LCD_WR_DATA8(0x00);
    LCD_WR_REG(0xC3);     /* Idle */
    LCD_WR_DATA8(0x8D); LCD_WR_DATA8(0x6A);
    LCD_WR_REG(0xC4);     /* Partial+Full */
    LCD_WR_DATA8(0x8D); LCD_WR_DATA8(0xEE);
    LCD_WR_REG(0xC5);     /* VCOM */
    LCD_WR_DATA8(0x0F);
    LCD_WR_REG(0xE0);     /* positive gamma */
    LCD_WR_DATA8(0x07); LCD_WR_DATA8(0x0E); LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x07); LCD_WR_DATA8(0x10); LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x02); LCD_WR_DATA8(0x07); LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x0F); LCD_WR_DATA8(0x25); LCD_WR_DATA8(0x36);
    LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x08); LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x10);
    LCD_WR_REG(0xE1);     /* negative gamma */
    LCD_WR_DATA8(0x0A); LCD_WR_DATA8(0x0D); LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x07); LCD_WR_DATA8(0x0F); LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x02); LCD_WR_DATA8(0x07); LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x0F); LCD_WR_DATA8(0x25); LCD_WR_DATA8(0x35);
    LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x09); LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x10);

    LCD_WR_REG(0xFC);
    LCD_WR_DATA8(0x80);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);   /* 16bit/pixel */

    LCD_WR_REG(0x36);     /* MADCTL */
    if      (USE_HORIZONTAL == 0) LCD_WR_DATA8(0x08);
    else if (USE_HORIZONTAL == 1) LCD_WR_DATA8(0xC8);
    else if (USE_HORIZONTAL == 2) LCD_WR_DATA8(0x78);
    else                          LCD_WR_DATA8(0xA8);

    LCD_WR_REG(0x21);     /* Display inversion */
    LCD_WR_REG(0x29);     /* Display on */

    LCD_WR_REG(0x2A);     /* Column address */
    LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x1A);
    LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x69);

    LCD_WR_REG(0x2B);     /* Page address */
    LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x00); LCD_WR_DATA8(0xA0);

    LCD_WR_REG(0x2C);

    /* 清屏为黑色 */
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
}

/*============================ 基本绘图 ============================*/

/**
  * @brief  在指定区域填充颜色
  * @param  xsta,ysta 起始坐标
  * @param  xend,yend 终止坐标
  * @param  color     要填充的颜色
  * @retval 无
  */
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1);
    for (i = ysta; i < yend; i++)
        for (j = xsta; j < xend; j++)
            LCD_WR_DATA(color);
}

/**
  * @brief  在指定位置画点
  * @param  x,y   坐标
  * @param  color 颜色
  * @retval 无
  */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_WR_DATA(color);
}

/**
  * @brief  画线（Bresenham 算法）
  * @param  x1,y1 起点坐标
  * @param  x2,y2 终点坐标
  * @param  color 颜色
  * @retval 无
  */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if (delta_x > 0) incx = 1;
    else if (delta_x == 0) incx = 0;
    else { incx = -1; delta_x = -delta_x; }

    if (delta_y > 0) incy = 1;
    else if (delta_y == 0) incy = 0;
    else { incy = -1; delta_y = -delta_y; }

    if (delta_x > delta_y) distance = delta_x;
    else                   distance = delta_y;

    for (t = 0; t < distance + 1; t++)
    {
        LCD_DrawPoint(uRow, uCol, color);
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) { xerr -= distance; uRow += incx; }
        if (yerr > distance) { yerr -= distance; uCol += incy; }
    }
}

/**
  * @brief  画矩形
  * @param  x1,y1 起点坐标
  * @param  x2,y2 终点坐标
  * @param  color 颜色
  * @retval 无
  */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

/**
  * @brief  画圆
  * @param  x0,y0 圆心坐标
  * @param  r     半径
  * @param  color 颜色
  * @retval 无
  */
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a = 0, b = r;
    while (a <= b)
    {
        LCD_DrawPoint(x0 - b, y0 - a, color);
        LCD_DrawPoint(x0 + b, y0 - a, color);
        LCD_DrawPoint(x0 - a, y0 + b, color);
        LCD_DrawPoint(x0 - a, y0 - b, color);
        LCD_DrawPoint(x0 + b, y0 + a, color);
        LCD_DrawPoint(x0 + a, y0 - b, color);
        LCD_DrawPoint(x0 + a, y0 + b, color);
        LCD_DrawPoint(x0 - b, y0 + a, color);
        a++;
        if ((a * a + b * b) > (r * r)) b--;
    }
}

/*============================ 汉字显示 ============================*/

/**
  * @brief  显示汉字串
  * @param  x,y   显示坐标
  * @param  s     汉字串（GB2312 编码）
  * @param  fc    字色
  * @param  bc    背景色
  * @param  sizey 字号（12/16/24/32）
  * @param  mode  0=非叠加模式  1=叠加模式
  * @retval 无
  */
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    while (*s != 0)
    {
        if      (sizey == 12) LCD_ShowChinese12x12(x, y, s, fc, bc, sizey, mode);
        else if (sizey == 16) LCD_ShowChinese16x16(x, y, s, fc, bc, sizey, mode);
        else if (sizey == 24) LCD_ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
        else if (sizey == 32) LCD_ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
        else return;
        s += 2;
        x += sizey;
    }
}

/**
  * @brief  显示单个 12x12 汉字
  * @retval 无
  */
void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t  i, j, m = 0;
    uint16_t k;
    uint16_t HZnum, TypefaceNum;
    uint16_t x0 = x;

    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont12) / sizeof(typFNT_GB12);

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode)
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j)) LCD_WR_DATA(fc);
                        else                                   LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) { m = 0; break; }
                    }
                    else
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j)) LCD_DrawPoint(x, y, fc);
                        x++;
                        if ((x - x0) == sizey) { x = x0; y++; break; }
                    }
                }
            }
        }
        continue;
    }
}

/**
  * @brief  显示单个 16x16 汉字
  * @retval 无
  */
void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t  i, j, m = 0;
    uint16_t k;
    uint16_t HZnum, TypefaceNum;
    uint16_t x0 = x;

    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16);

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode)
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j)) LCD_WR_DATA(fc);
                        else                                   LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) { m = 0; break; }
                    }
                    else
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j)) LCD_DrawPoint(x, y, fc);
                        x++;
                        if ((x - x0) == sizey) { x = x0; y++; break; }
                    }
                }
            }
        }
        continue;
    }
}

/**
  * @brief  显示单个 24x24 汉字
  * @retval 无
  */
void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t  i, j, m = 0;
    uint16_t k;
    uint16_t HZnum, TypefaceNum;
    uint16_t x0 = x;

    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24);

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode)
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j)) LCD_WR_DATA(fc);
                        else                                   LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) { m = 0; break; }
                    }
                    else
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j)) LCD_DrawPoint(x, y, fc);
                        x++;
                        if ((x - x0) == sizey) { x = x0; y++; break; }
                    }
                }
            }
        }
        continue;
    }
}

/**
  * @brief  显示单个 32x32 汉字
  * @retval 无
  */
void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t  i, j, m = 0;
    uint16_t k;
    uint16_t HZnum, TypefaceNum;
    uint16_t x0 = x;

    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32);

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode)
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j)) LCD_WR_DATA(fc);
                        else                                   LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) { m = 0; break; }
                    }
                    else
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j)) LCD_DrawPoint(x, y, fc);
                        x++;
                        if ((x - x0) == sizey) { x = x0; y++; break; }
                    }
                }
            }
        }
        continue;
    }
}

/*============================ 字符 / 字符串 / 数字 ============================*/

/**
  * @brief  显示单个 ASCII 字符
  * @param  x,y   显示坐标
  * @param  num   字符（ASCII）
  * @param  fc    字色
  * @param  bc    背景色
  * @param  sizey 字号（12/16/24/32）
  * @param  mode  0=非叠加  1=叠加
  * @retval 无
  */
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t  temp, sizex, t, m = 0;
    uint16_t i, TypefaceNum;
    uint16_t x0 = x;

    sizex = sizey / 2;
    TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num -= ' ';                       /* 转成字库偏移 */
    LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1);

    for (i = 0; i < TypefaceNum; i++)
    {
        if      (sizey == 12) temp = ascii_1206[num][i];
        else if (sizey == 16) temp = ascii_1608[num][i];
        else if (sizey == 24) temp = ascii_2412[num][i];
        else if (sizey == 32) temp = ascii_3216[num][i];
        else return;

        for (t = 0; t < 8; t++)
        {
            if (!mode)
            {
                if (temp & (0x01 << t)) LCD_WR_DATA(fc);
                else                    LCD_WR_DATA(bc);
                m++;
                if (m % sizex == 0) { m = 0; break; }
            }
            else
            {
                if (temp & (0x01 << t)) LCD_DrawPoint(x, y, fc);
                x++;
                if ((x - x0) == sizex) { x = x0; y++; break; }
            }
        }
    }
}

/**
  * @brief  显示字符串
  * @retval 无
  */
void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    while (*p != '\0')
    {
        LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}

/**
  * @brief  求 m 的 n 次幂
  * @retval 结果
  */
uint32_t mypow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--) result *= m;
    return result;
}

/**
  * @brief  显示整数
  * @param  x,y   坐标
  * @param  num   要显示的整数
  * @param  len   显示位数
  * @param  fc/bc 字色/背景色
  * @param  sizey 字号
  * @retval 无
  */
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex  = sizey / 2;

    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
                continue;
            }
            else enshow = 1;
        }
        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/**
  * @brief  显示两位小数
  * @retval 无
  */
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t  t, temp, sizex;
    uint16_t num1;

    sizex = sizey / 2;
    num1  = num * 100;

    for (t = 0; t < len; t++)
    {
        temp = (num1 / mypow(10, len - t - 1)) % 10;
        if (t == (len - 2))
        {
            LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
            t++;
            len += 1;
        }
        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/*============================ 图片显示 ============================*/

/**
  * @brief  显示 RGB565 图片
  * @param  x,y         起点坐标
  * @param  length,width 图片长宽
  * @param  pic         图片数据（小端字节流）
  * @retval 无
  */
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[])
{
    uint16_t i, j;
    uint32_t k = 0;
    LCD_Address_Set(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < width; j++)
        {
            LCD_WR_DATA8(pic[k * 2]);
            LCD_WR_DATA8(pic[k * 2 + 1]);
            k++;
        }
    }
}