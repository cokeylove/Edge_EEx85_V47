/*-----------------------------------------------------------------------------
 * TITLE: OEM_EVENT.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_INPUT_H
#define OEM_INPUT_H

//-----------------------------------------------------------------------------
// OEM_Event.C Debounce time setting
//-----------------------------------------------------------------------------
#define T_AC_DEBOUNCE          1       // 10 ms (timebase: 10 ms)
#define T_PSW_DEBOUNCE         5      // 50 ms (timebase: 10 ms)
#define T_BAT_DEBOUNCE         15      // 150 ms (timebase: 10 ms)
#define T_PSWOFF               500   // 5 secs (timebase: 10ms) EC 5sec override turn off system power
#define T_EXTEVT_DEBOUNCE      10      // 100 ms (timebase: 10 ms)
#define T_LAN_WAKE_DEBOUNCE    30      // 300 ms (timebase: 10 ms)
#define T_KBFN_DEBOUNCE        1      // 10 ms (timebase: 10 ms)


#define EventMaxNumber         9      //

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
extern void EventManager(BYTE device_id);
extern void LanWakeLow(void);
extern void PSWPressed(void);
extern void PSWReleased(void);

struct sDebounce
{
  XBYTE  *REG;
  BYTE  Flag;
  XBYTE  *Cunter;
  BYTE  Time;
  FUNCT_PTR_V_V  press;
  FUNCT_PTR_V_V  release;
  FUNCT_PTR_V_V  nochange;
};

//=============================================================================

//-----------------------------------------------------------------------------
// Pre-charge Voltage
//-----------------------------------------------------------------------------

#if (PLATFORM_CHIP == INTEL)      // 12V (wake up voltage)
#define  ChargeVoltL_L      0xE0 
#define  ChargeVoltL_H      0x2E
                                  // 16V (wake up voltage)
#define  ChargeVoltL_L_15   0x80                    
#define  ChargeVoltL_H_15   0x3E
#endif
	
#if(PLATFORM_CHIP == AMD)		  // 12V (wake up voltage) 
#define  ChargeVoltL_L      0xE0
#define  ChargeVoltL_H      0x2E

#define  ChargeVoltL_L_15   0xE0                    
#define  ChargeVoltL_H_15   0x2E
#endif
//=============================================================================

#endif
