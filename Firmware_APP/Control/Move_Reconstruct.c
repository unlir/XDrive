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
  ** @file     : Move_Reconstruct.c/h
  ** @brief    : 运动规划器
  ** @versions : 1.1.7
  ** @time     : 2019/12/06
  ** @reviser  : unli (JiXi XuanCheng China)
  ** @explain  : null
*****/

//Oneself
#include "Move_Reconstruct.h"

//Control.h
#include "Control_Config.h"

/****************************************  运动重构器  ****************************************/
/****************************************  运动重构器  ****************************************/
/****************************************  运动重构器  ****************************************/
//Move_Reconstruct实例
Move_Reconstruct_Typedef move_reco;

/**
  * 运动重构器设置减速加速度
  * @param  reconstruct		运动重构器实例
  * @retval true:成功 / false:错误
**/
void Move_Reconstruct_Set_DownAcc(int32_t value)
{
	value = abs(value);
	if((value > 0) && (value <= Move_Rated_DownAcc))
	{
		move_reco.down_acc = value;
		move_reco.valid_down_acc = true;
	}
	else{
		move_reco.valid_down_acc = false;
	}
}

/**
  * 运动重构器设置超时时间
  * @param  reconstruct		运动重构器实例
  * @retval true:成功 / false:错误
**/
void Move_Reconstruct_Set_OverTime(uint16_t value)
{
	if((value >= Reconstruct_Min_OverTime) && (value <= Reconstruct_Max_OverTime))
	{
		move_reco.overtime = value;
		move_reco.valid_overtime = true;
	}
	else{
		move_reco.valid_overtime = false;
	}
}

/**
  * 运动重构器参数恢复
  * @param  NULL
  * @retval NULL
**/
void Move_Reconstruct_Set_Default(void)
{
	Move_Reconstruct_Set_DownAcc(DE_DOWN_ACC);
	Move_Reconstruct_Set_OverTime(DE_OverTime);	
}

/**
  * 运动重构器初始化
  * @param  reconstruct		运动重构器实例
  * @retval NULL
**/
void Move_Reconstruct_Init(void)
{
	//前置配置无效时,加载默认配置
	if(!move_reco.valid_down_acc)	{	Move_Reconstruct_Set_DownAcc(DE_DOWN_ACC);	}
	if(!move_reco.valid_overtime)	{	Move_Reconstruct_Set_OverTime(DE_OverTime);	}
	
	//动态跟踪参数
	move_reco.dyn_speed_acc = 0;
	//源信号记录
	move_reco.record_timer = 0;
	move_reco.overtime_flag = false;
	move_reco.record_speed = 0;
	move_reco.record_location = 0;
	//计算过程数据
	move_reco.speed_course_dec = 0;
	move_reco.speed_course = 0;
	move_reco.location_course_dec = 0;
	move_reco.location_course = 0;
	//输出跟踪控制量
	move_reco.go_location = 0;
	move_reco.go_speed = 0;
}

/**
  * 运动重构器开始新任务
  * @param  reconstruct		运动重构器实例
  * @param  real_location	实时位置
  * @param  real_speed		实时速度
  * @retval NULL
**/
void Move_Reconstruct_NewTask(int32_t real_location, int32_t real_speed)
{
	//源信号记录
	move_reco.record_timer = 0;			//信号源计时器清零
	move_reco.overtime_flag = false;		//超时标志清位
	//更新计算过程数据
	move_reco.speed_course_dec = 0;				//过程速度
	move_reco.speed_course = real_speed;			//过程速度
	move_reco.location_course_dec = 0;			//过程位置
	move_reco.location_course = real_location;	//过程位置
}

/**
  * 运动重构器速度积分
  * @param  value	加速度
**/
#define Speed_Course_Integral(value)	\
{	\
	move_reco.speed_course_dec += value;												\
	move_reco.speed_course += move_reco.speed_course_dec / CONTROL_FREQ_HZ;		\
	move_reco.speed_course_dec = move_reco.speed_course_dec % CONTROL_FREQ_HZ;	\
}								//(C语言除法运算向0取整，直接取余即可)

/**
  * 运动重构器位置积分
  * @param  value	速度
**/
#define Location_Course_Integral(value)	\
{	\
	move_reco.location_course_dec += value;												\
	move_reco.location_course += move_reco.location_course_dec / CONTROL_FREQ_HZ;		\
	move_reco.location_course_dec = move_reco.location_course_dec % CONTROL_FREQ_HZ;	\
}								//(C语言除法运算向0取整，直接取余即可)

/**
  * 运动重构器获得立即位置和立即速度
  * @param  reconstruct		运动重构器实例
  * @param  goal_location	目标位置
  * @param  goal_speed		目标速度
  * @retval NULL
**/
void Move_Reconstruct_Capture_Goal(int32_t goal_location, int32_t goal_speed)
{
	//触发新目标
	if( (goal_speed != move_reco.record_speed)
	 || (goal_location != move_reco.record_location))
	{
		//源信号记录
		move_reco.record_timer = 0;
		move_reco.record_speed = goal_speed;
		move_reco.record_location = goal_location;
		//更新动态跟踪参数
		move_reco.dyn_speed_acc = (int32_t)(   (float)(goal_speed + move_reco.speed_course) * (float)(goal_speed - move_reco.speed_course)
											    / (float)(2 * (goal_location - move_reco.location_course))
											  );
		//过程数据
		move_reco.overtime_flag = false;
	}
	else
	{
		//源信号超时处理
		if(move_reco.record_timer >= (200 * 1000))
			move_reco.overtime_flag = true;
		else
			move_reco.record_timer += CONTROL_PERIOD_US;
	}

	//获得运动更改
	if(move_reco.overtime_flag)
	{//超时->开始减速为0
		if(move_reco.speed_course == 0)
		{
			move_reco.speed_course_dec = 0;
			move_reco.speed_course = 0;
		}
		else if(move_reco.speed_course > 0)
		{
			Speed_Course_Integral(-move_reco.down_acc);
			if(move_reco.speed_course <= 0)
			{
				move_reco.speed_course_dec = 0;
				move_reco.speed_course = 0;
			}
		}
		else
		{
			Speed_Course_Integral(move_reco.down_acc);
			if(move_reco.speed_course >= 0)
			{
				move_reco.speed_course_dec = 0;
				move_reco.speed_course = 0;
			}
		}
	}
	else
	{//未超时->按照预设加速度变速
		Speed_Course_Integral(move_reco.dyn_speed_acc);
	}

	//位置积分
	Location_Course_Integral(move_reco.speed_course);

	//输出
	move_reco.go_location = (int32_t)move_reco.location_course;
	move_reco.go_speed = (int32_t)move_reco.speed_course;
}



