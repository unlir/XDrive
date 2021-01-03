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
#include "usart.h"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/********** Modbus **********/
/********** Modbus **********/
/********** Modbus **********/
//动态串口结构体
Dynamic_Uart_Typedef dyn_uart1;

/**
  * @brief  Modbus串口配置
  * @param  NULL
  * @retval NULL
**/
void REIN_UART_Modbus_Config(void)
{
	//配置串口初始化配置表
	Modbus_UART_Get_HUART.Instance = Modbus_UART_Get_UART;						//参数:UART_HAL库实例
	Modbus_UART_Get_HUART.Init.BaudRate = dyn_uart1.baud_rate_run;		//参数:波特率
	switch(dyn_uart1.uart_run)
	{
		case Uart_Mode_1SNP:	//参数:8位数据字长,1停止位,无校验
			Modbus_UART_Get_HUART.Init.WordLength = UART_WORDLENGTH_8B;
			Modbus_UART_Get_HUART.Init.StopBits = UART_STOPBITS_1;
			Modbus_UART_Get_HUART.Init.Parity = UART_PARITY_NONE;
		break;
		case Uart_Mode_1SEP:	//参数:9位数据字长,1停止位,偶校验
			Modbus_UART_Get_HUART.Init.WordLength = UART_WORDLENGTH_9B;
			Modbus_UART_Get_HUART.Init.StopBits = UART_STOPBITS_1;
			Modbus_UART_Get_HUART.Init.Parity = UART_PARITY_EVEN;
		break;
		case Uart_Mode_1SOP:	//参数:9位数据字长,1停止位,奇校验
			Modbus_UART_Get_HUART.Init.WordLength = UART_WORDLENGTH_9B;
			Modbus_UART_Get_HUART.Init.StopBits = UART_STOPBITS_1;
			Modbus_UART_Get_HUART.Init.Parity = UART_PARITY_ODD;
		break;
		case Uart_Mode_2SNP:	//参数:8位数据字长,2停止位,无校验
			Modbus_UART_Get_HUART.Init.WordLength = UART_WORDLENGTH_8B;
			Modbus_UART_Get_HUART.Init.StopBits = UART_STOPBITS_2;
			Modbus_UART_Get_HUART.Init.Parity = UART_PARITY_NONE;
		break;
		case Uart_Mode_2SEP:	//参数:9位数据字长,2停止位,偶校验
			Modbus_UART_Get_HUART.Init.WordLength = UART_WORDLENGTH_9B;
			Modbus_UART_Get_HUART.Init.StopBits = UART_STOPBITS_2;
			Modbus_UART_Get_HUART.Init.Parity = UART_PARITY_EVEN;
		break;
		case Uart_Mode_2SOP:	//参数:9位数据字长,2停止位,奇校验
			Modbus_UART_Get_HUART.Init.WordLength = UART_WORDLENGTH_9B;
			Modbus_UART_Get_HUART.Init.StopBits = UART_STOPBITS_2;
			Modbus_UART_Get_HUART.Init.Parity = UART_PARITY_ODD;
		break;
		default:
			return;
	}
	Modbus_UART_Get_HUART.Init.Mode = UART_MODE_TX_RX;							//参数:收发模式(收发)
	Modbus_UART_Get_HUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;			//参数:硬件流(无)
	Modbus_UART_Get_HUART.Init.OverSampling = UART_OVERSAMPLING_16;	//参数:过采样(16)

	//重新初始化串口
	HAL_UART_Init(&Modbus_UART_Get_HUART);		//初始化HAL串口
}

/**
  * @brief  设置Uart模式
  * @param  NULL
  * @retval NULL
**/
void REIN_UART_Modbus_Set_Mode(Uart_Mode value)
{
	if(   (value == Uart_Mode_1SNP)
	   || (value == Uart_Mode_1SEP)
	   || (value == Uart_Mode_1SOP)
	   || (value == Uart_Mode_2SNP)
	   || (value == Uart_Mode_2SEP)
	   || (value == Uart_Mode_2SOP)
	)
	{
		dyn_uart1.uart_order = value;
		dyn_uart1.valid_uart_mode = true;
	}
	else
	{
		dyn_uart1.valid_uart_mode = false;
	}
}

/**
  * @brief  设置Uart波特率
  * @param  NULL
  * @retval NULL
**/
void REIN_UART_Modbus_Set_BaudRate(uint32_t value)
{
	if((value >= 9600) && (value <= 194000))
	{
		dyn_uart1.baud_rate_order = value;
		dyn_uart1.valid_uart_baudrate = true;
	}
	else
	{
		dyn_uart1.valid_uart_baudrate = false;
	}
}

/**
  * @brief  串口参数恢复
  * @param  NULL
  * @retval NULL
**/
void REIN_UART_Modbus_Set_Default(void)
{
	REIN_UART_Modbus_Set_Mode(			De_Uart_Mode);
	REIN_UART_Modbus_Set_BaudRate(	De_Uart_BaudRate);
}

/**
  * @brief  UART_Modbus初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_UART_Modbus_Init(void)
{
	//配置
	if(!dyn_uart1.valid_uart_mode)			REIN_UART_Modbus_Set_Mode(			De_Uart_Mode);			//设置Uart模式
	if(!dyn_uart1.valid_uart_baudrate)	REIN_UART_Modbus_Set_BaudRate(	De_Uart_BaudRate);	//设置Uart波特率
	
	//加载配置
	dyn_uart1.uart_run			= dyn_uart1.uart_order;
	dyn_uart1.baud_rate_run	= dyn_uart1.baud_rate_order;
	
	/* AFIO初始化 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*GPIO Ports Clock Enable*/
	Modbus_UART_TX_GPIO_CLK_ENABLE();
	Modbus_UART_RX_GPIO_CLK_ENABLE();
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = Modbus_UART_TX_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(Modbus_UART_TX_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = Modbus_UART_RX_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(Modbus_UART_RX_GPIO_Port, &GPIO_InitStruct);
	/*Configure AFIO*/
#ifdef Modbus_UART_AFIO_REMAP
	Modbus_UART_AFIO_REMAP				//启用备用的UART_AFIO映射
#endif
	
	/* USART1 DMA TX Init */
	Modbus_UART_TX_HDMA.Instance = Modbus_UART_TX_DMA_CH;
	Modbus_UART_TX_HDMA.Init.Direction = DMA_MEMORY_TO_PERIPH;						//内存到外设
	Modbus_UART_TX_HDMA.Init.PeriphInc = DMA_PINC_DISABLE;								//外设地址(保持)
	Modbus_UART_TX_HDMA.Init.MemInc = DMA_MINC_ENABLE;										//内存地址(递增)
	Modbus_UART_TX_HDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;		//外设宽度(半字)
	Modbus_UART_TX_HDMA.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;			//内存宽度(半字)
	Modbus_UART_TX_HDMA.Init.Mode = DMA_NORMAL;														//模式(循环)
	Modbus_UART_TX_HDMA.Init.Priority = DMA_PRIORITY_VERY_HIGH;						//传输优先级(最高)
	if (HAL_DMA_Init(&Modbus_UART_TX_HDMA) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_LINKDMA(&Modbus_UART_Get_HUART, hdmatx, Modbus_UART_TX_HDMA);
	
	/* USART1 DMA RX Init */
	Modbus_UART_RX_HDMA.Instance = Modbus_UART_RX_DMA_CH;
	Modbus_UART_RX_HDMA.Init.Direction = DMA_PERIPH_TO_MEMORY;						//内存到外设
	Modbus_UART_RX_HDMA.Init.PeriphInc = DMA_PINC_DISABLE;								//外设地址(保持)
	Modbus_UART_RX_HDMA.Init.MemInc = DMA_MINC_ENABLE;										//内存地址(递增)
	Modbus_UART_RX_HDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;		//外设宽度(半字)
	Modbus_UART_RX_HDMA.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;			//内存宽度(半字)
	Modbus_UART_RX_HDMA.Init.Mode = DMA_CIRCULAR;													//模式(循环)
	Modbus_UART_RX_HDMA.Init.Priority = DMA_PRIORITY_VERY_HIGH;						//传输优先级(最高)
	if (HAL_DMA_Init(&Modbus_UART_RX_HDMA) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_LINKDMA(&Modbus_UART_Get_HUART, hdmarx, Modbus_UART_RX_HDMA);
	
	/* UART初始化 */
	/*UART Clock Enable*/
	__HAL_RCC_USART1_CLK_ENABLE();
	/*Configure UART*/
  REIN_UART_Modbus_Config();		//Modbus串口配置
	
	//启用串口1中断
  HAL_NVIC_EnableIRQ(Modbus_UART_IRQn);
}

/**
  * @brief  Modbus低优先级回调
  * @param  NULL
  * @retval NULL
**/
void REIN_UART_Modbus_Low_Priority_Callback(void)
{
	//Modbus串口配置
	if( (dyn_uart1.baud_rate_order != dyn_uart1.baud_rate_run)
	 || (dyn_uart1.uart_order      != dyn_uart1.uart_run))
	{
		//加载配置
		dyn_uart1.baud_rate_run	= dyn_uart1.baud_rate_order;
		dyn_uart1.uart_run			= dyn_uart1.uart_order;
		
		//重新根据配置初始化部分串口配置
		HAL_UART_StateTypeDef		uart_state;
		uint32_t begin_time = HAL_GetTick();
		//等待串口发送就绪(100ms超时设置)
		while((HAL_GetTick() - begin_time) < 100)
		{
			uart_state = HAL_UART_GetState(&Modbus_UART_Get_HUART);
			if((uart_state & HAL_UART_STATE_BUSY_TX) != HAL_UART_STATE_BUSY_TX)	break;	//TX非忙碌则退出
		}
		//失能初始化
		HAL_UART_DeInit(&Modbus_UART_Get_HUART);	//失能HAL串口
		
		//Modbus串口配置
		REIN_UART_Modbus_Config();
	}
}


