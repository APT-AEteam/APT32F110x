/***********************************************************************//** 
 * \file  pin_demo.c
 * \brief  PIN_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-4-25 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "sys_clk.h"
#include <drv/gpio.h>
#include <drv/pin.h>
#include "board_config.h"
#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


/** \brief GPIO的PIN脚输出demo，PB00管脚为例，配置为开漏输出和推挽输出模式，PB00
 *         依次输出高、低、高
 * 
 *  \param[in] none
 *  \return error code
 */
int pin_output_demo(void)
{
	int iRet = 0;

#if (USE_GUI == 0)								//用户未选择图形化编程		
	csi_pin_set_mux(PB00,PB00_OUTPUT);			//PB00 配置为输出
	
	//开漏
	csi_pin_output_mode(PB00, GPIO_OPEN_DRAIN);	//PB00 配置为开漏输出
	csi_pin_set_high(PB00);						
	mdelay(100);
	csi_pin_set_low(PB00);						
	mdelay(100);
	csi_pin_set_high(PB00);						
	mdelay(100);
	
	//推挽
	csi_pin_output_mode(PB00, GPIO_PUSH_PULL);	//PB00 配置为推挽输出
	csi_pin_set_high(PB00);						
	mdelay(100);
	csi_pin_set_low(PB00);						
	mdelay(100);
	csi_pin_set_high(PB00);						
	mdelay(100);
#endif
	
	return iRet;
}

/** \brief GPIO的PIN脚输入demo，PA08管脚为例，配置为输入模式，依次配置无上下拉
 *         上拉、下拉。
 * 
 *  \param[in] none
 *  \return error code
 */
int pin_input_demo(void)
{
	int iRet = 0;

#if (USE_GUI == 0)								//用户未选择图形化编程	
	csi_pin_set_mux(PA08,PA08_INPUT);			//PA08 配置为输入
	
	csi_pin_pull_mode(PA08,GPIO_PULLNONE);		//无上下拉
	mdelay(100);
	
	csi_pin_pull_mode(PA08,GPIO_PULLUP);		//上拉
	mdelay(100);
	
	csi_pin_pull_mode(PA08,GPIO_PULLDOWN);		//下拉
	mdelay(100);
	
#endif
	
	return iRet;
}

/** \brief GPIO的PIN脚输入demo，PB01管脚为例,配置为双边沿中断模式，PB01需配置为输入模式，上下
 *         拉配置的选择可根据实际使用场景来选择，中断处理函数和gpio_demo里的公用，可参阅gpio
 *         demo的gpio_irqhandler函数
 * 
 *  \param[in] none
 *  \return error code
 */
int pin_irq_demo(void)
{
	int iRet = 0;
	
#if (USE_GUI == 0)												//用户未选择图形化编程	

	csi_pin_set_mux(PB01, PB01_INPUT);							//PB01 配置为输入
	csi_pin_pull_mode(PB01, GPIO_PULLUP);						//PB01 上拉
	csi_pin_irq_enable(PB01, ENABLE);							//PB01 中断使能	 
	csi_pin_irq_mode(PB01, EXI_GRP1, GPIO_IRQ_BOTH_EDGE);		//PB01 双边沿产生中断，选择中断组1
	csi_pin_vic_irq_enable(EXI_GRP1, ENABLE);					//PB01 VIC中断使能，选择中断组1
#endif
	
	return iRet;
}