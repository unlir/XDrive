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
  ** @file     : stockpile_f103cb.c/h
  ** @brief    : Flash存储库
  ** @versions : newest
  ** @time     : newest
  ** @reviser  : unli (WuHu China)
  ** @explain  : null
*****/

#ifndef STOCKPILE_F030K6_H
#define STOCKPILE_F030K6_H

#ifdef __cplusplus
extern "C" {
#endif

//引用端口定义
#include "kernel_port.h"

/*************************************************************** Stockpile_Start ***************************************************************/
/*************************************************************** Stockpile_Start ***************************************************************/
/*************************************************************** Stockpile_Start ***************************************************************/
/*********************STM32F103xx*************************/
//主储存块容量
//Flash Size(bytes)/RAM size(bytes)
// 大容量   1M / 96K                                     RG               VG           ZG
// 大容量 768K / 96K                                     RF               VF           ZF
// 大容量 512K / 64K                                     RE               VE           ZE
// 大容量 384K / 64K                                     RD               VD           ZD
// 大容量 256K / 48K                                     RC               VC           ZC
// 中容量 128K / 20K      TB           CB                RB               VB
// 中容量  64K / 20K      T8           C8                R8               V8
// 小容量  32K / 10K      T6           C6                R6
// 小容量  16K /  6K      T4           C4                R4
//        						 36pin-QFN	48pin-LQFP/QFN	64pin-BGA/CSP/LQFP  100pin-LQFP  144pin-BGA/LQFP  
/*************************************************************** Stockpile_End ***************************************************************/
/*************************************************************** Stockpile_End ***************************************************************/
/*************************************************************** Stockpile_End ***************************************************************/

/*************************************************************** FLASH_Start ***************************************************************/
/*************************************************************** FLASH_Start ***************************************************************/
/*************************************************************** FLASH_Start ***************************************************************/
/******************页配置(更换芯片必须修改这个配置)***********************/
#define Stockpile_Page_Size		0x400U		//扇区大小(默认1024字节)
#if (Stockpile_Page_Size != FLASH_PAGE_SIZE)	//和HAL库获取的Flash页大小比较,检查配置是否有效
	#error "Stockpile_Page_Size Error !!!"		
#endif

/**
* Flash分区表结构体
**/
typedef struct{
	//配置
	uint32_t	begin_add;			//起始地址
	uint32_t	area_size;			//区域大小
	uint32_t	page_num;				//芯片实体页数量
	//过程量
	uint32_t	asce_write_add;	//写地址
}Stockpile_FLASH_Typedef;

/********** Flash分区表 **********/
/***********************************Flash段名称*************起始地址******结束地址****大小******注释****/
//                                      ALL               0x08000000U   0x08020000U   128K    全部空间
//APP
extern Stockpile_FLASH_Typedef stockpile_app_firmware;  //0x08000000U ~ 0x0800F800U    62K    Firmware
//DATA
extern Stockpile_FLASH_Typedef stockpile_data;          //0x0800F800U ~ 0x08010000U     2K    DATA
//QUICK_CALI
extern Stockpile_FLASH_Typedef stockpile_quick_cali;  	//0x08010000U ~ 0x08020000U    64K    Quicak_Cali(可容纳32K校准数据-即最大支持15位编码器的校准数据)

//APP
#define		STOCKPILE_APP_FIRMWARE_ADDR		(              0x08000000U)		//起始地址
#define		STOCKPILE_APP_FIRMWARE_SIZE		(0x0800F800U - 0x08000000U)		//Flash容量
//DATA
#define		STOCKPILE_DATA_ADDR						(              0x0800F800U)		//起始地址
#define		STOCKPILE_DATA_SIZE						(0x08010000U - 0x0800F800U)		//Flash容量
//QUICK_CALI
#define		STOCKPILE_QUICAK_CALI_ADDR		(              0x08010000U)		//起始地址
#define		STOCKPILE_QUICAK_CALI_SIZE		(0x08020000U - 0x08010000U)		//Flash容量

void Stockpile_Flash_Data_Empty(Stockpile_FLASH_Typedef *stockpile);			//Flash数据清空
void Stockpile_Flash_Data_Begin(Stockpile_FLASH_Typedef *stockpile);			//Flash数据开始写入
void Stockpile_Flash_Data_End(Stockpile_FLASH_Typedef *stockpile);				//Flash数据结束写入
void Stockpile_Flash_Data_Set_Write_Add(Stockpile_FLASH_Typedef *stockpile, uint32_t write_add);					//Flash设置写地址
void Stockpile_Flash_Data_Write_Data16(Stockpile_FLASH_Typedef *stockpile, uint16_t *data, uint32_t num);	//Flash_16位数据写入
void Stockpile_Flash_Data_Write_Data32(Stockpile_FLASH_Typedef *stockpile, uint32_t *data, uint32_t num);	//Flash_32位数据写入
void Stockpile_Flash_Data_Write_Data64(Stockpile_FLASH_Typedef *stockpile, uint64_t *data, uint32_t num);	//Flash_64位数据写入

/*************************************************************** FLASH_End ***************************************************************/
/*************************************************************** FLASH_End ***************************************************************/
/*************************************************************** FLASH_End ***************************************************************/

#ifdef __cplusplus
}
#endif

#endif
