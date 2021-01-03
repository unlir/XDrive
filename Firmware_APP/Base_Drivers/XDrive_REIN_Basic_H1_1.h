/******
	************************************************************************
	******
	** @project : XDrive_Step
	** @brief   : 具有多功能接口和闭环功能的步进电机
	** @author  : unlir (知不知啊)
	** @contacts: QQ.1354077136
	******
	** @address : https://github.com/unlir/XDrive
	******
	** @issuer  : REIN ( 知驭 实验室) (QQ: 857046846)             (discuss)
	** @issuer  : IVES (艾维斯实验室) (QQ: 557214000)             (discuss)
	** @issuer  : X_Drive_Develop     (QQ: Contact Administrator) (develop)
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
  ** @file     : XDrive_REIN_Basic_H1_0.c/h
  ** @brief    : XDrive_REIN_Basic_H1_0
  ** @versions : newest
  ** @time     : newest
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef XDRIVE_REIN_BASIC_H1_0_H
#define XDRIVE_REIN_BASIC_H1_0_H

#ifdef __cplusplus
extern "C" {
#endif

/********** A **********/
//ADC
#define ADC_POWER_U_GROUP								(0)		//ADC1
#define ADC_POWER_U_CHANNEL							(0)				//第1组
#define ADC_POWER_I_GROUP								(0)		//ADC1
#define ADC_POWER_I_CHANNEL							(1)				//第2组

/********** B **********/
/********** BUTTON **********/
/********** BUTTON **********/
/********** BUTTON **********/
//(GPIO)
#define BUTTON_UP_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOB_CLK_ENABLE();		//PB12
#define BUTTON_UP_GPIO_Port							(GPIOB)
#define BUTTON_UP_Pin 									(GPIO_PIN_12)
#define BUTTON_DOWN_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE();		//PB2
#define BUTTON_DOWN_GPIO_Port						(GPIOB)
#define BUTTON_DOWN_Pin 								(GPIO_PIN_2)

/********** C **********/

/********** D **********/

/********** E **********/

/********** F **********/

/********** G **********/

/********** H **********/
/********** HW_ELEC **********/
/********** HW_ELEC **********/
/********** HW_ELEC **********/
//(GPIO)
#define HW_ELEC_AP_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()		//PA5
#define HW_ELEC_AP_GPIO_Port						(GPIOA)
#define HW_ELEC_AP_Pin									(GPIO_PIN_5)
#define HW_ELEC_AM_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()		//PA4
#define HW_ELEC_AM_GPIO_Port						(GPIOA)
#define HW_ELEC_AM_Pin									(GPIO_PIN_4)
#define HW_ELEC_BP_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()		//PA3
#define HW_ELEC_BP_GPIO_Port						(GPIOA)
#define HW_ELEC_BP_Pin									(GPIO_PIN_3)
#define HW_ELEC_BM_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()		//PA2
#define HW_ELEC_BM_GPIO_Port						(GPIOA)
#define HW_ELEC_BM_Pin									(GPIO_PIN_2)
//(AFIO & TIM)
#define HW_ELEC_APWM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()		//PB11
#define HW_ELEC_APWM_GPIO_Port					(GPIOB)
#define HW_ELEC_APWM_Pin								(GPIO_PIN_11)
#define HW_ELEC_APWM_CHANNEL						(TIM_CHANNEL_4)
#define HW_ELEC_BPWM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()		//PB10
#define HW_ELEC_BPWM_GPIO_Port					(GPIOB)
#define HW_ELEC_BPWM_Pin								(GPIO_PIN_10)
#define HW_ELEC_BPWM_CHANNEL						(TIM_CHANNEL_3)
#define HW_ELEC_PWM_TIM_CLK_ENABLE()		__HAL_RCC_TIM2_CLK_ENABLE()			//TIM2
#define	HW_ELEC_PWM_Get_TIM							(TIM2)
#define	HW_ELEC_PWM_Get_HTIM						(htim2)
#define	HwElec_TIM_AFIO_REMAP						__HAL_AFIO_REMAP_TIM2_PARTIAL_2();	//TIM_AFIO端口重新映射

/********** I **********/

/********** J **********/

/********** K **********/

/********** L **********/

/********** M **********/
/********** MT6816 **********/
/********** MT6816 **********/
/********** MT6816 **********/
//MT6816_SPI采集(GPIO)
#define MT6816_SPI_CS_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()	//PA15
#define MT6816_SPI_CS_GPIO_Port						(GPIOA)
#define MT6816_SPI_CS_Pin 								(GPIO_PIN_15)
//MT6816_SPI采集(AFIO & SPI)
#define MT6816_SPI_CLK_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()	//PB3
#define MT6816_SPI_CLK_GPIO_Port					(GPIOB)
#define MT6816_SPI_CLK_Pin 								(GPIO_PIN_3)
#define MT6816_SPI_MISO_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()	//PB4
#define MT6816_SPI_MISO_GPIO_Port					(GPIOB)
#define MT6816_SPI_MISO_Pin 							(GPIO_PIN_4)
#define MT6816_SPI_MOSI_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()	//PB5
#define MT6816_SPI_MOSI_GPIO_Port					(GPIOB)
#define MT6816_SPI_MOSI_Pin 							(GPIO_PIN_5)
#define MT6816_SPI_SPI_CLK_ENABLE()				__HAL_RCC_SPI1_CLK_ENABLE();	//SPI1
#define MT6816_SPI_Get_SPI								(SPI1)
#define MT6816_SPI_Get_HSPI								(hspi1)
#define	MT6816_SPI_AFIO_REMAP							__HAL_AFIO_REMAP_SPI1_ENABLE();	//SPI1_AFIO端口重新映射
#define	MT6816_SPI_Prescaler							(SPI_BAUDRATEPRESCALER_8)				//SPI1_8分频_9M

/********** Modbus **********/
/********** Modbus **********/
/********** Modbus **********/
//Modbus端口(GPIO)
#define Modbus_RS485DIR_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()	//PC14
#define Modbus_RS485DIR_GPIO_Port					(GPIOC)
#define Modbus_RS485DIR_GPIO_Pin 					(GPIO_PIN_14)
#define UART1_DIR_TX											(Modbus_RS485DIR_GPIO_Port -> BSRR = Modbus_RS485DIR_GPIO_Pin)
#define UART1_DIR_RX											(Modbus_RS485DIR_GPIO_Port ->  BRR = Modbus_RS485DIR_GPIO_Pin)
//Modbus端口(AFIO & UART)
#define Modbus_UART_TX_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()	//PB6
#define Modbus_UART_TX_GPIO_Port					(GPIOB)
#define Modbus_UART_TX_GPIO_Pin 					(GPIO_PIN_6)
#define Modbus_UART_RX_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()	//PB7
#define Modbus_UART_RX_GPIO_Port					(GPIOB)
#define Modbus_UART_RX_GPIO_Pin 					(GPIO_PIN_7)
#define Modbus_UART_Get_UART							(USART1)
#define Modbus_UART_Get_HUART							(huart1)
#define Modbus_UART_AFIO_REMAP						__HAL_AFIO_REMAP_USART1_ENABLE();	//UART1_AFIO端口重新映射
#define Modbus_UART_IRQn									(USART1_IRQn)						//串口1全局中断
//Modbus端口(REIN库)
#define Enabled_MUart1										//使能混合串口功能组
#define Enabled_Cus_Slave1								//使能Cus_Modbus从机功能组
#define Enabled_Cus_Master1								//使能Cus_Modbus主机功能组
#define Modbus_UART_Get_MUART							(muart1)			//Rein库实例
#define Modbus_UART_Get_Slave							(cus_slave1)	//Cus库实例
#define Modbus_UART_Get_Master 						(cus_master1)	//Cus库实例
//Modbus端口(DMA)
#define Modbus_UART_TX_DMA_CH							(DMA1_Channel4)
#define Modbus_UART_TX_HDMA								(hdma_usart1_tx)
#define Modbus_UART_TX_DMA_IRQn						(DMA1_Channel4_IRQn)		//DMA1_CH4中断向量
#define Modbus_UART_RX_DMA_CH							(DMA1_Channel5)
#define Modbus_UART_RX_HDMA								(hdma_usart1_rx)
#define Modbus_UART_RX_DMA_IRQn						(DMA1_Channel5_IRQn)		//DMA1_CH5中断向量


/********** N **********/

/********** O **********/
/********** OLED **********/
/********** OLED **********/
/********** OLED **********/
//(GPIO)
#define OLED_SPI_CS_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();		//PA10
#define OLED_SPI_CS_GPIO_Port						(GPIOA)
#define OLED_SPI_CS_Pin 								(GPIO_PIN_10)
#define OLED_SPI_RES_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE();		//PA9
#define OLED_SPI_RES_GPIO_Port					(GPIOA)
#define OLED_SPI_RES_Pin 								(GPIO_PIN_9)
#define OLED_SPI_DC_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();		//PA8
#define OLED_SPI_DC_GPIO_Port						(GPIOA)
#define OLED_SPI_DC_Pin 								(GPIO_PIN_8)
//(AFIO & SPI)
#define OLED_SPI_SCK_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE();		//PB13
#define OLED_SPI_SCK_GPIO_Port					(GPIOB)
#define OLED_SPI_SCK_Pin 								(GPIO_PIN_13)
#define OLED_SPI_MOSI_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE();		//PB15
#define OLED_SPI_MOSI_GPIO_Port					(GPIOB)
#define OLED_SPI_MOSI_Pin 							(GPIO_PIN_15)
#define OLED_SPI_CLK_ENABLE()						__HAL_RCC_SPI2_CLK_ENABLE();		//SPI2
#define OLED_SPI_Get_SPI								(SPI2)
#define OLED_SPI_Get_HSPI								(hspi2)
//#define	OLED_SPI_AFIO_REMAP							__HAL_AFIO_REMAP_SPI2_ENABLE();	//SPI2_AFIO端口重新映射
#define	OLED_SPI_Prescaler							(SPI_BAUDRATEPRESCALER_4)					//SPI2_4分频_9M
//(Interface)
#define OLED_Orien_Reversal							(true)			//OLED显示方向翻转

/********** P **********/

/********** Q **********/

/********** R **********/

/********** S **********/
/********** SIGNAL_PORT **********/
/********** SIGNAL_PORT **********/
/********** SIGNAL_PORT **********/
//SIGNAL_COUNT(GPIO)
#define SIGNAL_COUNT_ENA_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()	//PB0
#define SIGNAL_COUNT_ENA_GPIO_Port			(GPIOB)
#define SIGNAL_COUNT_ENA_Pin						(GPIO_PIN_0)
#define SIGNAL_COUNT_DIR_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()	//PA7
#define SIGNAL_COUNT_DIR_GPIO_Port			(GPIOA)
#define SIGNAL_COUNT_DIR_Pin						(GPIO_PIN_7)
#define SIGNAL_COUNT_DIR_Get_IRQn				(EXTI9_5_IRQn)	//EXTI7中断
//SIGNAL_COUNT(AFIO & TIM)
#define SIGNAL_COUNT_PUL_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()	//PA6
#define SIGNAL_COUNT_PUL_GPIO_Port			(GPIOA)
#define SIGNAL_COUNT_PUL_Pin						(GPIO_PIN_6)
#define SIGNAL_COUNT_TIM_CLK_ENABLE()		__HAL_RCC_TIM3_CLK_ENABLE()		//TIM3
#define	SIGNAL_COUNT_Get_TIM						(TIM3)
#define	SIGNAL_COUNT_Get_HTIM						(htim3)
//SIGNAL_PWM(GPIO)
#define SIGNAL_PWM_ENA_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()	//PB0
#define SIGNAL_PWM_ENA_GPIO_Port				(GPIOB)
#define SIGNAL_PWM_ENA_Pin							(GPIO_PIN_0)
#define SIGNAL_PWM_DIR_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()	//PA7
#define SIGNAL_PWM_DIR_GPIO_Port				(GPIOA)
#define SIGNAL_PWM_DIR_Pin							(GPIO_PIN_7)
//SIGNAL_PWM(AFIO & TIM)
#define SIGNAL_PWM_PUL_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()	//PA6
#define SIGNAL_PWM_PUL_GPIO_Port				(GPIOA)
#define SIGNAL_PWM_PUL_Pin							(GPIO_PIN_6)
#define SIGNAL_PWM_TIM_CLK_ENABLE()			__HAL_RCC_TIM3_CLK_ENABLE()		//TIM3
#define	SIGNAL_PWM_Get_TIM							(TIM3)
#define	SIGNAL_PWM_Get_HTIM							(htim3)
#define SIGNAL_PWM_Get_IRQn							(TIM3_IRQn)		//TIM3中断

/********** T **********/

/********** U **********/

/********** V **********/

/********** W **********/

/********** X **********/

/********** Y **********/

/********** Z **********/

#ifdef __cplusplus
}
#endif

#endif
