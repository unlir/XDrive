/******
	************************************************************************
	******
	** @versions : 1.1.4
	** @time     : 2020/09/15
	******
	************************************************************************
	******
	** @project : XDrive_Step
	** @brief   : 具有多功能接口和闭环功能的步进电机
	** @author  : unlir (知不知啊)
	******
	** @address : https://github.com/unlir/XDrive
	******
	** @issuer  : IVES ( 艾维斯 实验室) (QQ: 557214000)   (master)
	** @issuer  : REIN (  知驭  实验室) (QQ: 857046846)   (master)
	******
	************************************************************************
	******
	** {Stepper motor with multi-function interface and closed loop function.}
	** Copyright (c) {2020}  {unlir}
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
  ** @time     : 2020/09/13
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

//Oneself
#include "xdrive_ui.h"

//Control
#include "motor_control.h"
#include "encode_cali.h"

//Debug
#include "button.h"
#include "ssd1306.h"

/**
  * @brief  XDrive初始化
  * @param  NULL
  * @retval NULL
**/
void XDrive_UI_Init(void)
{
	
}

/**
  * @brief  XDrive任务回调
  * @param  NULL
  * @retval NULL
**/
void XDrive_UI_Callback_ms(uint32_t _time)
{
//	static uint8_t *hello = (uint8_t*)"Hello Word!";
	static Motor_Mode new_mode = Control_Mode_Stop;		//准备上载的模式
	
	/********** 操作一：扫描按键 **********/
	//按键定时扫描
	Button_Scan_ms(_time);
	
	/********** 操作二：处理界面 **********/
	//UP按键弹起边沿
	if(Button_Inquice_State(Button_UP) == Button_Bit_Rise){
		switch(new_mode){
				//测试
				case Motor_Mode_Debug_Location:		new_mode = Motor_Mode_Debug_Speed;				break;
				case Motor_Mode_Debug_Speed:			new_mode = Control_Mode_Stop;							break;
				//停止
				case Control_Mode_Stop:						new_mode = Motor_Mode_Digital_Location;		break;
				//DIG(CAN/RS485)
				case Motor_Mode_Digital_Location:	new_mode = Motor_Mode_Digital_Speed;			break;
				case Motor_Mode_Digital_Speed:		new_mode = Motor_Mode_Digital_Current;		break;
				case Motor_Mode_Digital_Current:	new_mode = Motor_Mode_Digital_Track;			break;
				case Motor_Mode_Digital_Track:		new_mode = Motor_Mode_PWM_Location;				break;
				//MoreIO(PWM/PUL)
				case Motor_Mode_PWM_Location:			new_mode = Motor_Mode_PWM_Speed;					break;
				case Motor_Mode_PWM_Speed:				new_mode = Motor_Mode_PWM_Current;				break;
				case Motor_Mode_PWM_Current:			new_mode = Motor_Mode_PULSE_Location;			break;
				case Motor_Mode_PULSE_Location:		new_mode = Motor_Mode_PULSE_Location;			break;
				//其他非法模式
				default:													new_mode = Control_Mode_Stop;							break;
		}
	}
	//DOWN按键弹起边沿
	if(Button_Inquice_State(Button_DOWN) == Button_Bit_Rise){
		switch(new_mode){
				//测试
				case Motor_Mode_Debug_Location:		new_mode = Motor_Mode_Debug_Location;			break;
				case Motor_Mode_Debug_Speed:			new_mode = Motor_Mode_Debug_Location;			break;
				//停止
				case Control_Mode_Stop:						new_mode = Motor_Mode_Debug_Speed;				break;
				//DIG(CAN/RS485)
				case Motor_Mode_Digital_Location:	new_mode = Control_Mode_Stop;							break;
				case Motor_Mode_Digital_Speed:		new_mode = Motor_Mode_Digital_Location;		break;
				case Motor_Mode_Digital_Current:	new_mode = Motor_Mode_Digital_Speed;			break;
				case Motor_Mode_Digital_Track:		new_mode = Motor_Mode_Digital_Current;		break;
				//MoreIO(PWM/PUL)
				case Motor_Mode_PWM_Location:			new_mode = Motor_Mode_Digital_Track;			break;
				case Motor_Mode_PWM_Speed:				new_mode = Motor_Mode_PWM_Location;				break;
				case Motor_Mode_PWM_Current:			new_mode = Motor_Mode_PWM_Speed;					break;
				case Motor_Mode_PULSE_Location:		new_mode = Motor_Mode_PWM_Current;				break;
				//其他非法模式
				default:													new_mode = Control_Mode_Stop;							break;
		}
	}
	//OK按键弹起边沿
	if(Button_Inquice_State(Button_OK) == Button_Bit_Rise){
		motor_control.mode_order = new_mode;
	}
	
	//OK按键长按下
	if(Button_Inquice_State(Button_OK) == Button_Bit_Long){
		encode_cali.trigger = true;
	}
	
	/********** 操作三：刷屏 **********/
	//清除SSD1306缓存
	GRAM_Clear();
	
	//显示校准错误类型(优先界面)
	if(encode_cali.error_code)
	{
		GRAM_ShowString(0, 8, (uint8_t*)"Cali Error:", CharFont_1608, true);
		GRAM_ShowNum_LU_Max10(88,  8, encode_cali.error_code, 5, CharFont_1608, true);
	}
	else
	{
		//显示模式
		GRAM_ShowString(0, 0, (uint8_t*)"Mode:", CharFont_0806, true);	//模式标签
		switch(motor_control.mode_run){
				//测试
				case Motor_Mode_Debug_Location:		GRAM_ShowString(30,  0, (uint8_t*)"DbLoc", CharFont_0806, true);						break;
				case Motor_Mode_Debug_Speed:			GRAM_ShowString(30,  0, (uint8_t*)"DbSpd", CharFont_0806, true);						break;
				//停止
				case Control_Mode_Stop:						GRAM_ShowString(30,  0, (uint8_t*)"Stop~", CharFont_0806, true);						break;
				//DIG(CAN/RS485)
				case Motor_Mode_Digital_Location:	GRAM_ShowString(30,  0, (uint8_t*)"DgLoc", CharFont_0806, true);						break;
				case Motor_Mode_Digital_Speed:		GRAM_ShowString(30,  0, (uint8_t*)"DgSpd", CharFont_0806, true);						break;
				case Motor_Mode_Digital_Current:	GRAM_ShowString(30,  0, (uint8_t*)"DgCur", CharFont_0806, true);						break;
				case Motor_Mode_Digital_Track:		GRAM_ShowString(30,  0, (uint8_t*)"DgTra", CharFont_0806, true);						break;
				//MoreIO(PWM/PUL)
				case Motor_Mode_PWM_Location:			GRAM_ShowString(30,  0, (uint8_t*)"PmLoc", CharFont_0806, true);						break;
				case Motor_Mode_PWM_Speed:				GRAM_ShowString(30,  0, (uint8_t*)"PmSpd", CharFont_0806, true);						break;
				case Motor_Mode_PWM_Current:			GRAM_ShowString(30,  0, (uint8_t*)"PmCur", CharFont_0806, true);						break;
				case Motor_Mode_PULSE_Location:		GRAM_ShowString(30,  0, (uint8_t*)"PuLoc", CharFont_0806, true);						break;
				//其他非法模式
				default:	break;
		}
		GRAM_ShowString(64, 0, (uint8_t*)"NwMd:", CharFont_0806, true);	//模式标签
		switch(new_mode){
				//测试
				case Motor_Mode_Debug_Location:		GRAM_ShowString(94,  0, (uint8_t*)"DbLoc", CharFont_0806, true);						break;
				case Motor_Mode_Debug_Speed:			GRAM_ShowString(94,  0, (uint8_t*)"DbSpd", CharFont_0806, true);						break;
				//停止
				case Control_Mode_Stop:						GRAM_ShowString(94,  0, (uint8_t*)"Stop~", CharFont_0806, true);						break;
				//DIG(CAN/RS485)
				case Motor_Mode_Digital_Location:	GRAM_ShowString(94,  0, (uint8_t*)"DgLoc", CharFont_0806, true);						break;
				case Motor_Mode_Digital_Speed:		GRAM_ShowString(94,  0, (uint8_t*)"DgSpd", CharFont_0806, true);						break;
				case Motor_Mode_Digital_Current:	GRAM_ShowString(94,  0, (uint8_t*)"DgCur", CharFont_0806, true);						break;
				case Motor_Mode_Digital_Track:		GRAM_ShowString(94,  0, (uint8_t*)"DgTra", CharFont_0806, true);						break;
				//MoreIO(PWM/PUL)
				case Motor_Mode_PWM_Location:			GRAM_ShowString(94,  0, (uint8_t*)"PmLoc", CharFont_0806, true);						break;
				case Motor_Mode_PWM_Speed:				GRAM_ShowString(94,  0, (uint8_t*)"PmSpd", CharFont_0806, true);						break;
				case Motor_Mode_PWM_Current:			GRAM_ShowString(94,  0, (uint8_t*)"PmCur", CharFont_0806, true);						break;
				case Motor_Mode_PULSE_Location:		GRAM_ShowString(94,  0, (uint8_t*)"PuLoc", CharFont_0806, true);						break;
				//其他非法模式
				default:	break;
		}
		//显示位置
		GRAM_ShowString(0, 8, (uint8_t*)"Loca:", CharFont_0806, true);	//位置标签
		GRAM_ShowNum_LI_Max11(30,  8, motor_control.est_location, 11, CharFont_0806, true);
		//显示速度
		GRAM_ShowString(0, 16, (uint8_t*)"Spee:", CharFont_0806, true);	//速度标签
		GRAM_ShowNum_LI_Max11(30, 16, motor_control.est_speed, 11, CharFont_0806, true);
		//显示电流
		GRAM_ShowString(0, 24, (uint8_t*)"Curr:", CharFont_0806, true);	//电流标签
		GRAM_ShowNum_LI_Max11(30, 24, motor_control.foc_current, 11, CharFont_0806, true);
		//显示XDrive
		GRAM_ShowString(96, 12, (uint8_t*)" IVES", CharFont_0806, true);	//IVES标签
		GRAM_ShowString(96, 20, (uint8_t*)" REIN", CharFont_0806, true);	//REIN标签
	}
	//刷入SSD1306
	SSD1306_Refresh();
}

