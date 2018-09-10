/*-----------------------------------------------------------------------------
 * TITLE: OEM_BEEP.H
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/

#ifndef OEM_BEEP_H
#define OEM_BEEP_H

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------

extern void BeepControl(void);

#define channel_group  7       // it used CTR0
extern void PwmModulator(BYTE percent, WORD freq, BYTE pwm_ch);
extern void PwmModulator_2(BYTE percent, WORD freq, BYTE pwm_ch);
extern void Keyboard_beep(void);

/*===========================================================================
 * EDGE Beep Sound
 *===========================================================================*/
#define BEEP_315HZ     PwmModulator(50,148,6)      //PwmModulator(50,(315/2)-10,6) CRITICAL_LOW_BATTERY
#define BEEP_440HZ     PwmModulator(50,220,6)	   //PwmModulator(50,440/2,6)
#define BEEP_1250HZ    PwmModulator(50,625,6)      //PwmModulator(50,1250/2,6) LOW_BATTERY,// SUSPEND,// VM_SUSPEND,// RESUME,// POWER_OFF
#define BEEP_1400HZ    PwmModulator(50,700,6)      //PwmModulator(50,1400/2,6) DC_IN_OUT
#define BEEP_1760HZ    PwmModulator(50,880,6)      //PwmModulator(50,1760/2,6) DC_IN_OUT,// POWER_OFF_WARN,// NO_HDD_WARN

//--------------------Audio Control Function---------------------------------------
#define audBEEPedgeon  (GPCRA6 = ALT)
#define audBEEPon      (GPCRA6 = ALT; PwmModulator(50,500,1);)  // active Low//detect audio's beep sound
#define audBEEPoff     (GPCRA6 = PortA6_Ctrl)

#endif
