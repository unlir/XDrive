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
  ** @file     : Speed_Tracker.c/h
  ** @brief    : 速度跟踪器
  ** @versions : 1.1.7
  ** @time     : 2019/12/06
  ** @reviser  : unli (JiXi XuanCheng China)
  ** @explain  : null
*****/

#ifndef SPEED_TRACKER_H
#define SPEED_TRACKER_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

/****************************************  速度跟踪器  ****************************************/
/****************************************  速度跟踪器  ****************************************/
/****************************************  速度跟踪器  ****************************************/
/**
  * Speed_Tracker类结构体定义
**/
typedef struct{
	//配置(加速加速度)
	#define	DE_UP_ACC			(Move_Rated_UpAcc / 10)
	bool		valid_up_acc;
	int32_t	up_acc;
	//配置(减速加速度)
	#define	DE_DOWN_ACC		(Move_Rated_DownAcc / 10)
	bool		valid_down_acc;
	int32_t	down_acc;
	//计算过程数据
	int32_t		course_mut;	//过程加速度积分(放大CONTROL_FREQ_HZ倍)
	int32_t		course;			//过程速度
	//输出跟踪量
	int32_t		go_speed;		//立即速度
}Speed_Tracker_Typedef;
extern Speed_Tracker_Typedef	speed_tck;

void Speed_Tracker_Set_UpAcc(int32_t value);		//速度跟踪器设置加速加速度
void Speed_Tracker_Set_DownAcc(int32_t value);	//速度跟踪器设置减速加速度
void Speed_Tracker_Set_Default(void);						//速度跟踪器参数恢复

void Speed_Tracker_Init(void);												//速度跟踪器初始化
void Speed_Tracker_NewTask(int32_t real_speed);				//速度控制器开始新任务
void Speed_Tracker_Capture_Goal(int32_t goal_speed);	//速度跟踪器获得立即速度

#ifdef __cplusplus
}
#endif

#endif //SPEED_TRACKER_H
