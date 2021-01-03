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
#include "spi.h"

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

/********** MT6816 **********/
/********** MT6816 **********/
/********** MT6816 **********/
/**
	* @brief  SPI初始化(MT6816)
	* @param  NULL
	* @retval NULL
**/
void REIN_SPI_MT6816_SPI_Init(void)
{
	/* AFIO初始化 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*GPIO Ports Clock Enable*/
	MT6816_SPI_CLK_GPIO_CLK_ENABLE();		//启用CLK端口时钟
	MT6816_SPI_MOSI_GPIO_CLK_ENABLE();	//启用MOSI端口时钟
	MT6816_SPI_MISO_GPIO_CLK_ENABLE();	//启用MISO端口时钟
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = MT6816_SPI_CLK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;					//复用推挽
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;		//高速
	HAL_GPIO_Init(MT6816_SPI_CLK_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = MT6816_SPI_MOSI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;					//复用推挽
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;		//高速
	HAL_GPIO_Init(MT6816_SPI_MOSI_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = MT6816_SPI_MISO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;					//输入
	GPIO_InitStruct.Pull = GPIO_NOPULL;							//禁用上下拉
	HAL_GPIO_Init(MT6816_SPI_MISO_GPIO_Port, &GPIO_InitStruct);
	/*Configure AFIO*/
#ifdef MT6816_SPI_AFIO_REMAP
	MT6816_SPI_AFIO_REMAP				//启用备用的SPI_AFIO映射
#endif
	
	/* SPI初始化 */
	MT6816_SPI_SPI_CLK_ENABLE();          //启用SPI时钟  
	MT6816_SPI_Get_HSPI.Instance = MT6816_SPI_Get_SPI;
	MT6816_SPI_Get_HSPI.Init.Mode = SPI_MODE_MASTER;												//主机模式
	MT6816_SPI_Get_HSPI.Init.Direction = SPI_DIRECTION_2LINES;							//双向传输
	MT6816_SPI_Get_HSPI.Init.DataSize = SPI_DATASIZE_16BIT;									//数据位宽:16
	MT6816_SPI_Get_HSPI.Init.CLKPolarity = SPI_POLARITY_HIGH;								//CLK空闲时高电平
	MT6816_SPI_Get_HSPI.Init.CLKPhase = SPI_PHASE_2EDGE;										//第二边沿采样
	MT6816_SPI_Get_HSPI.Init.NSS = SPI_NSS_SOFT;														//软件NSS
	MT6816_SPI_Get_HSPI.Init.BaudRatePrescaler = MT6816_SPI_Prescaler;			//输入预设分频
	MT6816_SPI_Get_HSPI.Init.FirstBit = SPI_FIRSTBIT_MSB;										//MSB传输
	MT6816_SPI_Get_HSPI.Init.TIMode = SPI_TIMODE_DISABLE;										//禁用中断模式
	MT6816_SPI_Get_HSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;		//禁用CRC校验
	MT6816_SPI_Get_HSPI.Init.CRCPolynomial = 10;														//CRC校验多项式
	if (HAL_SPI_Init(&MT6816_SPI_Get_HSPI) != HAL_OK)
	{
		Error_Handler();
	}
}

/********** OLED **********/
/********** OLED **********/
/********** OLED **********/
/**
	* @brief  SPI初始化(OLED)
	* @param  NULL
	* @retval NULL
**/
void REIN_SPI_OLED_SPI_Init(void)
{
	/* AFIO初始化 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*GPIO Ports Clock Enable*/
	OLED_SPI_SCK_GPIO_CLK_ENABLE();								//启用SCK端口时钟
	OLED_SPI_MOSI_GPIO_CLK_ENABLE();							//启用MOSI端口时钟
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = OLED_SPI_SCK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;				//复用输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	//高速
	HAL_GPIO_Init(OLED_SPI_SCK_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = OLED_SPI_MOSI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;				//复用输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	//高速
	HAL_GPIO_Init(OLED_SPI_MOSI_GPIO_Port, &GPIO_InitStruct);
	/*Configure AFIO*/
#ifdef OLED_SPI_AFIO_REMAP
	OLED_SPI_AFIO_REMAP				//启用备用的SPI_AFIO映射
#endif

	/* SPI初始化 */
	OLED_SPI_CLK_ENABLE();						//启用SPI时钟  
	OLED_SPI_Get_HSPI.Instance = OLED_SPI_Get_SPI;
	OLED_SPI_Get_HSPI.Init.Mode = SPI_MODE_MASTER;												//主机模式
	OLED_SPI_Get_HSPI.Init.Direction = SPI_DIRECTION_2LINES;							//双向传输
	OLED_SPI_Get_HSPI.Init.DataSize = SPI_DATASIZE_8BIT;									//数据位宽:8
	OLED_SPI_Get_HSPI.Init.CLKPolarity = SPI_POLARITY_LOW;								//CLK空闲低电平
	OLED_SPI_Get_HSPI.Init.CLKPhase = SPI_PHASE_1EDGE;										//第一边沿采样
	OLED_SPI_Get_HSPI.Init.NSS = SPI_NSS_SOFT;														//软件NSS
	OLED_SPI_Get_HSPI.Init.BaudRatePrescaler = OLED_SPI_Prescaler;				//输入预设分频
	OLED_SPI_Get_HSPI.Init.FirstBit = SPI_FIRSTBIT_MSB;										//MSB传输
	OLED_SPI_Get_HSPI.Init.TIMode = SPI_TIMODE_DISABLE;										//禁用中断模式
	OLED_SPI_Get_HSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;		//禁用CRC校验
	OLED_SPI_Get_HSPI.Init.CRCPolynomial = 10;														//CRC校验多项式
	if (HAL_SPI_Init(&OLED_SPI_Get_HSPI) != HAL_OK)
	{
		Error_Handler();
	}
}
