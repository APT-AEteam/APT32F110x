/***********************************************************************//** 
 * \file  iwdt_demo.c
 * \brief  IWDT_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY    <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/iwdt.h>
#include <drv/pin.h>

#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief IWDT非中断demo，溢出时间1s，不开启报警中断
 * 
 *  \param[in] none
 *  \return error code
 */
int iwdt_normal_demo(void)
{
	int iRet = 0;

	csi_iwdt_init(IWDT_TO_1024);			//初始化看门狗，溢出时间为1000ms(系统复位时间)
	csi_iwdt_open();						//打开看门狗
	csi_iwdt_feed();						//喂狗
			
	return iRet;
}
/** \brief IWDT中断demo，溢出时间1s，报警中断时间0.5s
 * 
 *  \param[in] none
 *  \return error code
 */
int iwdt_irq_demo(void)
{
	int iRet = 0;

	csi_iwdt_init(IWDT_TO_1024);					//初始化看门狗，溢出时间为1000ms(系统复位时间)
	csi_iwdt_irq_enable(IWDT_ALARMTO_4_8, ENABLE);	//使能看门狗报警中断，报警时间为4/8溢出时间
	csi_iwdt_open();								//打开看门狗

	return iRet;
}