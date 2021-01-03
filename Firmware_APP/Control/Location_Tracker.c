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
  ** @file     : Location_Tracker.c/h
  ** @brief    : 位置跟踪器
  ** @versions : 1.1.7
  ** @time     : 2019/12/06
  ** @reviser  : unli (JiXi XuanCheng China)
  ** @explain  : null
*****/

//Oneself
#include "Location_Tracker.h"

//Control
#include "Control_Config.h"

/****************************************  位置跟踪器  ****************************************/
/****************************************  位置跟踪器  ****************************************/
/****************************************  位置跟踪器  ****************************************/
//Location_Tracker结构体
Location_Tracker_Typedef	location_tck;

/**
  * 位置跟踪器设置最大速度
  * @param  value		最大速度
  * @retval true:成功 / false:错误
**/
void Location_Tracker_Set_MaxSpeed(int32_t value)
{
	value = abs(value);
	if((value > 0) && (value <= Move_Rated_Speed))
	{
		location_tck.max_speed = value;
		location_tck.valid_max_speed = true;
	}
	else
	{
		location_tck.valid_max_speed = false;
	}
}

/**
  * 位置跟踪器设置加速加速度
  * @param  value		加速加速度	
  * @retval true:成功 / false:错误
**/
void Location_Tracker_Set_UpAcc(int32_t value)
{
	value = abs(value);
	if((value > 0) && (value <= Move_Rated_UpAcc))
	{
		location_tck.up_acc = value;
		location_tck.valid_up_acc = true;
	}
	else
	{
		location_tck.valid_up_acc = false;
	}
}

/**
  * 位置跟踪器设置减速加速度
  * @param  tracker		位置跟踪器实例
  * @param  value		减速加速度	
  * @retval true:成功 / false:错误
**/
void Location_Tracker_Set_DownAcc(int32_t value)
{
	value = abs(value);
	if((value > 0) && (value <= Move_Rated_UpAcc))
	{
		location_tck.down_acc = value;
		location_tck.down_acc_quick = 0.5f / (float)location_tck.down_acc;
		location_tck.valid_down_acc = true;
	}
	else
	{
		location_tck.valid_down_acc = false;
	}
}

/**
  * 位置跟踪器参数恢复
  * @param  NULL
  * @retval NULL
**/
void Location_Tracker_Set_Default(void)
{
	Location_Tracker_Set_MaxSpeed(DE_MAX_SPEED);
	Location_Tracker_Set_UpAcc(DE_UP_ACC);
	Location_Tracker_Set_DownAcc(DE_Down_ACC);
}

/**
  * 位置跟踪器初始化
  * @param  tracker		位置跟踪器实例
  * @retval NULL
**/
void Location_Tracker_Init()
{
	//前置配置无效时,加载默认配置
	if(!location_tck.valid_max_speed)	{	Location_Tracker_Set_MaxSpeed(DE_MAX_SPEED);		}
	if(!location_tck.valid_up_acc)		{	Location_Tracker_Set_UpAcc(DE_UP_ACC);	}
	if(!location_tck.valid_down_acc)	{	Location_Tracker_Set_DownAcc(DE_Down_ACC);	}
	
	
	//静态配置的跟踪参数
	location_tck.speed_locking_stop = Move_Pulse_NUM;	//每秒1转时允许拉停	(最佳的值应该为加速度/1000，但是存在两个加速度，不方便取值)
																								//(正确解决方法应该为将减速位移运算转换为整形运算)
	//计算过程参数
	location_tck.course_acc_integral = 0;
	location_tck.course_speed = 0;
	location_tck.course_speed_integral = 0;
	location_tck.course_location = 0;
	//输出跟踪控制量
	location_tck.go_location = 0;
	location_tck.go_speed = 0;
}

/**
  * 位置跟踪器开始新任务
  * @param  tracker			位置跟踪器实例
  * @param  real_location	实时位置
  * @param  real_speed		实时速度
  * @retval NULL
**/
void Location_Tracker_NewTask(int32_t real_location, int32_t real_speed)
{
	//更新计算过程数据
	location_tck.course_acc_integral = 0;			//过程加速度积分
	location_tck.course_speed = real_speed;			//过程速度
	location_tck.course_speed_integral = 0;			//过程速度积分
	location_tck.course_location = real_location;	//过程位置
}

/**
  * 位置跟踪器速度积分
  * @param  value	加速度
**/
#define Speed_Course_Integral(value)	\
{	\
	location_tck.course_acc_integral += value;											\
	location_tck.course_speed += location_tck.course_acc_integral / CONTROL_FREQ_HZ;		\
	location_tck.course_acc_integral = location_tck.course_acc_integral % CONTROL_FREQ_HZ;	\
}								//(C语言除法运算向0取整，直接取余即可)

/**
  * 位置跟踪器位置积分
  * @param  value	速度
**/
#define Location_Course_Integral(value)	\
{	\
	location_tck.course_speed_integral += value;											\
	location_tck.course_location += location_tck.course_speed_integral / CONTROL_FREQ_HZ;		\
	location_tck.course_speed_integral = location_tck.course_speed_integral % CONTROL_FREQ_HZ;	\
}								//(C语言除法运算向0取整，直接取余即可)

/**
  * 位置跟踪器获得立即位置和立即速度
  * @param  tracker			位置跟踪器实例
  * @param  goal_location	目标位置
  * @retval NULL
**/
void Location_Tracker_Capture_Goal(int32_t goal_location)
{
	//整形位置差
	int32_t location_sub = goal_location - location_tck.course_location;

	/********************到达目标********************/
	if(location_sub == 0)
	{
		/******************** 速度小于刹停速度********************/
		if((location_tck.course_speed >= -location_tck.speed_locking_stop) && (location_tck.course_speed <= location_tck.speed_locking_stop))
		{
			//进入静止->取整浮点数据
			location_tck.course_acc_integral = 0;
			location_tck.course_speed = 0;
			location_tck.course_speed_integral = 0;
		}
		/********************速度 > 0********************/
		else if(location_tck.course_speed > 0)
		{
			//正向减速到0
			Speed_Course_Integral(-location_tck.down_acc)
			if(location_tck.course_speed <= 0)
			{
				location_tck.course_acc_integral = 0;
				location_tck.course_speed = 0;
			}
		}
		/********************速度 < 0********************/
		else if(location_tck.course_speed < 0)
		{
			//反向减速到0
			Speed_Course_Integral(location_tck.down_acc)
			if(location_tck.course_speed >= 0)
			{
				location_tck.course_acc_integral = 0;
				location_tck.course_speed = 0;
			}
		}
	}
	/********************未到达目标********************/
	else
	{
		/********************速度为0********************/
		if(location_tck.course_speed == 0)
		{
			if(location_sub > 0)
			{
				Speed_Course_Integral(location_tck.up_acc)	//开始正向加速
			}
			else
			{
				Speed_Course_Integral(-location_tck.up_acc)	//开始反向加速
			}
		}
		/********************速度与位移方向同向(正方向)********************/
		else if((location_sub > 0) && (location_tck.course_speed > 0))
		{
			if(location_tck.course_speed <= location_tck.max_speed)
			{
				//计算需要的减速位移
				int32_t need_down_location = (int32_t)((float)location_tck.course_speed * (float)location_tck.course_speed * (float)location_tck.down_acc_quick);	//浮点运算
				if(abs(location_sub) > need_down_location)
				{
					//正向加速到最大速度
					if(location_tck.course_speed < location_tck.max_speed)
					{
						Speed_Course_Integral(location_tck.up_acc);
						if(location_tck.course_speed >= location_tck.max_speed)
						{
							location_tck.course_acc_integral = 0;
							location_tck.course_speed = location_tck.max_speed;
						}
					}
					//正向稳速
					//else if(location_tck.course_speed == location_tck.max_speed_max)
					//{}
					//正向超速->减速
					else if(location_tck.course_speed > location_tck.max_speed)
					{
						Speed_Course_Integral(-location_tck.down_acc);
					}
				}
				else
				{
					//正向减速到0
					Speed_Course_Integral(-location_tck.down_acc);
					if(location_tck.course_speed <= 0)
					{
						location_tck.course_acc_integral = 0;
						location_tck.course_speed = 0;
					}
				}
			}
			else
			{
				//正向减速到0
				Speed_Course_Integral(-location_tck.down_acc);
				if(location_tck.course_speed <= 0)
				{
					location_tck.course_acc_integral = 0;
					location_tck.course_speed = 0;
				}
			}
		}
		/********************速度与位移方向同向(负方向)********************/
		else if((location_sub < 0) && (location_tck.course_speed < 0))
		{
			if(location_tck.course_speed >= -location_tck.max_speed)
			{
				//计算需要的减速位移(有溢出风险的运算)
				int32_t need_down_location = (int32_t)((float)location_tck.course_speed * (float)location_tck.course_speed * (float)location_tck.down_acc_quick);	//浮点运算
				if(abs(location_sub) > need_down_location)
				{
					//反向加速到最大速度
					if(location_tck.course_speed > -location_tck.max_speed)
					{
						Speed_Course_Integral(-location_tck.up_acc);
						if(location_tck.course_speed <= -location_tck.max_speed)
						{
							location_tck.course_acc_integral = 0;
							location_tck.course_speed = -location_tck.max_speed;
						}
					}
					//反向稳速
					//else if(location_tck.course_speed == -location_tck.max_speed_max)
					//{}
					//反向超速->减速
					else if(location_tck.course_speed < -location_tck.max_speed)
					{
						Speed_Course_Integral(location_tck.down_acc);
					}
				}
				else
				{
					//反向减速到0
					Speed_Course_Integral(location_tck.down_acc);
					if(location_tck.course_speed >= 0)
					{
						location_tck.course_acc_integral = 0;
						location_tck.course_speed = 0;
					}
				}
			}
			else
			{
				//反向减速到0
				Speed_Course_Integral(location_tck.down_acc);
				if(location_tck.course_speed >= 0)
				{
					location_tck.course_acc_integral = 0;
					location_tck.course_speed = 0;
				}
			}
		}
		/********************速度与位移方向反向********************/
		else if((location_sub < 0) && (location_tck.course_speed > 0))
		{
			//正向减速到0
			Speed_Course_Integral(-location_tck.down_acc);
			if(location_tck.course_speed <= 0)
			{
				location_tck.course_acc_integral = 0;
				location_tck.course_speed = 0;
			}
		}
		else if(((location_sub > 0) && (location_tck.course_speed < 0)))
		{
			//反向减速到0
			Speed_Course_Integral(location_tck.down_acc);
			if(location_tck.course_speed >= 0)
			{
				location_tck.course_acc_integral = 0;
				location_tck.course_speed = 0;
			}
		}		
	}

	//位置积分
	Location_Course_Integral(location_tck.course_speed);

	//输出
	location_tck.go_location = (int32_t)location_tck.course_location;
	location_tck.go_speed = (int32_t)location_tck.course_speed;
}

