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
  ** @file     : signal_port.c/h
  ** @brief    : 信号接口
  ** @versions : 2.1.13
  ** @time     : 2020/09/11
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

//Oneself
#include "signal_port.h"

//Application_User_Core
#include "gpio.h"
#include "tim.h"
#include "usart.h"

//Base_Drivers
#include "uart_mixed.h"

//Control
#include "Control_Config.h"

/****************************************** Modbus接口 ******************************************/
/****************************************** Modbus接口 ******************************************/
/****************************************** Modbus接口 ******************************************/
//Signal_Modbus接口
Signal_Modbus_Typedef signal_modbus;

/**
	* @brief  Modbus接口从机配置
  * @param  NULL
  * @retval NULL
**/
void Signal_Modbus_Slave_Config(void)
{
	if((signal_modbus.id_run >= 1) && (signal_modbus.id_run <= 247))	//App代码支持(1~247)
	{

	}
}

/**
  * @brief  设置Modbus接口ID
  * @param  value: 串口波特率
  * @retval NULL
**/
void Signal_Modbus_Set_ID(uint16_t value)
{
	if((value >= 1) && (value <= 247))
	{
		signal_modbus.id_order = value;
		signal_modbus.valid_modbus_id = true;
	}
	else
	{
		signal_modbus.valid_modbus_id = false;
	}
}

/**
  * @brief  Modbus配置恢复
  * @param  NULL
  * @retval NULL
**/
void Signal_Modbus_Set_Default(void)
{
	Signal_Modbus_Set_ID(	De_Modbus_ID);			//设置Modbus接口ID
}

/**
  * @brief  Modbus接口初始化
  * @param  NULL
  * @retval NULL
**/
void Signal_Modbus_Init(void)
{
	//配置
	if(!signal_modbus.valid_modbus_id)			Signal_Modbus_Set_ID(	De_Modbus_ID);			//设置Modbus接口ID
	
	//加载配置
	signal_modbus.id_run = signal_modbus.id_order;
}

/**
  * @brief  Modbus低优先级回调
  * @param  NULL
  * @retval NULL
**/
void Signal_Modbus_Low_Priority_Callback(void)
{
	//Modbus接口配置
	if(signal_modbus.id_order != signal_modbus.id_run)
	{
		//加载配置
		signal_modbus.id_run = signal_modbus.id_order;
		
#if   (Demo4_Dir == Modbus_Dir_Master)

#elif (Demo4_Dir == Modbus_Dir_Slaves)
		//从机配置更新
		Signal_Modbus_Slave_Config();
#endif
	}
}

/****************************************** Signal_Count接口 ******************************************/
/****************************************** Signal_Count接口 ******************************************/
/****************************************** Signal_Count接口 ******************************************/
//GPIO输入
#define SIGNAL_COUNT_READ_DIR_IO()	(SIGNAL_COUNT_DIR_GPIO_Port -> IDR & SIGNAL_COUNT_DIR_Pin)
#define SIGNAL_COUNT_READ_ENA_IO()	(SIGNAL_COUNT_ENA_GPIO_Port -> IDR & SIGNAL_COUNT_ENA_Pin)
//TIM输入
#define SIGNAL_COUNT_READ_COUNT()		(SIGNAL_COUNT_Get_TIM -> CNT)
//TIM输出
#define SIGNAL_COUNT_UP()						(SIGNAL_COUNT_Get_TIM -> CR1 &= ~(TIM_CR1_DIR))
#define SIGNAL_COUNT_DOWN()					(SIGNAL_COUNT_Get_TIM -> CR1 |=  (TIM_CR1_DIR))

//Signal_Count接口
Signal_Count_Typedef sg_cut;

/**
  * @brief  SignalPort设置细分值
  * @param  freq: 细分值
  * @retval NULL
**/
void Signal_Count_SetFraction(uint16_t freq)
{
	if( (freq == 2)
	 || (freq == 4)
	 || (freq == 8)
	 || (freq == 16)
	 || (freq == 32)
	 || (freq == 64)
	 || (freq == 128)
	 || (freq == 256))
	{
		sg_cut.subdivide = freq;													//加载细分
		sg_cut.subdivide_form = 256 / sg_cut.subdivide;
		sg_cut.valid_subdivide = true;
	}
	else{
		sg_cut.valid_subdivide = false;
	}
}

/**
  * @brief  接口设置使能翻转
	* @param  control (true:使能翻转) / (true:使能不翻转)
  * @retval NULL
**/
void Signal_Count_SetEnInve(bool control)
{
	sg_cut.en_inve = control;
	sg_cut.valid_dir_inve = true;
}

/**
  * @brief  接口设置方向翻转
	* @param  control (true:方向翻转) / (true:方向不翻转)
  * @retval NULL
**/
void Signal_Count_SetDirInve(bool control)
{
	sg_cut.dir_inve = control;
	Signal_Count_Dir_Res();
	sg_cut.valid_dir_inve = true;
}

/**
  * @brief  Signal_Count采集初始化
  * @param  NULL
  * @retval NULL
**/
void Signal_Count_Init(void)
{
	//配置
	if(!sg_cut.valid_subdivide)	Signal_Count_SetFraction(	De_SubDivide);	//脉冲信号细分
	if(!sg_cut.valid_dir_inve)	Signal_Count_SetEnInve(		De_EN_inve);		//使能信号翻转
	if(!sg_cut.valid_dir_inve)	Signal_Count_SetDirInve(	De_DIR_inve);		//方向信号翻转
}

/**
  * @brief  Signal_Count采集配置
  * @param  NULL
  * @retval NULL
**/
void Signal_Count_Config(void)
{
	//外设初始化
	REIN_GPIO_SIGNAL_COUNT_Init();
	REIN_TIM_SIGNAL_COUNT_Init();
	
	//更新计数方向
	Signal_Count_Dir_Res();
}

/**
  * @brief  Signal_Count清理配置
  * @param  NULL
  * @retval NULL
**/
void Signal_Count_DeConfig(void)
{
	//外设清理
	REIN_GPIO_SIGNAL_COUNT_DeInit();
	REIN_TIM_SIGNAL_COUNT_DeInit();
}

/**
  * @brief  SignalPort获取目标
  * @param  NULL
  * @retval NULL
**/
void Signal_Count_Capture_Goal(void)
{
	//读取En_Pin
	if(SIGNAL_COUNT_READ_ENA_IO()){
		if(sg_cut.en_inve)	sg_cut.en_valid = true;
		else								sg_cut.en_valid = false;
	}
	else{
		if(sg_cut.en_inve)	sg_cut.en_valid = false;
		else								sg_cut.en_valid = true;
	}

	//采样(对比上次的计数值)
	sg_cut.sampling_count_last = sg_cut.sampling_count;
	sg_cut.sampling_count = SIGNAL_COUNT_READ_COUNT();
	sg_cut.sampling_count_sub = sg_cut.sampling_count - sg_cut.sampling_count_last;
	
	//采样(缓冲输出)
	sg_cut.interp_out = sg_cut.sampling_count_sub * sg_cut.subdivide_form;
	
	//(由于XDrive采用多功能数据接口,脉冲速度估计移动到位置插补器执行)

//	//脉冲速度还原
//	sg_cut.restore_speed_mut += ( (sg_cut.interp_out * CONTROL_FREQ_HZ)
//															+ ((sg_cut.restore_speed << 5) - sg_cut.restore_speed));
//	sg_cut.restore_speed = sg_cut.restore_speed_mut >> 5;
//	sg_cut.restore_speed_mut = sg_cut.restore_speed_mut - (sg_cut.restore_speed << 5);
	
	//输出
	if(sg_cut.en_valid){
		signal_moreio.goal_location = sg_cut.interp_out;	//Count模式借用目标位置存放目标位置增量
		//signal_moreio.goal_speed = 0;
		//signal_moreio.goal_current = 0;
		signal_moreio.goal_disable = false;
		signal_moreio.goal_brake = false;
	}
	else{
		signal_moreio.goal_location = 0;	//Count模式借用目标位置存放目标位置增量
		//signal_moreio.goal_speed = 0;
		//signal_moreio.goal_current = 0;
		signal_moreio.goal_disable = true;
		signal_moreio.goal_brake = false;
	}
}

/**
  * @brief  Signal_Count计数器方向变更处理
  * @param  NULL
  * @retval NULL
**/
void Signal_Count_Dir_Res(void)
{
	//修改定时器计数方向
	if(sg_cut.dir_inve){
		if(SIGNAL_COUNT_READ_DIR_IO())	SIGNAL_COUNT_DOWN();	//DIR高电平-配置为向下计数
		else														SIGNAL_COUNT_UP();		//DIR低电平-配置为向上计数
	}
	else{
		if(SIGNAL_COUNT_READ_DIR_IO())	SIGNAL_COUNT_UP();		//DIR高电平-配置为向上计数
		else														SIGNAL_COUNT_DOWN();	//DIR低电平-配置为向下计数
	}
}

/****************************************** Signal_PWM接口 ******************************************/
/****************************************** Signal_PWM接口 ******************************************/
/****************************************** Signal_PWM接口 ******************************************/
//GPIO输入
#define SIGNAL_PWM_READ_DIR_IO()	(SIGNAL_PWM_DIR_GPIO_Port -> IDR & SIGNAL_PWM_DIR_Pin)
#define SIGNAL_PWM_READ_ENA_IO()	(SIGNAL_PWM_ENA_GPIO_Port -> IDR & SIGNAL_PWM_ENA_Pin)

//Signal_PWM接口
Signal_PWM_Typedef sg_pwm;

/**
  * @brief  Signal_PWM设置最大脉宽
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Set_TopWidth(uint16_t width)
{
	if(width <= 50000){
		sg_pwm.top_width = width;
		sg_pwm.valid_top_width = true;
	}
	else{
		sg_pwm.valid_top_width = false;
	}
}

/**
  * @brief  Signal_PWM设置最小脉宽
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Set_BottomWidth(uint16_t width)
{
	if(width <= 50000){
		sg_pwm.bottom_width = width;
		sg_pwm.valid_bottom_width = true;
	}
	else{
		sg_pwm.valid_bottom_width = false;
	}
}

/**
  * @brief  Signal_PWM设置最大脉宽表示位置
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Set_TopLocation(int32_t location)
{
	sg_pwm.top_location = location;
	sg_pwm.valid_top_location = true;
}

/**
  * @brief  Signal_PWM设置最小脉宽表示位置
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Set_BottomLocation(int32_t location)
{
	sg_pwm.bottom_location = location;
	sg_pwm.valid_bottom_location = true;
}

/**
  * @brief  Signal_PWM设置最大脉宽表示速度
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Set_TopSpeed(int32_t speed)
{
	if((speed >= -Move_Rated_Speed) && (speed <= Move_Rated_Speed)){
		sg_pwm.top_speed = speed;
		sg_pwm.valid_top_speed = true;
	}
	else{
		sg_pwm.valid_top_speed = true;
	}
}

/**
  * @brief  Signal_PWM设置最小脉宽表示速度
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Set_BottomSpeed(int32_t speed)
{
	if((speed >= -Move_Rated_Speed) && (speed <= Move_Rated_Speed)){
		sg_pwm.bottom_speed = speed;
		sg_pwm.valid_bottom_speed = true;
	}
	else{
		sg_pwm.valid_bottom_speed = true;
	}
}

/**
  * @brief  Signal_PWM设置最大脉宽表示电流
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Set_TopCurrent(int32_t current)
{
	if((current >= -Current_Rated_Current) && (current <= Current_Rated_Current)){
		sg_pwm.top_current = current;
		sg_pwm.valid_top_current = true;
	}
	else{
		sg_pwm.valid_top_current = true;
	}
}

/**
  * @brief  Signal_PWM设置最小脉宽表示电流
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Set_BottomCurrent(int32_t current)
{
	if((current >= -Current_Rated_Current) && (current <= Current_Rated_Current)){
		sg_pwm.bottom_current = current;
		sg_pwm.valid_bottom_current = true;
	}
	else{
		sg_pwm.valid_bottom_current = true;
	}
}

/**
  * @brief  Signal_PWM参数恢复
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Set_Default(void)
{
	Signal_PWM_Set_TopWidth(De_TOP_Width);
	Signal_PWM_Set_BottomWidth(De_BOTTOM_Width);
	Signal_PWM_Set_TopLocation(De_TOP_Location);
	Signal_PWM_Set_BottomLocation(De_BOTTOM_Location);
	Signal_PWM_Set_TopSpeed(De_TOP_Speed);
	Signal_PWM_Set_BottomSpeed(De_BOTTOM_Speed);
	Signal_PWM_Set_TopCurrent(De_TOP_Current);
	Signal_PWM_Set_BottomCurrent(De_BOTTOM_Current);
}

/**
  * @brief  Signal_PWM初始化
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Init(void)
{
	//配置
	if(!sg_pwm.valid_top_width)				Signal_PWM_Set_TopWidth(				De_TOP_Width				);
	if(!sg_pwm.valid_bottom_width)		Signal_PWM_Set_BottomWidth(			De_BOTTOM_Width			);
	if(!sg_pwm.valid_top_location)		Signal_PWM_Set_TopLocation(			De_TOP_Location			);
	if(!sg_pwm.valid_bottom_location)	Signal_PWM_Set_BottomLocation(	De_BOTTOM_Location	);
	if(!sg_pwm.valid_top_speed)				Signal_PWM_Set_TopSpeed(				De_TOP_Speed				);
	if(!sg_pwm.valid_bottom_speed)		Signal_PWM_Set_BottomSpeed(			De_BOTTOM_Speed			);
	if(!sg_pwm.valid_top_current)			Signal_PWM_Set_TopCurrent(			De_TOP_Current			);
	if(!sg_pwm.valid_bottom_current)	Signal_PWM_Set_BottomCurrent(		De_BOTTOM_Current		);
}

/**
  * @brief  Signal_PWM采集配置
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Config(void)
{
	//外设初始化
	REIN_GPIO_SIGNAL_PWM_Init();
	REIN_TIM_SIGNAL_PWM_Init();
	
	//采集数据
	sg_pwm.h_width = 0;
	sg_pwm.period = 0;
	sg_pwm.count_rising = 0;
	sg_pwm.count_falling = 0;
	sg_pwm.count_update = 0;
	sg_pwm.whole_h_flag = false;
	sg_pwm.whole_l_flag = false;
	sg_pwm.ready_first = false;
	sg_pwm.ready_second = false;
	sg_pwm.ready_third = false;
}

/**
  * @brief  Signal_PWM清理配置
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_DeConfig(void)
{
	//外设清理
	REIN_GPIO_SIGNAL_PWM_DeInit();
	REIN_TIM_SIGNAL_PWM_DeInit();
}

/**
  * @brief  Signal_PWM获取目标
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_Capture_Goal(void)
{	
	//各个都已经更新
}

/**
  * @brief  Signal_PWM采集中断回调
  * @param  NULL
  * @retval NULL
**/
void Signal_PWM_TIM_Callback(void)
{
	//定时器捕获中断通道1 (上升沿中断) (必须在更新中断前执行)
	if(__HAL_TIM_GET_FLAG(&SIGNAL_PWM_Get_HTIM, TIM_FLAG_CC1) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(&SIGNAL_PWM_Get_HTIM, TIM_IT_CC1) !=RESET)
		{
			__HAL_TIM_CLEAR_IT(&SIGNAL_PWM_Get_HTIM, TIM_IT_CC1);
			SIGNAL_PWM_Get_HTIM.Channel = HAL_TIM_ACTIVE_CHANNEL_1;

			//采集上升沿数据
			sg_pwm.period = HAL_TIM_ReadCapturedValue(&SIGNAL_PWM_Get_HTIM, TIM_CHANNEL_1) + 1;		//获取PWM周期
			sg_pwm.count_rising = sg_pwm.count_update;																						//更新计数器镜像
		}
	}
	//定时器捕获中断通道2 (下降沿中断)
	if(__HAL_TIM_GET_FLAG(&SIGNAL_PWM_Get_HTIM, TIM_FLAG_CC2) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(&SIGNAL_PWM_Get_HTIM, TIM_IT_CC2) !=RESET)
		{
			__HAL_TIM_CLEAR_IT(&SIGNAL_PWM_Get_HTIM, TIM_IT_CC2);
			SIGNAL_PWM_Get_HTIM.Channel = HAL_TIM_ACTIVE_CHANNEL_2;

			//采集下降沿数据
			sg_pwm.h_width = HAL_TIM_ReadCapturedValue(&SIGNAL_PWM_Get_HTIM, TIM_CHANNEL_2) + 1;	//获取PWM高宽度
			sg_pwm.count_falling = sg_pwm.count_update;																						//更新计数器镜像
		}
	}
	//定时器更新中断 (更新事件中断) (由更新事件触发 | 溢出事件触发)
	if(__HAL_TIM_GET_FLAG(&SIGNAL_PWM_Get_HTIM, TIM_FLAG_UPDATE) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(&SIGNAL_PWM_Get_HTIM, TIM_IT_UPDATE) !=RESET)
		{
			__HAL_TIM_CLEAR_IT(&SIGNAL_PWM_Get_HTIM, TIM_IT_UPDATE);
			
			//单次PWM全高全低检测
			if(	(sg_pwm.count_update != sg_pwm.count_rising)	//(上升沿计数器镜像,更新计数器不相等)
			 && (sg_pwm.count_update != sg_pwm.count_falling)	//(下降沿计数器镜像,更新计数器不相等)
			){
				//读取PWM电平用于判定全高或全低
				if(SIGNAL_PWM_PUL_GPIO_Port -> IDR & SIGNAL_PWM_PUL_Pin){
					sg_pwm.whole_h_flag = true;		//置位100%标志
					sg_pwm.whole_l_flag = false;	//清位0%标志
				}
				else{
					sg_pwm.whole_h_flag = false;	//清位100%标志
					sg_pwm.whole_l_flag = true;		//置位0%标志
				}
			}
			else{	
				sg_pwm.count_update ++;
				sg_pwm.whole_h_flag = false;		//清位100%标志
				sg_pwm.whole_l_flag = false;		//清位0%标志
			}
			
			///提取本周期PWM
			if(sg_pwm.whole_h_flag)	sg_pwm.h_width = 65535;
			if(sg_pwm.whole_l_flag)	sg_pwm.h_width = 0;
			
			//单次PWM有效性确认
			if(0){}
			else if((sg_pwm.whole_h_flag))																																sg_pwm.ready_first = false;	//100%_PWM无效
			else if((sg_pwm.whole_l_flag))																																sg_pwm.ready_first = false;	//0%_PWM无效
			else if((sg_pwm.top_width < (65535 - 100)) && ((sg_pwm.top_width + 100) < sg_pwm.h_width))		sg_pwm.ready_first = false;	//脉宽超长
			else if((sg_pwm.bottom_width > (0 + 100)) && ((sg_pwm.bottom_width - 100) > sg_pwm.h_width))	sg_pwm.ready_first = false;	//脉宽超短
			else																																													sg_pwm.ready_first = true;
			
			//可靠的PWM有效性确认
			if(sg_pwm.ready_first){
				if(sg_pwm.ready_second){
					sg_pwm.ready_third = true;
				}
				else{
					sg_pwm.ready_second = true;
					sg_pwm.ready_third = false;
				}
			}
			else{
				sg_pwm.ready_second = false;
				sg_pwm.ready_third = false;
			}
			
			//第三次采样成功时开始获取数据
			if(sg_pwm.ready_third){
				//提取有效PWM
				if(sg_pwm.h_width > sg_pwm.top_width)						sg_pwm.valid_width = sg_pwm.top_width;			//大于最大取最大
				else if(sg_pwm.h_width < sg_pwm.bottom_width)		sg_pwm.valid_width = sg_pwm.bottom_width;		//小于最小取最小
				else																						sg_pwm.valid_width = sg_pwm.h_width;				//不大不小就合适

				//提取有效目标
				if(signal_moreio.mode == MoreIO_Mode_PWM_Location){
					//提取目标位置
					signal_moreio.goal_location = (int32_t)	(		(float)((int32_t)sg_pwm.valid_width  - (int32_t)sg_pwm.bottom_width)
																										/ (float)((int32_t)sg_pwm.top_width    - (int32_t)sg_pwm.bottom_width)
																										* (float)((int32_t)sg_pwm.top_location - (int32_t)sg_pwm.bottom_location)
																									)
																			+ (int32_t)	(sg_pwm.bottom_location);
				}
				else if(signal_moreio.mode == MoreIO_Mode_PWM_Speed){
					//提取目标速度
					signal_moreio.goal_speed =	(int32_t)	(	(float)((int32_t)sg_pwm.valid_width - (int32_t)sg_pwm.bottom_width)
																								/ (float)((int32_t)sg_pwm.top_width   - (int32_t)sg_pwm.bottom_width)
																								* (float)((int32_t)sg_pwm.top_speed   - (int32_t)sg_pwm.bottom_speed)
																								)
																		+ (int32_t)	(sg_pwm.bottom_speed);
					//DIR引脚低电平--->目标速度取负值
					if(!SIGNAL_PWM_READ_DIR_IO())
						signal_moreio.goal_speed = -signal_moreio.goal_speed;
				}
				else if(signal_moreio.mode == MoreIO_Mode_PWM_Current)
				{
					//提取目标电流
					signal_moreio.goal_current =	(int32_t)	(	(float)((int32_t)sg_pwm.valid_width - (int32_t)sg_pwm.bottom_width)
																									/ (float)((int32_t)sg_pwm.top_width   - (int32_t)sg_pwm.bottom_width)
																									* (float)((int32_t)sg_pwm.top_current - (int32_t)sg_pwm.bottom_current)
																									)
																			+ (int32_t)	(sg_pwm.bottom_current);
					//DIR引脚低电平--->目标电流取负值
					if(!SIGNAL_PWM_READ_DIR_IO())
						signal_moreio.goal_current = -signal_moreio.goal_current;
				}

				//提取特殊目标
				signal_moreio.goal_disable = false;
				signal_moreio.goal_brake = false;
			}
			else{
				//提取特殊目标
				signal_moreio.goal_disable = true;
				signal_moreio.goal_brake = false;
			}
		}
	}
}

/****************************************** MoreIO接口 ******************************************/
/****************************************** MoreIO接口 ******************************************/
/****************************************** MoreIO接口 ******************************************/
Signal_MoreIO_Typedef signal_moreio;

/**
	* @brief  MoreIO初始化
	* @param  NULL
	* @retval NULL
**/
void Signal_MoreIO_Init(void)
{
	//预设模式(Disable)
	signal_moreio.mode = MoreIO_Mode_Disable;
	
	//MoreIO不同模式的参数初始化
	Signal_Count_Init();		//Signal_Count采集初始化
	Signal_PWM_Init();			//Signal_PWM采集初始化
}

/**
	* @brief  MoreIO配置工作模式
	* @param  mode: 工作模式
	* @retval NULL
**/
void Signal_MoreIO_Config(MoreIO_Mode mode)
{
	//判断模式是否变更
	if(mode == signal_moreio.mode)
		return;
	
	//清除当前模式
	switch(signal_moreio.mode)
	{
		case MoreIO_Mode_Disable:																			break;		//Disable
		case MoreIO_Mode_PWM_Location:			Signal_PWM_DeConfig();		break;		//PWM位置
		case MoreIO_Mode_PWM_Speed:					Signal_PWM_DeConfig();		break;		//PWM速度
		case MoreIO_Mode_PWM_Current:				Signal_PWM_DeConfig();		break;		//PWM电流
		case MoreIO_Mode_PULSE_Locatioon:		Signal_Count_DeConfig();	break;		//PULSE位置
	}

	//初始化新模式
	signal_moreio.mode = mode;
	switch(signal_moreio.mode)
	{
		case MoreIO_Mode_Disable:																			break;		//Disable
		case MoreIO_Mode_PWM_Location:			Signal_PWM_Config();			break;		//PWM位置
		case MoreIO_Mode_PWM_Speed:					Signal_PWM_Config();			break;		//PWM速度
		case MoreIO_Mode_PWM_Current:				Signal_PWM_Config();			break;		//PWM电流
		case MoreIO_Mode_PULSE_Locatioon:		Signal_Count_Config();		break;		//PULSE位置
	}
}

/**
	* @brief  MoreIO获取目标
	* @param  NULL
	* @retval NULL
**/
void Signal_MoreIO_Capture_Goal(void)
{
	//初始化新模式
	switch(signal_moreio.mode)
	{
		case MoreIO_Mode_Disable:																						break;		//Disable
		case MoreIO_Mode_PWM_Location:			Signal_PWM_Capture_Goal();			break;		//PWM位置
		case MoreIO_Mode_PWM_Speed:					Signal_PWM_Capture_Goal();			break;		//PWM速度
		case MoreIO_Mode_PWM_Current:				Signal_PWM_Capture_Goal();			break;		//PWM电流
		case MoreIO_Mode_PULSE_Locatioon:		Signal_Count_Capture_Goal();		break;		//PULSE位置
	}
}

