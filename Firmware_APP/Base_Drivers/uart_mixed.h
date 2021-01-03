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


#ifndef UART_MIXED_H
#define UART_MIXED_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

/***************************************** Config_Start ******************************************/
//发送模式
#define	MUART_TX_MODE_POPUP			0	//弹出模式: 遇到串口非就绪->直接放弃发送
#define	MUART_TX_MODE_BLOCKING	1	//阻塞模式: 遇到串口非就绪->等待串口就绪
#define MUART_MODE	 						MUART_TX_MODE_POPUP

//收发缓冲区
#define UART_BuffSize	256		//收发缓冲区容量
/***************************************** Config_End ******************************************/

/** 
  * UART混合功能结构体定义
**/
typedef struct
{
	//实例
	UART_HandleTypeDef	*puart;		//串口实例(HAL库定义的结构体)
	DMA_HandleTypeDef	*pdma_tx;	//DMA发送通道实例(HAL库定义的结构体)
	DMA_HandleTypeDef	*pdma_rx;	//DMA接收通道实例(HAL库定义的结构体)
	//缓冲区
	char		buff_tx[UART_BuffSize];				//发送缓冲区
	char		buff_rx[UART_BuffSize];				//接收缓冲区
	char		buff_rx_transfer[UART_BuffSize];	//接收中转缓冲区
	//统计
	uint16_t 	len_tx;				//收发数据量
	uint16_t	len_rx;
	uint32_t	count_tx;			//收发字节数量统计
	uint32_t	count_rx;
	//回调函数指针
	void		(*Xfer_External_RxCallback)(char *data, uint16_t len);	//接收完成回调函数指针
	void		(*Xfer_External_TxCallback)(void);						//发送完成回调函数指针
}UartMixed_TypeDef;

/******************************************声明*******************************************/
//声明引用
#ifdef Enabled_MUart1
extern UartMixed_TypeDef	muart1;
#endif
#ifdef Enabled_MUart2
extern UartMixed_TypeDef	muart2;
#endif
#ifdef Enabled_MUart3
extern UartMixed_TypeDef	muart3;
#endif
#ifdef Enabled_MUart4
extern UartMixed_TypeDef	muart4;
#endif
#ifdef Enabled_MUart5
extern UartMixed_TypeDef	muart5;
#endif
#ifdef Enabled_MUart6
extern UartMixed_TypeDef	muart6;
#endif

//接口函数
void Uart_Mixed_Init(UartMixed_TypeDef *muart, void(*xerc)(char*, uint16_t), void(*xetc)(void));	//混合串口初始化
void Uart_Mixed_Stop(UartMixed_TypeDef *muart);														//混合串口停止
void Uart_Mixed_Begin(UartMixed_TypeDef *muart);													//混合串口开始
//混合库功能函数
UartMixed_TypeDef *Seek_UART_Mixed(UART_HandleTypeDef *huart);	//由UART_HandleTypeDef实例(HAL库)寻找对应的UartMixed_TypeDef实例(Mixed库)
void UART_Dir_Control(UartMixed_TypeDef *muart, bool dir);		//总线方向控制
HAL_StatusTypeDef UART_Mixed_TxTrigger(UartMixed_TypeDef *muart, char *pData, uint16_t Size);	//串口混合Tx触发函数
//HAL库回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);		//HAL库串口Rx传输完成回调函数(源HAL库虚函数)
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);		//HAL库串口Tx传输完成回调函数(源HAL库虚函数)

#ifdef __cplusplus
}
#endif

#endif

