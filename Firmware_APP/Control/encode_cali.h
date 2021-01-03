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
  ** @file     : encode_cali.c/h
  ** @brief    : 编码器校准
  ** @versions : 1.1.4
  ** @time     : 2019/11/28
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef ENCODE_CALI_H
#define ENCODE_CALI_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

//引用电机定义
#include "control_config.h"

//校准数据检查报告
typedef enum{
	//无错误
	CALI_No_Error = 0x00,						//数据无错误
	//原始数据出错
	CALI_Error_Average_Dir,					//平均值方向错误
	CALI_Error_Average_Continuity,	//平均值连续性错误
	CALI_Error_PhaseStep,						//阶跃次数错误
	//解析数据出错
	CALI_Error_Analysis_Quantity,		//解析数据量错误
}CALI_Error;

//校准状态
typedef enum{
	CALI_Disable = 0x00,						//失能
	CALI_Forward_Encoder_AutoCali,	//编码器正转自动校准
	CALI_Forward_Measure,						//正向测量
	CALI_Reverse_Ret,								//反向回退
	CALI_Reverse_Gap,								//反向消差
	CALI_Reverse_Measure,						//反向测量
	CALI_Operation,									//解算
}CALI_State;

/**
  * Control校准结构体定义
**/
typedef struct{
	//信号
	bool				trigger;			//触发校准
	CALI_Error	error_code;		//校准错误代码
	uint32_t		error_data;		//校准错误数据
	//读取过程
	CALI_State	state;					//校准状态
	uint32_t		out_location;		//输出位置
	#define			Gather_Quantity	16	//每个采集点采集数量
	uint16_t		gather_count;												//采集计数
	uint16_t		coder_data_gather[Gather_Quantity];	//采集点每次数据
	uint16_t 		coder_data_f[Move_Step_NUM+1];	//校准读取的数据
	uint16_t 		coder_data_r[Move_Step_NUM+1];	//校准读取的数据
	bool				dir;		//校准数据方向
	//全段域校准过程数据
	int32_t		rcd_x, rcd_y;			//寻找区间下标及阶跃差值
	uint32_t	result_num;				//统计数量
}Encode_Cali_Typedef;
extern Encode_Cali_Typedef encode_cali;

void Calibration_Init(void);								//校准器初始化
void Calibration_Interrupt_Callback(void);	//校准器中断回调(稳定中断调用)
void Calibration_Loop_Callback(void);				//校准器主程序回调(非中断调用)

#ifdef __cplusplus
}
#endif

#endif
