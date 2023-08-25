/***********************************************************************//** 
 * \file  tkey_parameter.c
 * \brief  csi tkey parameter
 * \copyright Copyright (C) 2020-2025 @ ASMCHIP
 * <table>
 * <tr><th> Date  		<th>Version  	<th>Author  		<th>Description
 * <tr><td> 2023-08-18 	<td>V1.07  		<td>ASM AE Team   	<td>initial
 * </table>
 * *********************************************************************
*/ 

#include <drv/tkey.h>


/****************************************************
//TCHx GPIO Map
*****************************************************/  
/*
  ------------------     ------------------     ------------------     ------------------
 | PIN    | tkey CH |   | PIN    | tkey CH |   | PIN    | tkey CH |   | PIN    | tkey CH |   
 |--------|---------|   |--------|---------|   |--------|---------|   |--------|---------|
 | PA0.14 |  TCH0   |   | PA1.6  |  TCH8   |   | PB0.10 |  TCH16  |   | PA0.0  |  TCH24 *|
 |--------|---------|   |--------|---------|   |--------|---------|   |--------|---------|
 | PA0.15 |  TCH1   |   | PB0.5  |  TCH9   |   | PB0.11 |  TCH17  |   | PA0.1  |  TCH25  |
 |--------|---------|   |--------|---------|   |--------|---------|   |--------|---------|
 | PA1.0  |  TCH2   |   | PA1.7  |  TCH10 *|   | PA1.9  |  TCH18  |   | PA0.2  |  TCH26  |
 |--------|---------|   |--------|---------|   |--------|---------|   |--------|---------|
 | PA1.1  |  TCH3   |   | PB0.6  |  TCH11  |   | PA1.10 |  TCH19  |   | PA0.3  |  TCH27  |
 |--------|---------|   |--------|---------|   |--------|---------|   |--------|---------|
 | PA1.2  |  TCH4   |   | PB0.7  |  TCH12  |   | PA1.11 |  TCH20  |   | PA0.4  |  TCH28  |
 |--------|---------|   |--------|---------|   |--------|---------|   |--------|---------|
 | PA1.3  |  TCH5   |   | PA1.8  |  TCH13 *|   | PA1.12 |  TCH21  |   | PB0.0  |  TCH29  |
 |--------|---------|   |--------|---------|   |--------|---------|   |--------|---------|
 | PA1.4  |  TCH6   |   | PB0.8  |  TCH14  |   | PA1.13 |  TCH22  |   | PB0.1  |  TCH30  |
 |--------|---------|   |--------|---------|   |--------|---------|   |--------|---------|
 | PA1.5  |  TCH7   |   | PB0.9  |  TCH15  |   | PC0.0  |  TCH23  |   | PB0.2  |  TCH31  |
  ------------------     ------------------     ------------------     ------------------
 */

//Pins with * are special pins,Special pin description:
//TCH10 --->  swdio   TCH13 ---> swclk    TCH24 ---> fvr


/****************************************************
//define
*****************************************************/
//#define _110_Slider_Function


void csi_tkey_parameter_init(void)
{

/****************************************************
//TK basic parameters
*****************************************************/
	dwTkeyIoEnable = TCH_EN(0)|TCH_EN(1)|TCH_EN(2)|TCH_EN(3)|TCH_EN(4);
	byTkeyGlobalSens = 2;								//TK Global Tkey Sensitivity,0=invalid;
	hwTkeyGlobalTrigger = 50;							//TK Global Tkey Trigger,0=invalid;
	byTkeyGlobalIcon = 5;								//TK Global Tkey Icon,0=invalid;
	byPressDebounce = 5;								//Press debounce 1~10
	byReleaseDebounce = 5;							//Release debounce 1~10
	byKeyMode = 1;									//Key mode 0=first singlekey,1=multi key,2=strongest single-key
	byMultiTimesFilter = 0;							//MultiTimes Filter,>4 ENABLE <4 DISABLE
	byValidKeyNum = 4;								//Valid Key number when touched
	byBaseUpdateSpeed = 10;							//baseline update speed
	byTkeyRebuildTime = 16;							//longpress rebuild time = byTkeyRebuildTime*1s  0=disable
	hwTkeyBaseCnt = 59999;							//10ms  byTkeyBaseCnt=10ms*48M/8-1,this register need to modify when mcu's Freq changed
/****************************************************
//TK parameter fine-tuning
*****************************************************/
	byTkeyFineTurn = DISABLE;							//Tkey sensitivity fine tuning ENABLE/DISABLE
	byTkeyChxSens[0] = 3;								//TCHx manual Sensitivity
	byTkeyChxSens[1] = 3;								//TCHx manual Sensitivity
	byTkeyChxIcon[0] = 5;								//TCHx manual ICON
	byTkeyChxIcon[1] = 5;								//TCHx manual ICON
	byTkeyChxTrigger[0] = 60;							//TCHx manual Trigger Level
	byTkeyChxTrigger[1] = 60;							//TCHx manual Trigger Level
/****************************************************
//TK special parameter define
*****************************************************/
	hwTkeyPselMode = TK_PWRSRC_AVDD;					//tk power sel:TK_PWRSRC_FVR/TK_PWRSRC_AVDD   when select TK_PSEL_FVR PA0.0(TCH24) need a 104 cap
	hwTkeyEcLevel = TK_ECLVL_3V;						//C0 voltage sel:TK_ECLVL_1V/TK_ECLVL_2V/TK_ECLVL_3V/TK_ECLVL_3V6
	hwTkeyFvrLevel = TK_FVR_NONE;						//FVR level:TK_FVR_2048V/TK_FVR_4096V/TK_FVR_NONE
/****************************************************
//TK low power function define
*****************************************************/
	byTkeyLowPowerMode = ENABLE;						//touch key can goto sleep when TK lowpower mode enable
	byTkeyLowPowerLevel = 2;							//0=125ms 1=175ms 2=250ms 3=325ms 4=400ms 5=500ms 6=1s,7=2s;>=50 Can set exact time
	dwTkeyWakeupIoEnable = TCH_EN(0)|TCH_EN(1);			//Sleep wakeup TCHx configuration, Support single or multiple TCHx wake-up
/****************************************************
//TK Hidden function define
*****************************************************/
	//Used to modify the default state when TCHx is not enabled;0/3=Z,1=high,2=low(Default)
	//byTkeyDstStatus = 1;

	//Used to enable the default internal resistor of TCHx;0=disable(Default),1=enable
	//byTkeyIntrStatus = 1;	

	//Used to modify the scan interval;0=none(Default),1=16ms,2=32ms,3=64ms,4=128ms,>4=186ms
	//byTkeyImtvtimTime = 1;
	
	//Used to modify the Bounce of the forced negative Rebuild;>=1 (Default=3)
	//The higher the number, the longer it takes to trigger the update mechanism
	//byTkeyNegBuildBounce = 10;

	//Used to modify the Bounce of the forced postive Rebuild;>=1 (Default=3)
	//The higher the number, the longer it takes to trigger the update mechanism
	//byTkeyPosBuildBounce = 10;	
	
#ifdef _110_Slider_Function
/****************************************************
//TK slider/Wheel parameter define(support slider library only)
*****************************************************/
/*
    Take slider0 as an example,when the order of the slider0 is: TCH3→7→9→0, as shown below
     ______ _______ _______ _______
    |      \\      \\      \\      |
    | TCH3 // TCH7 // TCH9 // TCH0 |
    |      \\      \\      \\      |
    |______//______//______//______|
	
    byTkeySlider0Seq[0]~[3] need to be set to 3/7/9/0:

        byTkeySlider0Seq[0] = 3;
        byTkeySlider0Seq[1] = 7;
        byTkeySlider0Seq[2] = 9;
        byTkeySlider0Seq[3] = 0;

	The configuration of the Wheel is similar to the slider:

        byTkeySlider0Function = ENABLE;

	Select the right gear according to your needs：

        byTkeySlider0Level = 255;

	The configuration of Wheel is similar to that of reference slider。
    When the slider or Wheel function is used, byKeyMode must be set to 1, that is, the multi key function is used
*/
//-------------------slider 0---------------------------
	byTkeySlider0Function = DISABLE;	// ENABLE/DISABLE
	byTkeySlider0Level = 255;			//255/128/64
	byTkeySlider0Seq[0] = 3;
	byTkeySlider0Seq[1] = 2;
	byTkeySlider0Seq[2] = 1;
	byTkeySlider0Seq[3] = 0;
//-------------------slider 1---------------------------
	byTkeySlider1Function = DISABLE;	// ENABLE/DISABLE
	byTkeySlider1Level = 255;           //255/128/64
	byTkeySlider1Seq[0] = 10;
	byTkeySlider1Seq[1] = 9;
	byTkeySlider1Seq[2] = 8;
	byTkeySlider1Seq[3] = 7;
//-------------------wheel---------------------------
	byTkeyWheelFunction = DISABLE;		// ENABLE/DISABLE
	byTkeyWheelLevel = 128;				//128/64
	byTkeyWheelSeq[0] = 15;								
	byTkeyWheelSeq[1] = 14;
	byTkeyWheelSeq[2] = 6;
	byTkeyWheelSeq[3] = 5;	
#endif	
}


