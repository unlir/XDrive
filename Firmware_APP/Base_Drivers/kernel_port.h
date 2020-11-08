/******
	************************************************************************
	******
	** @versions : 1.1.4
	** @time     : 2020/09/15
	******
	************************************************************************
	******
	** @project : XDrive_Step
	** @brief   : 具有多功能接口和闭环功能的步进电机
	** @author  : unlir (知不知啊)
	******
	** @address : https://github.com/unlir/XDrive
	******
	** @issuer  : IVES ( 艾维斯 实验室) (QQ: 557214000)   (master)
	** @issuer  : REIN (  知驭  实验室) (QQ: 857046846)   (master)
	******
	************************************************************************
	******
	** {Stepper motor with multi-function interface and closed loop function.}
	** Copyright (c) {2020}  {unlir}
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
  ** @file     : kernel_port.c/h
  ** @brief    : 端口定义
  ** @versions : newest
  ** @time     : newest
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

#ifndef KERNEL_PORT_H
#define KERNEL_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

//C基本库
#include <stdint.h>		//基本数据类型
#include <stdbool.h>	//Bool数据类型
#include <string.h>		//变量类型,宏,字符数组函数
#include <stdlib.h>		//变量类型,宏,通用工具函数
#include <stdio.h>		//变量类型,宏,输入输出函数

//HAL库引用
#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "tim.h"

//HAL库函数引用
#define MEIN_Delay_MS(ms)		HAL_Delay(ms)

/********** A **********/

/********** B **********/
/********** BUTTON **********/
/********** BUTTON **********/
/********** BUTTON **********/
//(GPIO)
#define BUTTON_OK_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE();		//PA2
#define BUTTON_OK_GPIO_Port							(GPIOA)
#define BUTTON_OK_Pin 									(GPIO_PIN_2)
#define BUTTON_UP_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE();		//PA3
#define BUTTON_UP_GPIO_Port							(GPIOA)
#define BUTTON_UP_Pin 									(GPIO_PIN_3)
#define BUTTON_DOWN_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE();		//PC13
#define BUTTON_DOWN_GPIO_Port						(GPIOC)
#define BUTTON_DOWN_Pin 								(GPIO_PIN_13)

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
#define HW_ELEC_AP_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()		//PB1
#define HW_ELEC_AP_GPIO_Port						(GPIOB)
#define HW_ELEC_AP_Pin									(GPIO_PIN_1)
#define HW_ELEC_AM_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()		//PB0
#define HW_ELEC_AM_GPIO_Port						(GPIOB)
#define HW_ELEC_AM_Pin									(GPIO_PIN_0)
#define HW_ELEC_BP_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE()		//PC14
#define HW_ELEC_BP_GPIO_Port						(GPIOC)
#define HW_ELEC_BP_Pin									(GPIO_PIN_14)
#define HW_ELEC_BM_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE()		//PC15
#define HW_ELEC_BM_GPIO_Port						(GPIOC)
#define HW_ELEC_BM_Pin									(GPIO_PIN_15)
//(AFIO & TIM)
#define HW_ELEC_APWM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()		//PB6
#define HW_ELEC_APWM_GPIO_Port					(GPIOB)
#define HW_ELEC_APWM_Pin								(GPIO_PIN_6)
#define HW_ELEC_APWM_CHANNEL						(TIM_CHANNEL_1)
#define HW_ELEC_BPWM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()		//PB7
#define HW_ELEC_BPWM_GPIO_Port					(GPIOB)
#define HW_ELEC_BPWM_Pin								(GPIO_PIN_7)
#define HW_ELEC_BPWM_CHANNEL						(TIM_CHANNEL_2)
#define HW_ELEC_PWM_TIM_CLK_ENABLE()		__HAL_RCC_TIM4_CLK_ENABLE()			//TIM4
#define	HW_ELEC_PWM_Get_TIM							(TIM4)
#define	HW_ELEC_PWM_Get_HTIM						(htim4)

/********** I **********/

/********** J **********/

/********** K **********/

/********** L **********/

/********** M **********/
/********** MT6816 **********/
/********** MT6816 **********/
/********** MT6816 **********/
//MT6816模式控制(GPIO)
#define SENSOR_HVPP_CLK_ENABLE()				__HAL_RCC_GPIOA_CLK_ENABLE()	//PA7
#define SENSOR_HVPP_GPIO_Port						(GPIOA)
#define SENSOR_HVPP_Pin									(GPIO_PIN_7)
//MT6816_PWM采集(AFIO & TIM)
#define MT6816_PWM_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()	//PA6
#define MT6816_PWM_GPIO_Port						(GPIOA)
#define MT6816_PWM_Pin 									(GPIO_PIN_6)
#define MT6816_PWM_TIM_CLK_ENABLE()			__HAL_RCC_TIM3_CLK_ENABLE()		//TIM3
#define	MT6816_PWM_Get_TIM							(TIM3)
#define	MT6816_PWM_Get_HTIM							(htim3)
#define MT6816_PWM_Get_IRQn							(TIM3_IRQn)		//TIM3中断
//MT6816_ABZ采集(GPIO)
#define MT6816_ABZ_Z_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()	//PA10
#define MT6816_ABZ_Z_GPIO_Port					(GPIOA)
#define MT6816_ABZ_Z_Pin 								(GPIO_PIN_10)
#define MT6816_ABZ_Z_EXTI_IRQn 					(EXTI15_10_IRQn)	//EXTI15_10中断
//MT6816_ABZ采集(AFIO & TIM)
#define MT6816_ABZ_A_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()	//PA8
#define MT6816_ABZ_A_GPIO_Port					(GPIOA)
#define MT6816_ABZ_A_Pin 								(GPIO_PIN_8)
#define MT6816_ABZ_B_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()	//PA9
#define MT6816_ABZ_B_GPIO_Port					(GPIOA)
#define MT6816_ABZ_B_Pin 								(GPIO_PIN_9)
#define MT6816_ABZ_TIM_CLK_ENABLE()			__HAL_RCC_TIM1_CLK_ENABLE()		//TIM1
#define	MT6816_ABZ_Get_TIM							(TIM1)
#define	MT6816_ABZ_Get_HTIM							(htim1)
//MT6816_SPI采集(GPIO)
#define MT6816_SPI_CS_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()	//PB12
#define MT6816_SPI_CS_GPIO_Port						(GPIOB)
#define MT6816_SPI_CS_Pin 								(GPIO_PIN_12)
//MT6816_SPI采集(AFIO & SPI)
#define MT6816_SPI_CLK_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()	//PB13
#define MT6816_SPI_CLK_GPIO_Port					(GPIOB)
#define MT6816_SPI_CLK_Pin 								(GPIO_PIN_13)
#define MT6816_SPI_MISO_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()	//PB14
#define MT6816_SPI_MISO_GPIO_Port					(GPIOB)
#define MT6816_SPI_MISO_Pin 							(GPIO_PIN_14)
#define MT6816_SPI_MOSI_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()	//PB15
#define MT6816_SPI_MOSI_GPIO_Port					(GPIOB)
#define MT6816_SPI_MOSI_Pin 							(GPIO_PIN_15)
#define MT6816_SPI_SPI_CLK_ENABLE()				__HAL_RCC_SPI2_CLK_ENABLE();	//SPI2
#define MT6816_SPI_Get_SPI								(SPI2)
#define MT6816_SPI_Get_HSPI								(hspi2)
//#define	MT6816_SPI_AFIO_REMAP							__HAL_AFIO_REMAP_SPI2_ENABLE();	//SPI2_AFIO端口重新映射

/********** N **********/

/********** O **********/
/********** OLED **********/
/********** OLED **********/
/********** OLED **********/
//(GPIO)
#define OLED_SPI_RES_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE();		//PA15
#define OLED_SPI_RES_GPIO_Port					(GPIOA)
#define OLED_SPI_RES_Pin 								(GPIO_PIN_15)
#define OLED_SPI_DC_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE();		//PB4
#define OLED_SPI_DC_GPIO_Port						(GPIOB)
#define OLED_SPI_DC_Pin 								(GPIO_PIN_4)
//(AFIO & SPI)
#define OLED_SPI_SCK_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE();		//PB3
#define OLED_SPI_SCK_GPIO_Port					(GPIOB)
#define OLED_SPI_SCK_Pin 								(GPIO_PIN_3)
#define OLED_SPI_MOSI_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE();		//PB5
#define OLED_SPI_MOSI_GPIO_Port					(GPIOB)
#define OLED_SPI_MOSI_Pin 							(GPIO_PIN_5)
#define OLED_SPI_CLK_ENABLE()						__HAL_RCC_SPI1_CLK_ENABLE();		//SPI1
#define OLED_SPI_Get_SPI								(SPI1)
#define OLED_SPI_Get_HSPI								(hspi1)
#define	OLED_SPI_AFIO_REMAP							__HAL_AFIO_REMAP_SPI1_ENABLE();	//SPI1_AFIO端口重新映射

/********** P **********/

/********** Q **********/

/********** R **********/

/********** S **********/
/********** SIGNAL_PORT **********/
/********** SIGNAL_PORT **********/
/********** SIGNAL_PORT **********/
//SIGNAL_COUNT(GPIO)
#define SIGNAL_COUNT_ENA_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()	//PA4
#define SIGNAL_COUNT_ENA_GPIO_Port			(GPIOA)
#define SIGNAL_COUNT_ENA_Pin						(GPIO_PIN_4)
#define SIGNAL_COUNT_DIR_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()	//PA1
#define SIGNAL_COUNT_DIR_GPIO_Port			(GPIOA)
#define SIGNAL_COUNT_DIR_Pin						(GPIO_PIN_1)
#define SIGNAL_COUNT_DIR_Get_IRQn				(EXTI1_IRQn)	//EXTI1中断
//SIGNAL_COUNT(AFIO & TIM)
#define SIGNAL_COUNT_PUL_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()	//PA0
#define SIGNAL_COUNT_PUL_GPIO_Port			(GPIOA)
#define SIGNAL_COUNT_PUL_Pin						(GPIO_PIN_0)
#define SIGNAL_COUNT_TIM_CLK_ENABLE()		__HAL_RCC_TIM2_CLK_ENABLE()		//TIM2
#define	SIGNAL_COUNT_Get_TIM						(TIM2)
#define	SIGNAL_COUNT_Get_HTIM						(htim2)
#define SIGNAL_COUNT_Get_IRQn						(TIM2_IRQn)		//TIM2中断
//SIGNAL_PWM(GPIO)
#define SIGNAL_PWM_ENA_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()	//PA4
#define SIGNAL_PWM_ENA_GPIO_Port				(GPIOA)
#define SIGNAL_PWM_ENA_Pin							(GPIO_PIN_4)
#define SIGNAL_PWM_DIR_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()	//PA1
#define SIGNAL_PWM_DIR_GPIO_Port				(GPIOA)
#define SIGNAL_PWM_DIR_Pin							(GPIO_PIN_1)
//SIGNAL_PWM(AFIO & TIM)
#define SIGNAL_PWM_PUL_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()	//PA0
#define SIGNAL_PWM_PUL_GPIO_Port				(GPIOA)
#define SIGNAL_PWM_PUL_Pin							(GPIO_PIN_0)
#define SIGNAL_PWM_TIM_CLK_ENABLE()			__HAL_RCC_TIM2_CLK_ENABLE()		//TIM2
#define	SIGNAL_PWM_Get_TIM							(TIM2)
#define	SIGNAL_PWM_Get_HTIM							(htim2)
#define SIGNAL_PWM_Get_IRQn							(TIM2_IRQn)		//TIM2中断

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
