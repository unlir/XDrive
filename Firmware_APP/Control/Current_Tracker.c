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
  ** @file     : Current_Tracker.c/h
  ** @brief    : 电流跟踪器
  ** @versions : 1.1.7
  ** @time     : 2019/12/06
  ** @reviser  : unli (JiXi XuanCheng China)
  ** @explain  : null
*****/

//Oneself
#include "Current_Tracker.h"

//Control
#include "Control_Config.h"

/****************************************  电流跟踪器  ****************************************/
/****************************************  电流跟踪器  ****************************************/
/****************************************  电流跟踪器  ****************************************/
//Current_Tracker类结构体
Current_Tracker_Typedef	current_tck;

/**
  * 电流跟踪器设置增流变化率
  * @param  value:	增流梯度
  * @retval NULL
**/
void Current_Tracker_Set_UpRate(int32_t value)
{
	value = abs(value);
	if((value > 0) && (value <= Move_Rated_UpCurrentRate))
	{
		current_tck.up_rate = value;
		current_tck.valid_up_rate = true;
	}
	else{
		current_tck.valid_up_rate = false;
	}
}

/**
  * 电流跟踪器设置减流变化率
  * @param  value:	减流梯度
  * @retval NULL
**/
void Current_Tracker_Set_DownRate(int32_t value)
{
	value = abs(value);
	if((value > 0) && (value <= Move_Rated_DownCurrentRate))
	{
		current_tck.down_rate = value;
		current_tck.valid_down_rate = true;
	}
	else{
		current_tck.valid_down_rate = false;
	}
}

/**
  * 电流跟踪器参数恢复
  * @param  NULL
  * @retval NULL
**/
void Current_Tracker_Set_Default(void)
{
	Current_Tracker_Set_UpRate(De_Up_Rate);
	Current_Tracker_Set_DownRate(De_Down_Rate);
}

/**
  * 电流跟踪器初始化
  * @param  NULL
  * @retval NULL
**/
void Current_Tracker_Init(void)
{
	//前置配置无效时,加载默认配置
	if(!current_tck.valid_up_rate)			{	Current_Tracker_Set_UpRate(De_Up_Rate);			}
	if(!current_tck.valid_down_rate)		{	Current_Tracker_Set_DownRate(De_Down_Rate);	}

	//计算过程数据
	current_tck.course_mut = 0;
	current_tck.course = 0;
	//输出跟踪量
	current_tck.go_current = 0;
}

/**
  * 电流跟踪器初始化
  * @param  real_current	实时电流
  * @retval NULL
**/
void Current_Tracker_NewTask(int16_t real_current)
{
	//更新计算过程数据
	current_tck.course_mut = 0;				//过程电流
	current_tck.course = real_current;	//过程电流
}

/**
  * 电流跟踪器电流积分
  * @param  value	梯度
**/
#define Current_Course_Integral(value)	\
{	\
	current_tck.course_mut += value;											\
	current_tck.course += current_tck.course_mut / CONTROL_FREQ_HZ;		\
	current_tck.course_mut = current_tck.course_mut % CONTROL_FREQ_HZ;	\
}

/**
  * 电流跟踪器获得立即电流
  * @param  tracker			电流跟踪器实例
  * @param  goal_current	目标电流
  * @retval NULL
**/
void Current_Tracker_Capture_Goal(int32_t goal_current)
{
	//整形电流差
	int32_t electric_sub = goal_current - current_tck.course;	//电流差	= 控制器目标电流 - 跟踪器电流

	/********************电流到达目标电流********************/
	if(electric_sub == 0)
	{
		current_tck.course = goal_current;		//进入稳流状态
	}
	/********************矢量电流小于目标********************/
	else if(electric_sub > 0)
	{
		if(current_tck.course >= 0)
		{
			Current_Course_Integral(current_tck.up_rate);
			if(current_tck.course >= goal_current)
			{
				current_tck.course_mut = 0;
				current_tck.course = goal_current;
			}
		}
		else// if(current_tck.course < 0)
		{
			Current_Course_Integral(current_tck.down_rate);
			if((int32_t)current_tck.course >= 0)
			{
				current_tck.course_mut = 0;
				current_tck.course = 0;
			}
		}
	}
	/********************矢量电流大于目标********************/
	else if(electric_sub < 0)
	{
		if(current_tck.course <= 0)
		{
			Current_Course_Integral(-current_tck.up_rate);
			if((int32_t)current_tck.course <= (int32_t)goal_current)
			{
				current_tck.course_mut = 0;
				current_tck.course = goal_current;
			}
		}
		else// if(current_tck.course > 0)
		{
			Current_Course_Integral(-current_tck.down_rate);
			if((int32_t)current_tck.course <= 0)
			{
				current_tck.course_mut = 0;
				current_tck.course = 0;
			}
		}
	}

	//输出
	current_tck.go_current = (int32_t)current_tck.course;
}

