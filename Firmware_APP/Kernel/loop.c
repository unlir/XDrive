/******
	************************************************************************
	******
	** @versions : 1.1.4
	** @time     : 2020/09/15
	******
	************************************************************************
	******
	** @project : XDrive_Step
	** @brief   : 具有多功能接口和闭环功能的步进电机
	** @author  : unlir (知不知啊)
	******
	** @address : https://github.com/unlir/XDrive
	******
	** @issuer  : IVES ( 艾维斯 实验室) (QQ: 557214000)   (master)
	** @issuer  : REIN (  知驭  实验室) (QQ: 857046846)   (master)
	******
	************************************************************************
	******
	** {Stepper motor with multi-function interface and closed loop function.}
	** Copyright (c) {2020}  {unlir}
	** 
	** This program is free software: you can redistribute it and/or modify
	** it under the terms of the GNU General Public License as published by
	** the Free Software Foundation, either version 3 of the License, or
	** (at your option) any later version.
	** 
	** This program is distributed in the hope that it will be useful,
	** but WITHOUT ANY WARRANTY; without even the implied warranty of
	** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	** GNU General Public License for more details.
	** 
	** You should have received a copy of the GNU General Public License
	** along with this program.  If not, see <http://www.gnu.org/licenses/>.
	******
	************************************************************************
******/

/*****
  ** @file     : loop.c/h and loop_it.c
  ** @brief    : MCU系工程主框架
  ** @versions : newest
  ** @time     : newest
  ** @reviser  : unli (WuHu China)
  ** @explain  : null
*****/

/*****
  ************************************************************************
  ******************************  写在最前面  *****************************
  **
  ** 1.由于作者比较忙,目前只完成“公测版”,仅适用于开发者学习纠错,不建议用于实际工程.
  **
  ** 2.“公测版”代码将以一个较快的频率进行第4节更新(目前为1.1.4.1),恕无法进行通知,若需要获取及时的更新信息,请加上述发行QQ群了解更新情况.
  **
  ** 3.代码中所有预存参数均写好了接口函数,需要使用的用户可以直接调用.
  **
  ** 4.本版本代码暂时只支持(X_Drive_H1.0.1)(即单板三按键那个),
  **   作者将尽快完成对(X_Drive_Ext_H1.0.0)(即双板二按键那个)的移植测试工作.
  ** 
  ** 5.如果查阅代码或使用中发现了bug,最好及时联系作者指出,不然我自己就发现了(>_<)
  **   同理,如果有啥好玩的功能可以实现的,也最好连续作者更新,或者先行完成,再让我白嫖
  **
  ************************************************************************
  *****************************  本版使用方法  ****************************
  ** 
  ** 1.点按上下按键选择模式,点按确认按键确认加载的模式,目前已经完成开放的模式有
  **   (停止)(DIG位置)(DIG速度)(DIG电流)(DIG轨迹)(PWM位置_舵机)(PWM速度_调速机)(PWM电流_直流机)(PULSE位置)
  **
  ** 2.长按确认按键,进行编码器校准(若成功:继续执行任务)(若失败:显示失败识别码).
  **
  ************************************************************************
*****/

//Oneself
#include "loop.h"

//Application_User_Core

//Base_Drivers
#include "kernel_port.h"
#include "mt6816.h"
#include "hw_elec.h"

//Control
#include "signal_port.h"
#include "motor_control.h"
#include "encode_cali.h"

//Data_Link

//Debug
#include "button.h"
#include "ssd1306.h"
#include "xdrive_ui.h"

/**********************************************************************/
/*********************    Subclock processing    **********************/
/**********************************************************************/
uint32_t time_1ms_count = 0;
uint32_t major_cycle_count = 0;
static uint32_t time_second_1ms = 0;
static uint32_t time_second_10ms = 0;
static uint32_t time_second_20ms = 0;
static uint32_t time_second_50ms = 0;
static uint32_t time_second_100ms = 0;

/**
* @brief 副时钟10ms执行
*/
void time_second_10ms_serve(void)
{

}

/**
* @brief 副时钟20ms执行
*/
void time_second_20ms_serve(void)
{
	//XDrive_UI(50Hz刷新率)
	XDrive_UI_Callback_ms(20);
}

/**
* @brief 副时钟50ms执行
*/
void time_second_50ms_serve(void)
{

}

/**
* @brief 副时钟100ms执行
*/
void time_second_100ms_serve(void)
{

}
	
/**
* @brief 副时钟循环执行
**/
void time_second_run(void)
{
	if(time_second_10ms)		{time_second_10ms--;		time_second_10ms_serve();		}
	if(time_second_20ms)		{time_second_20ms--;		time_second_20ms_serve();		}
	if(time_second_50ms)		{time_second_50ms--;		time_second_50ms_serve();		}
	if(time_second_100ms)		{time_second_100ms--;		time_second_100ms_serve();	}
}

/**
* @brief 副时钟1ms时钟
**/
void loop_second_base_1ms(void)
{
	time_1ms_count++;
	time_second_1ms++;
	if(!(time_second_1ms % 10))		{time_second_10ms++;		}
	if(!(time_second_1ms % 20))		{time_second_20ms++;		}
	if(!(time_second_1ms % 50))		{time_second_50ms++;		}
	if(!(time_second_1ms % 100))	{time_second_100ms++;		}
	if(!(time_second_1ms % 1000))	{time_second_1ms = 0;		}
}

/*********************************************************************/
/****************************    LOOP    *****************************/
/*********************************************************************/
/**
* This is Main LOOP
*/
void loop(void)
{
	//基本外设初始化(Base_Drivers)
	REIN_MT6816_Init();		//MT6816传感器初始化
	REIN_HW_Elec_Init();	//硬件电流控制器
	Signal_MoreIO_Init();	//MoreIO接口初始化
	
	//控制初始化(Control)
	Calibration_Init();		//校准器初始化
	Motor_Control_Init();	//电机控制初始化
	
	//调试工具(Debug)
	Button_Init();				//按键初始化
	SSD1306_Init();				//OLED初始化
	XDrive_UI_Init();			//XDrive_UI初始化
	
	//调整中断配置
	LoopIT_Priority_Overlay();	//重写-中断优先级覆盖
	LoopIT_SysTick_20KHz();			//重写-系统计时器修改为20KHz
	
	for(;;)//FOR Circulation
	{
		major_cycle_count++;
		time_second_run();

		//校准器主程序回调
		Calibration_Loop_Callback();		
	}
}
/*************************************
*********** END Dispatch  ************
*************************************/

