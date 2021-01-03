/******
	************************************************************************
	******
	** @project : XDrive_Step
	** @brief   : Stepper motor with multi-function interface and closed loop function. 
	** @brief   : 具有多功能接口和闭环功能的步进电机
	** @author  : unlir (知不知啊)
	** @contacts: QQ.1354077136
	******
	** @address : https://github.com/unlir/XDrive
	******
	************************************************************************
	******
	** {Stepper motor with multi-function interface and closed loop function.}
	** Copyright (c) {2020}  {unlir(知不知啊)}
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
  ** @file     : button.c/h
  ** @brief    : 按键识别
  ** @versions : 1.1.2
  ** @time     : 2018/06/24
  ** @reviser  : unli (WuHu China)
  ** @explain  : null
*****/

//Oneself
#include "button.h"

//GPIO输入
#define	Read_Button_OK_IO()			(BUTTON_OK_GPIO_Port   -> IDR & BUTTON_OK_Pin)
#define	Read_Button_UP_IO()			(BUTTON_UP_GPIO_Port   -> IDR & BUTTON_UP_Pin)
#define	Read_Button_DOWN_IO()		(BUTTON_DOWN_GPIO_Port -> IDR & BUTTON_DOWN_Pin)

//定义按键实例
Button_Typedef button[Button_Quan];

/**
  * @brief  按键读取
  * @param  NULL
  * @retval true  : 按下
  * @retval false : 弹起
***/
bool Button_Read_Level(uint16_t button_num)
{
	switch(button_num)
	{
		#ifdef BUTTON_OK_GPIO_CLK_ENABLE
		case Button_OK:			return (!(bool)Read_Button_OK_IO());		//低电平为按下
		#endif
		#ifdef BUTTON_UP_GPIO_CLK_ENABLE
		case Button_UP:			return (!(bool)Read_Button_UP_IO());		//低电平为按下
		#endif
		#ifdef BUTTON_DOWN_GPIO_CLK_ENABLE
		case Button_DOWN:		return (!(bool)Read_Button_DOWN_IO());	//低电平为按下
		#endif
		default:						return false;
	}
}

/**
  * @brief  按键初始化
  * @param  NULL
  * @retval NULL
***/
void Button_Init(void)
{
	//配置外设
	REIN_GPIO_Button_Init();
	
	for(uint16_t i=0; i<Button_Quan; i++)
	{
		//数据
		button[i].level = false;
		button[i].state = Button_Bit_Up;
		button[i].time_ms = 0;
	}
}

/**
  * @brief  按键定时扫描
  * @param  NULL
  * @retval NULL
***/
void Button_Scan_ms(uint32_t _time)
{
	for(uint16_t i=0; i<Button_Quan; i++)
	{
		//提取按键
		button[i].level = Button_Read_Level(i);

		//按键状态处理
		switch(button[i].state)
		{
			//状态_弹起
			case Button_Bit_Up:
				if(button[i].level){
					button[i].state = Button_Bit_Shake;
					button[i].time_ms = 0;
				}
			break;
			//状态_抖动
			case Button_Bit_Shake:
				if(button[i].level){
					button[i].time_ms += _time;
					if(button[i].time_ms > Button_Shake_Time)
					{
						button[i].state = Button_Bit_Drop;
						button[i].time_ms = 0;
					}
				}
				else{
					button[i].state = Button_Bit_Up;
					button[i].time_ms = 0;
				}
			break;
			//状态_按下
			case Button_Bit_Press:
				if(button[i].level){
					button[i].time_ms += _time;
					if(button[i].time_ms > Button_Long_Time)
					{
						button[i].state = Button_Bit_LongDrop;
						button[i].time_ms = 0;
					}
				}
				else{
					button[i].state = Button_Bit_Rise;
					button[i].time_ms = 0;
				}
			break;
			//状态_长按
			case Button_Bit_Long:
				if(button[i].level){
					button[i].state = Button_Bit_Long;
				}
				else{
					button[i].state = Button_Bit_LongRise;
				}
			break;
			//边沿_按下
			case Button_Bit_Drop:
				if(button[i].level){
					button[i].state = Button_Bit_Press;
					button[i].time_ms = 0;
				}
				else{
					button[i].state = Button_Bit_Up;
				}
			break;
			//边沿_弹起
			case Button_Bit_Rise:
				button[i].state = Button_Bit_Up;
			break;
			//边沿_长按按下
			case Button_Bit_LongDrop:
				if(button[i].level){
					button[i].state = Button_Bit_Long;
					button[i].time_ms = 0;
				}
				else{
					button[i].state = Button_Bit_Up;
				}
			break;
			//边沿_长按弹起
			case Button_Bit_LongRise:
				button[i].state = Button_Bit_Up;
			break;
		}	
	}
}

/**
  * @brief  获取按键状态
  * @param  NULL
  * @retval NULL
***/
Button_State_Typedef Button_Inquice_State(uint16_t button_num)
{
	if(button_num < Button_Quan)
	{
		return button[button_num].state;
	}
	else
	{
		return Button_Bit_Up;
	}
}
