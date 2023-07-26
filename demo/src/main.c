/***********************************************************************//** 
 * \file  main.c
 * \brief  MAIN description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  		<th>Version <th>Author	<th>Description
 * <tr><td> 2021-02-21  <td>V0.0  	<td>ZJY   	<td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <csi_drv.h>
#include <iostring.h>

#include "demo.h"

/* Private macro-----------------------------------------------------------*/

/* externs function--------------------------------------------------------*/
extern void system_init(void);
extern void board_init(void);

extern void __ChipInitHandler(void);		//gui
extern void user_demo(void);				//demo


/* externs variablesr------------------------------------------------------*/

/* Private variablesr------------------------------------------------------*/

int main()
{

#if !defined(USE_GUI)
	//不使用图形化编程需调用system_init函数
    system_init();							//系统时钟、TICK等配置	
						
#else
	//使用图形化编程，用户需要在工程设置里Compiler的Define选项里面添加USE_GUI的编译开关
	//图形化编程支持SYSTEM(包括系统时钟、LVD/R、IWDT、TICK等)、I/O PORT和EXI
	//使用图形化编程请务必配置System Clk，否则系统的时钟默认选择IM_5.556M
	__ChipInitHandler();
	
#endif
	
	board_init();							//打印初始化, PA111/PA112 = UART2_TX/RX, 定义见board_config.h	
	user_demo();							//demo	
	
	my_printf("Hello World~~~~~~~\n");		//print message
	
	while(1)
	{
		mdelay(100);						//delay 100ms
	}
	
    return 0;
}
