/*
 * @Author: 吴学富
 * @Date: 2021-11-07 18:39:56
 * @LastEditors: 吴学富
 * @LastEditTime: 2021-11-07 21:53:11
 * @FilePath: \Timer\fun\lcd1602\1602.c
 */
#include "1602.h"

void Write1602_Com(uint8 com)
{
    E=0;
    RS=0;  //命令
    Delay(50);  //延时
    RW=0;  //写
    Delay(50);
    PORT=com;  //端口赋值
    Delay(50);
    E=1;  //高脉冲
    Delay(50);
    E=0;
}

void Write1602_Dat(uint8 dat)
{
    E=0;
    RS=1;  //数据
    Delay(50);  //延时
    RW=0;  //写
    Delay(50);
    PORT=dat;  //端口赋值
    Delay(50);
    E=1;  //高脉冲
    Delay(50);
    E=0;
}

void CheckBusy(void)
{
    uint8 temp;
    RS=0;  //命令
    RW=1;  //读
    E=0;
    while(1)
    {
       PORT=0xFF;  //端口为输入
       E=1;  //高脉冲
       temp=PORT;
       E=0;
       if ((temp&0x80)==0)  //检查BF位是否为0
           break;
    }
}

void init1602(void)
{
    Write1602_Com(SETMODE);  //模式设置
    Delay(500);
    Write1602_Com(DISOPEN);  //显示设置
    Delay(500);
    Write1602_Com(DISMODE);  //显示模式
    Delay(500);
    Write1602_Com(CLEAR);  //清屏
    Delay(500);
}

void Write1602_One_Dat(uint8 x,uint8 y,uint8 dat)
{
    x&=0x0f;
    y&=0x01;
    if(y)
       x|=0x40;
    x|=0x80;
    Write1602_Com(x);
    Write1602_Dat(dat);
}

void Write1602_Str(uint8 addr,uint8 length,uint8 *pbuf)
{
    uint8 i;
    Write1602_Com(addr);
    for(i=0;i < length; i++)
    {
       Write1602_Dat(pbuf[i]);
    }
}