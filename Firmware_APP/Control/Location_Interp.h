/******
	************************************************************************
	******
	** @project : XDrive_Step
	** @brief   : 具有多功能接口和闭环功能的步进电机
	** @author  : unlir (知不知啊)
	** @contacts: QQ.1354077136
	******
	** @address : https://github.com/unlir/XDrive
	******
	** @issuer  : REIN ( 知驭 实验室) (QQ: 857046846)             (discuss)
	** @issuer  : IVES (艾维斯实验室) (QQ: 557214000)             (discuss)
	** @issuer  : X_Drive_Develop     (QQ: Contact Administrator) (develop)
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
  ** @file     : Location_Interp.c/h
  ** @brief    : 位置插补器
  ** @versions : 1.1.7
  ** @time     : 2019/12/06
  ** @reviser  : unli (JiXi XuanCheng China)
  ** @explain  : null
*****/

#ifndef LOCATION_INTERP_H
#define LOCATION_INTERP_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

/****************************************  位置补插器(不带细分增强)  ****************************************/
/****************************************  位置补插器(不带细分增强)  ****************************************/
/****************************************  位置补插器(不带细分增强)  ****************************************/
/**
  * Location_Interp类结构体定义
**/
typedef struct{
	//源信号数据
	int32_t		record_location;				//记录的位置
	int32_t		record_location_last;		//记录的位置
	int32_t		est_location;						//估计的位置
	int32_t		est_speed_mut;					//估计速度倍值(放大n倍)
	int32_t		est_speed;							//估计的速度

	//输出跟踪控制量
	int32_t		go_location;		//立即位置
	int32_t		go_speed;				//立即速度
}Location_Interp_Typedef;
extern Location_Interp_Typedef location_interp;

void Location_Interp_Init(void);																					//位置补插器初始化
void Location_Interp_NewTask(int32_t real_location, int32_t real_speed);	//位置补插器开始新任务
void Location_Interp_Capture_Goal(int32_t goal_location);									//位置补插器获得立即位置和立即速度

#ifdef __cplusplus
}
#endif

#endif









