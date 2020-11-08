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
  ** @file     : loop.c/h and loop_it.c
  ** @brief    : MCU系工程主框架
  ** @versions : newest
	** @time     : newest
  ** @reviser  : unli (WuHu China)
	** @explain  : null
*****/

#include "main.h"

//Base_Drivers
#include "mt6816.h"

//Kernel
#include "loop.h"

//Control
#include "signal_port.h"
#include "motor_control.h"
#include "encode_cali.h"

//Debug
#include "button.h"

//核心中断标志数据
bool systick_20khz_flag = false;	//20KHz系统时钟标志
uint8_t lit_1ms_divider = 0;			//副时钟分频器(1ms)


struct Loop_IT_Typedef{
	//中断计数器
	uint32_t systick_count;
}loop_it;

/**
  * @brief  中断优先级覆盖
  * @param  NULL
  * @retval NULL
**/
void LoopIT_Priority_Overlay(void)
{
	/**********中断优先级**********/
	//中断号														优先级			中断源													用途
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,	0,	0);		//外部中断(MT6816_ABZ_Z脉冲)		矫正MT6816_ABZ信号
	HAL_NVIC_SetPriority(TIM3_IRQn,				1,	0);		//定时器捕获(MT6816_PWM_PWM)		获取MT6816_PWM两个边沿计数值
	HAL_NVIC_SetPriority(TIM2_IRQn,				2,	0);		//定时器捕获(SIGNAL_PWM_PWM)		获取SIGNAL_PWM两个边沿计数值
  HAL_NVIC_SetPriority(EXTI1_IRQn,			3,	0);		//外部中断(SIGNAL_COUNT_DIR)		获取SIGNAL_COUNT计数器方向
	HAL_NVIC_SetPriority(SysTick_IRQn,		4,	0);		//系统定时器										核心时钟
}

/**
* @brief  系统计时器修改为20KHz
  * @param  NULL
  * @retval NULL
**/
void LoopIT_SysTick_20KHz(void)
{
	systick_20khz_flag = true;
	HAL_SYSTICK_Config(SystemCoreClock / 20000);	//更新为20K中断
}

/******************** 中断向量调用区 ********************/
/******************** 中断向量调用区 ********************/
/******************** 中断向量调用区 ********************/
/**
  * @brief This function handles EXTI line[15:10] interrupts.
**/
void EXTI15_10_IRQHandler(void)
{
	//PA10中断
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != 0x00u)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
    
		//编码器器Z脉冲中断
		REIN_MT6816_ABZ_ZPulse_Callback();
  }
}

/**
  * @brief This function handles TIM3 global interrupt.
**/
void TIM3_IRQHandler(void)
{
	//TIM3的中断标志清除由PWM处理函数识别并清除
	REIN_MT6816_PWM_TIM_Callback();	//MT6816_PWM采集中断回调
}

/**
  * @brief This function handles TIM2 global interrupt.
**/
void TIM2_IRQHandler(void)
{
	//TIM2的中断标志清除由PWM处理函数识别并清除
	Signal_PWM_TIM_Callback();	//SIGNAL_PWM采集中断回调
}

/**
  * @brief This function handles EXTI line1 interrupts.
**/
void EXTI1_IRQHandler(void)
{
	//PA1中断
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != 0x00u){
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
		Signal_Count_Dir_Res();		//信号COUNT_DIR中断
  }
}

/**
  * @brief This function handles System tick timer.
	* 启动初期由HAL库自动初始化的SysTick为1KHz
	* 由REIN库接管后修改的SysTick为20KHz
**/
void SysTick_Handler(void)
{
	loop_it.systick_count++;
	if(systick_20khz_flag){
		//编码器数据采集
		REIN_MT6816_Get_AngleData();
		//电源数据采集
		
		
		//运动控制
		if(encode_cali.trigger)	Calibration_Interrupt_Callback();	//校准器中断回调
		else										Motor_Control_Callback();					//控制器任务回调
		
		//1ms分频
		lit_1ms_divider++;
		if(lit_1ms_divider >= 20){
			lit_1ms_divider  = 0;
			loop_second_base_1ms();		//副时钟1ms时钟
			HAL_IncTick();	//提供HAL库的1ms中断调用,维持HAL_Delay等超时动作的有效执行
		}
	}
	else{
		HAL_IncTick();	//提供HAL库的1ms中断调用,维持HAL_Delay等超时动作的有效执行
	}
}

