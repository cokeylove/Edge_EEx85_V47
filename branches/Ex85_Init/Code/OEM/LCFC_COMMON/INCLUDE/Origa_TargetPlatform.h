#ifndef ORIGA_TARGETPLATFORM_H
#define ORIGA_TARGETPLATFORM_H
#define COUNTER16BIT
#include "Origa_DataTypes.h"



/* ****************************************************************************
   *** v0.90; 2008-11-10:  1st version.
   *** v0.93; 2009-03-25:  target platform include prepared for target specific
                           data.
                           macro parameters enclosed in parentheses.
   *** v1.00b; 2010-03-04: added calculations for receive and nvm timeout.
   *** v1.01; 2011-07-21:  compile switch handling enhanced.
                           System_DebugTimings() added for system debugging.
              2011-07-21:  COUNTER32BIT and COUNTER16BIT introduced.
   *** v1.02; 2011-07-21:  COUNTER8BIT added.
   ************************************************************************* */

/* --- AVAILABLE COMPILE SWITCHES FOR PREPROCESSOR ---

  Tool-Chains:
   KEIL: compile with Keil tool chain.
    or
   TASKING: compile with Tasking tool chain.

  Example Hosts:
   IFX_XC2267: if Infineon XC2267 MCU is used.
    or
   IFX_XC2365: if Infineon XC2365 MCU is used.
    or
   STM_STR912: if STM STR912 ARM9 MCU is used.

   COUNTER32BIT: use 32bit counters for pulse timings.
    or
   COUNTER16BIT: use 16bit counters for pulse timings.
    or
   COUNTER8BIT: use 8bit counters for pulse timings.

*/

/* CLIB version 1.03 */
#define ORIGA_CLIB_MAIN_VERSION   (1u)
#define ORIGA_CLIB_SUB_VERSION    (3u)



/* example target platform includes */
#ifdef TASKING
  #ifdef IFX_XC2267
    #define IFX_XC2000
  /* #include "sfr/regxc2267.sfr" */
  #elif defined IFX_XC2365
    #define IFX_XC2000
  /* #include "sfr/regxc2365.sfr" */
  #endif

#elif defined KEIL
  #ifdef IFX_XC2267
    #define IFX_XC2000
    #include "XC2267.h"
  #elif defined IFX_XC2365
    #define IFX_XC2000
    #include "XC2365.h"
  #endif

#elif defined STM_STR912
  #define STM_STR912_GPIO         (GPIO9)
  #define STM_STR912_GPIOPORT     (9u)
  #define STM_STR912_GPIOPIN_OUT   (7u)
  #define STM_STR912_GPIOPIN_IN    (6u)

#else
//  #error "ERROR: Either compiler or target host not defined!"
#endif



/* use counter length as requested 32 or 16 bit length */
/* note: use 16 bit counters for weak systems! */
#ifdef COUNTER32BIT
typedef ULONG COUNTER;
#elif defined COUNTER16BIT
typedef UWORD COUNTER;
#elif defined COUNTER8BIT
typedef UBYTE COUNTER;
#else
#error "ERROR: Elease specify counter size!"
#endif



/* system constants
   NOTE: XLAL is CPU target CPU frequeny in Hz */
/* STM_STR912 */
//#define SYSTEM_CPU_XTAL  (96000000UL)
/* XC2365 */
/* #define SYSTEM_CPU_XTAL  (66000000UL) */
/* XC2267 */
/* #define SYSTEM_CPU_XTAL  (20000000UL) */
#define SYSTEM_CPU_XTAL  80000000L //to be inline with UART

/* system dependent Functions
   NOTE: The cycles to delay-loop calculation formulas have to be trimmed to the used target platform!
  - calculate required number of loops to get a delay of "cycles" with function System_IdleDelay() */
#define System_CyclesToLoops(cycles)            (((cycles)-0u)/(5u))

/* calculate required number of loops to get a delay of "cycles" within function Swi_SendRawWord() */
#define System_CyclesToResponseTimeOut(cycles)  (((cycles)-0u)/(5u))
#define System_CyclesToReceiveTimeOut(cycles)   (((cycles)-0u)/(5u))

/* calculate microsec delay time to loops required to get delay with function System_IdleDelay() */
#define System_MicrosToCycles(microsec)         ((SYSTEM_CPU_XTAL/1000000UL)*(microsec))

/* calculate baudrate to cpu cycles */
#define System_SwiBaudDelayCycles(baudrate)     ((SYSTEM_CPU_XTAL/(baudrate))/2u)

/* calculate IRQ times with another formula, as timing loop is different */
#define System_CyclesToImmedIntTimeOut(cycles)  (((cycles)-0u)/(5u))
#define System_CyclesIrqTimeOut(cycles)         (((cycles)-0u)/(5u))


/* NOTE: timeout should be set to 20ms */
#define System_CyclesToNvmOut(cycles)          (((cycles)-0u)/(100u))


/* function prototypes */
void System_InitializeGpios( void );
void System_DisableIrqs( void );
void System_EnableIrqs( void );
void System_SetSwiPort( BIT bit_State );
BIT  System_GetSwiPort( void );

/* only for timing debug purpose! */
void System_DebugTimings( void );
//typedef unsigned long  DWORD;
#endif

