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
  ** @file     : mt6816.c/h
  ** @brief    : MT6816传感器硬件驱动
  ** @versions : 1.1.1
  ** @time     : 2020/08/01
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef MT6816_H
#define MT6816_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

//MT6816工作模式定义
#define MT6816_Mode_PWM		(0x01)	//MT6816工作在PWM模式		(支持该模式的硬件版本：XDrive_REIN_Basic_H1_0)
#define MT6816_Mode_ABZ		(0x02)	//MT6816工作在ABZ模式		(支持该模式的硬件版本：XDrive_REIN_Basic_H1_0)
#define MT6816_Mode_SPI		(0x03)	//MT6816工作在SPI模式		(支持该模式的硬件版本：XDrive_REIN_Basic_H1_0 / XDrive_REIN_Basic_H1_1)

//MT6816工作模式配置
#define MT6816_Mode		MT6816_Mode_SPI

#if (MT6816_Mode == MT6816_Mode_PWM)
/********** MT6816_PWM **********/
/********** MT6816_PWM **********/
/********** MT6816_PWM **********/
typedef struct{
	//采集数据(PWM)
	uint16_t	h_width;				//PWM高宽度
	uint16_t	period;					//PWM周期
	uint8_t		count_rising;		//PWM上升沿计数器镜像
	uint8_t		count_falling;	//PWM下降沿计数器镜像
	uint8_t		count_update;		//PWM更新计数器
	bool		whole_h_flag;			//PWM全高标志
	bool		whole_l_flag;			//PWM全低标志
	bool		ready_once;				//PWM就绪标志(完成一次PWM采集后置位)
	bool		ready_again;			//PWM就绪标志(完成二次PWM采集后置位)
	uint16_t	valid_width;		//PWM有效宽度
	//输出数据(PWM)
	uint16_t	angle;						
}MT6816_PWM_Signal_Typedef;

//MT6816_PWM
void REIN_MT6816_PWM_Signal_Init(void);		//MT6816_PWM采集初始化
void REIN_MT6816_PWM_TIM_Callback(void);	//MT6816_PWM采集中断回调
void REIN_MT6816_PWM_Get_AngleData(void);	//MT6816_PWM获取角度数据

#elif (MT6816_Mode == MT6816_Mode_ABZ)
/********** MT6816_ABZ **********/
/********** MT6816_ABZ **********/
/********** MT6816_ABZ **********/
typedef struct{
	//采集数据(ABZ)
	uint16_t	sample_data;	//ABZ读取到的数据
	//输出数据(ABZ)
	uint16_t	angle;				//ABZ输出的角度
}MT6816_ABZ_Signal_Typedef;

//MT6816_ABZ
void REIN_MT6816_ABZ_Signal_Init(void);			//MT6816_ABZ采集初始化
void REIN_MT6816_ABZ_ZPulse_Callback(void);	//MT6816_ABZ采集Z脉冲中断回调
void REIN_MT6816_ABZ_Get_AngleData(void);		//MT6816_ABZ获取角度数据

#elif (MT6816_Mode == MT6816_Mode_SPI)
/********** MT6816_SPI **********/
/********** MT6816_SPI **********/
/********** MT6816_SPI **********/
typedef struct{
	//采集数据
	uint16_t	sample_data;	//SPI读取到的数据
	//输出数据
	uint16_t	angle;				//SPI输出的角度
	bool			no_mag_flag;	//磁铁数据有效标志
	bool			pc_flag;			//奇偶校验位
}MT6816_SPI_Signal_Typedef;

//MT6816_SPI
void REIN_MT6816_SPI_Signal_Init(void);		//MT6816_SPI采集初始化
void RINE_MT6816_SPI_Get_AngleData(void);	//MT6816_SPI采集获取角度数据

#endif

/********** MT6816 **********/
/********** MT6816 **********/
/********** MT6816 **********/
typedef struct{
	uint16_t	angle_data;				//角度数据
	uint16_t	rectify_angle;		//校准的角度数据
	bool			rectify_valid;		//校准数据有效标志
}MT6816_Typedef;
extern MT6816_Typedef	mt6816;

//MT6816
void REIN_MT6816_Init(void);							//MT6816初始化
void REIN_MT6816_Get_AngleData(void);			//MT6816获取角度数据

#ifdef __cplusplus
}
#endif
	 
#endif
