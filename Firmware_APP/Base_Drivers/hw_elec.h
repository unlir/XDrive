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
  ** @file     : hw_elec.c/h
  ** @brief    : 硬件电流控制器
  ** @versions : 2.1.2
  ** @time     : 2020/08/08
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef HW_ELEC_H
#define HW_ELEC_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

/**
  * 线圈定义
**/
typedef struct{
	uint16_t conver;		//SIN数组指针
	int16_t  sin_data;	//SIN换算数值
	uint16_t dac_reg;		//12位DAC数值
}Coil_Typedef;

extern Coil_Typedef		coil_a;	//电流控制
extern Coil_Typedef		coil_b;	//电流控制

void REIN_HW_Elec_Init(void);																				//硬件电流控制初始化
void REIN_HW_Elec_SetSleep(void);																		//硬件电流设置驱动睡眠
void REIN_HW_Elec_SetBrake(void);																		//硬件电流设置驱动刹车
void REIN_HW_Elec_SetDivideElec(uint32_t divide, int32_t elec_ma);	//硬件电流设置输出细分电流

#ifdef __cplusplus
}
#endif

#endif

