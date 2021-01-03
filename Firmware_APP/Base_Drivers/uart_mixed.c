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
  ** @file     : uart_mixed.c/h
  ** @brief    : Rein串口通信库
  ** @versions : 1.1.1
  ** @time     : 2019/11/1
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/


// 升级日志
/** V:1.0.6
  * 时间 2019/3/5
  * 1.UART_Mixed_RxCallback函数->使用__HAL库获取DMA剩余传输次数
  * 2.简化Uart_Mixed_DeInit函数
  * 3.增加Uart6
  * 4.修复UART_Dir_Control的Bug
  * 5.删除UART_Mixed_RxCallback函数
  
*** V:1.1.1
  * 时间 2019/11/1
  * 1.并入Rein开发包
  * 1.修改失能混合串口为停止混合串口，并添加开始混合串口，初始化也修改为调用开始函数
***/

//使用介绍
//1. 修改配置(发送模式,收发缓冲区)
//3. 使能对应串口DMA功能和中断
//4. 使能对应串口全局中断并在it文件中添加对应串口的空闲中断处理(HAL库未提供空闲处理)
//5. 可配置添加DIR引脚控制语句(见UART_Dir_Control函数)

//Oneself
#include "uart_mixed.h"

//Application_User_Core
#include "usart.h"

/**
  * @brief  混合串口实例定义
*/
#ifdef Enabled_MUart1
	UartMixed_TypeDef	muart1;
#endif
#ifdef Enabled_MUart2
	UartMixed_TypeDef	muart2;
#endif
#ifdef Enabled_MUart3
	UartMixed_TypeDef	muart3;
#endif
#ifdef Enabled_MUart4
	UartMixed_TypeDef	muart4;
#endif
#ifdef Enabled_MUart5
	UartMixed_TypeDef	muart5;
#endif
#ifdef Enabled_MUart6
	UartMixed_TypeDef	muart6;
#endif

/*********************************************************************/
/****************************    初始化   ****************************/
/*********************************************************************/
/**
  * @brief  混合串口使能初始化
  * @param  muart: 混合串口指针
  * @param  xerc:  接收完成回调函数
  * @param  xetc:  发送完成回调函数
  * @retval NULL 
**/
void Uart_Mixed_Init(UartMixed_TypeDef *muart, void(*xerc)(char*, uint16_t), void(*xetc)(void))
{
	//实例
#ifdef Enabled_MUart1
	if(muart == &muart1){	muart1.puart = &huart1;		muart1.pdma_tx = &hdma_usart1_tx;	muart1.pdma_rx = &hdma_usart1_rx;	}
#endif
#ifdef Enabled_MUart2
	if(muart == &muart2){	muart2.puart = &huart2;		muart2.pdma_tx = &hdma_usart2_tx;	muart2.pdma_rx = &hdma_usart2_rx;	}
#endif		
#ifdef Enabled_MUart3
	if(muart == &muart3){	muart3.puart = &huart3;		muart3.pdma_tx = &hdma_usart3_tx;	muart3.pdma_rx = &hdma_usart3_rx;	}
#endif		
#ifdef Enabled_MUart4
	if(muart == &muart4){	muart4.puart = &huart4;		muart4.pdma_tx = &hdma_uart4_tx;	muart4.pdma_rx = &hdma_uart4_rx;	}
#endif		
#ifdef Enabled_MUart5
	if(muart == &muart5){	muart5.puart = &huart5;		muart5.pdma_tx = &hdma_uart5_tx;	muart5.pdma_rx = &hdma_uart5_rx;	}
#endif	
#ifdef Enabled_MUart6
	if(muart == &muart6){	muart6.puart = &huart6;		muart6.pdma_tx = &hdma_usart6_tx;	muart6.pdma_rx = &hdma_usart6_rx;	}
#endif

	//统计
	muart->len_tx = 0;									//发字节数量
	muart->len_rx = 0;									//收字节数量
	muart->count_tx = 0;								//发字节数量统计
	muart->count_rx = 0;								//收字节数量统计
	//接收回调函数指针
	muart->Xfer_External_RxCallback = xerc;
	muart->Xfer_External_TxCallback = xetc;

	//开始传输
	Uart_Mixed_Begin(muart);
}

/**
  * @brief  混合串口停止
  * @param  muart: 混合串口指针
  * @retval NULL 
**/
void Uart_Mixed_Stop(UartMixed_TypeDef *muart)
{
	__HAL_UART_DISABLE_IT(muart->puart, UART_IT_IDLE);	//禁用UART空闲中断
	HAL_UART_DMAStop(muart->puart);						//停止DMA传输
	//HAL_UART_Abort(muart->puart);						//中止正在进行的传输
}

/**
  * @brief  混合串口开始
  * @param  muart: 混合串口指针
  * @retval NULL 
**/
void Uart_Mixed_Begin(UartMixed_TypeDef *muart)
{
	if(muart->Xfer_External_RxCallback != NULL)
	{
		if(HAL_UART_Receive_DMA(muart->puart, (uint8_t*)muart->buff_rx, UART_BuffSize) != HAL_OK)	//开启DMA接收
			Error_Handler();
		__HAL_UART_ENABLE_IT(muart->puart, UART_IT_IDLE);	//使能UART空闲中断
	}
	UART_Dir_Control(muart, false);
}

/*********************************************************************/
/***************************   混合库功能函数   ***********************/
/*********************************************************************/
/**
  * @brief  由UART_HandleTypeDef(HAL)实例寻找对应的UartMixed_TypeDef(Mixed)实例
  * @call   <- HAL_UART_RxCpltCallback(HAL)
  * @call   <- HAL_UART_TxCpltCallback(HAL)
  * @param  huart: 串口指针
  * @retval 混合串口实例
**/
UartMixed_TypeDef *Seek_UART_Mixed(UART_HandleTypeDef *huart)
{
	//寻找混合结构
	if(0)
		return NULL;
#ifdef Enabled_MUart1
	else if(muart1.puart == huart)
		return &muart1;
#endif
#ifdef Enabled_MUart2
	else if(muart2.puart == huart)
		return &muart2;
#endif
#ifdef Enabled_MUart3
	else if(muart3.puart == huart)
		return &muart3;
#endif
#ifdef Enabled_MUart4
	else if(muart4.puart == huart)
		return &muart4;
#endif
#ifdef Enabled_MUart5
	else if(muart5.puart == huart)
		return &muart5;
#endif
#ifdef Enabled_MUart6
	else if(muart6.puart == huart)
		return &muart6;
#endif
	else
		return NULL;
}

/**
  * @brief  总线方向控制
  * @call   <- HAL_UART_TxCpltCallback(HAL)
  * @call   <- UART_Mixed_TxTrigger(Mixed)
  * @param  muart: 混合串口指针
  * @param  dir: 方向 (true:发送 false:接收)
  * @retval NULL 
**/
void UART_Dir_Control(UartMixed_TypeDef *muart, bool dir)
{
#if defined(Enabled_MUart1) && defined(UART1_DIR_TX) && defined(UART1_DIR_RX)
	if(muart == &muart1){
		if(dir)		UART1_DIR_TX;
		if(!dir)	UART1_DIR_RX;
	}
#endif
#if defined(Enabled_MUart2) && defined(UART2_DIR_TX) && defined(UART2_DIR_RX)
	if(muart == &muart2){
		if(dir)		UART2_DIR_TX;
		if(!dir)	UART2_DIR_RX;
	}
#endif
#if defined(Enabled_MUart3) && defined(UART3_DIR_TX) && defined(UART3_DIR_RX)
	if(muart == &muart3){
		if(dir)		UART3_DIR_TX;
		if(!dir)	UART3_DIR_RX;
	}
#endif
#if defined(Enabled_MUart4) && defined(UART4_DIR_TX) && defined(UART4_DIR_RX)
	if(muart == &muart4){
		if(dir)		UART4_DIR_TX;
		if(!dir)	UART4_DIR_RX;
	}
#endif
#if defined(Enabled_MUart5) && defined(UART5_DIR_TX) && defined(UART5_DIR_RX)
	if(muart == &muart5){
		if(dir)		UART5_DIR_TX;
		if(!dir)	UART5_DIR_RX;
	}
#endif
#if defined(Enabled_MUart6) && defined(UART6_DIR_TX) && defined(UART6_DIR_RX)
	if(muart == &muart6){
		if(dir)		UART6_DIR_TX;
		if(!dir)	UART6_DIR_RX;
	}
#endif
}

/**
  * @brief  串口混合Tx触发函数
  * @param  mhuart: 混合串口指针
  * @param  pData: 源数组
  * @param  len: 发送数量
  * @return HAL_StatusTypeDef
**/
HAL_StatusTypeDef UART_Mixed_TxTrigger(UartMixed_TypeDef *muart, char *pData, uint16_t len)
{
#if(MUART_MODE == MUART_TX_MODE_POPUP)
	//检查串口是否就绪
	if(muart->puart->gState != HAL_UART_STATE_READY)
		return HAL_BUSY;
#elif(MUART_MODE == MUART_TX_MODE_BLOCKING)
	//等待就绪
	while(muart->puart->gState != HAL_UART_STATE_READY);
#endif
	
	//转移数据到发送缓冲区
	char *pdata;
	if(len > UART_BuffSize)
		pdata = pData;
	else
	{
		memcpy(muart->buff_tx, pData, len);
		pdata = muart->buff_tx;
	}
	
	//停止串口接收
	HAL_UART_AbortReceive(muart->puart);
	
	//开始DMA发送
	HAL_StatusTypeDef hal_status;
	UART_Dir_Control(muart, true);	//设置方向为发送
	hal_status = HAL_UART_Transmit_DMA(muart->puart, (uint8_t*)pdata, len);
	if(hal_status == HAL_OK)
	{
		muart->count_rx += len;
		return hal_status;
	}
	else
	{
		UART_Dir_Control(muart, false);	//发送失败-设置方向为接收
		return hal_status;
	}
}

/*********************************************************************/
/***************************   HAl库回调函数   ***********************/
/*********************************************************************/
/**
  * @brief  HAL库串口Rx传输完成回调函数(源HAL库虚函数)
  * @call   <- USART1_IRQHandler
  * @call   <- UART_DMAReceiveCplt <- HAL_DMA_IRQHandler
  * @param  huart: 串口指针
  * @retval NULL 
**/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UartMixed_TypeDef *muart;
	muart = Seek_UART_Mixed(huart);
	if(!muart)
		return;
	
	//停止串口接收
	HAL_UART_AbortReceive(muart->puart);
	
	//拷贝数据到接收中转区
	muart->len_rx = UART_BuffSize - __HAL_DMA_GET_COUNTER(muart->pdma_rx);	//获得接收到的数据量
	memcpy(muart->buff_rx_transfer, muart->buff_rx, muart->len_rx);					//拷贝
	muart->count_rx += muart->len_rx;

	//开始DMA接收
	HAL_UART_Receive_DMA(muart->puart, (uint8_t*)muart->buff_rx, UART_BuffSize);
	
	if(muart->len_rx > 1)
	{
		//调用外部接收完成回调函数
		if(muart->Xfer_External_RxCallback != NULL)
			muart->Xfer_External_RxCallback((muart->buff_rx_transfer), (muart->len_rx));
	}
}

/**
  * @brief  HAL库串口Tx传输完成回调函数(源HAL库虚函数)
  * @call   <- UART_DMATransmitCplt <- HAL_DMA_IRQHandler
  * @param  huart: 串口指针
  * @retval NULL 
**/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	UartMixed_TypeDef *muart;
	muart = Seek_UART_Mixed(huart);
	if(!muart)
		return;
	
	UART_Dir_Control(muart, false);	//设置方向为接收
	
	//开始DMA接收
	HAL_UART_Receive_DMA(muart->puart, (uint8_t*)muart->buff_rx, UART_BuffSize);
	
	//调用外部发送完成回调函数
	if(muart->Xfer_External_TxCallback != NULL)
		muart->Xfer_External_TxCallback();
}
