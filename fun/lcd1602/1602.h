/*
 * @Author: 吴学富
 * @Date: 2021-11-07 18:39:30
 * @LastEditors: 吴学富
 * @LastEditTime: 2021-11-07 21:53:14
 * @FilePath: \Timer\fun\lcd1602\1602.h
 */
#ifndef __1602_H__
#define __1602_H__

#include "reg51.h"
#include "..\Type.h"  //变量类型
#include "..\fun.h"  //常用函数 

#define SETMODE  0x38  //16*2显示,5*7点阵,8位数据接口
#define DISOPEN  0x0C  //显示开,不显示光标,光标不闪烁
#define DISMODE  0x06  //读写字符后地址加1,屏显不移动
#define SETADDR  0x80  //设置数据地址指针初始值
#define CLEAR  0x01  //清屏,数据指针清零
#define RET  0x02  //回车,数据指针清零
#define PORT  P2  //I/O口

sbit RS = P1^0;
sbit RW = P1^1;
sbit E = P1^2;

void init1602(void);  //初始化1602
void Write1602_Com(uint8 com);  //写命令
void Write1602_Dat(uint8 dat);  //写数据
void CheckBusy(void);  //检查忙
void Write1602_One_Dat(uint8 X,uint8 Y,uint8 dat);//写一个数据
void Write1602_Str(uint8 addr,uint8 length,uint8 *pbuf);//写一个数据串
#endif//
