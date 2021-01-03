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


#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

/********** HwElec **********/
/********** HwElec **********/
/********** HwElec **********/
void REIN_TIM_HwElec_Init(void);       //TIM初始化(HwElec)

/********** MT6816 **********/
/********** MT6816 **********/
/********** MT6816 **********/
void REIN_TIM_MT6816_ABZ_Init(void);		//TIM初始化(MT6816_ABZ)
void REIN_TIM_MT6816_PWM_Init(void);		//TIM初始化(MT6816_PWM)

/********** SIGNAL **********/
/********** SIGNAL **********/
/********** SIGNAL **********/
void REIN_TIM_SIGNAL_COUNT_Init(void);		//TIM初始化(SIGNAL_COUNT)
void REIN_TIM_SIGNAL_COUNT_DeInit(void);	//TIM清理(SIGNAL_COUNT)
void REIN_TIM_SIGNAL_PWM_Init(void);			//TIM初始化(SIGNAL_PWM)
void REIN_TIM_SIGNAL_PWM_DeInit(void);		//TIM清理(SIGNAL_PWM)

#ifdef __cplusplus
}
#endif

#endif
