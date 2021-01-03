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

//Oneself
#include "mt6816.h"

//Config
#include "stockpile_f103cb.h"

//Control
#include "control_config.h"

//编码器矫正数据位置(Quick)(Flash)
uint16_t *Read_QuickCali_DATA = (uint16_t*)STOCKPILE_APP_CALI_ADDR;

//GPIO输出
#define MT6816_SPI_CS_H()		(MT6816_SPI_CS_GPIO_Port -> BSRR = MT6816_SPI_CS_Pin)
#define MT6816_SPI_CS_L()		(MT6816_SPI_CS_GPIO_Port -> BRR  = MT6816_SPI_CS_Pin)

#if (MT6816_Mode == MT6816_Mode_PWM)
/****************************** MT6816_PWM ******************************/
/****************************** MT6816_PWM ******************************/
/****************************** MT6816_PWM ******************************/
MT6816_PWM_Signal_Typedef mt6816_pwm;

/**
  * @brief  MT6816_PWM采集初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_MT6816_PWM_Signal_Init(void)
{
	//采集数据
	mt6816_pwm.h_width = 0;
	mt6816_pwm.period = 0;
	mt6816_pwm.count_rising = 0;
	mt6816_pwm.count_falling = 0;
	mt6816_pwm.count_update = 0;
	mt6816_pwm.whole_h_flag = false;
	mt6816_pwm.whole_l_flag = false;
	mt6816_pwm.ready_once = false;
	mt6816_pwm.ready_again = false;
	//输出数据
	mt6816_pwm.valid_width = 0;
	
	//配置外设
	REIN_TIM_MT6816_PWM_Init();
}

/**
  * @brief  MT6816_PWM采集中断回调
  * @param  NULL
  * @retval NULL
**/
void REIN_MT6816_PWM_TIM_Callback(void)
{
	//定时器捕获中断通道1 (上升沿中断) (必须在更新中断前执行)
	if(__HAL_TIM_GET_FLAG(&MT6816_PWM_Get_HTIM, TIM_FLAG_CC1) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(&MT6816_PWM_Get_HTIM, TIM_IT_CC1) !=RESET)
		{
			__HAL_TIM_CLEAR_IT(&MT6816_PWM_Get_HTIM, TIM_IT_CC1);
			MT6816_PWM_Get_HTIM.Channel = HAL_TIM_ACTIVE_CHANNEL_1;

			//采集上升沿数据
			mt6816_pwm.period = HAL_TIM_ReadCapturedValue(&MT6816_PWM_Get_HTIM, TIM_CHANNEL_1) + 1;		//获取PWM周期
			mt6816_pwm.count_rising = mt6816_pwm.count_update;																				//更新计数器镜像
		}
	}
	//定时器捕获中断通道2 (下降沿中断)
	if(__HAL_TIM_GET_FLAG(&MT6816_PWM_Get_HTIM, TIM_FLAG_CC2) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(&MT6816_PWM_Get_HTIM, TIM_IT_CC2) !=RESET)
		{
			__HAL_TIM_CLEAR_IT(&MT6816_PWM_Get_HTIM, TIM_IT_CC2);
			MT6816_PWM_Get_HTIM.Channel = HAL_TIM_ACTIVE_CHANNEL_2;

			//采集下降沿数据
			mt6816_pwm.h_width = HAL_TIM_ReadCapturedValue(&MT6816_PWM_Get_HTIM, TIM_CHANNEL_2) + 1;	//获取PWM高宽度
			mt6816_pwm.count_falling = mt6816_pwm.count_update;																				//更新计数器镜像
		}
	}
	//定时器更新中断 (更新事件中断) (由更新事件触发 | 溢出事件触发)
	if(__HAL_TIM_GET_FLAG(&MT6816_PWM_Get_HTIM, TIM_FLAG_UPDATE) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(&MT6816_PWM_Get_HTIM, TIM_IT_UPDATE) !=RESET)
		{
			__HAL_TIM_CLEAR_IT(&MT6816_PWM_Get_HTIM, TIM_IT_UPDATE);
			
			//单次PWM全高全低检测
			if(	(mt6816_pwm.count_update != mt6816_pwm.count_rising)	//(上升沿计数器镜像,更新计数器不相等)
			 && (mt6816_pwm.count_update != mt6816_pwm.count_falling)	//(下降沿计数器镜像,更新计数器不相等)
			){
				//读取PWM电平用于判定全高或全低
				if(MT6816_PWM_GPIO_Port -> IDR & MT6816_PWM_Pin){
					mt6816_pwm.whole_h_flag = true;		//置位100%标志
					mt6816_pwm.whole_l_flag = false;	//清位0%标志
				}
				else{
					mt6816_pwm.whole_h_flag = false;	//清位100%标志
					mt6816_pwm.whole_l_flag = true;		//置位0%标志
				}
			}
			else{	
				mt6816_pwm.count_update ++;
				mt6816_pwm.whole_h_flag = false;		//清位100%标志
				mt6816_pwm.whole_l_flag = false;		//清位0%标志
			}
			
			//单次PWM有效性确认
			if(0){}
			else if((mt6816_pwm.whole_h_flag))	mt6816_pwm.ready_once = false;	//100%_PWM无效
			else if((mt6816_pwm.whole_l_flag))	mt6816_pwm.ready_once = false;	//0%_PWM无效
			//脉冲超长检测(无需检测)
			//脉冲超短检测(无需检测)
			else																mt6816_pwm.ready_once = true;
			
			//可靠的PWM有效性确认
			if(mt6816_pwm.ready_once){
				if(mt6816_pwm.ready_again){
					mt6816_pwm.valid_width = mt6816_pwm.h_width;
				}
				else{
					mt6816_pwm.valid_width = 0;
					mt6816_pwm.ready_again = true;
				}
			}
			else{
				mt6816_pwm.valid_width = 0;
				mt6816_pwm.ready_again = false;
			}
		}
	}
}

/**
  * @brief  MT6816_PWM获取角度数据
  * @param  NULL
  * @retval NULL
**/
void REIN_MT6816_PWM_Get_AngleData(void)
{	
	//输出数据
	int32_t out = (mt6816_pwm.valid_width << 1) - 64;
	if(out < 0)
		mt6816_pwm.angle = 0;
	else if(out > 16383)
		mt6816_pwm.angle = 16383;
	else
		mt6816_pwm.angle = out;
}

#elif (MT6816_Mode == MT6816_Mode_ABZ)
/****************************** MT6816_ABZ ******************************/
/****************************** MT6816_ABZ ******************************/
/****************************** MT6816_ABZ ******************************/
MT6816_ABZ_Signal_Typedef	mt6816_abz;

/**
  * @brief  MT6816_ABZ采集初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_MT6816_ABZ_Signal_Init(void)
{
	//采集数据
	mt6816_abz.sample_data = 0;
	//输出数据
	mt6816_abz.angle = 0;
	
	//配置外设
	REIN_TIM_MT6816_ABZ_Init();
	REIN_GPIO_MT6816_ABZ_Init();
}

/**
  * @brief  MT6816_ABZ采集Z脉冲中断回调
  * @param  NULL
  * @retval NULL
**/
void REIN_MT6816_ABZ_ZPulse_Callback(void)
{
	//清除定时器计数值
	__HAL_TIM_SET_COUNTER(&MT6816_ABZ_Get_HTIM, 0);
}

/**
  * @brief  MT6816_ABZ获取角度数据
  * @param  NULL
  * @retval NULL
**/
void REIN_MT6816_ABZ_Get_AngleData(void)
{
	//采集数据
	mt6816_abz.sample_data = __HAL_TIM_GET_COUNTER(&MT6816_ABZ_Get_HTIM) & 0x0FFF;	//读取定时器计数值
	//输出数据
	mt6816_abz.angle = ((mt6816_abz.sample_data + 1) << 2) - 1;		//获得数值0~16383
}

#elif (MT6816_Mode == MT6816_Mode_SPI)
/****************************** MT6816_SPI ******************************/
/****************************** MT6816_SPI ******************************/
/****************************** MT6816_SPI ******************************/
MT6816_SPI_Signal_Typedef	mt6816_spi;

/**
  * @brief  MT6816_SPI采集初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_MT6816_SPI_Signal_Init(void)
{
	//采集数据
	mt6816_spi.sample_data = 0;
	//输出数据
	mt6816_spi.angle = 0;
	
	//配置外设
	REIN_GPIO_MT6816_SPI_Init();
	REIN_SPI_MT6816_SPI_Init();
}

/**
  * @brief  MT6816_SPI采集获取角度数据
  * @param  NULL
  * @retval NULL
**/
void RINE_MT6816_SPI_Get_AngleData(void)
{
	uint16_t data_t[2];
	uint16_t data_r[2];
	uint8_t h_count;
	
	data_t[0] = (0x80 | 0x03) << 8;
	data_t[1] = (0x80 | 0x04) << 8;
	
	for(uint8_t i=0; i<3; i++){
		//读取SPI数据
		MT6816_SPI_CS_L();
		HAL_SPI_TransmitReceive(&MT6816_SPI_Get_HSPI, (uint8_t*)&data_t[0], (uint8_t*)&data_r[0], 1, HAL_MAX_DELAY);
		MT6816_SPI_CS_H();
		MT6816_SPI_CS_L();
		HAL_SPI_TransmitReceive(&MT6816_SPI_Get_HSPI, (uint8_t*)&data_t[1], (uint8_t*)&data_r[1], 1, HAL_MAX_DELAY);
		MT6816_SPI_CS_H();
		mt6816_spi.sample_data = ((data_r[0] & 0x00FF) << 8) | (data_r[1] & 0x00FF);
		
		//奇偶校验
		h_count = 0;
		for(uint8_t j=0; j<16; j++){
			if(mt6816_spi.sample_data & (0x0001 << j))
				h_count++;
		}
		if(h_count & 0x01){
			mt6816_spi.pc_flag = false;
		}
		else{
			mt6816_spi.pc_flag = true;
			break;
		}
	}
	
	if(mt6816_spi.pc_flag){
		mt6816_spi.angle = mt6816_spi.sample_data >> 2;
		mt6816_spi.no_mag_flag = (bool)(mt6816_spi.sample_data & (0x0001 << 1));
	}
}

#endif

/****************************** MT6816 ******************************/
/****************************** MT6816 ******************************/
/****************************** MT6816 ******************************/
MT6816_Typedef	mt6816;

/**
  * @brief  MT6816初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_MT6816_Init(void)
{
#ifndef MT6816_Mode
	#error "NO define MT6816_Mode"
#endif
#if   (MT6816_Mode == MT6816_Mode_PWM)
	REIN_MT6816_PWM_Signal_Init();		//初始化PWM接口
#elif (MT6816_Mode == MT6816_Mode_SPI)
	REIN_MT6816_SPI_Signal_Init();		//初始化SPI接口
#elif (MT6816_Mode == MT6816_Mode_ABZ)
	REIN_MT6816_ABZ_Signal_Init();		//初始化ABZ接口
#else
	#error "MT6816_Mode Invalid !!!"
#endif
	
	//初始化阶段获取一次角度数据(过滤错误数据)(暂未查明复位后第一次读取数据丢失的原因)
	REIN_MT6816_Get_AngleData();
	
	//检查校准区数据是否有效
	mt6816.rectify_valid = true;
	for(uint16_t i=0; i<(CALI_Encode_Res); i++){
		if(Read_QuickCali_DATA[i] == 0xFFFF)
			mt6816.rectify_valid = false;
	}
}

/**
  * @brief  MT6816获取角度数据
  * @param  NULL
  * @retval NULL
**/
void REIN_MT6816_Get_AngleData(void)
{
#ifndef MT6816_Mode
	#error "NO define MT6816_Mode"
#endif
#if   (MT6816_Mode == MT6816_Mode_PWM)
	REIN_MT6816_PWM_Get_AngleData();	//MT6816_PWM获取角度数据
	mt6816.angle_data = mt6816_pwm.angle;
	mt6816.rectify_angle = Read_QuickCali_DATA[mt6816.angle_data];
#elif (MT6816_Mode == MT6816_Mode_SPI)
	RINE_MT6816_SPI_Get_AngleData();
	mt6816.angle_data = mt6816_spi.angle;
	mt6816.rectify_angle = Read_QuickCali_DATA[mt6816.angle_data];
#elif (MT6816_Mode == MT6816_Mode_ABZ)
	REIN_MT6816_ABZ_Get_AngleData();	//MT6816_ABZ获取角度数据
	
#else
	#error "MT6816_Mode Invalid !!!"
#endif
}


















