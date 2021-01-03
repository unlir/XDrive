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
#include "adc.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
//ADC_HandleTypeDef hadc2;
//DMA_HandleTypeDef hdma_adc2;

uint16_t whole_adc_data[2][12];

/**
  * @brief  REIN_ADC综合初始化
  * @param  NULL
  * @retval NULL
**/
void REIN_ADC_Init(void)
{
#if(XDrive_Run == XDrive_REIN_Basic_H1_0)
	
	#error "undefined"

#elif (XDrive_Run == XDrive_REIN_Basic_H1_1)
	
  /* AFIO初始化 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* DMA初始化 */
	/* ADC1 Init */
	hdma_adc1.Instance = DMA1_Channel1;														//DMA通道1
	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;							//外设到内存
	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;									//外设地址(保持)
	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;											//内存地址(递增)
	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;	//外设宽度(半字)
	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;		//内存宽度(半字)
	hdma_adc1.Init.Mode = DMA_CIRCULAR;														//模式(循环)
	hdma_adc1.Init.Priority = DMA_PRIORITY_VERY_HIGH;							//传输优先级(最高)
	if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
	
  /* ADC初始化 */
  ADC_ChannelConfTypeDef sConfig = {0};
	//ADC初始化
	__HAL_RCC_ADC1_CLK_ENABLE();
  hadc1.Instance = ADC1;																	//ADC1
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;							//扫描模式
  hadc1.Init.ContinuousConvMode = ENABLE;									//连续转换(启用)
  hadc1.Init.DiscontinuousConvMode = DISABLE;							//失能连续转换(禁用)
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;				//触发转换(软件触发)
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;							//数据对齐(右对齐)
  hadc1.Init.NbrOfConversion = 2;													//转换次数(2)
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  //配置列队1
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  //配置列队2
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
	HAL_ADCEx_Calibration_Start(&hadc1);
	
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&whole_adc_data[0][0], 2);
	
#else

	#error "undefined"
	
#endif
}


