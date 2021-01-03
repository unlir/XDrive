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

#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kernel_port.h"

//工程配置
#define Button_Quan		3		//按键支持数量
#define Button_OK			0		//按键OK编号
#define Button_UP			1		//按键UP编号
#define Button_DOWN		2		//按键DOWN编号
//识别配置
#define Button_Shake_Time	20		//ms去抖时间
#define Button_Long_Time	1000	//ms长按触发时间

typedef enum{
	//保持状态
	Button_Bit_Up = 0,			//状态_弹起
	Button_Bit_Shake,				//状态_抖动
	Button_Bit_Press,				//状态_按下
	Button_Bit_Long,				//状态_长按
	//边沿状态
	Button_Bit_Drop,				//边沿_按下
	Button_Bit_Rise,				//边沿_弹起
	Button_Bit_LongDrop,		//边沿_长按按下
	Button_Bit_LongRise,		//边沿_长按弹起
}Button_State_Typedef;

typedef struct{
	//数据
	bool									level;		//按键水平
	Button_State_Typedef	state;		//按键状态
	uint32_t							time_ms;	//按下时间
}Button_Typedef;

void Button_Init(void);								//按键初始化
void Button_Scan_ms(uint32_t _time);	//定时扫描
Button_State_Typedef Button_Inquice_State(uint16_t button_num);		//获取按键状态

#ifdef __cplusplus
}
#endif

#endif //	BUTTON_H

