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
  ** @file     : control_config.c/h
  ** @brief    : 电机控制
  ** @versions : 2.2.3
  ** @time     : 2020/09/15
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

//Oneself
#include "motor_control.h"

//Base_Drivers
#include "mt6816.h"
#include "hw_elec.h"
#include "signal_port.h"

//Control
#include "control_config.h"
#include "Location_Tracker.h"
#include "Speed_Tracker.h"
#include "Current_Tracker.h"
#include "Move_Reconstruct.h"
#include "Location_Interp.h"

/****************************************  电流输出(电流控制)  ****************************************/
/****************************************  电流输出(电流控制)  ****************************************/
/**
  * @brief  电流输出
  * @param  current: 输出电流
  * @retval NULL
**/
void Control_Cur_To_Electric(int16_t current)
{
	//输出FOC电流
	motor_control.foc_current = current;
	//输出FOC位置
	if(motor_control.foc_current > 0)				motor_control.foc_location = motor_control.est_location + Move_Divide_NUM;
	else if(motor_control.foc_current < 0)	motor_control.foc_location = motor_control.est_location - Move_Divide_NUM;
	else																		motor_control.foc_location = motor_control.est_location;
	//输出任务到驱动
	REIN_HW_Elec_SetDivideElec(motor_control.foc_location, motor_control.foc_current);
	//CurrentControl_Out_FeedTrack(motor_control.foc_location, motor_control.foc_current, false, true);
}

/****************************************  PID控制(速度控制)  ****************************************/
/****************************************  PID控制(速度控制)  ****************************************/
//PID控制
Control_PID_Typedef pid;

/**
  * @brief  参数配置
  * @param  _k
  * @retval NULL
**/
void Control_PID_SetKP(uint16_t _k)
{
	if(_k <= 1024){		pid.kp = _k;		pid.valid_kp = true;		}
	else{															pid.valid_kp = false;		}
}

/**
  * @brief  参数配置
  * @param  _k
  * @retval NULL
**/
void Control_PID_SetKI(uint16_t _k)
{
	if(_k <= 1024){		pid.ki = _k;		pid.valid_ki = true;		}
	else{															pid.valid_ki = false;		}
}

/**
  * @brief  参数配置
  * @param  _k
  * @retval NULL
**/
void Control_PID_SetKD(uint16_t _k)
{
	if(_k <= 1024){		pid.kd = _k;		pid.valid_kd = true;		}
	else{															pid.valid_kd = false;		}
}

/**
  * @brief  PID参数恢复
  * @param  NULL
  * @retval NULL
**/
void Control_PID_Set_Default(void)
{
	Control_PID_SetKP(De_PID_KP);
	Control_PID_SetKI(De_PID_KI);
	Control_PID_SetKD(De_PID_KD);
}

/**
  * @brief  控制器PID初始化
  * @param  NULL
  * @retval NULL
**/
void Control_PID_Init(void)
{
	//前置配置无效时,加载默认配置
	if(!pid.valid_kp)				{	Control_PID_SetKP(De_PID_KP);		}
	if(!pid.valid_ki)				{	Control_PID_SetKI(De_PID_KI);		}
	if(!pid.valid_kd)				{	Control_PID_SetKD(De_PID_KD);		}
	
	//控制参数
	pid.v_error = 0;	pid.v_error_last = 0;
	pid.op = 0;				pid.oi = 0;			pid.od = 0;	
	pid.i_mut = 0;		pid.i_dec = 0;
	pid.out = 0;
}

/**
  * @brief  PID电流控制
  * @param  _speed    控制速度
  * @retval NULL
**/
void Control_PID_To_Electric(int32_t _speed)
{
	//误差
	pid.v_error_last = pid.v_error;
	pid.v_error = _speed - motor_control.est_speed;	//速度误差
	if(pid.v_error > ( 1024 * 1024))	pid.v_error = ( 1024 * 1024);
	if(pid.v_error < (-1024 * 1024))	pid.v_error = (-1024 * 1024);
	//op输出
	pid.op = ((pid.kp) * (pid.v_error));
	//oi输出
	pid.i_mut += ((pid.ki) * (pid.v_error));
	pid.i_dec  = (pid.i_mut >> 10);
	pid.i_mut -= (pid.i_dec << 10);
	pid.oi    += (pid.i_dec);
	if(pid.oi >      (  Current_Rated_Current << 10 ))	pid.oi = (  Current_Rated_Current << 10 );	//限制为额定电流 * 1024
	else if(pid.oi < (-(Current_Rated_Current << 10)))	pid.oi = (-(Current_Rated_Current << 10));	//限制为额定电流 * 1024
	//od输出
	pid.od = (pid.kd) * (pid.v_error - pid.v_error_last);
	//综合输出计算
	pid.out = (pid.op + pid.oi + pid.od) >> 10;
	if(pid.out > 			Current_Rated_Current)		pid.out =  Current_Rated_Current;
	else if(pid.out < -Current_Rated_Current)		pid.out = -Current_Rated_Current;
	
	//输出FOC电流
	motor_control.foc_current = pid.out;
	//输出FOC位置
	if(motor_control.foc_current > 0)				motor_control.foc_location = motor_control.est_location + Move_Divide_NUM;
	else if(motor_control.foc_current < 0)	motor_control.foc_location = motor_control.est_location - Move_Divide_NUM;
	else																		motor_control.foc_location = motor_control.est_location;
	//输出任务到驱动
	REIN_HW_Elec_SetDivideElec(motor_control.foc_location, motor_control.foc_current);
	//CurrentControl_Out_FeedTrack(motor_control.foc_location, motor_control.foc_current, false, true);
}

/****************************************  DCE控制器(位置控制)  ****************************************/
/****************************************  DCE控制器(位置控制)  ****************************************/
//DCE控制
Control_DCE_Typedef dce;

/**
  * @brief  参数配置
  * @param  _k
  * @retval NULL
**/
void Control_DCE_SetKP(uint16_t _k)
{
	if(_k <= 1024){		dce.kp = _k;		dce.valid_kp = true;		}
	else{															dce.valid_kp = false;		}
}

/**
  * @brief  参数配置
  * @param  _k
  * @retval NULL
**/
void Control_DCE_SetKI(uint16_t _k)
{
	if(_k <= 1024){		dce.ki = _k;		dce.valid_ki = true;		}
	else{															dce.valid_ki = false;		}
}

/**
  * @brief  参数配置
  * @param  _k
  * @retval NULL
**/
void Control_DCE_SetKV(uint16_t _k)
{
	if(_k <= 1024){		dce.kv = _k;		dce.valid_kv = true;		}
	else{															dce.valid_kv = false;		}
}

/**
  * @brief  参数配置
  * @param  _k
  * @retval NULL
**/
void Control_DCE_SetKD(uint16_t _k)
{
	if(_k <= 1024){		dce.kd = _k;		dce.valid_kd = true;		}
	else{															dce.valid_kd = false;		}
}

/**
  * @brief  DCE参数恢复
  * @param  NULL
  * @retval NULL
**/
void Control_DCE_Set_Default(void)
{
	Control_DCE_SetKP(De_DCE_KP);
	Control_DCE_SetKI(De_DCE_KI);
	Control_DCE_SetKV(De_DCE_KV);
	Control_DCE_SetKD(De_DCE_KD);
}

/**
  * @brief  控制器DCE初始化
  * @param  NULL
  * @retval NULL
**/
void Control_DCE_Init(void)
{
	//前置配置无效时,加载默认配置
	if(!dce.valid_kp)				{	Control_DCE_SetKP(De_DCE_KP);		}
	if(!dce.valid_ki)				{	Control_DCE_SetKI(De_DCE_KI);		}
	if(!dce.valid_kv)				{	Control_DCE_SetKV(De_DCE_KV);		}
	if(!dce.valid_kd)				{	Control_DCE_SetKD(De_DCE_KD);		}
	
	//控制参数(基本部分)
	dce.p_error = 0;	dce.v_error = 0;
	dce.op = 0;				dce.oi = 0;			dce.od = 0;	
	dce.i_mut = 0;		dce.i_dec = 0;
	dce.out = 0;
}

/**
  * @brief  DCE电流控制
  * @param  _location 控制位置
  * @param  _speed    控制速度
  * @retval NULL
**/
void Control_DCE_To_Electric(int32_t _location, int32_t _speed)
{
	//误差
	dce.p_error = _location - motor_control.est_location;
	dce.v_error = (_speed - motor_control.est_speed) >> 7;	//速度误差缩小至1/128
	if(dce.p_error > ( 3200))	dce.p_error = ( 3200);				//限制位置误差在1/16圈内(51200/16)
	if(dce.p_error < (-3200))	dce.p_error = (-3200);
	if(dce.v_error > ( 4000))	dce.v_error = ( 4000);				//限制速度误差在10r/s内(51200*10/128)
	if(dce.v_error < (-4000))	dce.v_error = (-4000);
	//op输出计算
	dce.op     = ((dce.kp) * (dce.p_error));
	//oi输出计算
	dce.i_mut += ((dce.ki) * (dce.p_error));
	dce.i_mut += ((dce.kv) * (dce.v_error));
	dce.i_dec  = (dce.i_mut >> 7);
	dce.i_mut -= (dce.i_dec << 7);
	dce.oi    += (dce.i_dec);
	if(dce.oi >      (  Current_Rated_Current << 10 ))	dce.oi = (  Current_Rated_Current << 10 );	//限制为额定电流 * 1024
	else if(dce.oi < (-(Current_Rated_Current << 10)))	dce.oi = (-(Current_Rated_Current << 10));	//限制为额定电流 * 1024
	//od输出计算
	dce.od = ((dce.kd) * (dce.v_error));
	//综合输出计算
	dce.out = (dce.op + dce.oi + dce.od) >> 10;
	if(dce.out > 			Current_Rated_Current)		dce.out =  Current_Rated_Current;
	else if(dce.out < -Current_Rated_Current)		dce.out = -Current_Rated_Current;

	//输出FOC电流
	motor_control.foc_current = dce.out;
	//输出FOC位置
	if(motor_control.foc_current > 0)			motor_control.foc_location = motor_control.est_location + Move_Divide_NUM;
	else if(motor_control.foc_current < 0)	motor_control.foc_location = motor_control.est_location - Move_Divide_NUM;
	else																		motor_control.foc_location = motor_control.est_location;
	//输出任务到驱动
	REIN_HW_Elec_SetDivideElec(motor_control.foc_location, motor_control.foc_current);
	//CurrentControl_Out_FeedTrack(motor_control.foc_location, motor_control.foc_current, false, true);
}

/****************************************  Motor_Contro_Debug  ****************************************/
/****************************************  Motor_Contro_Debug  ****************************************/
Motor_Control_Debug_Typedef		mc_debug;	//控制调试

/**
  * @brief  多功能调试速度
  * @param  NULL
  * @retval NULL
**/
void Motor_MultiDebug_Location(void)
{
	mc_debug.mut += mc_debug.speed * 420;
	mc_debug.dec  = mc_debug.mut >> 23;
	mc_debug.mut -= mc_debug.dec << 23;
	
	motor_control.soft_location += mc_debug.dec;
	motor_control.soft_speed = mc_debug.speed;
}

/**
  * @brief  多功能调试速度
  * @param  NULL
  * @retval NULL
**/
void Motor_MultiDebug_Speed(void)
{
	motor_control.soft_speed = mc_debug.speed;
}

/****************************************  Motor_Control  ****************************************/
/****************************************  Motor_Control  ****************************************/
Motor_Control_Typedef 				motor_control;				//控制主结构

/**
  * @brief  电机模式配置
  * @param  _mode
  * @retval NULL
**/
void Motor_Control_SetMotorMode(Motor_Mode _mode)
{
	motor_control.mode_order = _mode;
	motor_control.valid_mode = true;
}

/**
  * @brief  堵转保护开关
  * @param  _switch
  * @retval NULL
**/
void Motor_Control_SetStallSwitch(bool _switch)
{
	motor_control.stall_switch = _switch;
	motor_control.valid_stall_switch = true;
}

/**
  * @brief  控制模式参数恢复
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_SetDefault(void)
{
	Motor_Control_SetMotorMode(De_Motor_Mode);
	Motor_Control_SetStallSwitch(De_Motor_Stall);
}

/**
  * @brief  写入目标位置
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Location(int32_t value)
{
	motor_control.goal_location = value;
}
	
/**
  * @brief  写入目标速度
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Speed(int32_t value)
{
	if((value >= -Move_Rated_Speed) && (value <= Move_Rated_Speed))
	{
		motor_control.goal_speed = value;
	}
}

/**
  * @brief  写入目标电流
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Current(int16_t value)
{
	if((value >= -Current_Rated_Current) && (value <= Current_Rated_Current))
	{
		motor_control.goal_current = value;
	}
}

/**
  * @brief  写入目标失能
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Disable(uint16_t value)
{
	motor_control.goal_disable = (bool)value;
}

/**
  * @brief  写入目标刹车
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Brake(uint16_t value)
{
	motor_control.goal_brake = (bool)value;
}

/**
  * @brief  电机控制初始化
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Init(void)
{	
	//前置配置无效时,加载默认配置
	if(!motor_control.valid_mode)					{		Motor_Control_SetMotorMode(De_Motor_Mode);		}
	if(!motor_control.valid_stall_switch)	{		Motor_Control_SetStallSwitch(De_Motor_Stall);	}

	//模式
	motor_control.mode_run = Control_Mode_Stop;
	//读取
	motor_control.real_lap_location = 0;
	motor_control.real_lap_location_last = 0;
	motor_control.real_location = 0;
	motor_control.real_location_last = 0;
	//估计
	motor_control.est_speed_mut = 0;
	motor_control.est_speed = 0;
	motor_control.est_lead_location = 0;
	motor_control.est_location = 0;
	motor_control.est_error = 0;
	//硬目标
	motor_control.goal_location = 0;
	motor_control.goal_speed = 0;
	motor_control.goal_current = 0;
	motor_control.goal_disable = false;
	motor_control.goal_brake = false;
	//软目标
	motor_control.soft_location = 0;
	motor_control.soft_speed = 0;
	motor_control.soft_current = 0;
	motor_control.soft_disable = false;
	motor_control.soft_brake = false;
	motor_control.soft_new_curve = false;
	//输出
	motor_control.foc_location = 0;
	motor_control.foc_current = 0;
	//堵转识别
	motor_control.stall_time_us = 0;
	motor_control.stall_flag = false;
	//过载识别
	motor_control.overload_time_us = 0;
	motor_control.overload_flag = false;
	//状态
	motor_control.state = Control_State_Stop;		
	
	/**********  控制算法初始化  **********/
	Control_PID_Init();
	Control_DCE_Init();
	
	/********** 轨迹规划 **********/
	Location_Tracker_Init();	//位置跟踪器初始化
	Speed_Tracker_Init();			//速度跟踪器初始化
	Current_Tracker_Init();		//电流跟踪器初始化
	Move_Reconstruct_Init();	//运动重构器初始化
	Location_Interp_Init();		//位置插补器初始化
}

/**
  * @brief  控制器任务回调
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Callback(void)
{
	/************************************ 首次进入控制回调 ************************************/
	/************************************ 首次进入控制回调 ************************************/
	static bool first_call = true;
	if(first_call)
	{
		//读取(为了方便将XDrive代码移植到软件编码器,将位置读取初始化部分全部放置在此处运行)
		motor_control.real_lap_location				= mt6816.rectify_angle;
		motor_control.real_lap_location_last	= mt6816.rectify_angle;
		motor_control.real_location						= mt6816.rectify_angle;
		motor_control.real_location_last			= mt6816.rectify_angle;
		//第一次运行强制退出
		first_call = false;
		return;
	}
	
	/************************************ 数据采集 ************************************/
	/************************************ 数据采集 ************************************/
	int32_t		sub_data;		//用于各个算差
	//读取单圈位置
	motor_control.real_lap_location_last = motor_control.real_lap_location;
	motor_control.real_lap_location = mt6816.rectify_angle;
	//回环检测
	sub_data = motor_control.real_lap_location - motor_control.real_lap_location_last;
	if(sub_data > (Move_Pulse_NUM >> 1))				sub_data -= Move_Pulse_NUM;
	else if(sub_data < -(Move_Pulse_NUM >> 1))	sub_data += Move_Pulse_NUM;
	//读取位置
	motor_control.real_location_last = motor_control.real_location;
	motor_control.real_location += sub_data;
	
	/************************************ 数据估计 ************************************/
	/************************************ 数据估计 ************************************/
	//估计速度
	motor_control.est_speed_mut += (	((motor_control.real_location - motor_control.real_location_last) * (CONTROL_FREQ_HZ))
																	+ ((int32_t)(motor_control.est_speed  << 5) - (int32_t)(motor_control.est_speed))
																	);
	motor_control.est_speed      = (motor_control.est_speed_mut >> 5);																	//(取整)(向0取整)(保留符号位)
	motor_control.est_speed_mut  = ((motor_control.est_speed_mut) - ((motor_control.est_speed << 5)));	//(取余)(向0取整)(保留符号位)
	//估计位置
	motor_control.est_lead_location = Motor_Control_AdvanceCompen(motor_control.est_speed);
	motor_control.est_location = motor_control.real_location + motor_control.est_lead_location;
	//估计误差
	motor_control.est_error = motor_control.soft_location - motor_control.est_location;
	
	/************************************ 运动控制 ************************************/
	/************************************ 运动控制 ************************************/
	if(0){}
	//输出休眠(禁用了输出IO,恢复输出需要单独处理)
	else if(
		 (motor_control.stall_flag)			//堵转标志置位
	|| (motor_control.soft_disable)		//软目标_失能指令
	|| ((!mt6816.rectify_valid))			//编码器校准表无效
	){
		Motor_Control_Clear_Integral();		//清除积分
		motor_control.foc_location = 0;		//清FOC位置
		motor_control.foc_current = 0;		//清FOC电流
		REIN_HW_Elec_SetSleep();					//驱动休眠
		//CurrentControl_OutSleep();			//XDrive采用硬件逻辑电流控制,自动休眠
	}
	//输出刹车
	else if(
		 (motor_control.soft_brake)			//软目标_刹车指令
	){
		Motor_Control_Clear_Integral();		//清除积分
		motor_control.foc_location = 0;		//清FOC位置
		motor_control.foc_current = 0;		//清FOC电流
		REIN_HW_Elec_SetBrake();					//驱动刹车
		//CurrentControl_OutBrake();			//XDrive采用硬件逻辑电流控制,自动刹车
	}
	else{
		//运行模式分支
		switch(motor_control.mode_run)
		{
			//测试
			case Motor_Mode_Debug_Location:		Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			case Motor_Mode_Debug_Speed:			Control_PID_To_Electric(motor_control.soft_speed);																		break;
			//停止
			case Control_Mode_Stop:						REIN_HW_Elec_SetSleep();																															break;
			//DIG(CAN/RS485)
			case Motor_Mode_Digital_Location:	Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			case Motor_Mode_Digital_Speed:		Control_PID_To_Electric(motor_control.soft_speed);																		break;
			case Motor_Mode_Digital_Current:	Control_Cur_To_Electric(motor_control.soft_current);																	break;
			case Motor_Mode_Digital_Track:		Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			//MoreIO(PWM/PUL)
			case Motor_Mode_PWM_Location:			Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			case Motor_Mode_PWM_Speed:				Control_PID_To_Electric(motor_control.soft_speed);																		break;
			case Motor_Mode_PWM_Current:			Control_Cur_To_Electric(motor_control.soft_current);																	break;
			case Motor_Mode_PULSE_Location:		Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			//其他非法模式
			default:	break;
		}
	}

	/************************************ 模式变更 ************************************/
	/************************************ 模式变更 ************************************/
	//变更
	if(motor_control.mode_run != motor_control.mode_order)
	{
		motor_control.mode_run = motor_control.mode_order;
		switch(motor_control.mode_run){
			//测试
			case Motor_Mode_Debug_Location:		Signal_MoreIO_Config(MoreIO_Mode_Disable);					motor_control.soft_new_curve = true;	break;
			case Motor_Mode_Debug_Speed:			Signal_MoreIO_Config(MoreIO_Mode_Disable);					motor_control.soft_new_curve = true;	break;
			//停止
			case Control_Mode_Stop:						Signal_MoreIO_Config(MoreIO_Mode_Disable);					motor_control.soft_new_curve = true;	break;
			//DIG(CAN/RS485)
			case Motor_Mode_Digital_Location:	Signal_MoreIO_Config(MoreIO_Mode_Disable);					motor_control.soft_new_curve = true;	break;
			case Motor_Mode_Digital_Speed:		Signal_MoreIO_Config(MoreIO_Mode_Disable);					motor_control.soft_new_curve = true;	break;
			case Motor_Mode_Digital_Current:	Signal_MoreIO_Config(MoreIO_Mode_Disable);					motor_control.soft_new_curve = true;	break;
			case Motor_Mode_Digital_Track:		Signal_MoreIO_Config(MoreIO_Mode_Disable);					motor_control.soft_new_curve = true;	break;
			//MoreIO(PWM/PUL)
			case Motor_Mode_PWM_Location:			Signal_MoreIO_Config(MoreIO_Mode_PWM_Location);			motor_control.soft_new_curve = true;	break;
			case Motor_Mode_PWM_Speed:				Signal_MoreIO_Config(MoreIO_Mode_PWM_Speed);				motor_control.soft_new_curve = true;	break;
			case Motor_Mode_PWM_Current:			Signal_MoreIO_Config(MoreIO_Mode_PWM_Current);			motor_control.soft_new_curve = true;	break;
			case Motor_Mode_PULSE_Location:		Signal_MoreIO_Config(MoreIO_Mode_PULSE_Locatioon);	motor_control.soft_new_curve = true;	break;
			//其他非法模式
			default:	break;
		}
	}

	/************************************ 硬目标提取 ************************************/
	/************************************ 硬目标提取 ************************************/
	//提取
	switch(motor_control.mode_run){
		//测试
		case Motor_Mode_Debug_Location:		/*无目标可提取*/			break;
		case Motor_Mode_Debug_Speed:			/*无目标可提取*/			break;
		//停止
		case Control_Mode_Stop:						/*无目标可提取*/			break;
		//DIG(CAN/RS485)
		case Motor_Mode_Digital_Location:	/*硬目标已由通讯任务推入*/	break;
		case Motor_Mode_Digital_Speed:		/*硬目标已由通讯任务推入*/	break;
		case Motor_Mode_Digital_Current:	/*硬目标已由通讯任务推入*/	break;
		case Motor_Mode_Digital_Track:		/*硬目标已由通讯任务推入*/	break;
		//MoreIO(PWM/PUL)
		case Motor_Mode_PWM_Location:
		case Motor_Mode_PWM_Speed:
		case Motor_Mode_PWM_Current:
			Signal_MoreIO_Capture_Goal();	//MoreIO接口获取数据
			motor_control.goal_location	= signal_moreio.goal_location;	//提取目标位置
			motor_control.goal_speed	  = signal_moreio.goal_speed;			//提取目标速度
			motor_control.goal_current  = signal_moreio.goal_current;		//提取目标电流
			motor_control.goal_disable	= signal_moreio.goal_disable;		//提取目标失能
			motor_control.goal_brake		= signal_moreio.goal_brake;			//提取目标刹车
		break;
		case Motor_Mode_PULSE_Location:			
			Signal_MoreIO_Capture_Goal();	//MoreIO接口获取数据
			motor_control.goal_location	+= signal_moreio.goal_location;	//提取目标位置(Count模式借用目标位置存放目标位置增量)
			motor_control.goal_disable	= signal_moreio.goal_disable;		//提取目标失能
			motor_control.goal_brake		= signal_moreio.goal_brake;			//提取目标刹车
		break;
			//其他非法模式
		default:	break;
	}
	//限制
	if(motor_control.goal_speed					>  Move_Rated_Speed)			motor_control.goal_speed		=  Move_Rated_Speed;
	else if(motor_control.goal_speed		< -Move_Rated_Speed)			motor_control.goal_speed		= -Move_Rated_Speed;
	if(motor_control.goal_current				>  Current_Rated_Current)	motor_control.goal_current	=  Current_Rated_Current;
	else if(motor_control.goal_current	< -Current_Rated_Current)	motor_control.goal_current	= -Current_Rated_Current;

	/************************************ 触发信号发生器新动作 ************************************/
	/************************************ 触发信号发生器新动作 ************************************/
	//额外的触发新发生器
	if(	 ((motor_control.soft_disable) && (!motor_control.goal_disable))	//失能指令关闭
		|| ((motor_control.soft_brake)   && (!motor_control.goal_brake))		//刹车指令关闭
	){
		motor_control.soft_new_curve = true;
	}
	//信号发生器刷新
	if(motor_control.soft_new_curve){
		motor_control.soft_new_curve = false;
		//控制重载和功率模块唤醒
		Motor_Control_Clear_Integral();	//清除控制积分项目
		Motor_Control_Clear_Stall();		//清除堵转识别
		//CurrentControl_OutWakeUp();		//XDrive采用硬件逻辑电流控制,自动唤醒
		//CurrentControl_OutRunning();	//XDrive采用硬件逻辑电流控制,自动唤醒
		switch(motor_control.mode_run){
			//测试
			case Motor_Mode_Debug_Location:		break;
			case Motor_Mode_Debug_Speed:			break;
			//停止
			case Control_Mode_Stop:						break;
			//DIG(CAN/RS485)
			case Motor_Mode_Digital_Location:	Location_Tracker_NewTask(	motor_control.est_location,	motor_control.est_speed);	break;
			case Motor_Mode_Digital_Speed:		Speed_Tracker_NewTask(		motor_control.est_speed);															break;
			case Motor_Mode_Digital_Current:	Current_Tracker_NewTask(	motor_control.foc_current);														break;
			case Motor_Mode_Digital_Track:		Move_Reconstruct_NewTask(	motor_control.est_location,	motor_control.est_speed);	break;
			//MoreIO(PWM/PUL)
			case Motor_Mode_PWM_Location:			Location_Tracker_NewTask(	motor_control.est_location,	motor_control.est_speed);	break;
			case Motor_Mode_PWM_Speed:				Speed_Tracker_NewTask(		motor_control.est_speed);															break;
			case Motor_Mode_PWM_Current:			Current_Tracker_NewTask(	motor_control.foc_current);														break;
			case Motor_Mode_PULSE_Location:		Location_Interp_NewTask(	motor_control.est_location,	motor_control.est_speed);
											/** 脉冲位置获取相对值,需要初始化 **/	motor_control.goal_location = motor_control.est_location;		break;
			//其他非法模式
			default:	break;
		}
	}

	/************************************ 软目标提取 ************************************/
	/************************************ 软目标提取 ************************************/
	//提取(软位置,软速度,软电流)
	switch(motor_control.mode_run){
		//测试
		case Motor_Mode_Debug_Location:		Motor_MultiDebug_Location();	break;
		case Motor_Mode_Debug_Speed:			Motor_MultiDebug_Speed();			break;
		//停止
		case Control_Mode_Stop:						break;
		//DIG(CAN/RS485)
		case Motor_Mode_Digital_Location:	Location_Tracker_Capture_Goal(motor_control.goal_location);
																			motor_control.soft_location = location_tck.go_location;
																			motor_control.soft_speed    = location_tck.go_speed;
																			break;
		case Motor_Mode_Digital_Speed:		Speed_Tracker_Capture_Goal(motor_control.goal_speed);
																			motor_control.soft_speed    = speed_tck.go_speed;
																			break;
		case Motor_Mode_Digital_Current:	Current_Tracker_Capture_Goal(motor_control.goal_current);
																			motor_control.soft_current  = current_tck.go_current;
																			break;
		case Motor_Mode_Digital_Track:		Move_Reconstruct_Capture_Goal(motor_control.goal_location, motor_control.goal_speed);
																			motor_control.soft_location = move_reco.go_location;
																			motor_control.soft_speed    = move_reco.go_speed;
																			break;
		//MoreIO(PWM/PUL)
		case Motor_Mode_PWM_Location:			Location_Tracker_Capture_Goal(motor_control.goal_location);
																			motor_control.soft_location = location_tck.go_location;
																			motor_control.soft_speed    = location_tck.go_speed;
																			break;			
		case Motor_Mode_PWM_Speed:				Speed_Tracker_Capture_Goal(motor_control.goal_speed);
																			motor_control.soft_speed    = speed_tck.go_speed;
																			break;
		case Motor_Mode_PWM_Current:			Current_Tracker_Capture_Goal(motor_control.goal_current);
																			motor_control.soft_current  = current_tck.go_current;
																			break;
		case Motor_Mode_PULSE_Location:		Location_Interp_Capture_Goal(motor_control.goal_location);
																			motor_control.soft_location = location_interp.go_location;
																			motor_control.soft_speed    = location_interp.go_speed;
																			break;
		//其他非法模式
		default:	break;
	}
	//提取(软失能,软刹车)
	motor_control.soft_disable = motor_control.goal_disable;
	motor_control.soft_brake = motor_control.goal_brake;
	
	/************************************ 状态识别 ************************************/
	/************************************ 状态识别 ************************************/
	int32_t abs_out_electric = abs(motor_control.foc_current);
	//堵转检测
	if( ((motor_control.mode_run == Motor_Mode_Digital_Current) || (motor_control.mode_run == Motor_Mode_PWM_Current))	//电流模式
	 && (abs_out_electric != 0)																																													//有输出电流
	 && (abs(motor_control.est_speed) < (Move_Pulse_NUM/5))																															//低于1/5转/s
	){
		if(motor_control.stall_time_us >= (1000 * 1000))	motor_control.stall_flag = true;
		else																							motor_control.stall_time_us += CONTROL_PERIOD_US;
	}
	else if( (abs_out_electric == Current_Rated_Current)						//额定电流
				&& (abs(motor_control.est_speed) < (Move_Pulse_NUM/5))		//低于1/5转/s
	){
		if(motor_control.stall_time_us >= (1000 * 1000))	motor_control.stall_flag = true;
		else																							motor_control.stall_time_us += CONTROL_PERIOD_US;
	}
	else{
		motor_control.stall_time_us = 0;
		//堵转标志不能自清除，需要外部指令才能清除
	}

	//过载检测
	if(abs_out_electric == Current_Rated_Current){		//额定电流
		if(motor_control.overload_time_us >= (1000 * 1000))	motor_control.overload_flag = true;
		else																								motor_control.overload_time_us += CONTROL_PERIOD_US;
	}
	else{
		motor_control.overload_time_us = 0;
		motor_control.overload_flag = false;//过载标志可自清除
	}
	
	/************************************ 状态记录 ************************************/
	/************************************ 状态记录 ************************************/
	//统一的电机状态
	if(motor_control.mode_run == Control_Mode_Stop)	//停止模式
		motor_control.state = Control_State_Stop;
	else if(motor_control.stall_flag)								//堵转标志置位
		motor_control.state = Control_State_Stall;
	else if(motor_control.overload_flag)						//过载标志置位
		motor_control.state = Control_State_Overload;
	else
	{
		if(motor_control.mode_run == Motor_Mode_Digital_Location){
			if( (motor_control.soft_location == motor_control.goal_location)
			 && (motor_control.soft_speed == 0))
				motor_control.state = Control_State_Finish;		//软硬目标匹配
			else
				motor_control.state = Control_State_Running;
		}
		else if(motor_control.mode_run == Motor_Mode_Digital_Speed){
			if(motor_control.soft_speed == motor_control.goal_speed)
				motor_control.state = Control_State_Finish;		//软硬目标匹配
			else
				motor_control.state = Control_State_Running;
		}
		else if(motor_control.mode_run == Motor_Mode_Digital_Current){
			if(motor_control.soft_current == motor_control.goal_current)
				motor_control.state = Control_State_Finish;		//软硬目标匹配
			else
				motor_control.state = Control_State_Running;
		}
		else{
			motor_control.state = Control_State_Finish;			//软硬目标匹配
		}
	}
}

/**
  * @brief  设清除积分
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Clear_Integral(void)
{
	//在此处清除整个控制段代码中的积分项
	
	//PID
	pid.i_mut = 0;
	pid.i_dec = 0;
	pid.oi = 0;
	
	//DCE
	dce.i_mut = 0;
	dce.i_dec = 0;
	dce.oi = 0;
	
	//Debug
	mc_debug.mut = 0;
	mc_debug.dec = 0;
}

/**
  * @brief  清除堵转识别
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Clear_Stall(void)
{
	motor_control.stall_time_us = 0;		//堵转计时器
	motor_control.stall_flag = false;		//堵转标志
}

/**
  * @brief  超前角补偿
  * @param  _speed:补偿速度
  * @retval 补偿角度
**/
int32_t Motor_Control_AdvanceCompen(int32_t _speed)
{
	/******************** !!!!! 重要1：本补偿表提取自DPS系列代码                                                  !!!!! ********************/
	/******************** !!!!! 重要2：由于源于其他传感器数据，本补偿表并不完全适合TLE5012和MT6816                !!!!! ********************/
	/******************** !!!!! 重要3：由于测量传感器的最佳补偿曲线十分费时繁琐，作者并不准备在近期进行校准表测量 !!!!! ********************/

	int32_t compen;
	if(_speed < 0){
		if(_speed > -100000)				compen = 0;
		else if(_speed > -1300000)	compen = (((_speed +  100000) * 262) >> 20) -   0;
		else if(_speed > -2200000)	compen = (((_speed + 1300000) * 105) >> 20) - 300;
		else												compen = (((_speed + 2200000) *  52) >> 20) - 390;
		if(compen < -430)						compen = -430;
	}
	else{
		if(_speed < 100000)					compen = 0;																					//(      0,  0) ~ ( 100000,  0)
		else if(_speed <  1300000)	compen = (((_speed -  100000) * 262) >> 20) +   0;	//( 100000,  0) ~ (1300000,300)
		else if(_speed <  2200000)	compen = (((_speed - 1300000) * 105) >> 20) + 300;	//(1300000,300) ~ (2200000,390)
		else												compen = (((_speed - 2200000) *  52) >> 20) + 390;	//(2200000,390) ~ 
		if(compen > 430)						compen = 430;
	}
	return compen;
}



