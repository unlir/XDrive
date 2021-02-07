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
  ** @file     : xdrive_ui.c/h
  ** @brief    : XDrive UI界面
  ** @versions : 1.1.1
  ** @time     : 2020/11/11
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kernel_port.h"

/******************** 通用数据 ********************/
/******************** 通用数据 ********************/
/******************** 通用数据 ********************/
/**
  * UI数据包
**/
typedef struct{
	uint32_t	period_ms;	//周期
	uint32_t	flicker_timer_100ms;		//闪烁计时器
	uint32_t	flicker_timer_500ms;		//闪烁计时器
	bool			flicker_flag_100ms;			//闪烁标志
	bool			flicker_flag_500ms;			//闪烁标志
}REINui_Data_Typedef;

/******************** 控件 ********************/
/******************** 控件 ********************/
/******************** 控件 ********************/
/**
  * 基本控件
**/
typedef struct{
	//用户辨识
	uint8_t		*name;				//控件名称
	uint8_t		*icon_3232;		//图标(32*32)
	//拓扑
	void			*source_widget;		//源控件指针(指向来源控件) 
	void			*linked_widget;		//链表指针(指向同级别下一个控件)
	//绘制
	//动态刷新
	bool			update_drawing;											//绘制刷新
	void			(*drawing_function)(void* widget);	//绘制函数
	//高级功能包(由于C无法实现继承,故使用该方法实现在统一引用下定义不同数据)
	void			*feature_pack;		//高级功能包
}REINui_Widget_Typedef;

/**
  * 高级包(过度动画)
**/
typedef struct{
	REINui_Widget_Typedef		*begin_widget;		//起始控件
	REINui_Widget_Typedef		*end_widget;			//结束控件
	uint32_t	duration_ms;			//过度时间
}REINui_Switches_Typedef;

/**
  * 高级包(目录)
**/
typedef struct{
	//项目指示
	void			*target_widget;		//目标控件指针(指向列表的第一个单元)
	uint32_t	list_size;				//目录列表大小
	uint32_t	pitch_number;			//选中的项目编号(0开始)
	//过程变量(无需全局)
	REINui_Widget_Typedef		*up_widget;				//上项目
	REINui_Widget_Typedef		*middle_widget;		//中项目
	REINui_Widget_Typedef		*down_widget;			//下项目
}REINui_Directory_Typedef;

//接口函数
void XDrive_REINui_Init(void);									//XDrive_REINui初始化
void XDrive_REINui_Callback_ms(uint32_t _time);	//XDrive_REINui任务回调
void XDrive_REINui_ToCalibration(void);					//XDrive_REINui进入校准界面

#ifdef __cplusplus
}
#endif

#endif
