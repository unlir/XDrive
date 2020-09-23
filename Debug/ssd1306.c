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
  ** @file     : ssd1306.c/h
  ** @brief    : SSD1306驱动
  ** @versions : 1.1.1
  ** @time     : 2019/10/11
  ** @reviser  : unli (HeFei China)
  ** @explain  : null
*****/

//Oneself
#include "ssd1306.h"

//Font
#include "oled_font.h"

//GPIO输出
#define SSD1306_SPI_RES_H()		(OLED_SPI_RES_GPIO_Port -> BSRR = OLED_SPI_RES_Pin)
#define SSD1306_SPI_RES_L()		(OLED_SPI_RES_GPIO_Port -> BRR  = OLED_SPI_RES_Pin)
#define SSD1306_SPI_DC_H()		(OLED_SPI_DC_GPIO_Port -> BSRR = OLED_SPI_DC_Pin)
#define SSD1306_SPI_DC_L()		(OLED_SPI_DC_GPIO_Port -> BRR  = OLED_SPI_DC_Pin)

//实例
uint8_t	gram[(Quantity_Row)][(Quantity_Column/8)];	//GRAM空间

/**
  * @brief  写数据
  * @param  _data: 数据
  * @retval NULL
***/
void SSD1306_Write_Data(uint8_t _data)
{
	SSD1306_SPI_DC_H();
	HAL_SPI_Transmit(&OLED_SPI_Get_HSPI, &_data, 1, HAL_MAX_DELAY);
	SSD1306_SPI_DC_L();
}

/**
  * @brief  写指令
  * @param  _comm: 指令
  * @retval NULL
***/
void SSD1306_Write_Comm(uint8_t _comm)
{
	SSD1306_SPI_DC_L();
	HAL_SPI_Transmit(&OLED_SPI_Get_HSPI, &_comm, 1, HAL_MAX_DELAY);
	SSD1306_SPI_DC_H();
}

/**
  * @brief  屏幕显示开
  * @param  NULL
  * @retval NULL
***/
void SSD1306_Display_On(void)
{
	SSD1306_Write_Comm(0x8D);
	SSD1306_Write_Comm(0x14);
	SSD1306_Write_Comm(0xAF);
}

/**
  * @brief  屏幕显示关
  * @param  NULL
  * @retval NULL
***/
void SSD1306_Display_Off(void)
{
	SSD1306_Write_Comm(0x8D);
	SSD1306_Write_Comm(0x10);
	SSD1306_Write_Comm(0xAE);
}

/**
  * @brief  颜色反转
  * @param  _switch:开关
  * @retval NULL
***/
void SSD1306_Color_Reversal(bool _switch)
{
	if(_switch){
		SSD1306_Write_Comm(0xA6);
	}
	else{
		SSD1306_Write_Comm(0xA7);
	}
}

/**
  * @brief  方向反转
  * @param  _switch:开关
  * @retval NULL
***/
void SSD1306_Orien_Reversal(bool _switch)
{
	if(_switch){
		SSD1306_Write_Comm(0xC0);
		SSD1306_Write_Comm(0xA0);
	}
	else{
		SSD1306_Write_Comm(0xC8);
		SSD1306_Write_Comm(0xA1);
	}
}

/**
  * @brief  更新显示
  * @param  NULL
  * @retval NULL
***/
void SSD1306_Refresh(void)
{
	for(uint8_t y=0; y<(Quantity_Column/8); y++){
		SSD1306_Write_Comm(0xB0+y);
		SSD1306_Write_Comm(0x00);
		SSD1306_Write_Comm(0x10);
		for(uint8_t x=0; x<Quantity_Row; x++){
			SSD1306_Write_Data(gram[x][y]);
		}
	}
}

/**
  * @brief  清空GRAM
  * @param  NULL
  * @retval NULL
***/
void GRAM_Clear(void)
{
	for(uint8_t y=0; y<(Quantity_Column/8); y++){
		for(uint8_t x=0; x<Quantity_Row; x++){
			gram[x][y] = 0x00;
		}
	}
}

/**
  * @brief  打点
  * @param  multi
  * @retval NULL
***/
void GRAM_DrawPoint(uint8_t _x, uint8_t _y, bool _switch)
{
	uint8_t block = (_y / 8);
	uint8_t data = 0x01 << (_y & 0x07);

	if(_switch){
		gram[_x][block] |= data;
	}
	else{
		gram[_x][block] &= ~data;
	}
}

/**
  * @brief  打线
  * @param  multi
  * @retval NULL
***/
void GRAM_DrawLine(uint8_t _x, uint8_t _y, uint8_t _xe, uint8_t _ye, bool _switch)
{
	//用不到
}

/**
  * @brief  打圆
  * @param  multi
  * @retval NULL
***/
void GRAM_DrawCircle(uint8_t _x, uint8_t _y, uint8_t _radius, bool _switch)
{
	//用不到
}

/**
  * @brief  显示字符
  * @param  multi
  * @retval NULL
***/
void GRAM_ShowChar(uint8_t _x, uint8_t _y, uint8_t _char, CharFont _format, bool _switch)
{
	//行程
	uint8_t jou_x;		//X轴行程
	uint8_t jou_y;		//Y轴行程
	//过程变量
	uint8_t step_y;				//Y轴步进
	uint8_t data;					//刷写数据
	uint8_t	count = 0;		//刷些计数

	switch(_format){
		case CharFont_0806:	jou_x =  6;	jou_y =  8;	break;
		case CharFont_1206:	jou_x =  6;	jou_y = 12;	break;
		case CharFont_1608:	jou_x =  8;	jou_y = 16;	break;
		case CharFont_2412:	jou_x = 12;	jou_y = 24;	break;
	}

	//刷写Y轴
	for(;jou_y;){
		//提取Y行程
		if(jou_y > 8)		step_y = 8;
		else						step_y = jou_y;
		//刷写X轴
		for(uint8_t x=0; x<jou_x; x++){
			switch(_format){
				case CharFont_0806:	data = asc2_0806[_char - ' '][count];	break;
				case CharFont_1206:	data = asc2_1206[_char - ' '][count];	break;
				case CharFont_1608:	data = asc2_1608[_char - ' '][count];	break;
				case CharFont_2412:	data = asc2_2412[_char - ' '][count];	break;
			}
			for(uint8_t y=0; y<step_y; y++){
				if(data & (0x01<<y))	GRAM_DrawPoint((_x+x), (_y+y), _switch);
			}
			count++;
		}

		//Y行程处理
		_y += step_y;
		jou_y -= step_y;
	}
}

/**
  * @brief  显示字符串
  * @param  multi
  * @retval NULL
***/
void GRAM_ShowString(uint8_t _x, uint8_t _y, uint8_t *_char, CharFont _format, bool _switch)
{
	while((*_char >= ' ') && (*_char <= '~'))
	{
		GRAM_ShowChar(_x, _y, *_char, _format, _switch);
		switch(_format){
			case CharFont_0806:	_x +=  6;	break;
			case CharFont_1206:	_x +=  6;	break;
			case CharFont_1608:	_x +=  8;	break;
			case CharFont_2412:	_x += 12;	break;
		}
		_char++;
  }
}

/**
  * @brief  显示LU型数字
  * @param  multi
  * @retval NULL
***/
void GRAM_ShowNum_LU_Max10(uint8_t _x, uint8_t _y, uint32_t _num, uint8_t _width, CharFont _format, bool _switch)
{
	#define Max_Digit		10	//10bit数值
	uint8_t buff[Max_Digit + 1];	
	int8_t  result;
	
	//限制显示宽度
	if(_width > Max_Digit)	_width = Max_Digit;
	
	//应对尚未解决的"Sprintf格式化指定宽度的零值时出现字符串全部为空格"的问题采用的强制措施
	if(_num == 0){
		for(uint8_t i=0; i<(Max_Digit-1); i++)
			buff[i] = ' ';					//载入空格
		buff[Max_Digit-1] = '0';	//载入'0'字符
		buff[Max_Digit] = 0x00;		//载入空(结束)
		GRAM_ShowString(_x, _y, (buff + Max_Digit - _width), _format, _switch);
		return;
	}
	
	//格式化数字
	result = sprintf((char*)buff, "%10.lu", (unsigned long)_num);
	if((result > 0) && (result <= (Max_Digit + 1)))
		GRAM_ShowString(_x, _y, (buff + Max_Digit - _width), _format, _switch);
	#undef Max_Digit
}

/**
  * @brief  显示LI型数字
  * @param  multi
  * @retval NULL
***/
void GRAM_ShowNum_LI_Max11(uint8_t _x, uint8_t _y, int32_t _num, uint8_t _width, CharFont _format, bool _switch)
{
	#define Max_Digit 	11	//1bit符号 + 10bit数值
	uint8_t buff[Max_Digit + 1];
	int8_t	result;
	
	//限制显示宽度
	if(_width > Max_Digit)	_width = Max_Digit;
	
	//应对尚未解决的"Sprintf格式化指定宽度的零值时出现字符串全部为空格"的问题采用的强制措施
	if(_num == 0){
		for(uint8_t i=0; i<(Max_Digit-1); i++)
			buff[i] = ' ';					//载入空格
		buff[Max_Digit-1] = '0';	//载入'0'字符
		buff[Max_Digit] = 0x00;		//载入空(结束)
		GRAM_ShowString(_x, _y, (buff + Max_Digit - _width), _format, _switch);
		return;
	}
	
	//格式化数字
	result = sprintf((char*)buff, "%11.li", (long)_num);
	if((result > 0) && (result <= (Max_Digit + 1)))
		GRAM_ShowString(_x, _y, (buff + Max_Digit - _width), _format, _switch);
	#undef Max_Digit
}

/**
  * @brief  显示Float型数字
  * @param  multi
  * @retval NULL
***/
void GRAM_ShowNum_Float_Max15(uint8_t _x, uint8_t _y, float _num, uint8_t _width, CharFont _format, bool _switch)
{
	#define Max_Digit 	15	//1bit符号 + 10bit整数 + 1bit小数点 + 3bit小数
	uint8_t buff[Max_Digit + 1];
	int8_t	result;
	
	//格式化数字
	result = sprintf((char*)buff, "%15.3f", _num);
	if((result > 0) && (result <= (Max_Digit + 1)))
		GRAM_ShowString(_x, _y, (buff + Max_Digit - _width), _format, _switch);
	#undef Max_Digit
}

/**
  * @brief  xxxxxxxxx
  * @param  NULL
  * @retval NULL
***/

/**
  * @brief  SSD1306初始化
  * @param  NULL
  * @retval NULL
***/
void SSD1306_Init(void)
{
	//外设配置
	REIN_GPIO_OLED_Init();
	REIN_SPI_OLED_SPI_Init();
	
	//复位
	SSD1306_SPI_RES_L();
	HAL_Delay(10);
	SSD1306_SPI_RES_H();
	HAL_Delay(10);
	
	//初始化序列
	SSD1306_Write_Comm(0xAE); /*display off*/
	SSD1306_Write_Comm(0x00); /*set lower column address*/ 
	SSD1306_Write_Comm(0x10); /*set higher column address*/
	SSD1306_Write_Comm(0x00); /*set display start line*/ 
	SSD1306_Write_Comm(0xB0); /*set page address*/ 
	SSD1306_Write_Comm(0x81); /*contract control*/ 
	SSD1306_Write_Comm(0xff); /*128*/ 
	SSD1306_Write_Comm(0xA1); /*set segment remap*/ 
	SSD1306_Write_Comm(0xA6); /*normal / reverse*/ 
	SSD1306_Write_Comm(0xA8); /*multiplex ratio*/ 
	SSD1306_Write_Comm(0x1F); /*duty = 1/32*/ 
	SSD1306_Write_Comm(0xC8); /*Com scan direction*/ 
	SSD1306_Write_Comm(0xD3); /*set display offset*/ 
	SSD1306_Write_Comm(0x00); 
	SSD1306_Write_Comm(0xD5); /*set osc division*/ 
	SSD1306_Write_Comm(0x80); 
	SSD1306_Write_Comm(0xD9); /*set pre-charge period*/ 
	SSD1306_Write_Comm(0x1f); 
	SSD1306_Write_Comm(0xDA); /*set COM pins*/ 
	SSD1306_Write_Comm(0x00); 
	SSD1306_Write_Comm(0xdb); /*set vcomh*/ 
	SSD1306_Write_Comm(0x40); 
	SSD1306_Write_Comm(0x8d); /*set charge pump enable*/ 
	SSD1306_Write_Comm(0x14);
	
	SSD1306_Write_Comm(0xAF); /*display ON*/
	
	//测试刷入
//	for(uint8_t y=0; y<(Quantity_Column/8); y++){
//		for(uint8_t x=0; x<Quantity_Row; x++){
//			gram[x][y] = 0x55;
//		}
//	}

}


