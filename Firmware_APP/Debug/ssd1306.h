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
  ** @file     : ssd1306.c/h
  ** @brief    : SSD1306驱动
  ** @versions : 1.1.1
  ** @time     : 2019/10/11
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef SSD1306_H
#define SSD1306_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kernel_port.h"

#define Quantity_Row			128		//行
#define Quantity_Column		32		//列

//实例
extern uint8_t	ssd1306_gram[(Quantity_Row) * (Quantity_Column/8)];	//GRAM空间

typedef enum{
	CharFont_0806 = 0x01,		//0806字符
	CharFont_1206,					//1206字符
	CharFont_1608,					//1608字符
	CharFont_2412,					//1608字符
}CharFont;

//读写操作
void SSD1306_Write_Data(uint8_t _data);		//写数据
void SSD1306_Write_Comm(uint8_t _comm);		//写指令
//基本指令操作
void SSD1306_Display_On(void);							//屏幕显示开
void SSD1306_Display_Off(void);							//屏幕显示关
void SSD1306_Color_Reversal(bool	_switch);	//颜色反转
void SSD1306_Orien_Reversal(bool _switch);	//方向反转
//基本数据操作
void SSD1306_Refresh(uint8_t *_gram);				//更新显示
//读写GRAM空间操作
void GRAM_Clear(							uint8_t *_gram                                                                                       );	//GRAM清空
void GRAM_DrawPoint(          uint8_t *_gram, uint8_t _x, uint8_t _y,                                                  bool _switch);	//GRAM打点
void GRAM_DrawLine(           uint8_t *_gram, uint8_t _x, uint8_t _y, uint8_t _xe, uint8_t _ye,                        bool _switch);	//GRAM打线
void GRAM_DrawCircle(         uint8_t *_gram, uint8_t _x, uint8_t _y, uint8_t _radius,                                 bool _switch);	//GRAM打圆
void GRAM_ShowPicture(				uint8_t *_gram, uint8_t _x, uint8_t _y, uint8_t _xs, uint8_t _ys, uint8_t* pic,          bool _switch);	//GRAM显示图片
void GRAM_ShowChar(           uint8_t *_gram, uint8_t _x, uint8_t _y, uint8_t _char,                 CharFont _format, bool _switch);	//GRAM显示字符
void GRAM_ShowString(         uint8_t *_gram, uint8_t _x, uint8_t _y, uint8_t *_char,                CharFont _format, bool _switch);	//GRAM显示字符串
void GRAM_ShowNum_LU_Max10(   uint8_t *_gram, uint8_t _x, uint8_t _y, uint32_t _num, uint8_t _width, CharFont _format, bool _switch);	//GRAM显示LU型数字
void GRAM_ShowNum_LI_Max11(   uint8_t *_gram, uint8_t _x, uint8_t _y, int32_t _num,  uint8_t _width, CharFont _format, bool _switch);	//GRAM显示LI型数字
void GRAM_ShowNum_Float_Max15(uint8_t *_gram, uint8_t _x, uint8_t _y, float _num,    uint8_t _width, CharFont _format, bool _switch);	//GRAM显示Float型数字

//接口函数
void SSD1306_Init(void);		//SSD1306初始化

#ifdef __cplusplus
}
#endif

#endif

