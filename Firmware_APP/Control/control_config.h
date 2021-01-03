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
  ** @brief    : 控制配置
  ** @versions : 2.2.3
  ** @time     : 2020/09/15
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/********************  硬件配置区  ********************/
#define Current_Rated_Current		(3000)		//额定电流(mA)
#define Current_Cali_Current		(2000)		//校准电流(mA)

/********************  运动参数配置区  ********************/
#define Move_Step_NUM				((int32_t)(200))																//(使用的电机单圈步数)(每步磁场旋转90°)
#define Move_Divide_NUM			((int32_t)(256))																//(每步柔性件控制细分量)
#define Move_Pulse_NUM			((int32_t)(Move_Step_NUM * Move_Divide_NUM))		//(电机单圈脉冲数)
#define Move_Rated_Speed			((int32_t)(50 * Move_Pulse_NUM))							//(额定转速)(50转每秒)
#define Move_Rated_UpAcc				((int32_t)(1000 * Move_Pulse_NUM))					//(固件额定加速加速度)(1000r/ss)
#define Move_Rated_DownAcc			((int32_t)(1000 * Move_Pulse_NUM))					//(固件额定减速加速度)(1000r/ss)
#define Move_Rated_UpCurrentRate		((int32_t)(20 * Current_Rated_Current))	//(固件额定增流梯度)(20倍额定/s)
#define Move_Rated_DownCurrentRate	((int32_t)(20 * Current_Rated_Current))	//(固件额定减流梯度)(20倍额定/s)

/****************************************  校准器配置区  ****************************************/
#define CALI_Encode_Bit					((int32_t)(14))																//(编码器位宽)(14位输出精度)
#define CALI_Encode_Res					((int32_t)((0x00000001U) << CALI_Encode_Bit))	//(编码器分辨率)(2^14 = 16384)(16k分辨率)(占用32k校准空间)
#define CALI_Gather_Encode_Res	((int32_t)(CALI_Encode_Res / Move_Step_NUM))	//(校准每采集步编码器分辨率)

/****************************************  控制器频率配置区  ****************************************/
#define CONTROL_FREQ_HZ			(20000)												//控制频率_hz
#define CONTROL_PERIOD_US		(1000000 / CONTROL_FREQ_HZ)		//控制周期_us

//void Control_Config_Init_Static(void);	//控制静态配置
//void Control_Config_Init_Dynamic(void);	//控制动态配置

#ifdef __cplusplus
}
#endif

#endif
