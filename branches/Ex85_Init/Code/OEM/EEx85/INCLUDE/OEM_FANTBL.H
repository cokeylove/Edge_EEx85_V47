/*-----------------------------------------------------------------------------
 * TITLE: OEM_FANTBL.H
 *
 * Author : ITE
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef  OEM_FANTBL_H
#define OEM_FANTBL_H

// AVC Fan
#define FAN_PCH_Max_AVC105_14   Cal_PWM_Duty(95,FAN_PWM_Max)  // 95% PCH PWM Max. (AVC)   Tj105 14"
#define RPM_PCH_Max_AVC105_14	39							  // 3900 PCH Max RPM. (AVC)   Tj105 14"

#define FAN_PCH_Max_AVC100_14   Cal_PWM_Duty(95,FAN_PWM_Max)  // 95% PCH PWM Max. (AVC)   Tj100 14"
#define RPM_PCH_Max_AVC100_14	39							  // 3900 PCH Max RPM. (AVC)   Tj100 14"

#define FAN_PCH_Max_AVC105_15   Cal_PWM_Duty(80,FAN_PWM_Max)  // 80% PCH PWM Max. (AVC)   Tj105 15"
#define RPM_PCH_Max_AVC105_15	35							  // 3500 PCH Max RPM. (AVC)   Tj105 15"

#define FAN_PCH_Max_AVC100_15   Cal_PWM_Duty(80,FAN_PWM_Max)  // 80% PCH PWM Max. (AVC)   Tj100 15"
#define RPM_PCH_Max_AVC100_15	35							  // 3500 PCH Max RPM. (AVC)   Tj100 15"

// Delta Fan
#define FAN_PCH_Max_Delta105_14 Cal_PWM_Duty(74,FAN_PWM_Max)  // 74% PCH PWM Max. (Delta) Tj105 14"
#define RPM_PCH_Max_Delta105_14	40							  // 4000 PCH Max RPM. (Delta) Tj105 14"

#define FAN_PCH_Max_Delta100_14 Cal_PWM_Duty(74,FAN_PWM_Max)  // 74% PCH PWM Max. (Delta) Tj100 14"
#define RPM_PCH_Max_Delta100_14	40							  // 4000 PCH Max RPM. (Delta) Tj100 14"

#define FAN_PCH_Max_Delta105_15 Cal_PWM_Duty(71,FAN_PWM_Max)  // 71% PCH PWM Max. (Delta) Tj105 15"
#define RPM_PCH_Max_Delta105_15	37							  // 3700 PCH Max RPM. (Delta) Tj105 15"

#define FAN_PCH_Max_Delta100_15 Cal_PWM_Duty(71,FAN_PWM_Max)  // 71% PCH PWM Max. (Delta) Tj100 15"
#define RPM_PCH_Max_Delta100_15	37							  // 3700 PCH Max RPM. (Delta) Tj100 15"

//Sunon Fan
#define FAN_PCH_Max_SUN105_14   Cal_PWM_Duty(52,FAN_PWM_Max)  // 62% PCH PWM Max. (SUN)   Tj105 14"
#define RPM_PCH_Max_SUN105_14	40							  // 4000 PCH Max RPM. (SUN)   Tj105 14"

#define FAN_PCH_Max_SUN100_14	Cal_PWM_Duty(52,FAN_PWM_Max)  // 68% PCH PWM Max. (SUN)   Tj100 14"
#define RPM_PCH_Max_SUN100_14	40							  // 4000 PCH Max RPM. (SUN)   Tj100 14"

#define FAN_PCH_Max_SUN105_15   Cal_PWM_Duty(49,FAN_PWM_Max)  // 62% PCH PWM Max. (SUN)   Tj105 15"
#define RPM_PCH_Max_SUN105_15	37							  // 3600 PCH Max RPM. (SUN)   Tj105 15"

#define FAN_PCH_Max_SUN100_15	Cal_PWM_Duty(49,FAN_PWM_Max)  // 68% PCH PWM Max. (SUN)   Tj100 15"
#define RPM_PCH_Max_SUN100_15	37							  // 4000 PCH Max RPM. (SUN)   Tj100 15"

#define FAN_PWM_Min      Cal_PWM_Duty(30,FAN_PWM_Max)   // 30% Standard PWM Minimum.
#define FAN_Std_RR1      Cal_PWM_Duty(1,FAN_PWM_Max)    // 1% Ramp rate1.
#define FAN_Std_RR2      Cal_PWM_Duty(2,FAN_PWM_Max)    // 2% Ramp rate2.
#define FAN_duty_offset  Cal_PWM_Duty(5,FAN_PWM_Max)    // 5% Duty cycle offset to target for duty ramp rate stting.

extern void oem_FanTblPtrHook(void);

#endif
