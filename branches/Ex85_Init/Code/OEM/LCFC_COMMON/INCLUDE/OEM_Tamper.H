/*-----------------------------------------------------------------------------
 * TITLE: OEM_Tamper.H
 *
 * Author : Ivan Chen
 *
 * Note : 
 *
 *---------------------------------------------------------------------------*/

#ifndef OEM_Tamper_H
#define OEM_Tamper_H



/**********************************************************
*     #EC ram define
**********************************************************/
#define  Tamper_status      Tamper_ram0           
#define  Tamper_Dev_Ste     Tamper_ram1
// BIT7~4 last status
// BIT0~3 current status
 #define  Tamper_BayHDD    BIT0
 #define  Tamper_SSD       BIT1
 #define  Tamper_HDD       BIT2
 #define  Tamper_DEBOUNCE  BIT3
#define  Tamper_Dev_temp     Tamper_ram2

#define Tamper_ADC    1   //detect by ADC
#define Tamper_GPIO   2   //detect by GPIO

extern void Tamper_DET(void);

#define TamperADC_LV0      993
#define TamperADC_LV1      923
#define TamperADC_LV2      831
#define TamperADC_LV3      744
#define TamperADC_LV4      671
#define TamperADC_LV5      612
#define TamperADC_LV6      572
#define Tamper_detect_tolerance 8

#endif
