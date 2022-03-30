/*
 * @Author: 吴学富
 * @Date: 2021-11-07 19:06:47
 * @LastEditors: 吴学富
 * @LastEditTime: 2021-11-07 22:58:28
 * @FilePath: \Timer\main\main.c
 */

#include "_main.h"

uint8 data dis_buf[] = "00:00:00   Timer";//显示缓存，时-分-秒

uint8 data dis_set[]="Set time";
uint8 data dis_timeing[]="Timing";
uint8 data dis_ultimate[] = "Timer's up";
uint8 data dis_pause[] = "Paused";

int8 data time[3] = {0, 0, 0};//秒，分，时 取值范围:-1~60

uint8 data i = 0, j = 0;//计数次数 取值范围：0~20

int8 data setting_i = 3;//时间位 取值范围:-1~3

bit end = 0;     //结束状态
bit runing = 0;  //计时状态
bit setting = 0; //设置状态

void main()
{

    init();
    while (1)
    {
        display();
        if (end == 1)
        {
            alarm();
        }
    }
}

/* **************************************
 * Function:dl_ms()
 * Description:延迟函数
 * *************************************/
void dl_ms()
{
    uint32 k;
    for (k = 0; k < 130; k++);
}

/* **************************************
 * Function:display()
 * Description:lcd1602显示函数(数字部分)
 * Calls : 
 * *************************************/
void display() //显示函数
{
    Write1602_Str(0x80,strlen(dis_buf),dis_buf);
}

/* **************************************
 * Function:updataDisBuf()
 * Description:更新显示缓存
 * input:time[]
 * output"dis_buf[]
 * *************************************/
void updataDisBuf()
{
    //秒赋值
    dis_buf[7] = '0'+ time[0] % 10;
    dis_buf[6] = '0'+ time[0] / 10;

    //分赋值
    dis_buf[4] = '0'+ time[1] % 10;
    dis_buf[3] = '0'+ time[1] / 10;

    //时赋值
    dis_buf[1] = '0'+ time[2] % 10;
    dis_buf[0] = '0'+ time[2] / 10;
}

/* **************************************
 * Function : init()
 * Description : 初始化各个寄存器
 * input : TMOD, THx, TLx, LED, IE, IT0, IT1
 * *************************************/
void init()
{
    TMOD = 0x11;

    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;

    TH1 = (65536 - 50000) / 256;
    TL1 = (65536 - 50000) % 256;

    LED = 0;

    IE = 0x8f;
    IT0 = 1;
    IT1 = 1;

    init1602();  //初始化1602
}

/* **************************************
 * Function : alarm()
 * Description : 蜂鸣器响、绿色LED灯闪烁
 * Calls : dl_ms()
 * *************************************/
void alarm()
{
    BUZZER = ~BUZZER;
    LED = ~LED;
    dl_ms();   
}

/* **************************************
 * Function : button_i0()
 * Description : 在设置状态下，使对应时间位加1
 *               在未计时状态下，开始计时
 *               在计时状态下，暂停计时
 *               在结束状态下，停止报警
 * Calls : updataDisBuf()
 * input : end, setting, time[], TR0, EX1
 * output : end, EX1, time[], TR0, runing
 * *************************************/
void button_i0() interrupt 0
{
    if (1 == end)
    {
        end = 0;
        LED = 0;
        EX1 = 1;
        dis_buf[2] = ':';
        dis_buf[5] = ':';
        Write1602_Com(CLEAR);
        Delay(50);
    }

    if (1 == setting) //对应位加1
    {
        time[setting_i]++;
        if (time[setting_i] == 60)
        {
            time[setting_i] = 0;
        }
        updataDisBuf();
    }
    else if (0 != (time[0] + time[1] + time[2])) //保证不能从零开始计时
    {
        TR0 = ~TR0;
        EX1 = ~EX1;
        runing = 1;
        Write1602_Str(0x80+0x40,6,dis_timeing);
    }
}

/* **************************************
 * Function : button_i1()
 * Description : 在未计时状态下,对倒计时的时间进行设置;
 *               在计时状态下，暂停计时条件下进行清零
 * Calls : updataDisBuf()
 * input : time[],setting_i, runing, setting
 * output : runing, time[], setting, dis_buf[], setting_i, TR1
 * *************************************/
void button_i1() interrupt 2
{
    if (1 == runing) //清零
    {
        //从计时状态转换到未计时状态
        runing = 0;

        time[0] = 0;
        time[1] = 0;
        time[2] = 0;
        dis_buf[2] = ':';
        dis_buf[5] = ':';
        Write1602_Com(CLEAR);
        Delay(50);
        updataDisBuf();
    }
    else //设置
    {
        //从未计时状态转换到设置状态
        setting = 1;
        Write1602_Str(0x80+0x40,7,dis_set);
        //保证上一位时间被显示
        if (setting_i != 3)
        {
            dis_buf[7 - 3 * setting_i] = '0'+ time[setting_i] % 10;
            dis_buf[6 - 3 * setting_i] = '0'+ time[setting_i] / 10;
        }
        setting_i--;
        TR1 = 1;

        //从设置状态转换到未计时状态
        if (-1 == setting_i)
        {
            setting = 0;
            setting_i = 3;
            TR1 = 0;
            dis_buf[2] = ':';
            dis_buf[5] = ':';
            Write1602_Com(CLEAR);
            Delay(50);
        }
    }
}

/* **************************************
 * Function : time_t0
 * Description : 倒计时，使用计数器0，工作方式1，计时50ms
 * Calls : updataDisBuf()
 * input : TH0, TH1, time[], i
 * output : TR0, time[], end, runing
 * *************************************/
void time_t0() interrupt 1
{
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;

    i++;
    if (10 == i)
    {
        dis_buf[2] = ' ';
        dis_buf[5] = ' ';
    }
    
    if (20 == i)
    {
        dis_buf[2] = ':';
        dis_buf[5] = ':';
        i = 0;
        time[0]--;
        if (-1 == time[0])
        {
            time[1]--;
            time[0] = 59;
            if (-1 == time[1])
            {
                time[2]--;
                time[1] = 59;
            } //end of if(-1 == time[1])
        }//end of if(-1 == time[0])

        if (0 == (time[0] + time[1] + time[2])) //计时结束
        {
            TR0 = 0;

            //从计时状态转换到结束状态
            end = 1;
            runing = 0;
            Write1602_Str(0x80+0x40,10,dis_ultimate);
        } //end of if(0 == (time[0]+time[1]+time[2]))

        updataDisBuf();
    } //end of if(20 == i)
}

/* **************************************
 * Function : time_t1()
 * Description : 设置对应时间位进行闪烁，使用计数器1，工作方式1，计时50ms
 * input : TH1, TL1, j, setting_i, time[]
 * output : dis_buf[]
 * *************************************/
void time_t1() interrupt 3
{
    TH1 = (65536 - 50000) / 256;
    TL1 = (65536 - 50000) % 256;

    j++;
    if (10 == j)
    {
        dis_buf[7 - 3 * setting_i] = ' ';
        dis_buf[6 - 3 * setting_i] = ' ';
    }
    if (20 == j)
    {
        j = 0;
        dis_buf[7 - 3 * setting_i] = '0' + time[setting_i] % 10;
        dis_buf[6 - 3 * setting_i] = '0' + time[setting_i] / 10;
    }
}