/*
 * @Author: 吴学富
 * @Date: 2021-11-07 21:18:57
 * @LastEditors: 吴学富
 * @LastEditTime: 2021-11-07 21:21:17
 * @FilePath: \Timer\main\_main.h
 */
#ifndef ___MAIN_H__
#define ___MAIN_H__

#include <reg51.h>
#include "..\fun\lcd1602\1602.h"
#include "..\fun\fun.h"
#include"..\fun\Type.h"
#include "string.h"

sbit BUZZER = P3 ^ 0;
sbit LED = P3 ^ 1;

void dl_ms();
void display();
void updataDisBuf();
void init();
void alarm();
#endif