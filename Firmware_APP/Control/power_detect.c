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

//Oneself
#include "power_detect.h"

//Application_User_Core
#include "adc.h"

Power_Typedef power;

/**
  * @brief  电源检测运行
  * @param  NULL
  * @retval NULL
**/
void Power_Detection_Run(void)
{
	//电压采样
	power.adc_voltage = whole_adc_data[ADC_POWER_U_GROUP][ADC_POWER_U_CHANNEL];
	power.est_voltage = ((power.est_voltage * 31) + (((uint32_t)power.adc_voltage * 36300) >> 12)) >> 5;
	
	//电流采样(没有硬件版本支持电流检测)
	power.adc_current = whole_adc_data[ADC_POWER_I_GROUP][ADC_POWER_I_CHANNEL];
	power.est_current = 0;
}





