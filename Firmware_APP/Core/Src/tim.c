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
#include "tim.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/********** HwElec **********/
/********** HwElec **********/
/********** HwElec **********/
/**
  * @brief  TIM_PWMtoDAC初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_TIM_HwElec_Init(void)
{
	/* AFIO初始化 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*GPIO Ports Clock Enable*/
	HW_ELEC_APWM_GPIO_CLK_ENABLE();		//启用APWM端口时钟
	HW_ELEC_BPWM_GPIO_CLK_ENABLE();		//启用BPWM端口时钟
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = HW_ELEC_APWM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;				//复用推挽
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	//高速
	HAL_GPIO_Init(HW_ELEC_APWM_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = HW_ELEC_BPWM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;				//复用推挽
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	//高速
	HAL_GPIO_Init(HW_ELEC_BPWM_GPIO_Port, &GPIO_InitStruct);	
	/*Configure AFIO*/
#ifdef HwElec_TIM_AFIO_REMAP
	HwElec_TIM_AFIO_REMAP				//启用备用的TIM_AFIO映射
#endif
	
	/* TIM初始化 */
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	/*TIM Clock Enable*/
	HW_ELEC_PWM_TIM_CLK_ENABLE();			//启用TIM时钟
	/*Configure TIM*/
	HW_ELEC_PWM_Get_HTIM.Instance = HW_ELEC_PWM_Get_TIM;
	HW_ELEC_PWM_Get_HTIM.Init.Prescaler = 0;																			//无预分频
	HW_ELEC_PWM_Get_HTIM.Init.CounterMode = TIM_COUNTERMODE_UP;										//向上计数
	HW_ELEC_PWM_Get_HTIM.Init.Period = (1024 - 1);																//10位周期(70.312KHz)
	HW_ELEC_PWM_Get_HTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;							//不分频
	HW_ELEC_PWM_Get_HTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;	//禁用自动重新装载
	if (HAL_TIM_PWM_Init(&HW_ELEC_PWM_Get_HTIM) != HAL_OK)
	{
	  Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;							//主机模式:复位
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;		//禁用主机模式
	if (HAL_TIMEx_MasterConfigSynchronization(&HW_ELEC_PWM_Get_HTIM, &sMasterConfig) != HAL_OK)
	{
	  Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;						//PWM模式1
	sConfigOC.Pulse = 0;													//预设PWM输出为0
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;		//有效电平:高
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;		//禁用快速模式
	if (HAL_TIM_PWM_ConfigChannel(&HW_ELEC_PWM_Get_HTIM, &sConfigOC, HW_ELEC_APWM_CHANNEL) != HAL_OK)
	{
	  Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&HW_ELEC_PWM_Get_HTIM, &sConfigOC, HW_ELEC_BPWM_CHANNEL) != HAL_OK)
	{
	  Error_Handler();
	}
	HAL_TIM_PWM_Start(&HW_ELEC_PWM_Get_HTIM, HW_ELEC_APWM_CHANNEL);
	HAL_TIM_PWM_Start(&HW_ELEC_PWM_Get_HTIM, HW_ELEC_BPWM_CHANNEL);
}

/********** MT6816 **********/
/********** MT6816 **********/
/********** MT6816 **********/
#if (XDrive_Run == XDrive_REIN_Basic_H1_0)
/**
  * @brief  TIM_MT6816_ABZ初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_TIM_MT6816_ABZ_Init(void)
{ 
	/* GPIO初始化 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*GPIO Ports Clock Enable*/
	MT6816_ABZ_A_GPIO_CLK_ENABLE();		//启用ABZ_A端口时钟
	MT6816_ABZ_B_GPIO_CLK_ENABLE();		//启用ABZ_B端口时钟
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = MT6816_ABZ_A_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;	//输入模式
	GPIO_InitStruct.Pull = GPIO_NOPULL;			//禁用上下拉
	HAL_GPIO_Init(MT6816_ABZ_A_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = MT6816_ABZ_B_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;	//输入模式
	GPIO_InitStruct.Pull = GPIO_NOPULL;			//禁用上下拉
	HAL_GPIO_Init(MT6816_ABZ_B_GPIO_Port, &GPIO_InitStruct);
	
	/* TIM初始化 */
	TIM_Encoder_InitTypeDef sConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	__HAL_RCC_TIM1_CLK_ENABLE();
	MT6816_ABZ_Get_HTIM.Instance = MT6816_ABZ_Get_TIM;
	MT6816_ABZ_Get_HTIM.Init.Prescaler = 0;																				//无预分频								
	MT6816_ABZ_Get_HTIM.Init.CounterMode = TIM_COUNTERMODE_UP;										//向上计数
	MT6816_ABZ_Get_HTIM.Init.Period = (4096 - 1);																	//12位周期
	MT6816_ABZ_Get_HTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;							//不分频
	MT6816_ABZ_Get_HTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;	//禁用自动重新加载
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;				//双边沿四倍频采样
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;			//上升沿计数
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;	//TI1FP1
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;						//不分频
	sConfig.IC1Filter = 0;														//禁用滤波器
	sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;			//下降沿计数
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;	//TF2FP2
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;						//不分频
	sConfig.IC2Filter = 0;														//禁用滤波器
	if (HAL_TIM_Encoder_Init(&MT6816_ABZ_Get_HTIM, &sConfig) != HAL_OK)
	{
	  Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;							//主机模式:复位
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;		//禁用主机模式
	if (HAL_TIMEx_MasterConfigSynchronization(&MT6816_ABZ_Get_HTIM, &sMasterConfig) != HAL_OK)
	{
	  Error_Handler();
	}
	/*begin work*/
	HAL_TIM_Base_Stop(&MT6816_ABZ_Get_HTIM);
	HAL_TIM_Encoder_Start(&MT6816_ABZ_Get_HTIM, TIM_CHANNEL_ALL);
}
#endif

#if (XDrive_Run == XDrive_REIN_Basic_H1_0)
/**
  * @brief  TIM_MT6816_PWM初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_TIM_MT6816_PWM_Init(void)
{
	/* GPIO初始化 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*GPIO Ports Clock Enable*/
	MT6816_PWM_GPIO_CLK_ENABLE();		//启用PWM端口时钟
  /*Configure GPIO pin*/
	GPIO_InitStruct.Pin = MT6816_PWM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;	//输入模式
	GPIO_InitStruct.Pull = GPIO_PULLUP;			//禁用上下拉
	HAL_GPIO_Init(MT6816_PWM_GPIO_Port, &GPIO_InitStruct);
	
	/* 定时器初始化 */
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
	MT6816_PWM_TIM_CLK_ENABLE();		//启用TIM时钟
  MT6816_PWM_Get_HTIM.Instance = MT6816_PWM_Get_TIM;
  MT6816_PWM_Get_HTIM.Init.Prescaler = (9 - 1);																	//预分频:9		采样频率 72M/9 = 8M   分辨精度125ns
  MT6816_PWM_Get_HTIM.Init.CounterMode = TIM_COUNTERMODE_UP;										//向上计数
  MT6816_PWM_Get_HTIM.Init.Period = (65536 - 1);																//采样宽度 65536 * 125ns = 8.192ms  频率122.07Hz (满足MT6816两种PWM频率输出)
  MT6816_PWM_Get_HTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;							//不分频
  MT6816_PWM_Get_HTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;	//禁用自动重新加载
  if (HAL_TIM_Base_Init(&MT6816_PWM_Get_HTIM) != HAL_OK){
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&MT6816_PWM_Get_HTIM) != HAL_OK){
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;											//从模式:复位模式
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;												//从模式触发信号:TI1FP1
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;		//从模式触发极性:上升沿触发
  sSlaveConfig.TriggerFilter = 0;																		//禁用滤波器
  if (HAL_TIM_SlaveConfigSynchro(&MT6816_PWM_Get_HTIM, &sSlaveConfig) != HAL_OK){
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;						//主模式:复位
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;	//禁用主机模式
  if (HAL_TIMEx_MasterConfigSynchronization(&MT6816_PWM_Get_HTIM, &sMasterConfig) != HAL_OK){
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;   //上升沿捕获
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;					//TI1FP1
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;										//不分频
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&MT6816_PWM_Get_HTIM, &sConfigIC, TIM_CHANNEL_1) != HAL_OK){
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;	//下降沿捕获
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;				//TI1FP2
  if (HAL_TIM_IC_ConfigChannel(&MT6816_PWM_Get_HTIM, &sConfigIC, TIM_CHANNEL_2) != HAL_OK){
    Error_Handler();
  }
  /*interrupt init*/
	HAL_NVIC_EnableIRQ(MT6816_PWM_Get_IRQn);		//使能定时中断
	/*begin work*/
	HAL_TIM_Base_Stop(&MT6816_PWM_Get_HTIM);									//停用TIM
	HAL_TIM_OC_Start_IT(&MT6816_PWM_Get_HTIM, TIM_CHANNEL_1);	//启动CH捕获比较中断
	HAL_TIM_OC_Start_IT(&MT6816_PWM_Get_HTIM, TIM_CHANNEL_2);	//启动CH捕获比较中断
	HAL_TIM_Base_Start_IT(&MT6816_PWM_Get_HTIM);							//启动TIM中断模式				
}
#endif

/********** SIGNAL **********/
/********** SIGNAL **********/
/********** SIGNAL **********/
/**
  * @brief  TIM_SIGNAL_PUL初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_TIM_SIGNAL_COUNT_Init(void)
{
	/* GPIO初始化 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable*/
	SIGNAL_COUNT_PUL_CLK_ENABLE();			//启用SIGNAL_COUNT_PUL端口时钟
  /*Configure GPIO pin*/
	GPIO_InitStruct.Pin = SIGNAL_COUNT_PUL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		//输入模式
	GPIO_InitStruct.Pull = GPIO_NOPULL;				//禁用上下拉
	HAL_GPIO_Init(SIGNAL_COUNT_PUL_GPIO_Port, &GPIO_InitStruct);
	
	/* TIM初始化 */
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
	SIGNAL_COUNT_TIM_CLK_ENABLE();		//启用TIM时钟
  SIGNAL_COUNT_Get_HTIM.Instance = SIGNAL_COUNT_Get_TIM;
  SIGNAL_COUNT_Get_HTIM.Init.Prescaler = 0;														//预分频:0
  SIGNAL_COUNT_Get_HTIM.Init.CounterMode = TIM_COUNTERMODE_UP;				//向上计数
  SIGNAL_COUNT_Get_HTIM.Init.Period = 65535;													//16位周期
  SIGNAL_COUNT_Get_HTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;	//不分频
  SIGNAL_COUNT_Get_HTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;	//禁用自动重新加载
  if (HAL_TIM_Base_Init(&SIGNAL_COUNT_Get_HTIM) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;						//外部时钟模式
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;									//TI1FP1
  sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;	//上升沿触发
  sSlaveConfig.TriggerFilter = 4;															//滤波参数(FDIV2_N6)
  if (HAL_TIM_SlaveConfigSynchro(&SIGNAL_COUNT_Get_HTIM, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;							//主机模式触发复位
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;		//禁用主机模式
  if (HAL_TIMEx_MasterConfigSynchronization(&SIGNAL_COUNT_Get_HTIM, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
	/*begin work*/
	HAL_TIM_Base_Start(&SIGNAL_COUNT_Get_HTIM);
}

/**
  * @brief  TIM_SIGNAL_COUNT清理
  * @param  NULL
  * @retval NULL
**/
void REIN_TIM_SIGNAL_COUNT_DeInit(void)
{
	HAL_TIM_Base_Stop(&SIGNAL_COUNT_Get_HTIM);										//停止TIM
	HAL_GPIO_DeInit(SIGNAL_COUNT_PUL_GPIO_Port, SIGNAL_COUNT_PUL_Pin);	//重置GPIO
}	

/**
  * @brief  TIM_SIGNAL_PWM初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_TIM_SIGNAL_PWM_Init(void)
{
	/* GPIO初始化 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable*/
	SIGNAL_PWM_PUL_CLK_ENABLE();		//启用SIGNAL_PWM_PUL端口时钟
  /*Configure GPIO pin*/
	GPIO_InitStruct.Pin = SIGNAL_PWM_PUL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;	//输入模式
	GPIO_InitStruct.Pull = GPIO_PULLUP;			//禁用上下拉
	HAL_GPIO_Init(SIGNAL_PWM_PUL_GPIO_Port, &GPIO_InitStruct);
	
	/* 定时器初始化 */
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
	SIGNAL_PWM_TIM_CLK_ENABLE();		//启用TIM时钟
  SIGNAL_PWM_Get_HTIM.Instance = SIGNAL_PWM_Get_TIM;
  SIGNAL_PWM_Get_HTIM.Init.Prescaler = 71;																			//采样频率 72M/(71+1) = 1M   周期1us
  SIGNAL_PWM_Get_HTIM.Init.CounterMode = TIM_COUNTERMODE_UP;										//向上计数模式
  SIGNAL_PWM_Get_HTIM.Init.Period = 65535;																			//采样宽度 65536 * 1us = 65.536ms  频率15.23Hz (满足20Hz及以上频率PWM信号)
  SIGNAL_PWM_Get_HTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;							//不分频
  SIGNAL_PWM_Get_HTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;	//禁用自动重新加载
  if (HAL_TIM_Base_Init(&SIGNAL_PWM_Get_HTIM) != HAL_OK){
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&SIGNAL_PWM_Get_HTIM) != HAL_OK){
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;											//从模式:复位模式
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;												//从模式触发信号:TI1FP1
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;		//从模式触发极性:上升沿触发
  sSlaveConfig.TriggerFilter = 0;																		//禁用滤波器
  if (HAL_TIM_SlaveConfigSynchro(&SIGNAL_PWM_Get_HTIM, &sSlaveConfig) != HAL_OK){
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;						//主模式:复位
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;	//禁用主机模式
  if (HAL_TIMEx_MasterConfigSynchronization(&SIGNAL_PWM_Get_HTIM, &sMasterConfig) != HAL_OK){
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;   //上升沿捕获
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;					//TI1FP1
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;										//不分频
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&SIGNAL_PWM_Get_HTIM, &sConfigIC, TIM_CHANNEL_1) != HAL_OK){
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;	//下降沿捕获
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;				//TI1FP2
  if (HAL_TIM_IC_ConfigChannel(&SIGNAL_PWM_Get_HTIM, &sConfigIC, TIM_CHANNEL_2) != HAL_OK){
    Error_Handler();
  }
	
  /* interrupt init*/
	HAL_NVIC_EnableIRQ(SIGNAL_PWM_Get_IRQn);		//使能定时中断
	
	//开始TIM工作
	HAL_TIM_Base_Stop(&SIGNAL_PWM_Get_HTIM);									//停用TIM
	HAL_TIM_OC_Start_IT(&SIGNAL_PWM_Get_HTIM, TIM_CHANNEL_1);	//启动CH1捕获比较中断
	HAL_TIM_OC_Start_IT(&SIGNAL_PWM_Get_HTIM, TIM_CHANNEL_2);	//启动CH2捕获比较中断
	HAL_TIM_Base_Start_IT(&SIGNAL_PWM_Get_HTIM);							//启动TIM中断	
}

/**
  * @brief  TIM_SIGNAL_PWM清理
  * @param  NULL
  * @retval NULL
**/
void REIN_TIM_SIGNAL_PWM_DeInit(void)
{
	HAL_TIM_Base_Stop_IT(&SIGNAL_PWM_Get_HTIM);									//停止TIM中断
	HAL_TIM_OC_Stop_IT(&SIGNAL_PWM_Get_HTIM, TIM_CHANNEL_2);		//停止CH2捕获比较中断
	HAL_TIM_OC_Stop_IT(&SIGNAL_PWM_Get_HTIM, TIM_CHANNEL_1);		//停止CH1捕获比较中断
	HAL_GPIO_DeInit(SIGNAL_PWM_PUL_GPIO_Port, SIGNAL_PWM_PUL_Pin);	//重置GPIO
}
