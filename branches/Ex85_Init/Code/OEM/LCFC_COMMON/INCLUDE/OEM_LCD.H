/*-----------------------------------------------------------------------------
 * TITLE: OEM_LCD.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_LCD_H
#define OEM_LCD_H

/*===========================================================================
 * PWM mode definition
 *===========================================================================*/
#define LCD_PWM_Frequency  6150  // PWM base frequency.
#define BRIGHT_MAX_STEP    0x10  // Max. PWM duty step.
#define WIN8_BRI_MAX_STEP  0x15  // Max. PWM duty step.
#define BriTable_MAX       12    // Max. panel value.

#define _LID_DEBOUNCE      2

extern BYTE Cal_PWM_Duty(BYTE BCalDuty, BYTE BPWMBase);  // Calculate PWM duty.
extern void Init_LCD_Freq(void);  // PWM frequency.
/* ========================================================================
 * Brightness table define here
 * ======================================================================== */
extern BYTE code Bri_Table[BriTable_MAX][BRIGHT_MAX_STEP+4];
extern BYTE code WIN8_Bri_Tab[BriTable_MAX][WIN8_BRI_MAX_STEP+4];
extern void Brightness_Control(void);
extern void Backlight_Control(void);
extern void Lid_Process(void);
#if NV_GC6
extern void GC6Process(void);
#endif // NV_GC6
#endif
