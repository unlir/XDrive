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
  ** @file     : encode_cali.c/h
  ** @brief    : 编码器校准
  ** @versions : 1.1.4
  ** @time     : 2019/11/28
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

//Oneself
#include "encode_cali.h"

//Config
#include "stockpile_f103cb.h"

//Base_Drivers
#include "mt6816.h"
#include "hw_elec.h"

//Control
#include "signal_port.h"
#include "motor_control.h"

//Kernel
#include "loop.h"

//校准器实例
Encode_Cali_Typedef encode_cali;	//定义一个校准器

//私有函数
uint32_t CycleRem(uint32_t a,uint32_t b);									//取余(函数形式实现,防止编译器优化)
int32_t CycleSub(int32_t a, int32_t b, int32_t cyc);			//取循环差(函数形式实现,防止编译器优化)
int32_t CycleAverage(int32_t a, int32_t b, int32_t cyc);	//取循环平均值(函数形式实现,防止编译器优化)

void Calibration_Data_Check(void);		//校准器原始数据检查

/**
  * 取余,兼容一个周期的负数(核心控制-参数自动转换为无符号整形)
  * @param  NULL
  * @retval NULL
**/
uint32_t CycleRem(uint32_t a,uint32_t b)
{
	return (a+b)%b;
}

/**
  * 取循环差
  * @param  NULL
  * @retval NULL
**/
int32_t CycleSub(int32_t a, int32_t b, int32_t cyc)
{
	int32_t sub_data;

	sub_data = a - b;
	if(sub_data > (cyc >> 1))		sub_data -= cyc;
	if(sub_data < (-cyc >> 1))	sub_data += cyc;
	return sub_data;
}

/**
  * 取循环平均值
  * @param  NULL
  * @retval NULL
**/
int32_t CycleAverage(int32_t a, int32_t b, int32_t cyc)
{
	int32_t sub_data;
	int32_t ave_data;

	sub_data = a - b;
	ave_data = (a + b) >> 1;

	if(abs(sub_data) > (cyc >> 1))
	{
		if(ave_data >= (cyc >> 1))	ave_data -= (cyc >> 1);
		else												ave_data += (cyc >> 1);
	}
	return ave_data;
}

/**
  * 取循环平均值
  * @param  NULL
  * @retval NULL
**/
int32_t CycleDataAverage(uint16_t *data, uint16_t length, int32_t cyc)
{
	int32_t sum_data = 0;	//积分和
	int32_t sub_data;			//差
	int32_t	diff_data;		//微分

	//加载第一值
	sum_data += (int32_t)data[0];
	//加载后面的值
	for(uint16_t i=1; i<length; i++){
		diff_data = (int32_t)data[i];
		sub_data = (int32_t)data[i] - (int32_t)data[0];
		if(sub_data > (cyc >> 1))		diff_data = (int32_t)data[i] - cyc;
		if(sub_data < (-cyc >> 1))	diff_data = (int32_t)data[i] + cyc;
		sum_data += diff_data;
	}
	//计算平均值
	sum_data = sum_data / length;
	//平均值标准化
	if(sum_data < 0)		sum_data += cyc;
	if(sum_data > cyc)	sum_data -= cyc;
	return sum_data;
}

/**
  * @brief  校准器原始数据检查
  * @param  NULL
  * @retval NULL
  */
void Calibration_Data_Check(void)
{
	uint32_t	count;			//用于各个计数
	int32_t		sub_data;		//用于各个算差

	/******************** 检查平均值连续性及方向 ********************/
	//求解平均值数据
	for(count=0; count<Move_Step_NUM+1; count++){
		encode_cali.coder_data_f[count] = (uint16_t)CycleAverage((int32_t)encode_cali.coder_data_f[count], (int32_t)encode_cali.coder_data_r[count], CALI_Encode_Res);
	}
	//平均值数据检查
	sub_data = CycleSub((int32_t)encode_cali.coder_data_f[0], (int32_t)encode_cali.coder_data_f[Move_Step_NUM-1], CALI_Encode_Res);
	if(sub_data == 0)	{	encode_cali.error_code = CALI_Error_Average_Dir; return;	}
	if(sub_data > 0)	{	encode_cali.dir = true;	}
	if(sub_data < 0)	{	encode_cali.dir = false;	}
	for(count=1; count<Move_Step_NUM; count++)
	{
		sub_data = CycleSub((int32_t)encode_cali.coder_data_f[count], (int32_t)encode_cali.coder_data_f[count-1], CALI_Encode_Res);
		if(abs(sub_data) > (CALI_Gather_Encode_Res * 3 / 2))	{	encode_cali.error_code = CALI_Error_Average_Continuity;	encode_cali.error_data = count;	return;	}	//两次数据差过大
		if(abs(sub_data) < (CALI_Gather_Encode_Res * 1 / 2))	{	encode_cali.error_code = CALI_Error_Average_Continuity;	encode_cali.error_data = count;	return;	}	//两次数据差过小
		if(sub_data == 0)																			{	encode_cali.error_code = CALI_Error_Average_Dir;				encode_cali.error_data = count;	return;	}
		if((sub_data > 0) && (!encode_cali.dir))							{	encode_cali.error_code = CALI_Error_Average_Dir;				encode_cali.error_data = count;	return;	}
		if((sub_data < 0) && (encode_cali.dir))								{	encode_cali.error_code = CALI_Error_Average_Dir;				encode_cali.error_data = count;	return;	}
	}

	/******************** 全段域校准 完全拟合传感器数据与电机实际相位角非线性关系 ********************/
	//寻找区间下标及阶跃差值
	uint32_t step_num = 0;
	if(encode_cali.dir){
		for(count=0; count<Move_Step_NUM; count++){
			sub_data = (int32_t)encode_cali.coder_data_f[CycleRem(count+1, Move_Step_NUM)] - (int32_t)encode_cali.coder_data_f[CycleRem(count, Move_Step_NUM)];
			if(sub_data < 0){
				step_num++;
				encode_cali.rcd_x = count;//使用区间前标
				encode_cali.rcd_y = (CALI_Encode_Res-1) - encode_cali.coder_data_f[CycleRem(encode_cali.rcd_x, Move_Step_NUM)];
			}
		}
		if(step_num != 1){
			encode_cali.error_code = CALI_Error_PhaseStep;
			return;
		}
	}
	else{
		for(count=0; count<Move_Step_NUM; count++){
			sub_data = (int32_t)encode_cali.coder_data_f[CycleRem(count+1, Move_Step_NUM)] - (int32_t)encode_cali.coder_data_f[CycleRem(count, Move_Step_NUM)];
			if(sub_data > 0){
				step_num++;
				encode_cali.rcd_x = count;//使用区间前标
				encode_cali.rcd_y = (CALI_Encode_Res-1) - encode_cali.coder_data_f[CycleRem(encode_cali.rcd_x+1, Move_Step_NUM)];
			}
		}
		if(step_num != 1){
			encode_cali.error_code = CALI_Error_PhaseStep;
			return;
		}
	}

	//校准OK
	encode_cali.error_code = CALI_No_Error;
	return;
}

/**
  * @brief  校准器初始化
  * @param  NULL
  * @retval NULL
  */
void Calibration_Init(void)
{
	//信号
	encode_cali.trigger = false;
	encode_cali.error_code = CALI_No_Error;
	encode_cali.error_data = 0;
	//读取过程
	encode_cali.state = CALI_Disable;
	encode_cali.out_location = 0;
		//coder_data_f[]
		//coder_data_r[]
		//dir
	//全段域校准过程数据
	encode_cali.rcd_x = 0;
	encode_cali.rcd_y = 0;
	encode_cali.result_num = 0;
}


/**
  * @brief  校准器中断回调
  * @param  NULL
  * @retval NULL
  */
void Calibration_Interrupt_Callback(void)
{
#define AutoCali_Speed	2		//自动校准速度(主要用于编码器预数据采集)
#define Cali_Speed			1		//校准速度(用于精确的数据采集)

	//唤醒电流控制
	//CurrentControl_OutWakeUp();
	
	//状态变换
	switch(encode_cali.state)
	{
		//失能状态
		case CALI_Disable:
			//ELEC_Set_Sleep();
			if(encode_cali.trigger)
			{
				REIN_HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
				//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
				encode_cali.out_location = Move_Pulse_NUM;			//输出到1圈位置
				encode_cali.gather_count = 0;										//采集清零
				encode_cali.state = CALI_Forward_Encoder_AutoCali;	//--->编码器正转自动校准
				//初始化标志
				encode_cali.error_code = CALI_No_Error;
				encode_cali.error_data = 0;
			}
		break;
		//编码器正转自动校准
		case CALI_Forward_Encoder_AutoCali://正转个1圈 (1 * Motor_Pulse_NUM) -> (2 * Motor_Pulse_NUM)
			encode_cali.out_location += AutoCali_Speed;
			REIN_HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location == 2 * Move_Pulse_NUM)
			{
				encode_cali.out_location = Move_Pulse_NUM;
				encode_cali.state = CALI_Forward_Measure;//--->正向测量
			}
		break;
		//正向测量
		case CALI_Forward_Measure://(Motor_Pulse_NUM) -> (2 * Motor_Pulse_NUM)
			if((encode_cali.out_location % Move_Divide_NUM) == 0)//每到达采集细分量点采集一次数据
			{
				//采集
				encode_cali.coder_data_gather[encode_cali.gather_count++] = mt6816.angle_data;
				if(encode_cali.gather_count == Gather_Quantity){
					//记录数据
					encode_cali.coder_data_f[(encode_cali.out_location - Move_Pulse_NUM) / Move_Divide_NUM]
						= CycleDataAverage(encode_cali.coder_data_gather, Gather_Quantity, CALI_Encode_Res);
					//采集计数清零
					encode_cali.gather_count = 0;
					//移动位置
					encode_cali.out_location += Cali_Speed;
				}
			}
			else{
				//移动位置
				encode_cali.out_location += Cali_Speed;
			}	
			REIN_HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location > (2 * Move_Pulse_NUM))
			{
				encode_cali.state = CALI_Reverse_Ret;//--->反向回退
			}
		break;
		//反向回退
		case CALI_Reverse_Ret://(2 * Motor_Pulse_NUM) -> (2 * Motor_Pulse_NUM + Motor_Divide_NUM * 20)
			encode_cali.out_location += Cali_Speed;
			REIN_HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location == (2 * Move_Pulse_NUM + Move_Divide_NUM * 20))
			{
				encode_cali.state = CALI_Reverse_Gap;//--->反向消差
			}
		break;
		//反向消差
		case CALI_Reverse_Gap://(2 * Motor_Pulse_NUM + Motor_Divide_NUM * 20) -> (2 * Motor_Pulse_NUM)
			encode_cali.out_location -= Cali_Speed;
			REIN_HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location == (2 * Move_Pulse_NUM))
			{
				encode_cali.state = CALI_Reverse_Measure;//--->反向测量
			}
		break;
		//反向测量
		case CALI_Reverse_Measure://(2 * Motor_Pulse_NUM) -> (Motor_Pulse_NUM)
			if((encode_cali.out_location % Move_Divide_NUM) == 0)//每到达采集细分量点采集一次数据
			{
				//采集
				encode_cali.coder_data_gather[encode_cali.gather_count++] = mt6816.angle_data;
				if(encode_cali.gather_count == Gather_Quantity){
					//记录数据
					encode_cali.coder_data_r[(encode_cali.out_location - Move_Pulse_NUM) / Move_Divide_NUM]
						= CycleDataAverage(encode_cali.coder_data_gather, Gather_Quantity, CALI_Encode_Res);
					//采集计数清零
					encode_cali.gather_count = 0;
					//移动位置
					encode_cali.out_location -= Cali_Speed;
				}
			}
			else{
				//移动位置
				encode_cali.out_location -= Cali_Speed;
			}	
			REIN_HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location < Move_Pulse_NUM)
			{
				encode_cali.state = CALI_Operation;//--->解算
			}
		break;
		//解算
		case CALI_Operation:
			//进行校准运算中
			REIN_HW_Elec_SetDivideElec(0, 0);
			//CurrentControl_Out_FeedTrack(0, 0, true, true);
		break;
		default:
		break;
	}
}

/**
  * @brief  校准器主程序回调
  * @param  NULL
  * @retval NULL
  */
void Calibration_Loop_Callback(void)
{
	int32_t		data_i32;
	uint16_t	data_u16;
//	uint16_t	data_u8;
	
	//非解算态退出
	if(encode_cali.state != CALI_Operation)
		return;
	
	//PWM输出衰减态
	REIN_HW_Elec_SetSleep();

	//校准器原始数据检查
	Calibration_Data_Check();
	
	/**********  进行快速表表格建立  **********/
	if(encode_cali.error_code == CALI_No_Error)
	{
		//数据解析
		/******************** 全段域校准 完全拟合传感器数据与电机实际相位角非线性关系 ********************/
		int32_t step_x, step_y;
		encode_cali.result_num = 0;
		Stockpile_Flash_Data_Empty(&stockpile_quick_cali);		//擦除数据区
		Stockpile_Flash_Data_Begin(&stockpile_quick_cali);		//开始写数据区
		if(encode_cali.dir){
			for(step_x = encode_cali.rcd_x; step_x < encode_cali.rcd_x + Move_Step_NUM + 1; step_x++){
				data_i32 = CycleSub(	encode_cali.coder_data_f[CycleRem(step_x+1, Move_Step_NUM)],
															encode_cali.coder_data_f[CycleRem(step_x,   Move_Step_NUM)],
															CALI_Encode_Res);
				if(step_x == encode_cali.rcd_x){//开始边缘
					for(step_y = encode_cali.rcd_y; step_y < data_i32; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * step_x + Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
				else if(step_x == encode_cali.rcd_x + Move_Step_NUM){//结束边缘
					for(step_y = 0; step_y < encode_cali.rcd_y; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * step_x + Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
				else{//中间
					for(step_y = 0; step_y < data_i32; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * step_x + Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
			}
		}
		else
		{
			for(step_x = encode_cali.rcd_x + Move_Step_NUM; step_x > encode_cali.rcd_x - 1; step_x--)
			{
				data_i32 = CycleSub(	encode_cali.coder_data_f[CycleRem(step_x, Move_Step_NUM)],
															encode_cali.coder_data_f[CycleRem(step_x+1, Move_Step_NUM)],
															CALI_Encode_Res);
				if(step_x == encode_cali.rcd_x+Move_Step_NUM){//开始边缘
					for(step_y = encode_cali.rcd_y; step_y < data_i32; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * (step_x+1) - Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
				else if(step_x == encode_cali.rcd_x){//结束边缘
					for(step_y = 0; step_y < encode_cali.rcd_y; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * (step_x+1) - Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
				else{//中间
					for(step_y = 0; step_y < data_i32; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * (step_x+1) - Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
			}
		}
		Stockpile_Flash_Data_End(&stockpile_quick_cali);	//结束写数据区
		
		if(encode_cali.result_num != CALI_Encode_Res)
			encode_cali.error_code = CALI_Error_Analysis_Quantity;
	}

	//确认校准结果
	if(encode_cali.error_code == CALI_No_Error){
		mt6816.rectify_valid = true;
	}
	else{
		mt6816.rectify_valid = false;
		Stockpile_Flash_Data_Empty(&stockpile_quick_cali);	//清除校准区数据
	}
	
	//运动配置覆盖
	Signal_MoreIO_Capture_Goal();			//读取信号端口数据以清除校准期间采样的信号
	motor_control.stall_flag = true;	//触发堵转保护,即校准后禁用运动控制
	
	//清理校准信号
	encode_cali.state = CALI_Disable;
	encode_cali.trigger = false;			//清除校准触发
}




