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
#include "gpio.h"

/********** Button **********/
/********** Button **********/
/********** Button **********/
/**
  * @brief  GPIO初始化(Button)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_Button_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
#ifdef BUTTON_OK_GPIO_CLK_ENABLE
  /* GPIO Ports Clock Enable*/
  BUTTON_OK_GPIO_CLK_ENABLE();			//启用OK端口时钟
  /*Configure GPIO pin*/
	GPIO_InitStruct.Pin = BUTTON_OK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		//输入
  GPIO_InitStruct.Pull = GPIO_PULLUP;				//上拉
	HAL_GPIO_Init(BUTTON_OK_GPIO_Port, &GPIO_InitStruct);
#endif
	
#ifdef BUTTON_UP_GPIO_CLK_ENABLE
  /* GPIO Ports Clock Enable*/
  BUTTON_UP_GPIO_CLK_ENABLE();			//启用UP端口时钟
  /*Configure GPIO pin*/
	GPIO_InitStruct.Pin = BUTTON_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		//输入
  GPIO_InitStruct.Pull = GPIO_PULLUP;				//上拉
	HAL_GPIO_Init(BUTTON_UP_GPIO_Port, &GPIO_InitStruct);
#endif
	
#ifdef BUTTON_DOWN_GPIO_CLK_ENABLE
  /* GPIO Ports Clock Enable*/
	BUTTON_DOWN_GPIO_CLK_ENABLE();		//启用DOWN端口时钟
  /*Configure GPIO pin*/
	GPIO_InitStruct.Pin = BUTTON_DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		//输入
  GPIO_InitStruct.Pull = GPIO_PULLUP;				//上拉
	HAL_GPIO_Init(BUTTON_DOWN_GPIO_Port, &GPIO_InitStruct);
#endif
}

/********** HwElec **********/
/********** HwElec **********/
/********** HwElec **********/
/**
  * @brief  GPIO初始化(HwElec)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_HwElec_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable*/
  HW_ELEC_AP_GPIO_CLK_ENABLE();		//启用AP端口时钟
  HW_ELEC_AM_GPIO_CLK_ENABLE();		//启用AM端口时钟
  HW_ELEC_BP_GPIO_CLK_ENABLE();		//启用BP端口时钟
  HW_ELEC_BM_GPIO_CLK_ENABLE();		//启用BM端口时钟
  /*Configure GPIO pin Output Level*/
	HW_ELEC_AP_GPIO_Port -> BRR = HW_ELEC_AP_Pin;		//AP引脚启动输出低电平
	HW_ELEC_AM_GPIO_Port -> BRR = HW_ELEC_AM_Pin;		//AM引脚启动输出低电平
	HW_ELEC_BP_GPIO_Port -> BRR = HW_ELEC_BP_Pin;		//BP引脚启动输出低电平
	HW_ELEC_BM_GPIO_Port -> BRR = HW_ELEC_BM_Pin;		//BM引脚启动输出低电平
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = HW_ELEC_AP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	//高速
	HAL_GPIO_Init(HW_ELEC_AP_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = HW_ELEC_AM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	//高速
	HAL_GPIO_Init(HW_ELEC_AM_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = HW_ELEC_BP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	//高速
	HAL_GPIO_Init(HW_ELEC_BP_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = HW_ELEC_BM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	//高速输出
	HAL_GPIO_Init(HW_ELEC_BM_GPIO_Port, &GPIO_InitStruct);
}

/********** MT6816 **********/
/********** MT6816 **********/
/********** MT6816 **********/
/**
  * @brief  GPIO初始化(MT6816_ABZ)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_MT6816_ABZ_Init(void)
{
#if (defined SENSOR_HVPP_CLK_ENABLE) || (defined MT6816_ABZ_Z_GPIO_CLK_ENABLE)
  GPIO_InitTypeDef GPIO_InitStruct = {0};
#endif

#ifdef SENSOR_HVPP_CLK_ENABLE
  /* GPIO Ports Clock Enable */
	SENSOR_HVPP_CLK_ENABLE();						//启用MT6816_HVPP端口时钟
  /*Configure GPIO pin Output Level*/
	SENSOR_HVPP_GPIO_Port -> BRR = SENSOR_HVPP_Pin;  //MT6816_HVPP引脚输出低电平,启动PWM+ABZ模式
  /*Configure GPIO pins*/
  GPIO_InitStruct.Pin = SENSOR_HVPP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	//低速
  HAL_GPIO_Init(SENSOR_HVPP_GPIO_Port, &GPIO_InitStruct);
#endif

#ifdef MT6816_ABZ_Z_GPIO_CLK_ENABLE
  /* GPIO Ports Clock Enable */
  MT6816_ABZ_Z_GPIO_CLK_ENABLE();			//启用MT6816_ABZ_Z端口时钟
  /*Configure GPIO pins*/
  GPIO_InitStruct.Pin = MT6816_ABZ_Z_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;		//上升沿触发中断
  GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
  HAL_GPIO_Init(MT6816_ABZ_Z_GPIO_Port, &GPIO_InitStruct);
  /* EXTI interrupt init*/
  HAL_NVIC_EnableIRQ(MT6816_ABZ_Z_EXTI_IRQn);		//启用MT6816_Z中断
#endif
}

/********** Modbus **********/
/********** Modbus **********/
/********** Modbus **********/
/**
  * @brief  GPIO初始化(Modbus)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_Modbus_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
#ifdef Modbus_RS485DIR_GPIO_CLK_ENABLE
	/* GPIO Ports Clock Enable */
	Modbus_RS485DIR_GPIO_CLK_ENABLE();		//启用RS485DIR端口时钟
	/*Configure GPIO pin Output Level*/
	Modbus_RS485DIR_GPIO_Port -> BRR = Modbus_RS485DIR_GPIO_Pin;	//DIR引脚启动输出低电平
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = Modbus_RS485DIR_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	//低速
	HAL_GPIO_Init(Modbus_RS485DIR_GPIO_Port, &GPIO_InitStruct);
#endif
}

/**
  * @brief  GPIO初始化(MT6816_SPI)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_MT6816_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
#ifdef SENSOR_HVPP_CLK_ENABLE
	/* GPIO Ports Clock Enable */
	SENSOR_HVPP_CLK_ENABLE();						//启用MT6816_HVPP端口时钟
	/*Configure GPIO pin Output Level*/
	SENSOR_HVPP_GPIO_Port -> BSRR = SENSOR_HVPP_Pin;			//MT6816_HVPP引脚输出高电平,启动PWM+SPI模式
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = SENSOR_HVPP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	//低速
	HAL_GPIO_Init(SENSOR_HVPP_GPIO_Port, &GPIO_InitStruct);
#endif
	
	/* GPIO Ports Clock Enable */
	MT6816_SPI_CS_GPIO_CLK_ENABLE();		//启用MT6816_SPI_CS端口时钟
	/*Configure GPIO pin Output Level*/
	MT6816_SPI_CS_GPIO_Port -> BSRR = MT6816_SPI_CS_Pin;	//CS引脚启动输出高电平
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = MT6816_SPI_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	//低速
	HAL_GPIO_Init(MT6816_SPI_CS_GPIO_Port, &GPIO_InitStruct);
}

/********** OLED **********/
/********** OLED **********/
/********** OLED **********/
/**
  * @brief  GPIO初始化(OLED)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
#ifdef OLED_SPI_CS_GPIO_CLK_ENABLE
	/********** CS **********/
	/*GPIO Ports Clock Enable*/
	OLED_SPI_CS_GPIO_CLK_ENABLE();		//启用CS端口时钟
	/*Configure GPIO pin Output Level*/
	OLED_SPI_CS_GPIO_Port -> BRR = OLED_SPI_CS_Pin;		//CS启动输出低电平
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = OLED_SPI_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	//低速
	HAL_GPIO_Init(OLED_SPI_CS_GPIO_Port, &GPIO_InitStruct);
#endif
	
	/********** RES **********/
	/*GPIO Ports Clock Enable*/
	OLED_SPI_RES_GPIO_CLK_ENABLE();		//启用RES端口时钟
	/*Configure GPIO pin Output Level*/
	OLED_SPI_RES_GPIO_Port -> BRR = OLED_SPI_RES_Pin;		//RES启动输出低电平
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = OLED_SPI_RES_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	//低速
	HAL_GPIO_Init(OLED_SPI_RES_GPIO_Port, &GPIO_InitStruct);
	
	/********** DC **********/
	/*GPIO Ports Clock Enable*/
	OLED_SPI_DC_GPIO_CLK_ENABLE();		//启用DC端口时钟
	/*Configure GPIO pin Output Level*/
	OLED_SPI_DC_GPIO_Port -> BRR = OLED_SPI_DC_Pin;			//DC启动输出低电平
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = OLED_SPI_DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_InitStruct.Pull = GPIO_NOPULL;						//禁用上下拉
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	//低速
	HAL_GPIO_Init(OLED_SPI_DC_GPIO_Port, &GPIO_InitStruct);
}

/********** SIGNAL **********/
/********** SIGNAL **********/
/********** SIGNAL **********/
/**
  * @brief  GPIO初始化(SIGNAL_COUNT)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_SIGNAL_COUNT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
#ifdef SIGNAL_COUNT_ENA_CLK_ENABLE
	/*GPIO Ports Clock Enable*/
	SIGNAL_COUNT_ENA_CLK_ENABLE();		//启用ENA端口时钟
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin =SIGNAL_COUNT_ENA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		//输入模式
	GPIO_InitStruct.Pull = GPIO_NOPULL;				//禁用上下拉
	HAL_GPIO_Init(SIGNAL_COUNT_ENA_GPIO_Port, &GPIO_InitStruct);
#endif
	
#ifdef SIGNAL_COUNT_DIR_CLK_ENABLE
	/*GPIO Ports Clock Enable*/
	SIGNAL_COUNT_DIR_CLK_ENABLE();		//启用DIR端口时钟
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = SIGNAL_COUNT_DIR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;	//双边沿中断
	GPIO_InitStruct.Pull = GPIO_NOPULL;									//禁用上下拉
	HAL_GPIO_Init(SIGNAL_COUNT_DIR_GPIO_Port, &GPIO_InitStruct);
	/*EXTI interrupt init*/
	HAL_NVIC_EnableIRQ(SIGNAL_COUNT_DIR_Get_IRQn);	//使能DIR中断
#endif
}

/**
  * @brief  GPIO清理(SIGNAL_COUNT)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_SIGNAL_COUNT_DeInit(void)
{
	/*EXTI interrupt deinit*/
	HAL_NVIC_DisableIRQ(SIGNAL_COUNT_DIR_Get_IRQn);	//失能DIR中断
	/*Configure GPIO pins */
	HAL_GPIO_DeInit(SIGNAL_COUNT_DIR_GPIO_Port, SIGNAL_COUNT_DIR_Pin);	//重置DIR
	HAL_GPIO_DeInit(SIGNAL_COUNT_ENA_GPIO_Port, SIGNAL_COUNT_ENA_Pin);	//重置ENA
}

/**
  * @brief  GPIO初始化(SIGNAL_PWM)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_SIGNAL_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*GPIO Ports Clock Enable*/
	SIGNAL_PWM_ENA_CLK_ENABLE();		//启用ENA端口时钟
	SIGNAL_PWM_DIR_CLK_ENABLE();		//启用DIR端口时钟
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = SIGNAL_PWM_ENA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		//输入模式
	GPIO_InitStruct.Pull = GPIO_NOPULL;				//禁用上下拉
	HAL_GPIO_Init(SIGNAL_PWM_ENA_GPIO_Port, &GPIO_InitStruct);
	/*Configure GPIO pins*/
	GPIO_InitStruct.Pin = SIGNAL_PWM_DIR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		//输入模式
	GPIO_InitStruct.Pull = GPIO_NOPULL;				//禁用上下拉
	HAL_GPIO_Init(SIGNAL_PWM_DIR_GPIO_Port, &GPIO_InitStruct);
}

/**
  * @brief  GPIO清理(SIGNAL_PWM)
  * @param  NULL
  * @retval NULL
*/
void REIN_GPIO_SIGNAL_PWM_DeInit(void)
{
	/*Configure GPIO pins*/
	HAL_GPIO_DeInit(SIGNAL_PWM_DIR_GPIO_Port, SIGNAL_PWM_DIR_Pin);	//重置DIR
	HAL_GPIO_DeInit(SIGNAL_PWM_ENA_GPIO_Port, SIGNAL_PWM_ENA_Pin);	//重置ENA
}
