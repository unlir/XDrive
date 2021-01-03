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
  ** @file     : signal_port.c/h
  ** @brief    : 信号接口
  ** @versions : 2.1.13
  ** @time     : 2020/09/11
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef SIGNAL_PORT_H
#define SIGNAL_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

/****************************************** Modbus接口 ******************************************/
/****************************************** Modbus接口 ******************************************/
/****************************************** Modbus接口 ******************************************/
//Demo_Dir
#define Modbus_Dir_Master			0x00			//主机
#define Modbus_Dir_Slaves			0x01			//从机

#define Demo4_Dir			Modbus_Dir_Master

/**
  * Modbus接口结构体
**/
typedef struct{
	//配置(ID)
	#define		De_Modbus_ID	1			//默认
	bool			valid_modbus_id;			//Modbus_ID配置有效
	//配置
	uint16_t	id_run;					//ID
	uint16_t	id_order;				//Modbus_ID(存储数据,下次一定)
}Signal_Modbus_Typedef;
extern Signal_Modbus_Typedef signal_modbus;

void Signal_Modbus_Set_ID(uint16_t value);				//设置Modbus接口ID
void Signal_Modbus_Set_Default(void);							//Modbus配置恢复

void Signal_Modbus_Init(void);										//Modbus接口初始化
void Signal_Modbus_Low_Priority_Callback(void);		//Modbus低优先级回调

/****************************************** Signal_Count接口 ******************************************/
/****************************************** Signal_Count接口 ******************************************/
/****************************************** Signal_Count接口 ******************************************/
/**
  * Signal_Count接口结构体
**/
typedef struct{
	//配置
	#define 	De_SubDivide	16			//默认
	#define		De_EN_inve		false		//默认
	#define		De_DIR_inve		false		//默认
	bool			valid_subdivide;	//细分配置有效
	bool			valid_en_inve;		//使能信号配置有效
	bool			valid_dir_inve;		//方向信号配置有效
	//配置
	uint16_t	subdivide;			//脉冲信号细分
	uint16_t	subdivide_form;	//细分映射
	bool			en_inve;				//使能信号翻转(false:)
	bool			dir_inve;				//方向信号翻转(false:高电平增-低电平减   true:高电平减-低电平增)	//采集数据(公共EN)
	//采集数据(公共EN)
	bool			en_valid;				//En有效标志
	//采集数据(STEP)(基本脉冲处理)
	int16_t		sampling_count_last;	//采样脉冲计数器(必须i16)
	int16_t		sampling_count;				//采样脉冲计数器(必须i16)
	int16_t		sampling_count_sub;		//采样脉冲差值(必须i16)
	//采集数据(STEP)(缓冲输出)
	int32_t		interp_out;					//插补输出(用于输出到控制)
}Signal_Count_Typedef;
extern Signal_Count_Typedef sg_cut;

//参数配置
void Signal_Count_SetFraction(uint16_t freq);		//Signal_Count设置细分值
void Signal_Count_SetEnInve(bool control);			//Signal_Count设置使能翻转
void Signal_Count_SetDirInve(bool control);			//Signal_Count设置方向翻转
//等待添加恢复函数

//初始化
void Signal_Count_Init(void);					//Signal_Count采集初始化

//功能接口
void Signal_Count_Config(void);				//Signal_Count采集配置
void Signal_Count_DeConfig(void);			//Signal_Count清理配置
void Signal_Count_Capture_Goal(void);	//Signal_Count获取目标
void Signal_Count_Dir_Res(void);			//Signal_Count计数器方向变更处理,由(初始化/方向设置/外部中断)触发

/****************************************** Signal_PWM接口 ******************************************/
/****************************************** Signal_PWM接口 ******************************************/
/****************************************** Signal_PWM接口 ******************************************/
typedef struct{
	//配置
	#define		De_TOP_Width				(20 * 1000)							//默认(20ms)
	#define		De_BOTTOM_Width			(0)											//默认(0ms)
	#define		De_TOP_Location			(Move_Pulse_NUM / 2)		//默认(半圈)
	#define		De_BOTTOM_Location	(0)											//默认(0p)
	#define		De_TOP_Speed				(Move_Rated_Speed)			//默认(额定速度)
	#define		De_BOTTOM_Speed			(0)											//默认(0p/s)
	#define		De_TOP_Current			(Current_Rated_Current)	//默认(额定电流)
	#define		De_BOTTOM_Current		(0)											//默认(0A)
	bool			valid_top_width;				//各种有效标志
	bool			valid_bottom_width;			//各种有效标志
	bool			valid_top_location;			//各种有效标志
	bool			valid_bottom_location;	//各种有效标志
	bool			valid_top_speed;				//各种有效标志
	bool			valid_bottom_speed;			//各种有效标志
	bool			valid_top_current;			//各种有效标志
	bool			valid_bottom_current;		//各种有效标志
	//配置
	uint16_t	top_width;				//Top_PWM
	uint16_t	bottom_width;			//Bottom_PWM
	int32_t		top_location;			//Top_Location
	int32_t		bottom_location;	//Bottom_Location
	int32_t		top_speed;				//Top_Speed
	int32_t		bottom_speed;			//Bottom_Speed
	int16_t		top_current;			//Top_Current
	int16_t		bottom_current;		//Bottom_Current
	//采集数据(PWM)
	uint16_t	h_width;				//PWM高宽度
	uint16_t	period;					//PWM周期
	uint8_t		count_rising;		//PWM上升沿计数器镜像
	uint8_t		count_falling;	//PWM下降沿计数器镜像
	uint8_t		count_update;		//PWM更新计数器
	bool		whole_h_flag;			//PWM全高标志
	bool		whole_l_flag;			//PWM全低标志
	bool		ready_first;			//PWM就绪标志(完成一次PWM采集后置位)
	bool		ready_second;			//PWM就绪标志(完成二次PWM采集后置位)
	bool		ready_third;			//PWM就绪标志(完成三次PWM采集后置位)
	uint16_t	valid_width;		//PWM有效宽度
}Signal_PWM_Typedef;
extern Signal_PWM_Typedef sg_pwm;

//参数配置
void Signal_PWM_Set_TopWidth(uint16_t width);					//Signal_PWM设置最大脉宽
void Signal_PWM_Set_BottomWidth(uint16_t width);			//Signal_PWM设置最小脉宽
void Signal_PWM_Set_TopLocation(int32_t location);		//Signal_PWM设置最大脉宽表示位置
void Signal_PWM_Set_BottomLocation(int32_t location);	//Signal_PWM设置最小脉宽表示位置
void Signal_PWM_Set_TopSpeed(int32_t speed);					//Signal_PWM设置最大脉宽表示速度
void Signal_PWM_Set_BottomSpeed(int32_t speed);				//Signal_PWM设置最小脉宽表示速度
void Signal_PWM_Set_TopCurrent(int32_t current);			//Signal_PWM设置最大脉宽表示电流
void Signal_PWM_Set_BottomCurrent(int32_t current);		//Signal_PWM设置最小脉宽表示电流
void Signal_PWM_Set_Default(void);										//Signal_PWM参数恢复

//初始化
void Signal_PWM_Init(void);							//Signal_PWM采集初始化

//功能接口
void Signal_PWM_Config(void);						//Signal_PWM采集配置
void Signal_PWM_DeConfig(void);					//Signal_PWM清理配置
void Signal_PWM_Capture_Goal(void);			//Signal_PWM获取目标
void Signal_PWM_TIM_Callback(void);			//Signal_PWM采集中断回调

/****************************************** MoreIO接口(即公共接口) ******************************************/
/****************************************** MoreIO接口(即公共接口) ******************************************/
/****************************************** MoreIO接口(即公共接口) ******************************************/
/**
  * MoreIO接口模式
***/
typedef enum{
	MoreIO_Mode_Disable = 0x00,		//Disable
	MoreIO_Mode_PWM_Location,			//PWM位置		(	EN(IO)			PWM(TIM)			NC(IO)		)
	MoreIO_Mode_PWM_Speed,				//PWM速度		(	EN(IO)			PWM(TIM)			DIR(IO)		)
	MoreIO_Mode_PWM_Current,			//PWM电流		(	EN(IO)			PWM(TIM)			DIR(IO)		)
	MoreIO_Mode_PULSE_Locatioon,	//PULSE位置	(	EN(IO)			PULSE(TIM)		DIR(EXTI)	)
}MoreIO_Mode;

/**
  * MoreIO接口结构体
**/
typedef struct{
	//模式
	MoreIO_Mode		mode;			//信号模式
	//解析运动数据
	int32_t		goal_location;	//目标位置
	int32_t		goal_speed;			//目标速度
	int16_t		goal_current;		//目标电流
	bool			goal_disable;		//失能指令
	bool			goal_brake;			//刹车指令
}Signal_MoreIO_Typedef;
extern Signal_MoreIO_Typedef signal_moreio;

//功能接口
void Signal_MoreIO_Init(void);									//MoreIO初始化

void Signal_MoreIO_Config(MoreIO_Mode data);		//MoreIO配置工作模式
void Signal_MoreIO_Capture_Goal(void);					//MoreIO获取目标


#ifdef __cplusplus
}
#endif

#endif
