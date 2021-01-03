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


#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;

//串口接口默模式
typedef enum{
	Uart_Mode_1SNP = 0x00,	//(1个停止位) (无校验)
	Uart_Mode_1SEP = 0x01,	//(1个停止位) (偶数校验)
	Uart_Mode_1SOP = 0x02,	//(1个停止位) (奇数校验)
	Uart_Mode_2SNP = 0x03,	//(2个停止位) (无校验)
	Uart_Mode_2SEP = 0x04,	//(2个停止位) (偶数校验)
	Uart_Mode_2SOP = 0x05,	//(2个停止位) (奇数校验)
}Uart_Mode;

/**
  * 动态串口结构体
**/
typedef struct{
	//配置(串口模式)
	#define			De_Uart_Mode			Uart_Mode_1SNP	//默认
	bool				valid_uart_mode;									//串口模式配置有效
	Uart_Mode		uart_run;					//运行模式
	Uart_Mode		uart_order;				//模式(存储数据,下次一定)
	//配置(波特率)
	#define		De_Uart_BaudRate	(115200)		//默认
	bool			valid_uart_baudrate;				//串口波特率配置有效
	uint32_t	baud_rate_run;		//波特率
	uint32_t	baud_rate_order;	//波特率(存储数据,下次一定)
}Dynamic_Uart_Typedef;

/********** Modbus **********/
/********** Modbus **********/
/********** Modbus **********/
extern Dynamic_Uart_Typedef dyn_uart1;

void REIN_UART_Modbus_Set_Mode(Uart_Mode value);		//设置Uart模式
void REIN_UART_Modbus_Set_BaudRate(uint32_t value);	//设置Uart波特率
void REIN_UART_Modbus_Set_Default(void);						//串口参数恢复

void REIN_UART_Modbus_Init(void);										//UART_Modbus初始化
void REIN_UART_Modbus_Low_Priority_Callback(void);	//Modbus低优先级回调

#ifdef __cplusplus
}
#endif

#endif
