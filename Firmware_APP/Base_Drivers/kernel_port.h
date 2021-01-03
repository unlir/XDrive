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
  ** @file     : kernel_port.c/h
  ** @brief    : 端口定义
  ** @versions : newest
  ** @time     : newest
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef KERNEL_PORT_H
#define KERNEL_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

//C基本库
#include <stdint.h>		//基本数据类型
#include <stdbool.h>	//Bool数据类型
#include <string.h>		//变量类型,宏,字符数组函数
#include <stdlib.h>		//变量类型,宏,通用工具函数
#include <stdio.h>		//变量类型,宏,输入输出函数

//HAL库引用
#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "tim.h"

//固件版本
#define XDrive_Firmware		"F1.1.5 S"

//硬件列表
#define XDrive_REIN_Basic_H1_0		(0x00000100)	//(REIN - Basic - 01 - 00)
#define XDrive_REIN_Basic_H1_1		(0x00000101)	//(REIN - Basic - 01 - 01)

//选中的硬件(在支持列表中选择型号)
#define XDrive_Run		XDrive_REIN_Basic_H1_1

//引用板级宏
#if   (XDrive_Run == XDrive_REIN_Basic_H1_0)
	#include "XDrive_REIN_Basic_H1_0.h"
#elif (XDrive_Run == XDrive_REIN_Basic_H1_1)
	#include "XDrive_REIN_Basic_H1_1.h"
#else
	#error "Undefined XDrive_Run !!!"
#endif

#ifdef __cplusplus
}
#endif

#endif
