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

//Oneself
#include "xdrive_ui.h"

//Base_Drivers
#include "mt6816.h"
#include "signal_port.h"

//Control
#include "motor_control.h"
#include "encode_cali.h"
#include "power_detect.h"

//Debug
#include "button.h"
#include "ssd1306.h"
#include "ui_picture.h"

/******************** 通用数据 ********************/
/******************** 通用数据 ********************/
/******************** 通用数据 ********************/
REINui_Data_Typedef		ui_data;

/******************** 控件 ********************/
/******************** 控件 ********************/
/******************** 控件 ********************/
//控件(过渡动画)
REINui_Widget_Typedef			ui_switches;				//过度动画
//控件(过渡高级包)
REINui_Switches_Typedef		ui_switches_pack;
//控件(目录)
REINui_Widget_Typedef			ui_main_catalog;		//主目录
REINui_Widget_Typedef			ui_sensor;					//传感器
REINui_Widget_Typedef			ui_variable;				//被控对象
REINui_Widget_Typedef			ui_parameters;			//参数
//控件(目录高级包)
REINui_Directory_Typedef	ui_main_pack;
REINui_Directory_Typedef	ui_sensor_pack;
REINui_Directory_Typedef	ui_variable_pack;
REINui_Directory_Typedef	ui_parameters_pack;
//控件(显示块)
REINui_Widget_Typedef			ui_desktop;					//桌面
REINui_Widget_Typedef			ui_power;						//电源数据
REINui_Widget_Typedef			ui_encoder;					//编码器数据
REINui_Widget_Typedef			ui_location;				//位置数据
REINui_Widget_Typedef			ui_speed;						//速度数据
REINui_Widget_Typedef			ui_current;					//电流数据
REINui_Widget_Typedef			ui_control_dce;			//位置控制参数
REINui_Widget_Typedef			ui_control_pid;			//速度控制参数
REINui_Widget_Typedef			ui_signal_count;		//Signal_Count接口参数
REINui_Widget_Typedef			ui_signal_pwm;			//Signal_PWM接口参数
REINui_Widget_Typedef			ui_calibration;			//校准
REINui_Widget_Typedef			ui_information;			//信息显示
//拓扑指针
REINui_Widget_Typedef			*ui_activated = NULL;	//激活的控件指针
REINui_Widget_Typedef			*ui_pitch = NULL;			//选中的控件指针

/******************** 执行函数 ********************/
/******************** 执行函数 ********************/
/******************** 执行函数 ********************/
/**
  * @brief  初始化控件
  * @param  multi
  * @retval NULL
**/
void REINui_Widget_Init(REINui_Widget_Typedef *_widget, uint8_t *_name, uint8_t *_icon_3232, void (*_drawing)(void*), void *_pack)
{
	//用户辨识
	if(strlen((char*)_name) <= 15)
		_widget->name = _name;
	else
		_widget->name = (uint8_t*)"Very long name.";
	_widget->icon_3232 = _icon_3232;
	//拓扑
	_widget->source_widget	= NULL;
	_widget->linked_widget	= NULL;
	//绘制
	_widget->update_drawing = true;
	_widget->drawing_function = _drawing;
	//高级功能包
	_widget->feature_pack = _pack;
}

/**
  * @brief  初始化高级包(过度动画)
  * @param  multi
  * @retval NULL
**/
void REINui_Switches_Init(REINui_Switches_Typedef *_switches)
{
	_switches->begin_widget = NULL;
	_switches->end_widget = NULL;
	_switches->duration_ms = 200;
}

/**
  * @brief  初始化高级包(目录)
  * @param  multi
  * @retval NULL
**/
void REINui_Directory_Init(REINui_Directory_Typedef *_directory)
{
	//项目指示
	_directory->target_widget = NULL;
	_directory->list_size = 0;
	_directory->pitch_number = 0;
}

/**
  * @brief  为目录添加子控件
  * @param  multi
  * @retval NULL
**/
void REINui_Directory_Append_Widget(REINui_Widget_Typedef* _host, REINui_Widget_Typedef* _slave)
{
	if(_host->feature_pack == NULL)
		return;
	
	REINui_Directory_Typedef	*directory = (REINui_Directory_Typedef*)_host->feature_pack;
	
	//向主机添加目标控件
	if(directory->target_widget != NULL)
	{
		//寻找主机的下级链接中(同级链接为NULL的单元)
		REINui_Widget_Typedef *seek_pointer = (REINui_Widget_Typedef*)directory->target_widget;
		for(;;)
		{
			if(seek_pointer->linked_widget == NULL)
			{
				seek_pointer->linked_widget = (void*)_slave;
				break;
			}
			seek_pointer = (REINui_Widget_Typedef*)(seek_pointer->linked_widget);
		}
	}
	else
	{
		//直接将从机放在目标指针下
		directory->target_widget = (void*)_slave;
	}
	
	//向从机添加源控件
	_slave->source_widget = (void*)_host;
}

/**
  * @brief  切换动画绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Switches_drawing_function(void* _widget)
{
	REINui_Widget_Typedef			*widget   = (REINui_Widget_Typedef*)_widget;
	REINui_Switches_Typedef		*switches = (REINui_Switches_Typedef*)widget->feature_pack;
	//先不做动画,直接跳转到目标界面
	ui_activated = switches->end_widget;
}

/**
  * @brief  目录绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Directory_drawing_function(void* _widget)
{
	REINui_Widget_Typedef			*widget    = (REINui_Widget_Typedef*)_widget;										//主控件
	REINui_Directory_Typedef	*directory = (REINui_Directory_Typedef*)widget->feature_pack;		//主高级包
	REINui_Widget_Typedef			*seek_pointer;	//目标控件指针

	//响应刷新指令
	if(widget->update_drawing)
	{
		widget->update_drawing = false;
		
		//遍历列表获取列表大小
		directory->list_size = 0;
		seek_pointer = (REINui_Widget_Typedef*)directory->target_widget;	//获取第一个目标
		if(seek_pointer != NULL)
		{
			directory->list_size ++;
			//寻找主机的下级链接中(同级链接为NULL的单元)
			for(;;)
			{
				if(seek_pointer->linked_widget == NULL)
					break;
				seek_pointer = (REINui_Widget_Typedef*)(seek_pointer->linked_widget);
				directory->list_size ++;
			}
		}

		//获取目录表显示项目
		directory->up_widget = NULL;
		directory->middle_widget = NULL;
		directory->down_widget = NULL;
		seek_pointer = (REINui_Widget_Typedef*)directory->target_widget;	//获取第一个目标
		for(uint32_t i=0; i<directory->list_size; i++)
		{
			//寻找上项目
			if(i == ((directory->pitch_number + directory->list_size - 1) % directory->list_size))
				directory->up_widget = seek_pointer;
			//寻找中项目
			if(i == (directory->pitch_number))
				directory->middle_widget = seek_pointer;
			//寻找中项目
			if(i == ((directory->pitch_number + 1) % directory->list_size))
				directory->down_widget = seek_pointer;

			//读取链表下一个控件
			if(seek_pointer->linked_widget == NULL)
				break;
			seek_pointer = (REINui_Widget_Typedef*)(seek_pointer->linked_widget);
		}
		
		//绘制开始
		GRAM_Clear(ssd1306_gram);
		//绘制外框
		GRAM_DrawLine(ssd1306_gram,   0,  0,  95,  0, true);
		GRAM_DrawLine(ssd1306_gram,   0, 31,  95, 31, true);
		GRAM_DrawLine(ssd1306_gram,   0,  0,   0, 31, true);
		GRAM_DrawLine(ssd1306_gram,   3,  0,   3, 31, true);
		
		//绘制位置指示
		uint8_t slid_length;		//滑块长度
		if(directory->list_size >= 2){
			if(directory->list_size >= 30)	slid_length = 1;
			else														slid_length = 30 / (directory->list_size);
			uint8_t slid_excursion;	//滑块偏移
			slid_excursion = (30 - slid_length) * directory->pitch_number / (directory->list_size - 1);
			GRAM_DrawLine(ssd1306_gram, 1, (slid_excursion + 1), 1, (slid_excursion + slid_length), true);
			GRAM_DrawLine(ssd1306_gram, 2, (slid_excursion + 1), 2, (slid_excursion + slid_length), true);
		}

		//绘制数据
		if(directory->up_widget != NULL)
			GRAM_ShowString(ssd1306_gram, 5, 1, directory->up_widget->name, CharFont_0806, true);
		if(directory->middle_widget != NULL)
			GRAM_ShowString(ssd1306_gram, 5, 10, directory->middle_widget->name, CharFont_1206, false);	//反色突出
		if(directory->down_widget != NULL)
			GRAM_ShowString(ssd1306_gram, 5, 23, directory->down_widget->name, CharFont_0806, true);

		//绘制选中图标
		if(directory->middle_widget->icon_3232 != NULL)
			GRAM_ShowPicture(ssd1306_gram, 96, 0, 32, 32, directory->middle_widget->icon_3232, true);
		else
			GRAM_ShowPicture(ssd1306_gram, 96, 0, 32, 32, (uint8_t*)Default_ICON_3232, true);

		//刷新SSD1306
		SSD1306_Refresh(ssd1306_gram);
	}
	
	//用户输入响应
	if(Button_Inquice_State(Button_UP) == Button_Bit_Rise){
		widget->update_drawing = true;
		if(directory->pitch_number)		directory->pitch_number --;
		else													directory->pitch_number = directory->list_size - 1;
	}
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_Rise){
		widget->update_drawing = true;
		if((directory->pitch_number + 1) < directory->list_size)	directory->pitch_number ++;
		else																											directory->pitch_number = 0;
	}
	if(Button_Inquice_State(Button_UP) == Button_Bit_LongDrop){
		if(directory->middle_widget != NULL)
		{
			ui_pitch = directory->middle_widget;	//进入选中项目
			ui_pitch->update_drawing = true;
		}
	}
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  桌面绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Desktop_drawing_function(void* _widget)
{
	//静态参数
	static Motor_Mode	ui_motor_mode = De_Motor_Mode;	//UI模式
	
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件
	
	//桌面全局刷新
	widget->update_drawing = false;
	
	//绘制开始
	GRAM_Clear(ssd1306_gram);
	
	//绘制模式行
	bool	match;
	GRAM_ShowPicture(ssd1306_gram,   1, 0, 8, 8, (uint8_t*)Left_Arrow_88, false);
	GRAM_ShowPicture(ssd1306_gram, 112, 0, 8, 8, (uint8_t*)Forward_Arrow_88, false);
	if(ui_motor_mode == motor_control.mode_order){
		match = true;
		GRAM_ShowPicture(ssd1306_gram, 120, 0, 8, 8, (uint8_t*)Yes_88, !match);
	}
	else{
		match = ui_data.flicker_flag_500ms;	//闪烁
		GRAM_ShowPicture(ssd1306_gram, 120, 0, 8, 8, (uint8_t*)No_88, match);
	}
	switch(ui_motor_mode){
		//测试
		case Motor_Mode_Debug_Location:		GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"Debug Location   ", CharFont_0806, match);	break;
		case Motor_Mode_Debug_Speed:			GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"Debug Speed      ", CharFont_0806, match);	break;
		//停止
		case Control_Mode_Stop:						GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"Stop             ", CharFont_0806, match);	break;
		//DIG(CAN/RS485)
		case Motor_Mode_Digital_Location:	GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"Digital Location ", CharFont_0806, match);	break;
		case Motor_Mode_Digital_Speed:		GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"Digital Speed    ", CharFont_0806, match);	break;
		case Motor_Mode_Digital_Current:	GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"Digital Current  ", CharFont_0806, match);	break;
		case Motor_Mode_Digital_Track:		GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"Digital Track    ", CharFont_0806, match);	break;
		//MoreIO(PWM/PUL)
		case Motor_Mode_PWM_Location:			GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"PWM Location     ", CharFont_0806, match);	break;
		case Motor_Mode_PWM_Speed:				GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"PWM Speed        ", CharFont_0806, match);	break;
		case Motor_Mode_PWM_Current:			GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"PWM Current      ", CharFont_0806, match);	break;
		case Motor_Mode_PULSE_Location:		GRAM_ShowString(ssd1306_gram, 9, 0, (uint8_t*)"PULSE Location   ", CharFont_0806, match);	break;
		//其他非法模式
		default:	break;
	}
	
	//绘制关键状态行
	if(!motor_control.soft_disable){	//启用使能
		GRAM_ShowString(	ssd1306_gram,   0, 24, (uint8_t*)"enabl", CharFont_0806, true);
		GRAM_ShowPicture(	ssd1306_gram,  30, 24, 8, 8, (uint8_t*)Yes_88, false);
	}
	else{
		GRAM_ShowString(	ssd1306_gram,   0, 24, (uint8_t*)"enabl", CharFont_0806, ui_data.flicker_flag_500ms);	//闪烁
		GRAM_ShowPicture(	ssd1306_gram,  30, 24, 8, 8, (uint8_t*)No_88, ui_data.flicker_flag_500ms);
	}
	if(!motor_control.stall_flag){		//未堵转
		GRAM_ShowString(	ssd1306_gram,  45, 24, (uint8_t*)"unsta", CharFont_0806, true);
		GRAM_ShowPicture(	ssd1306_gram,  75, 24, 8, 8, (uint8_t*)Yes_88, false);
	}
	else{
		GRAM_ShowString(	ssd1306_gram,  45, 24, (uint8_t*)"unsta", CharFont_0806, ui_data.flicker_flag_500ms);	//闪烁
		GRAM_ShowPicture(	ssd1306_gram,  75, 24, 8, 8, (uint8_t*)No_88, ui_data.flicker_flag_500ms);
	}
	if(mt6816.rectify_valid){					//校准有效
		GRAM_ShowString(	ssd1306_gram,  90, 24, (uint8_t*)"calib", CharFont_0806, true);
		GRAM_ShowPicture(	ssd1306_gram, 120, 24, 8, 8, (uint8_t*)Yes_88, false);
	}
	else{
		GRAM_ShowString(	ssd1306_gram,  90, 24, (uint8_t*)"calib", CharFont_0806, ui_data.flicker_flag_500ms);	//闪烁
		GRAM_ShowPicture(	ssd1306_gram, 120, 24, 8, 8, (uint8_t*)No_88, ui_data.flicker_flag_500ms);
	}
	
	//绘制不同模式下的特色指示
	switch(motor_control.mode_run){
		//测试
		case Motor_Mode_Debug_Location:		break;
		case Motor_Mode_Debug_Speed:			break;
		//停止
		case Control_Mode_Stop:						GRAM_ShowString(				ssd1306_gram,  0,  8, (uint8_t*)"      STOP     ",              CharFont_1608, true);	break;
		//DIG(CAN/RS485)
		case Motor_Mode_Digital_Location:	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"goal_loca:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.goal_location, 11, CharFont_0806, true);
																			GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"mens_loca:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)motor_control.est_location,  11, CharFont_0806, true);	break;
		case Motor_Mode_Digital_Speed:		GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"goal_spee:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.goal_speed,    11, CharFont_0806, true);
																			GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"mens_spee:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)motor_control.est_speed,     11, CharFont_0806, true);	break;
		case Motor_Mode_Digital_Current:	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"goal_curr:",                   CharFont_0806, true);	
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.goal_current,  11, CharFont_0806, true);	
																			GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"out_curr :",                   CharFont_0806, true);	
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)motor_control.foc_current ,  11, CharFont_0806, true);	break;
		case Motor_Mode_Digital_Track:		GRAM_ShowString(				ssd1306_gram,  0,  8, (uint8_t*)"      Track    ",              CharFont_1608, true);	break;
		//MoreIO(PWM/PUL)
		case Motor_Mode_PWM_Location:			GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"goal_loca:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.goal_location, 11, CharFont_0806, true);
																			GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"mens_loca:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)motor_control.est_location,  11, CharFont_0806, true);	break;
		case Motor_Mode_PWM_Speed:				GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"goal_spee:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.goal_speed,    11, CharFont_0806, true);
																			GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"mens_spee:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)motor_control.est_speed,     11, CharFont_0806, true);	break;
		case Motor_Mode_PWM_Current:			GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"goal_curr:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.goal_current,  11, CharFont_0806, true);
																			GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"out_curr :",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)motor_control.foc_current ,  11, CharFont_0806, true);	break;
		case Motor_Mode_PULSE_Location:		GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"goal_loca:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.goal_location, 11, CharFont_0806, true);
																			GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"mens_loca:",                   CharFont_0806, true);
																			GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)motor_control.est_location,  11, CharFont_0806, true);	break;
		//其他非法模式
		default:	break;
	}
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_UP) == Button_Bit_Rise){
		switch(ui_motor_mode){
			//测试
			case Motor_Mode_Debug_Location:		ui_motor_mode = Motor_Mode_Debug_Location;		break;
			case Motor_Mode_Debug_Speed:			ui_motor_mode = Motor_Mode_Debug_Location;		break;
			//停止
			case Control_Mode_Stop:						ui_motor_mode = Motor_Mode_Debug_Speed;				break;
			//DIG(CAN/RS485)
			case Motor_Mode_Digital_Location:	ui_motor_mode = Control_Mode_Stop;						break;
			case Motor_Mode_Digital_Speed:		ui_motor_mode = Motor_Mode_Digital_Location;	break;
			case Motor_Mode_Digital_Current:	ui_motor_mode = Motor_Mode_Digital_Speed;			break;
			case Motor_Mode_Digital_Track:		ui_motor_mode = Motor_Mode_Digital_Current;		break;
			//MoreIO(PWM/PUL)
			case Motor_Mode_PWM_Location:			ui_motor_mode = Motor_Mode_Digital_Track;			break;
			case Motor_Mode_PWM_Speed:				ui_motor_mode = Motor_Mode_PWM_Location;			break;
			case Motor_Mode_PWM_Current:			ui_motor_mode = Motor_Mode_PWM_Speed;					break;
			case Motor_Mode_PULSE_Location:		ui_motor_mode = Motor_Mode_PWM_Current;				break;
			//其他非法模式
			default:													ui_motor_mode = Control_Mode_Stop;						break;
		}
	}
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_Rise){
		switch(ui_motor_mode){
			//测试
			case Motor_Mode_Debug_Location:		ui_motor_mode = Motor_Mode_Debug_Speed;				break;
			case Motor_Mode_Debug_Speed:			ui_motor_mode = Control_Mode_Stop;						break;
			//停止
			case Control_Mode_Stop:						ui_motor_mode = Motor_Mode_Digital_Location;	break;
			//DIG(CAN/RS485)
			case Motor_Mode_Digital_Location:	ui_motor_mode = Motor_Mode_Digital_Speed;			break;
			case Motor_Mode_Digital_Speed:		ui_motor_mode = Motor_Mode_Digital_Current;		break;
			case Motor_Mode_Digital_Current:	ui_motor_mode = Motor_Mode_Digital_Track;			break;
			case Motor_Mode_Digital_Track:		ui_motor_mode = Motor_Mode_PWM_Location;			break;
			//MoreIO(PWM/PUL)
			case Motor_Mode_PWM_Location:			ui_motor_mode = Motor_Mode_PWM_Speed;					break;
			case Motor_Mode_PWM_Speed:				ui_motor_mode = Motor_Mode_PWM_Current;				break;
			case Motor_Mode_PWM_Current:			ui_motor_mode = Motor_Mode_PULSE_Location;		break;
			case Motor_Mode_PULSE_Location:		ui_motor_mode = Motor_Mode_PULSE_Location;		break;
			//其他非法模式
			default:													ui_motor_mode = Control_Mode_Stop;						break;
		}
	}
	if(Button_Inquice_State(Button_UP) == Button_Bit_LongDrop){
		motor_control.mode_order = ui_motor_mode;		//将界面模式下载到控制指令
	}
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  电源绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Power_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	GRAM_ShowString(				ssd1306_gram,  1,  0, (uint8_t*)"u adc    :",         CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  0, (int32_t)power.adc_voltage, 11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"u est(mV):",         CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)power.est_voltage, 11, CharFont_0806, true);
//	GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"i adc    :",         CharFont_0806, true);
//	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)power.adc_current, 11, CharFont_0806, true);
//	GRAM_ShowString(				ssd1306_gram,  1, 24, (uint8_t*)"i est(mA):",         CharFont_0806, true);
//	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 24, (int32_t)power.est_current, 11, CharFont_0806, true);
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  编码器绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Encoder_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件
	
	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	GRAM_ShowString(				ssd1306_gram,  1,  0, (uint8_t*)"primeval :",            CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  0, (int32_t)mt6816.angle_data,    11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"rectify  :",            CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)mt6816.rectify_angle, 11, CharFont_0806, true);
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);

	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  位置绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Location_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	GRAM_ShowString(				ssd1306_gram,  1,  0, (uint8_t*)"goal_loca:",                   CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  0, (int32_t)motor_control.goal_location, 11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"mens_loca:",                   CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.est_location,  11, CharFont_0806, true);
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  速度绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Speed_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	GRAM_ShowString(				ssd1306_gram,  1,  0, (uint8_t*)"goal_spee:",                  CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  0, (int32_t)motor_control.goal_current, 11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"mens_spee:",                  CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.est_speed,    11, CharFont_0806, true);
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  电流绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Current_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	GRAM_ShowString(				ssd1306_gram,  1,  0, (uint8_t*)"goal_curr:",                 CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  0, (int32_t)motor_control.goal_speed, 11,  CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"out_curr :",                 CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)motor_control.foc_current, 11, CharFont_0806, true);
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  DCE参数绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_ControlDCE_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	GRAM_ShowString(				ssd1306_gram,  1,  0, (uint8_t*)"dce_kp   :",     CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  0, (int32_t)dce.kp,        11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"dce_ki   :",     CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)dce.ki,        11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"dce_kv   :",     CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)dce.kv,        11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1, 24, (uint8_t*)"dce_kd   :",     CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 24, (int32_t)dce.kd,        11, CharFont_0806, true);
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  PID参数绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_ControlPID_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	GRAM_ShowString(				ssd1306_gram,  1,  0, (uint8_t*)"pid_kp   :",     CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  0, (int32_t)pid.kp,        11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"pid_ki   :",     CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)pid.ki,        11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"pid_kd   :",     CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)pid.kd,        11, CharFont_0806, true);
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  计数信号绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_SignalCount_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	GRAM_ShowString(				ssd1306_gram,   1,  0, (uint8_t*)"en_valid :",             CharFont_0806, true);
	if(sg_cut.en_valid)
		GRAM_ShowPicture(			ssd1306_gram, 119,  0, 8, 8, (uint8_t*)Yes_88, false);
	else
		GRAM_ShowPicture(			ssd1306_gram, 119,  0, 8, 8, (uint8_t*)No_88, true);
	GRAM_ShowString(				ssd1306_gram,   1,  8, (uint8_t*)"sam_count:",             CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram,  61,  8, (int32_t)sg_cut.sampling_count, 11, CharFont_0806, true);
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  PWM信号绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_SignalPWM_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	GRAM_ShowString(				ssd1306_gram,  1,  0, (uint8_t*)"h_width  :",          CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  0, (int32_t)sg_pwm.h_width,     11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1,  8, (uint8_t*)"period   :",          CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61,  8, (int32_t)sg_pwm.period,      11, CharFont_0806, true);
	GRAM_ShowString(				ssd1306_gram,  1, 16, (uint8_t*)"val_width:",          CharFont_0806, true);
	GRAM_ShowNum_LI_Max11(	ssd1306_gram, 61, 16, (int32_t)sg_pwm.valid_width, 11, CharFont_0806, true);
	if(sg_pwm.ready_third)
	{
		uint16_t duty_ratio = 128 * (uint32_t)sg_pwm.valid_width / (uint32_t)sg_pwm.period;
		if(duty_ratio >= 127)	duty_ratio = 127;
		GRAM_DrawLine(ssd1306_gram,          0, 25, duty_ratio, 25, true);
		GRAM_DrawLine(ssd1306_gram, duty_ratio, 25, duty_ratio, 30, true);
		GRAM_DrawLine(ssd1306_gram, duty_ratio, 30,        127, 30, true);
	}
	else
	{
		GRAM_DrawLine(ssd1306_gram, 0, 30, 127, 30, true);
	}	
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  校准绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Calibration_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);

	//绘制数据
	switch(encode_cali.state){
		case CALI_Disable:									GRAM_ShowString(	ssd1306_gram,  4, 0, (uint8_t*)"      Disable       ", CharFont_0806, true);	break;
		case CALI_Forward_Encoder_AutoCali:	GRAM_ShowString(	ssd1306_gram,  4, 0, (uint8_t*)"  Encoder_AutoCali  ", CharFont_0806, true);	break;
		case CALI_Forward_Measure:					GRAM_ShowString(	ssd1306_gram,  4, 0, (uint8_t*)"  Forward_Measure   ", CharFont_0806, true);	break;
		case CALI_Reverse_Ret:							GRAM_ShowString(	ssd1306_gram,  4, 0, (uint8_t*)"    Reverse_Ret     ", CharFont_0806, true);	break;
		case CALI_Reverse_Gap:							GRAM_ShowString(	ssd1306_gram,  4, 0, (uint8_t*)"    Reverse_Gap     ", CharFont_0806, true);	break;
		case CALI_Reverse_Measure:					GRAM_ShowString(	ssd1306_gram,  4, 0, (uint8_t*)"  Reverse_Measure   ", CharFont_0806, true);	break;
		case CALI_Operation:								GRAM_ShowString(	ssd1306_gram,  4, 0, (uint8_t*)"  Reverse_Measure   ", CharFont_0806, true);	break;
		default:														GRAM_ShowString(	ssd1306_gram,  4, 0, (uint8_t*)"      Invalid       ", CharFont_0806, true);	break;
	}
	switch(encode_cali.error_code){
		case CALI_No_Error:									GRAM_ShowString(	ssd1306_gram,  0, 8, (uint8_t*)"    No_Error    ", CharFont_1608, true);												break;
		case CALI_Error_Average_Dir:				GRAM_ShowString(	ssd1306_gram,  0, 8, (uint8_t*)" Error_Ave_Dir  ", CharFont_1608, ui_data.flicker_flag_500ms);	break;
		case CALI_Error_Average_Continuity:	GRAM_ShowString(	ssd1306_gram,  0, 8, (uint8_t*)"Error_Ave_Contin", CharFont_1608, ui_data.flicker_flag_500ms);	break;
		case CALI_Error_PhaseStep:					GRAM_ShowString(	ssd1306_gram,  0, 8, (uint8_t*)"Error_PhaseStep ", CharFont_1608, ui_data.flicker_flag_500ms);	break;
		case CALI_Error_Analysis_Quantity:	GRAM_ShowString(	ssd1306_gram,  0, 8, (uint8_t*)"Error_Analy_Quan", CharFont_1608, ui_data.flicker_flag_500ms);	break;
		default:														GRAM_ShowString(	ssd1306_gram,  0, 8, (uint8_t*)"     Invalid    ", CharFont_1608, ui_data.flicker_flag_500ms);	break;
	}
	if(mt6816.rectify_valid){					//校准有效
		GRAM_ShowString(	ssd1306_gram,  22, 24, (uint8_t*)"calibration:", CharFont_0806, true);
		GRAM_ShowPicture(	ssd1306_gram, 100, 24, 8, 8, (uint8_t*)Yes_88, false);
	}
	else{
		GRAM_ShowString(	ssd1306_gram,  22, 24, (uint8_t*)"calibration:", CharFont_0806, ui_data.flicker_flag_500ms);	//闪烁
		GRAM_ShowPicture(	ssd1306_gram, 100, 24, 8, 8, (uint8_t*)No_88, ui_data.flicker_flag_500ms);
	}
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_UP) == Button_Bit_LongDrop){
		encode_cali.trigger = true;		//触发校准
	}
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/**
  * @brief  信息绘画函数
  * @param  multi
  * @retval NULL
**/
void REINui_Information_drawing_function(void* _widget)
{
	REINui_Widget_Typedef		*widget = (REINui_Widget_Typedef*)_widget;		//主控件

	//绘制开始
	GRAM_Clear(ssd1306_gram);
	
	//绘制硬件版本
#if   (XDrive_Run == XDrive_REIN_Basic_H1_0)
	GRAM_ShowString(ssd1306_gram,  0,  0, (uint8_t*)"Hardware:Basic H1.0  ", CharFont_0806, true);
#elif (XDrive_Run == XDrive_REIN_Basic_H1_1)
	GRAM_ShowString(ssd1306_gram,  0,  0, (uint8_t*)"Hardware:Basic H1.1  ", CharFont_0806, true);
#else
	#error "Undefined XDrive_Run !!!"
#endif
	//绘制软件版本
	GRAM_ShowString(ssd1306_gram,  0,  8, (uint8_t*)"Software:",     CharFont_0806, true);
	GRAM_ShowString(ssd1306_gram, 54,  8, (uint8_t*)XDrive_Firmware, CharFont_0806, true);
	//绘制GitHub链接
	GRAM_ShowString(ssd1306_gram,  0, 16, (uint8_t*)"https://github.com   ", CharFont_0806, true);
	GRAM_ShowString(ssd1306_gram,  0, 24, (uint8_t*)"/unlir/XDrive        ", CharFont_0806, true);
	
	//刷新SSD1306
	SSD1306_Refresh(ssd1306_gram);
	
	//用户输入响应
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_LongDrop){
		if(widget->source_widget != NULL)
		{
			ui_pitch = widget->source_widget;		//退回上一个项目
			ui_pitch->update_drawing = true;
		}
	}
}

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/

/**
  * @brief  UI初始化
  * @param  NULL
  * @retval NULL
**/
void XDrive_REINui_Init(void)
{
	//控件(过度动画)
	REINui_Widget_Init(&ui_switches,			(uint8_t*)"Cartoon        ",	(uint8_t*)NULL,										REINui_Switches_drawing_function		, &ui_switches_pack		);
	REINui_Switches_Init(&ui_switches_pack);
	//控件(定向为菜单)
	REINui_Widget_Init(&ui_main_catalog,	(uint8_t*)"Main Catalog   ",	(uint8_t*)Main_Catalog_ICON_3232,	REINui_Directory_drawing_function		, &ui_main_pack				);
	REINui_Widget_Init(&ui_sensor,				(uint8_t*)"Sensor         ",	(uint8_t*)Sensor_ICON_3232,				REINui_Directory_drawing_function		, &ui_sensor_pack			);
	REINui_Widget_Init(&ui_variable,			(uint8_t*)"Variable       ",	(uint8_t*)Variable_ICON_3232,			REINui_Directory_drawing_function		, &ui_variable_pack		);
	REINui_Widget_Init(&ui_parameters,		(uint8_t*)"Parameters     ",	(uint8_t*)Parameters_ICON_3232,		REINui_Directory_drawing_function		, &ui_parameters_pack	);
	REINui_Directory_Init(&ui_main_pack);
	REINui_Directory_Init(&ui_sensor_pack);
	REINui_Directory_Init(&ui_variable_pack);
	REINui_Directory_Init(&ui_parameters_pack);
	//控件(定向为显示块)
	REINui_Widget_Init(&ui_desktop,				(uint8_t*)"Desktop        ",	(uint8_t*)Desktop_ICON_3232,			REINui_Desktop_drawing_function			, NULL								);
	REINui_Widget_Init(&ui_power,					(uint8_t*)"Power          ",	(uint8_t*)Power_ICON_3232,				REINui_Power_drawing_function				, NULL								);
	REINui_Widget_Init(&ui_encoder,				(uint8_t*)"Encoder        ",	(uint8_t*)Encoder_ICON_3232,			REINui_Encoder_drawing_function			, NULL								);
	REINui_Widget_Init(&ui_location,			(uint8_t*)"Location       ",	(uint8_t*)Location_ICON_3232,			REINui_Location_drawing_function		, NULL								);
	REINui_Widget_Init(&ui_speed,					(uint8_t*)"Speed          ",	(uint8_t*)Speed_ICON_3232,				REINui_Speed_drawing_function				, NULL								);
	REINui_Widget_Init(&ui_current,				(uint8_t*)"Current        ",	(uint8_t*)Current_ICON_3232,			REINui_Current_drawing_function			, NULL								);
	REINui_Widget_Init(&ui_control_dce,		(uint8_t*)"Control DCE    ",	(uint8_t*)NULL,										REINui_ControlDCE_drawing_function	, NULL								);
	REINui_Widget_Init(&ui_control_pid,		(uint8_t*)"Control PID    ",	(uint8_t*)NULL,										REINui_ControlPID_drawing_function	, NULL								);
	REINui_Widget_Init(&ui_signal_count,	(uint8_t*)"Signal Count   ",	(uint8_t*)NULL,										REINui_SignalCount_drawing_function	, NULL								);
	REINui_Widget_Init(&ui_signal_pwm,		(uint8_t*)"Signal PWM     ",	(uint8_t*)NULL,										REINui_SignalPWM_drawing_function		, NULL								);
	REINui_Widget_Init(&ui_calibration,		(uint8_t*)"Calibration    ",	(uint8_t*)Calibration_ICON_3232,	REINui_Calibration_drawing_function	, NULL								);
	REINui_Widget_Init(&ui_information,		(uint8_t*)"Information    ",	(uint8_t*)Information_ICON_3232,	REINui_Information_drawing_function ,	NULL								);

	//添加界面拓扑关系
	//--------------------------------------------------------------------------------------------------------------------------------
	//|| Main_Catalog ------------------------------ Desktop         || 主目录 ------------------------------------ 桌面            ||
	//||                   |                                         ||                   |                                         ||
	//||                   |-- Sensor -------------- Power           ||                   |-- 传感器 -------------- 电源            ||
	//||                   |                     |-- Encoder         ||                   |                     |-- 编码器          ||
	//||                   |                                         ||                   |                                         ||
	//||                   |-- Variable ------------ Location        ||                   |-- 控制变量 ------------ 位置数据        ||
	//||                   |                     |-- Speed           ||                   |                     |-- 速度数据        ||
	//||                   |                     |-- Current         ||                   |                     |-- 电流数据        ||
	//||                   |                                         ||                   |                                         ||
	//||                   |-- Parameters----------- Control_DCE     ||                   |-- 参数 ---------------- DCE参数         ||
	//||                   |                     |-- Control_PID     ||                   |                     |-- PID参数         ||
	//||                   |                     |-- Signal_Count    ||                   |                     |-- 计数信号接口    ||
	//||                   |                     |-- Signal_PWM      ||                   |                     |-- PWM信号接口     ||
	//||                   |                                         ||                   |                                         ||
	//||                   |------------------------ Calibration     ||                   |------------------------ 校准            ||
	//||                   |                                         ||                   |                                         ||
	//||                   |------------------------ Information     ||                   |------------------------ 信息            ||
	//--------------------------------------------------------------------------------------------------------------------------------
	
	REINui_Directory_Append_Widget(&ui_main_catalog,	&ui_desktop);
	
	REINui_Directory_Append_Widget(&ui_main_catalog,	&ui_sensor);
	REINui_Directory_Append_Widget(&ui_sensor,				&ui_power);
	REINui_Directory_Append_Widget(&ui_sensor,				&ui_encoder);
	
	REINui_Directory_Append_Widget(&ui_main_catalog,	&ui_variable);
	REINui_Directory_Append_Widget(&ui_variable,			&ui_location);
	REINui_Directory_Append_Widget(&ui_variable,			&ui_speed);
	REINui_Directory_Append_Widget(&ui_variable,			&ui_current);
	
	REINui_Directory_Append_Widget(&ui_main_catalog,	&ui_parameters);
	REINui_Directory_Append_Widget(&ui_parameters,		&ui_control_dce);
	REINui_Directory_Append_Widget(&ui_parameters,		&ui_control_pid);
	REINui_Directory_Append_Widget(&ui_parameters,		&ui_signal_count);
	REINui_Directory_Append_Widget(&ui_parameters,		&ui_signal_pwm);
	
	REINui_Directory_Append_Widget(&ui_main_catalog,	&ui_calibration);
	
	REINui_Directory_Append_Widget(&ui_main_catalog,	&ui_information);

	//(默认选择"桌面")
	ui_pitch = &ui_desktop;
	
	//显示XDrive
	GRAM_Clear(ssd1306_gram);
	GRAM_ShowPicture(ssd1306_gram, 0, 0, 128, 32, (uint8_t*)XDrive_ICON_12832, true);
	SSD1306_Refresh(ssd1306_gram);
}

/**
  * @brief  UI任务回调
  * @param  NULL
  * @retval NULL
**/
void XDrive_REINui_Callback_ms(uint32_t _time)
{
	//记录刷新周期
	ui_data.period_ms = _time;

	ui_data.flicker_timer_100ms += _time;
	if(ui_data.flicker_timer_100ms > 100){
		ui_data.flicker_timer_100ms = 0;
		ui_data.flicker_flag_100ms = !ui_data.flicker_flag_100ms;
	}
	ui_data.flicker_timer_500ms += _time;
	if(ui_data.flicker_timer_500ms > 500){
		ui_data.flicker_timer_500ms = 0;
		ui_data.flicker_flag_500ms = !ui_data.flicker_flag_500ms;
	}
	
	//按键扫描
	Button_Scan_ms(_time);

	//调用图形控件
	if(ui_pitch){
		ui_activated = ui_pitch;
		ui_activated->drawing_function(ui_activated);
	}
}

/**
  * @brief  XDrive_REINui进入校准界面
  * @param  NULL
  * @retval NULL
**/
void XDrive_REINui_ToCalibration(void)
{
	ui_pitch = &ui_calibration;
}
