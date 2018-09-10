
#include "Origa_DataTypes.h"
#include "Origa_TargetPlatform.h"
#include "Origa_Swi.h"

extern COUNTER g_cntBaudLow;
extern COUNTER g_cntBaudHigh;
extern COUNTER g_cntBaudStop;
extern COUNTER g_cntResponseTimeOut;
extern ULONG g_ulImmediateIntTimeOut;
extern const ULONG g_culIrqTimeout;


/* ****************************************************************************
   name:      System_SetSwiPort()

   function:  set the defined SWI port pin to selected bit state.
              replaces: #define System_SetSwiPort(state) (P0_OUT_P6 = (state))
                        e.g. for XC2000: (P0_OUT_P6 = (state))

   input:     IN: bit_State
                new state of the SWI port.
   output:    -

   return:    -

   date:      1.01; 2011-07-09: compile switch evaluation IFX and ARM9 host
                                added.
   ************************************************************************* */
void System_SetSwiPort( BIT bit_State )
{

#ifdef IFX_XC2000

  // P0_OUT_P6 = bit_State;
  P4_OUT_P0 = bit_State;

#elif defined STM_STR912

  /* set port */
  if( bit_State == 0u )
  {
    STM_STR912_GPIO->DR[((1u << STM_STR912_GPIOPIN_OUT) << 2u)] &= ~(1u << STM_STR912_GPIOPIN_OUT);
  }
  else
  {
    STM_STR912_GPIO->DR[((1u << STM_STR912_GPIOPIN_OUT) << 2u)] |= (1u << STM_STR912_GPIOPIN_OUT);
  }

#else

#error "Please define target host!"

#endif

}



/* ****************************************************************************
   name:      System_GetSwiPort()

   function:  get the sate of the SWI port pin.
              replaces: (P0_IN_P6)
                          e.g. for XC2000: (P0_IN_P6)

   input:     -
   output:    BIT

   return:    state of the SWI port.

   date:      1.01; 2011-07-09: compile switch evaluation IFX and ARM9 host
                                added.
   ************************************************************************* */
BIT System_GetSwiPort( void )
{

#ifdef IFX_XC2000

  //return P0_IN_P6;
  return P4_IN_P0;

#elif defined STM_STR912

  /* get port state */
  if( (STM_STR912_GPIO->DR[((1u << STM_STR912_GPIOPIN_IN) << 2u)] & (1u << STM_STR912_GPIOPIN_IN)) != 0u )
  {
    return 1;
  }
  else
  {
    return 0;
  }

#else

#error "Please define target host!"

#endif

}



/* ****************************************************************************
   name:      System_InitializeGpios()

   function:  pin port definitions like open drain configuration and driver
              strength settings have to be done within this function.

   input:     -
   output:    -

   return:    -

   date:      1.01; 2011-07-09: compile switch evaluation IFX and ARM9 host
                                added.
   ************************************************************************* */
void System_InitializeGpios( void )
{

#ifdef IFX_XC2000

  /* push output to high before doing changes to avoid spike. */
  System_SetSwiPort(1u);

  /* set port control register P0.6 as open drain. */
  //P0_IOCR06 =  0x00C0u;
  P4_IOCR00 =  0x00C0u;      // set port control register P4.0 as open drain.

#elif defined STM_STR912

  /* select output pad type as open drain */
  SCU->GPIOTYPE[STM_STR912_GPIOPORT] |= (1u << STM_STR912_GPIOPIN_OUT);
  /* select input pad type as push/pull */
  SCU->GPIOTYPE[STM_STR912_GPIOPORT] &= ~(1u << STM_STR912_GPIOPIN_IN);

  /* set pin sates */
  STM_STR912_GPIO->DDR |= (1u << STM_STR912_GPIOPIN_OUT);
  STM_STR912_GPIO->DDR &= ~(1u << STM_STR912_GPIOPIN_IN);

#else

#error "Please define target host!"

#endif

}



/* ****************************************************************************
   name:      System_DisableIrqs()

   function:  disalbe IRQs from host system.
              this function is called before SWI transaction to be able to have
              an not interrupred send process.
              NOTE: if the host must not be blocked due to system
                    considerations, then the content of the function has to
                    be erased to get an empty function.

   input:     -
   output:    -

   return:    -

   date:      1.01; 2011-07-09: compile switch evaluation IFX and ARM9 host
                    added.
   ************************************************************************* */
void System_DisableIrqs( void )
{

#ifdef IFX_XC2000

  PSW_IEN = 0u;

#elif defined STM_STR912

  /* disable IRQ if needed */

#else

#error "Please define target host!"

#endif

}



/* ****************************************************************************
   name:      System_EnableIrqs()

   function:  enable IRQs from host system.
              this function is called after SWI transactions to be re-able
              host interrupts.
              NOTE: if the host must not be blocked due to system
                    considerations, then the content of the function has to
                    be erased to get an empty function.

   input:     -
   output:    -

   return:    -

   date:      1.01; 2011-07-09: compile switch evaluation IFX and ARM9 host
                                added.
   ************************************************************************* */
void System_EnableIrqs( void )
{

#ifdef IFX_XC2000

  PSW_IEN = 1u;

#elif defined STM_STR912

  /* enable IRQ if disabled before */

#else

#error "Please define target host!"

#endif

}



/* ****************************************************************************
   name:      System_DebugTimings()

   function:  debug delay timings.
              the function toggles the SWI GPIO with following timings:
              1.) low  for 1TAU  -->short
              2.) high for 3TAU  -->long
              3.) low  for 5TAU  -->stop
              4.) high for 11TAU -->irq timeout
              5.) low  for 11TAU -->immed timeout
              6.) high for 200us -->powerdown time
              7.) low for  1ms   -->timebase check

   input:     -
   output:    -

   return:    -

   date:      1.02; 2011-07-21: added to better adjust SWI timings.
   ************************************************************************* */
void System_DebugTimings( void )
{

  COUNTER cntTimeOut;
  volatile ULONG ulTimeOut;


  /* - low for 1TAU  -->short */
  System_SetSwiPort(0u);
  System_IdleDelay( g_cntBaudLow );

  /* - high for 3TAU  -->long */
  System_SetSwiPort(1u);
  System_IdleDelay( g_cntBaudHigh );

  /* - low  for 5TAU  -->stop */
  System_SetSwiPort(0u);
  System_IdleDelay( g_cntBaudStop );

  /* - high for 11TAU -->response timeout */
  System_SetSwiPort(1u);
  cntTimeOut = g_cntResponseTimeOut;
  while( (System_GetSwiPort() == 1u) && (cntTimeOut != 0u) )
  {
    cntTimeOut--;
  }

  /* part from Swi_WaitForIrq() */
  /* - low  for 11TAU -->immed timeout */
  System_SetSwiPort(0u);
  ulTimeOut = g_ulImmediateIntTimeOut;
  /* part from Swi_WaitForIrq() */
  while( ulTimeOut != 0u )
  {
    if(System_GetSwiPort() == 1u) /* must be never true! */
    {
      break;
    }
    ulTimeOut--;
  }

  /* - low long IRQ timeout with 200ms */
  System_SetSwiPort(1);
  ulTimeOut = g_culIrqTimeout;
  /* part from Swi_WaitForIrq() */
  while( ulTimeOut != 0u )
  {
    if(System_GetSwiPort() == 0u) /* must be never true! */
    {
      break;
    }
    ulTimeOut--;
  }



  /* - high for 200us -->powerdown timoke */
  System_SetSwiPort(0u);
#ifdef COUNTER16BIT
  #if( (System_CyclesToLoops( System_MicrosToCycles(200u))) > UWORD_MAX )
  #error "ERROR: 200us counter loop setup does not fit into 16bit counter!"
  #endif
#elif defined COUNTER8BIT
  #if( (System_CyclesToLoops( System_MicrosToCycles(200u))) > UBYTE_MAX )
  #error "ERROR: 200us counter loop setup does not fit into 8bit counter!"
  #endif
#endif
  System_IdleDelay( (COUNTER)(System_CyclesToLoops(System_MicrosToCycles(200u))) );

  /*- low for  1ms   -->timebase check */
  System_SetSwiPort(1u);
#ifdef COUNTER16BIT
  #if( (System_CyclesToLoops( System_MicrosToCycles(1000u))) > UWORD_MAX )
  #error "ERROR: 1ms counter loop setup does not fit into 16bit counter!"
  #endif
#elif defined COUNTER8BIT
  #if( (System_CyclesToLoops( System_MicrosToCycles(1000u))) > UBYTE_MAX )
  #error "ERROR: 1ms counter loop setup does not fit into 8bit counter!"
  #endif
#endif
  System_IdleDelay( (COUNTER)(System_CyclesToLoops(System_MicrosToCycles(1000u))) );

  /*- end */
  System_SetSwiPort(0u);

}

