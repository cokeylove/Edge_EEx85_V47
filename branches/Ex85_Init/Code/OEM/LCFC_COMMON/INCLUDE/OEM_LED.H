/*-----------------------------------------------------------------------------
 * TITLE: OEM_LED.H
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/

#ifndef OEM_LED_H
#define OEM_LED_H

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
extern void OEM_Write_Leds(BYTE data_byte);
extern void TPG_LED(void);
extern void TPG_LED_control(void);
extern void LedPowerOn(void);
extern void LedPowerOff(void);
#if Support_MuteLED
extern void MuteLED(void);
#endif //Support_MuteLED
#if Support_FnLKLED
extern void FnLKLED(void);
#endif //Support_FnLKLED
#if Support_Microphone_Mute_LED
extern void Microphone_Mute_LED(void);
#endif //Support_Microphone_Mute_LED
extern BYTE ACin_LED(void);
extern void LED_SLOW_BLINK(void);
extern void AC_Charging_LED (void);
extern void Charging_LED_Blink(void);
#if Support_KBD_BL_LED
extern void KbdBacklightLED(void);
#endif //Support keyboard backlight

//LED ID
#define  POWER_LED           0
#define  Battery_LED_0       1 // battery Orange LED
#define  Battery_LED_1       2 // battery Green LED
//                      3-6: reserved
#define  Suspend_LED         7
#define  Dock_LED_1          8
#define  Dock_LED_2          9
#define  THINKPAD_LOGO_LED   10
//                      11-12: reserved
#define  Microphone_mute_LED 14

#define FNLED_ONOFF(x)    ( x== 1 ? FN_LED_ON() : FN_LED_OFF() )

#define S3POWERLED_OFF_Time  60  //60 * 50mS = 3S

#define LOGO_LED    DCR0
#if Support_PWRBTN_LED
#define PWRBTN_LED  DCR3
#endif //Support_PWRBTN_LED

#if Support_KBD_BL_LED
#define KBD_BL_LED_PWM_Max  0xC8//CTR2
#define KBD_BL_LED          DCR1
#define KBD_BL_LED_PWM_REG  GPCRA1
#define KBD_BL_LED_HIGH  		100	//Duty 100%
#define KBD_BL_LED_LOW  		30	//Duty 30%
#endif //Support keyboard backlight

#define LED_ONOFF(x)    ( x== 1 ? LED_ON : LED_OFF )
#define LED_ON      0xFF
#define LED_OFF     0x00


//charging LED
#if (Support_BATTCHGLED)
#define Charging_LED_OFF {BATTCHG_LED_OUTPUT;	BATTCHG_LED_OFF();  PWRSTATUS_LED_OFF();}
#define Charging_LED_G   {BATTCHG_LED_OUTPUT;	BATTCHG_LED_OFF();  PWRSTATUS_LED_ON();}  
#define Charging_LED_A   {BATTCHG_LED_OUTPUT;	PWRSTATUS_LED_OFF(); BATTCHG_LED_ON();} 
#define Charging_LED_Green  90
#define Charging_LED_Amber  20
#endif  //(Support_BATTCHGLED && Support_PWRSTATUSLED)


#endif
