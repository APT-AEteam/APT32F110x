/***********************************************************************//** 
 * \file  uart_demo.c
 * \brief  UART_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-28 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/uart.h>
#include <drv/pin.h>

#include "demo.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

ringbuffer_t g_tRingbuf;						//循环buffer
uint8_t 	 g_byRxBuf[UART_RECV_MAX_LEN];		//接收缓存

volatile uint8_t byRvUart[30]={0};

/** \brief uart dma send data
 *  \brief 串口通过DMA发送数据，使用时请确认ETCB已初始化(使能)，ETCB初始化调用csi_etb_init()函数
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_send_dma_demo(void)
{
	int iRet = 0;
	uint8_t bySdData[36]={31,32,33,34,5,6,7,8,9,10,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,1,2,3};
	volatile uint8_t byRecv;
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_UART1_TX);		//TX	
	csi_pin_set_mux(PA06, PA06_UART1_RX);		//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
	tUartConfig.byParity = UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate = 115200;				//波特率，115200
	tUartConfig.wInt = UART_INTSRC_NONE;		//UART中断关闭，轮询(同步)方式
	tUartConfig.byTxMode = UART_TX_MODE_POLL;	//发送 轮询模式
	tUartConfig.byRxMode = UART_RX_MODE_POLL;	//接收 轮询模式
	
	csi_uart_init(UART1, &tUartConfig);			//初始化串口
	csi_uart_start(UART1, UART_FUNC_RX_TX);		//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	csi_uart_dma_tx_init(UART1, DMA_CH1, ETB_CH10);	
	
	while(1)
	{
		byRecv = csi_uart_getc(UART1);
		if(byRecv == 0x06)
			csi_uart_send_dma(UART1, DMA_CH1, (void *)bySdData, 26);	
		mdelay(10);
		if(csi_dma_get_msg(DMA_CH1, ENABLE))	//获取发送完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}
		nop;
		nop;
	}
	
	return iRet;
}
/** \brief uart dma receive data
 *  \brief 串口通过DMA接收数据，使用时请确认ETCB已初始化(使能)，ETCB初始化调用csi_etb_init()函数
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_recv_dma_demo(void)
{
	int iRet = 0;
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_UART1_TX);		//TX	
	csi_pin_set_mux(PA06, PA06_UART1_RX);		//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
	tUartConfig.byParity = UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate = 115200;				//波特率，115200
	tUartConfig.wInt = UART_INTSRC_NONE;		//UART中断关闭，轮询(同步)方式
	tUartConfig.byTxMode = UART_TX_MODE_POLL;	//发送 轮询模式
	tUartConfig.byRxMode = UART_RX_MODE_POLL;	//接收 轮询模式
	
	csi_uart_init(UART1, &tUartConfig);			//初始化串口
	csi_uart_start(UART1, UART_FUNC_RX_TX);		//开启UART的RX和TX功能，也可单独开启RX或者TX功能

	csi_uart_dma_rx_init(UART1, DMA_CH3, ETB_CH8);
	csi_uart_recv_dma(UART1, DMA_CH3, (void*)byRvUart,22);
	
	while(1)
	{
		if(csi_dma_get_msg(DMA_CH3, ENABLE))	//获取接收完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}							
		mdelay(10);
		nop;
	}
	
	return iRet;
}
/** \brief uart char receive and send 
 *  \brief 串口接收/发送一个字符，轮询方式
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_char_demo(void)
{
	int iRet = 0;
	volatile uint8_t byRecv;
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_UART1_TX);		//TX	
	csi_pin_set_mux(PA06, PA06_UART1_RX);		//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
	tUartConfig.byParity = UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate = 115200;				//波特率，115200
	tUartConfig.wInt = UART_INTSRC_NONE;		//无中断
	tUartConfig.byTxMode = UART_TX_MODE_POLL;	//发送 轮询模式
	tUartConfig.byRxMode = UART_RX_MODE_POLL;	//接收 轮询模式
	
	csi_uart_init(UART1, &tUartConfig);			//初始化串口
	csi_uart_start(UART1, UART_FUNC_RX_TX);		//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		byRecv = csi_uart_getc(UART1);
		csi_uart_putc(UART1, byRecv+1);
		
		mdelay(10);
	}
	
	return iRet;
}


/** \brief uart send a bunch of data; polling(sync,no interrupt)mode
 *  \brief 串口发送一串数据，TX使用轮询
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_send_demo(void)
{
	int iRet = 0;
	uint8_t bySendData[30]={1,2,3,4,5,6,7,8,9,10,21,22,23,24,25,26,10,11,12,13,14,15,16,17,18,19,1,2,3};
	volatile uint8_t byRecv;
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_UART1_TX);		//TX	
	csi_pin_set_mux(PA06, PA06_UART1_RX);		//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
	tUartConfig.byParity = UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate = 115200;				//波特率，115200
	tUartConfig.wInt = UART_INTSRC_NONE;		//UART中断关闭，轮询(同步)方式
	tUartConfig.byTxMode = UART_TX_MODE_POLL;	//发送 轮询模式
	tUartConfig.byRxMode = UART_RX_MODE_POLL;	//接收 轮询模式
	
	csi_uart_init(UART1, &tUartConfig);			//初始化串口
	csi_uart_start(UART1, UART_FUNC_RX_TX);		//开启UART的RX和TX功能，也可单独开启RX或者TX功能

	while(1)
	{
		byRecv = csi_uart_getc(UART1);
		if(byRecv == 0x06)
			byRecv = csi_uart_send(UART1,(void *)bySendData,18);		//采用轮询方式,调用该函数时，UART发送中断关闭
		
		mdelay(5);
//		if(byRecv == 16)
//			csi_uart_putc(UART1, 0x03);
	}
	
	return iRet;
}

/** \brief uart send a bunch of data; interrupt(nsync) mode
 *  \brief 串口发送一串数据，TX使用中断
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_send_int_demo(void)
{
	int iRet = 0;
	uint8_t bySendData[30]={1,2,3,4,5,6,7,8,9,21,22,23,24,25,26,27,28,29,30,10,11,12,13,14,15,16,17,18,19};
	volatile uint8_t byRecv;
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_UART1_TX);		//TX	
	csi_pin_set_mux(PA06, PA06_UART1_RX);		//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
	tUartConfig.byParity = UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate = 115200;				//波特率，115200
	tUartConfig.wInt = UART_INTSRC_TXDONE ;	//UART发送中断使能，采用(发送完成)TXDONE中断
	tUartConfig.byTxMode = UART_TX_MODE_INT;	//发送模式：中断模式
	tUartConfig.byRxMode = UART_RX_MODE_POLL;	//接收模式：轮询模式
	
	csi_uart_init(UART1, &tUartConfig);			//初始化串口
	csi_uart_start(UART1, UART_FUNC_RX_TX);		//开启UART的RX和TX功能，也可单独开启RX或者TX功能

	while(1)
	{
		byRecv = csi_uart_getc(UART1);
		if(byRecv == 0x06)
			csi_uart_send(UART1,(void *)bySendData,28);		//采用中断方式。调用改函数时，UART发送中断使能
		
		while(1)			
		{
			//如果有需要，可用于判断发送是否完成；
			if(csi_uart_get_msg(UART1,UART_SEND, 1))		//获取发送完成消息，并清除消息(设置为idle)，串口发送一串数据
			{
				//发送状态有三种，IDLE(空闲)/SEND(发送中)/DONE(发送完成)
				//具体定义参考：uart.h中csi_uart_state_e,
				//csi_uart_clr_send_status(UART1);						//清除发送状态位，状态设置为空闲(idle)
				nop;
				break;
			}
		}
		
		//TODO
		//mdelay(10);
	}
	
	return iRet;
}

/** \brief uart receive a bunch of data; polling(sync) mode
 *  \brief 串口接收指定长度数据，RX使用轮询(不使用中断)，带超时处理(单位：ms)
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_receive_demo(void)
{
	int iRet = 0;
	//uint8_t bySendData[20]={1,2,3,4,5,6,7,8,9,21,22,23,24,25,26,27,28,29};
	uint8_t byRecvData[20]={0};
	volatile uint8_t byRecv;
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_UART1_TX);		//TX	
	csi_pin_set_mux(PA06, PA06_UART1_RX);		//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
	tUartConfig.byParity = UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate = 115200;				//波特率，115200
	tUartConfig.wInt = UART_INTSRC_NONE;		//串口中断关闭
	
	tUartConfig.byTxMode = UART_TX_MODE_POLL;	//发送 轮询模式
	tUartConfig.byRxMode = UART_RX_MODE_POLL;	//接收 轮询模式
	
	csi_uart_init(UART1, &tUartConfig);			//初始化串口
	csi_uart_start(UART1, UART_FUNC_RX_TX);		//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		byRecv = csi_uart_receive(UART1,byRecvData,16,2000);	//UART接收采用轮询方式(同步)	
		if(byRecv  == 16)
			csi_uart_send(UART1,(void *)byRecvData,byRecv);		//UART发送采用轮询方式(同步)	
	}
	
	return iRet;
}

/** \brief uart receive assign(fixed) length data; interrupt(async) mode
 *  \brief 串口接收指定长度数据，RX使用中断，TX不使用中断
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_recv_int_demo(void)
{
	int iRet = 0;
	uint8_t  byRxBuf[32];
	uint16_t hwRecvNum = 1;
	volatile uint16_t hwRecvLen;
	
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_UART1_TX);		//TX	
	csi_pin_set_mux(PA06, PA06_UART1_RX);		//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
	//接收缓存配置，实例化接收ringbuf，将ringbuf接收数据缓存指向用户定义的的接收buffer(g_byRxBuf)
	//需要传入参数：串口设备/ringbuf结构体指针/接收buffer/接收buffer长度
	csi_uart_set_buffer(UART1, &g_tRingbuf, g_byRxBuf, sizeof(g_byRxBuf));
	
	tUartConfig.byParity = UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate = 115200;				//波特率，115200
	tUartConfig.wInt = UART_INTSRC_RXFIFO;	//串口接收中断打开，使用RXFIFO中断(默认推荐使用)
	tUartConfig.byTxMode = UART_TX_MODE_POLL;	//发送模式：轮询模式
	tUartConfig.byRxMode = UART_RX_MODE_INT_FIX;//接收模式：中断指定接收模式
	
	csi_uart_init(UART1, &tUartConfig);			//初始化串口
	csi_uart_start(UART1, UART_FUNC_RX_TX);		//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		//从串口缓存（UART接收循环buffer）里面读取数据，返回读取数据个数
		//用户应用根据实际不同协议来处理数据
		
		if(hwRecvNum == 1)		//单个字节收数据(读接收ringbuf)
		{
			hwRecvLen = csi_uart_receive(UART1,(void *)byRxBuf, hwRecvNum, 0);	//读取接收循环buffer数据, 有数据返回数据
			if(hwRecvLen == hwRecvNum)
				csi_uart_putc(UART1,*byRxBuf);
		}
		else if(hwRecvNum > 1)	//多个字节收数据(读接收ringbuf)
		{
			hwRecvLen = csi_uart_receive(UART1,(void *)byRxBuf, hwRecvNum, 0);	//读取接收循环buffer数据
			if(hwRecvLen == hwRecvNum)
				csi_uart_send(UART1,(void *)byRxBuf, hwRecvNum);				//UART发送采用轮询方式(同步)
		}
	}
	
	return iRet;
}


/** \brief uart receive a bunch of data; interrupt(async) mode
 *  \brief 串口接收到一串字符串，RX使用中断模式，TX不使用中断
 *
 *  \param[in] none
 *  \return error code
 */
int uart_recv_dynamic_demo(void)
{
	int iRet = 0;
	uint8_t byRxBuf[128];
	
	volatile uint16_t hwRecvLen;
	
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_UART1_TX);		//TX	
	csi_pin_set_mux(PA06, PA06_UART1_RX);		//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
	//接收缓存配置，实例化接收ringbuf，将ringbuf接收数据缓存指向用户定义的的接收buffer(g_byRxBuf)
	//需要传入参数：串口设备/ringbuf结构体指针/接收buffer/接收buffer长度
	csi_uart_set_buffer(UART1, &g_tRingbuf, g_byRxBuf, sizeof(g_byRxBuf));
	
	//串口参数配置
	tUartConfig.byParity = UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate = 115200;				//波特率，115200
	tUartConfig.wInt = UART_INTSRC_RXFIFO | UART_INTSRC_RXBRK;	//串口接收中断打开，使用RXFIFO中断(默认推荐使用)
	tUartConfig.byTxMode = UART_TX_MODE_POLL;	//发送模式：轮询模式
	tUartConfig.byRxMode = UART_RX_MODE_INT_DYN;//接收模式：中断动态接收模式
	
	csi_uart_init(UART1, &tUartConfig);			//初始化串口
	csi_uart_start(UART1, UART_FUNC_RX_TX);		//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		if(csi_uart_get_msg(UART1,UART_RECV, ENABLE))					//获取判接收完成消息，并清除消息(设置为idle)，串口接收到一串字符
		{
			hwRecvLen = csi_uart_receive(UART1,(void*)byRxBuf,0,0);		//获取接收到的一串数据，返回数据长度, 后面两个参数无意义
			csi_uart_send(UART1,(void *)byRxBuf,hwRecvLen);				//UART发送采用轮询方式(同步)
		}
	}
	return iRet;
}

/** \brief uart receive a bunch of data; interrupt(async) mode
 *  \brief 串口接收到一串字符串, RX使用中断模式,TX使用中断方式
 *
 *  \param[in] none
 *  \return error code
 */
int uart_recv_dynamic_int_demo(void)
{
	int iRet = 0;
	uint8_t byRxBuf[128];
	
	volatile uint16_t hwRecvLen;
	
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_UART1_TX);		//TX	
	csi_pin_set_mux(PA06, PA06_UART1_RX);		//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
	//接收缓存配置，实例化接收ringbuf，将ringbuf接收数据缓存指向用户定义的的接收buffer(g_byRxBuf)
	//需要传入参数：串口设备/ringbuf结构体指针/接收buffer/接收buffer长度
	csi_uart_set_buffer(UART1, &g_tRingbuf, g_byRxBuf, sizeof(g_byRxBuf));
	
	//串口参数配置
	tUartConfig.byParity = UART_PARITY_ODD;							//校验位，奇校验
	tUartConfig.wBaudRate = 115200;									//波特率，115200
	tUartConfig.wInt = UART_INTSRC_RXFIFO | UART_INTSRC_TXDONE;	//串口接收/发送中断打开，使用RXFIFO/TXDONE中断(默认推荐使用)
	tUartConfig.byTxMode = UART_TX_MODE_INT;						//发送模式：中断发送
	tUartConfig.byRxMode = UART_RX_MODE_INT_DYN;					//接收模式：中断动态接收模式
	
	csi_uart_init(UART1, &tUartConfig);								//初始化串口
	csi_uart_start(UART1, UART_FUNC_RX_TX);							//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		if(csi_uart_get_msg(UART1,UART_RECV, ENABLE))				//获取判接收完成消息，并清除消息(设置为idle)，串口接收到一串字符
		{
			hwRecvLen = csi_uart_receive(UART1,(void*)byRxBuf,0,0);	//获取接收到的一串数据，返回数据长度, 后面两个参数无意义
			csi_uart_send(UART1,(void *)byRxBuf,hwRecvLen);			//UART发送采用中断方式(异步)
		}
	}
	
	return iRet;
}