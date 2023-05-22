
/***********************************************************************//** 
 * \file  sys_clk.h
 * \brief  head file for sys_clk.c  
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0 <td>WNN     <td>initial for csi
 * <tr><td> 2020-5-13 <td>V0.0 <td>ZJY     <td>initial for csi
 * </table>
 * *********************************************************************
*/
 
#ifndef _SYS_CLK_H_
#define _SYS_CLK_H_

#include <stdint.h>
#include <soc.h>
#include "csp_syscon.h"
#include "csp_hwdiv.h"

#define TIMER_16BIT_MODE              1
#define TIMER_24BIT_MODE              2

#define TIMER_16BIT_MAX               0xffff
#define TIMER_24BIT_MAX               0xffffff

#define TIMER_16BIT_DIVISOR           60000UL
#define TIMER_24BIT_DIVISOR           16000000UL

typedef enum {
    SRC_IMOSC = 0,
    SRC_EMOSC,
    SRC_HFOSC,
	SRC_ISOSC = 4,
	SRC_ESOSC
} csi_clk_src_e;

typedef enum{
	PCLK_PM = 8,
	HCLK_PM,
	ISOSC_PM = 12, 
	IMOSC_PM,
	EMOSC_PM = 15
}csi_clk_pm_e;

typedef enum {
	IM_5M = IMOSC_5M_VALUE,
	IM_4M = IMOSC_4M_VALUE,
	IM_2M = IMOSC_2M_VALUE, 
	IM_131K = IMOSC_131K_VALUE
}csi_imo_freq_e;

typedef enum{
	HF_48M 	= HFOSC_48M_VALUE,
	HF_24M  = HFOSC_24M_VALUE,
	HF_12M  = HFOSC_12M_VALUE,
	HF_6M   = HFOSC_6M_VALUE
}csi_hfo_freq_e;

typedef enum{
	PCLK_DIV1	= 0,
	PCLK_DIV2,
	PCLK_DIV4,
	PCLK_DIV8 	= 4,
	PCLK_DIV16 	= 8
}csi_pclk_div_e;

typedef enum{
	SCLK_DIV1	= 1,
	SCLK_DIV2,
	SCLK_DIV3,
	SCLK_DIV4,
	SCLK_DIV5,
	SCLK_DIV6,
	SCLK_DIV9	= 7,
	SCLK_DIV12,
	SCLK_DIV16,
	SCLK_DIV24,
	SCLK_DIV32,
	SCLK_DIV36,
	SCLK_DIV64,
	SCLK_DIV128,
	SCLK_DIV256
}csi_hclk_div_e;

/// \struct system_clk_config_t
/// members to be used in system clock management, including osc src, osc freq(if seletable), HCLK and PCLK divider
typedef struct {
	csi_clk_src_e		eClkSrc;	//clock source
	uint32_t		    wFreq;		//clock frequency
	csi_hclk_div_e		eSdiv;		//SDIV
	csi_pclk_div_e		ePdiv;		//PDIV
	uint32_t		    wSclk;		//SCLK
	uint32_t		    wPclk;
}csi_clk_config_t;

extern csi_clk_config_t g_tClkConfig;

//typedef struct {
//    csi_clk_src_e	eSysClkSrc;      /* select sysclk source clock */
//	  uint32_t 	wOscFreq;        /* select frequence */
//    csi_hclk_div_e 	eHclkDivider;    /* ratio between fs_mclk clock and mclk clock */
//    csi_pclk_div_e 	ePclkDivider;    /* ratio between mclk clock and apb0 clock */
//} system_clk_config_t;

/// \enum clk_module_t
/// \brief all the peri clock enable bits in SYSCON level
/// \todo  clk_module_t or csi_clk_module_e
typedef enum {
     //PCER0/PCDR0
	IFC_SYS_CLK		= 0U,
    ADC_SYS_CLK		= 4U,
    TKEY_SYS_CLK	= 6U,
    ETCB_SYS_CLK	= 7U,
    UART0_SYS_CLK	= 8U,
	UART1_SYS_CLK	= 9U,
	UART2_SYS_CLK	= 10U,
	USART_SYS_CLK	= 13U,
    SPI_SYS_CLK		= 16U,
    SIO_SYS_CLK		= 20U,
    I2C_SYS_CLK		= 22U,
	//PCER1/PCDR1
	EPT0_SYS_CLK	= 32U,
	GPTA0_SYS_CLK	= 36U,
	GPTA1_SYS_CLK	= 37U,
	GPTB0_SYS_CLK	= 40U,
	GPTB1_SYS_CLK	= 41U,
    WWDT_SYS_CLK	= 43U,
    LPT_SYS_CLK		= 44U,
	CNTA_SYS_CLK	= 45U,
	RTC_SYS_CLK		= 46U,
	BT0_SYS_CLK		= 47U,
	BT1_SYS_CLK		= 48U,
	LED_SYS_CLK		= 49U,
	CMP0_SYS_CLK	= 50U,
	CMP1_SYS_CLK	= 51U,
	LCD_SYS_CLK		= 54U,
	
	//CORET CLK
	CORET_SYS_CLK	= 11U
} csi_clk_module_e;


//global variable: sysclkclk 
extern uint32_t g_wSystemClk;


/** 
  \brief sysctem clock (HCLK) configuration
   To set CPU frequence according to tClkConfig
  \param[in] none.
  \return csi_error_t.
 */ 
csi_error_t csi_sysclk_config(csi_clk_config_t tClkCfg);
/** 
  \brief Clock output configuration
  \param[in] eCloSrc: source to output
  \param[in] eCloDiv: clo divider 
  \return csi_error_t.
 */
csi_error_t csi_clo_config(clo_src_e, clo_div_e);

/** 
  \brief to set clock status in PM mode 
   when IWDT is enabled, trying to stop ISOSC in stop mode would be invalid
   refer to GCER in SYSCON chapter for detailed description
  \param[in] eClk: clock to be configured
  \param[in] bEnable: enable or disable
  \return none.
 */ 
//void csi_clk_pm_enable(clk_pm_e eClk, bool bEnable);

/** 
  \brief to calculate SCLK and PCLK frequence according to the current reg content
   tClkConfig.wSclk and tClkConfig.wPclk will be updated after excuting this function
  \param[in] none.
  \return csi_error_t.
 */
csi_error_t csi_calc_clk_freq(void);

/**
  \brief       Soc get sclk frequence.
  \param[in]   none
  \return      system clk frequence
*/
uint32_t csi_get_sclk_freq(void);

/**
  \brief       Soc get pclk frequence.
  \param[in]   none
  \return      Peripherals clk frequence
*/
uint32_t csi_get_pclk_freq(void);

/**
  \brief       Soc get coret frequence.
  \param[in]   none
  \return      coret frequence
*/
uint32_t soc_get_coret_freq(void);

/**
  \brief       Soc get bt frequence.
  \param[in]   byIdx	id of bt
  \return      bt frequence
*/
uint32_t soc_get_bt_freq(uint8_t byIdx);

/** 
  \brief       Soc get rtc frequence.
  \param[in]   none
  \return      rtc frequence
*/
uint32_t soc_get_rtc_freq(void);

/*
  \brief       Soc enable device clock
  \param[in]   eModule   clock module, defined in sys_clk.h, \ref clk_module_t
  \return      none
*/
void soc_clk_enable(csi_clk_module_e eModule);

/*
  \brief       Soc disable device clock
  \param[in]   eModule   clock module, defined in sys_clk.h, \ref clk_module_t
  \return      none
*/
void soc_clk_disable(csi_clk_module_e eModule);

/** \brief       timer set load times out
 *  \param[in]   wTimeOut: the timeout, unit: us, 20us < wTimeOut < 3S
 *  \param[in]   byBitMode: TIMER_16BIT_MODE  or TIMER_24BIT_MODE
 *  \return      none
*/
void apt_timer_set_load_value(uint32_t wTimesOut,uint8_t byBitMode);

/** \brief       get timer prdr load value
 *  \return      load prdr value 
*/
uint32_t apt_timer_get_prdrload_value(void);

/** \brief       get timer clk div
 *  \return      clk div 
*/
uint32_t apt_timer_get_clkdiv_value(void);



#endif /* _SYS_CLK_H_ */