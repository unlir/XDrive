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
  ** @file     : sin_form.c/h
  ** @brief    : SIN数组库
  ** @versions : 1.1.1
  ** @time     : 2019/12/13
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef SIN_FORM_H
#define SIN_FORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

////sin_pi_d2 (对应原始sin函数图形0~pi/2)
//#define sin_pi_d2_dpix			256		//水平分辨率
//#define sin_pi_d2_dpiybit		12		//垂直分辨率位数(4096)
//extern const int16_t sin_pi_d2[sin_pi_d2_dpix + 1];

//sin_pi_m2 (对应原始sin函数图形0~pi*2)
#define sin_pi_m2_dpix			1024	//水平分辨率
#define sin_pi_m2_dpiybit		12		//垂直分辨率位数(4096)
extern const int16_t sin_pi_m2[sin_pi_m2_dpix + 1];

#ifdef __cplusplus
}
#endif
	 
#endif	//SIN_FORM_H

