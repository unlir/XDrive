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
  ** @file     : loop.c/h and loop_it.c
  ** @brief    : MCU系工程主框架
  ** @versions : newest
	** @time     : newest
  ** @reviser  : unli (WuHu China)
	** @explain  : null
*****/

#include "main.h"

//Application_User_Core
#include "usart.h"

//Base_Drivers
#include "mt6816.h"

//Kernel
#include "loop.h"

//Control
#include "signal_port.h"
#include "motor_control.h"
#include "encode_cali.h"
#include "power_detect.h"

//Debug
#include "button.h"

//核心中断标志数据
bool systick_20khz_flag = false;	//20KHz系统时钟标志
uint8_t lit_1ms_divider = 0;			//副时钟分频器(1ms)

struct Loop_IT_Typedef{
	//中断计数器
	uint32_t	systick_count;
	uint32_t	exti7_count;
	uint32_t	time3_count;
	uint32_t	dma1_ch4_count;
	uint32_t	dma1_ch5_count;
	uint32_t	uart1_count;
}loop_it;

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

/**
  * @brief This function handles System tick timer.
	* 启动初期由HAL库自动初始化的SysTick为1KHz
	* 由REIN库接管后修改的SysTick为20KHz
**/
void SysTick_Handler(void)
{
	if(systick_20khz_flag){
		//编码器数据采集
		REIN_MT6816_Get_AngleData();
		//电源数据采集
		Power_Detection_Run();
		
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
	
	loop_it.systick_count++;
}

#if (XDrive_Run == XDrive_REIN_Basic_H1_1)
/**
  * @brief  中断优先级覆盖
  * @param  NULL
  * @retval NULL
**/
void LoopIT_Priority_Overlay(void)
{
	/**********中断优先级**********/
	//中断优先级规则
	//0：输入信号关键中断
	//1：传感器关键中断
	//2：任务中断
	//3: 通讯中断

	//中断号																				优先级			中断源												用途
  HAL_NVIC_SetPriority(SIGNAL_COUNT_DIR_Get_IRQn,	0,	0);		//外部中断(SIGNAL_COUNT_DIR)		获取SIGNAL_COUNT计数器方向
	HAL_NVIC_SetPriority(SIGNAL_PWM_Get_IRQn,				0,	0);		//定时器捕获(SIGNAL_PWM_PWM)		获取SIGNAL_PWM两个边沿计数值
	HAL_NVIC_SetPriority(SysTick_IRQn,							2,	0);		//系统定时器										核心时钟

	HAL_NVIC_SetPriority(Modbus_UART_TX_DMA_IRQn,		3,	0);		//Modbus_TX_DMA中断							响应发送完成			
	HAL_NVIC_SetPriority(Modbus_UART_RX_DMA_IRQn,		3,	0);		//Modbus_RX_DMA中断							响应接收完成
	HAL_NVIC_SetPriority(Modbus_UART_IRQn,					3,	0);		//Modbus_UART中断								响应串口收发中断
}

/**
  * @brief EXTI9_5_IRQHandler
**/
void EXTI9_5_IRQHandler(void)
{
	//PA1中断
  if(__HAL_GPIO_EXTI_GET_IT(SIGNAL_COUNT_DIR_Pin) != 0x00u){
    __HAL_GPIO_EXTI_CLEAR_IT(SIGNAL_COUNT_DIR_Pin);
		Signal_Count_Dir_Res();		//信号COUNT_DIR中断
  }
	
	loop_it.exti7_count++;
}

/**
  * @brief TIM3_IRQHandler
**/
void TIM3_IRQHandler(void)
{
	//TIM3的中断标志清除由PWM处理函数识别并清除
	Signal_PWM_TIM_Callback();	//SIGNAL_PWM采集中断回调
	
	loop_it.time3_count++;
}

/**
  * @brief DMA1_Channel4_IRQHandler
**/
void DMA1_Channel4_IRQHandler(void)
{
	//调用HAL的DMA中断处理函数,以调用串口发送函数(在uart_mixed中实例化)
	HAL_DMA_IRQHandler(&hdma_usart1_tx);
	
	loop_it.dma1_ch4_count++;
}

/**
  * @brief DMA1_Channel5_IRQHandler
**/
void DMA1_Channel5_IRQHandler(void)
{
	//调用HAL的DMA中断处理函数,以调用串口接收函数(在uart_mixed中实例化)
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
	
	loop_it.dma1_ch5_count++;
}

/**
  * @brief USART1_IRQHandler
**/
void USART1_IRQHandler(void)
{
	//调用HAL的UART中断处理函数,以清理未知的中断事件
  HAL_UART_IRQHandler(&huart1);
	
	//检测串口是否产生了空闲中断
  if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
		
		//调用串口接收函数(在uart_mixed中实例化)
    HAL_UART_RxCpltCallback(&huart1);
  }
	
	loop_it.uart1_count++;
}

#elif (XDrive_Run == XDrive_REIN_Basic_H1_0)
/**
  * @brief  中断优先级覆盖
  * @param  NULL
  * @retval NULL
**/
void LoopIT_Priority_Overlay(void)
{
	/**********中断优先级**********/
	//中断优先级规则
	//0：输入信号关键中断
	//1：传感器关键中断
	//2：任务中断

	//中断号																				优先级			中断源												用途
	HAL_NVIC_SetPriority(SIGNAL_PWM_Get_IRQn,				0,	0);		//定时器捕获(SIGNAL_PWM_PWM)		获取SIGNAL_PWM两个边沿计数值
  HAL_NVIC_SetPriority(SIGNAL_COUNT_DIR_Get_IRQn,	0,	0);		//外部中断(SIGNAL_COUNT_DIR)		获取SIGNAL_COUNT计数器方向
	HAL_NVIC_SetPriority(MT6816_PWM_Get_IRQn,				1,	0);		//定时器捕获(MT6816_PWM_PWM)		获取MT6816_PWM两个边沿计数值
	HAL_NVIC_SetPriority(MT6816_ABZ_Z_EXTI_IRQn,		1,	0);		//外部中断(MT6816_ABZ_Z脉冲)		矫正MT6816_ABZ信号
	HAL_NVIC_SetPriority(SysTick_IRQn,							2,	0);		//系统定时器										核心时钟
}

#if (MT6816_Mode == MT6816_Mode_PWM)
/**
  * @brief TIM3_IRQHandler
**/
void TIM3_IRQHandler(void)
{
	//TIM3的中断标志清除由PWM处理函数识别并清除
	REIN_MT6816_PWM_TIM_Callback();	//MT6816_PWM采集中断回调
}
#endif

#if (MT6816_Mode == MT6816_Mode_ABZ)
/**
  * @brief EXTI15_10_IRQHandler
**/
void EXTI15_10_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(MT6816_ABZ_Z_Pin) != 0x00u)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(MT6816_ABZ_Z_Pin);
		REIN_MT6816_ABZ_ZPulse_Callback();	//编码器器Z脉冲中断
  }
}
#endif

/**
  * @brief TIM2_IRQHandler
**/
void TIM2_IRQHandler(void)
{
	//TIM2的中断标志清除由PWM处理函数识别并清除
	Signal_PWM_TIM_Callback();	//SIGNAL_PWM采集中断回调
}

/**
  * @brief EXTI1_IRQHandler
**/
void EXTI1_IRQHandler(void)
{
	//PA1中断
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != 0x00u){
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
		Signal_Count_Dir_Res();		//信号COUNT_DIR中断
  }
}

#else
	#error "undefined"
#endif
