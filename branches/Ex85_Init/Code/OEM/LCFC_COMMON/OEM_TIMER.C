/*-----------------------------------------------------------------------------
 * TITLE: OEM_TIMER.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// Enable internal watch dog and waiting EC reset
//----------------------------------------------------------------------------
void InternalWDTNow(void)
{
  //
  // Watchdog would be failed if EC debug board is connected.
  //
  CKCON = 0x00;        // set 26 bit counter // bit7-6 00=14ms,01=113ms, 10=911ms, 11(1/9.2MHz)*2'26=7.2S
  WDTCON|=0x01;   // WDTRST = 1 Reset watch dog timer.
  WDTCON|=0x02;   // WDTEN  = 1 Enable watch dog.
  while(1);       // Wait for watch dog time-out
}

//----------------------------------------------------------------------------
// Enable internal watch dog
//----------------------------------------------------------------------------
void EnableInternalWDT(void)
{
  CKCON |= 0xC0;        // set 26 bit counter // bit7-6 00=14ms,01=113ms, 10=911ms, 11(1/9.2MHz)*2'26=7.2S
  WDTRST = 1;        // reset internal watch dog
  WDTEB = 1;        // enable internal watch dog
}

//----------------------------------------------------------------------------
// Disable internal watch dog
//----------------------------------------------------------------------------
void DisableInternalWDT(void)
{
  WDTEB = 0;        // Disable internal watch dog
}

//----------------------------------------------------------------------------
// Disable internal watch dog
//----------------------------------------------------------------------------
void ResetInternalWDT(void)
{
  WDTRST = 1;        // Reset internal watch dog counter
}

void SystemAutoOn(void)
{
  if( (AutoTimer&0x7F) == 0x01 )
  {
    AutoTimer--;
    if( IS_MASK_SET(AutoTimer,b7AutoBootTm) )
    {
      AutoTimer = 0;  // Clear Auto timer.
      PWSeqStep = 1;
      PowSeqDelay = 0x00;
      SysPowState=SYSTEM_S5_S0;
    }
    else
    {
      if ( SystemIsS5 )
      { AutoTimer = 0x83; }
      else
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(BOOTFAIL_ID);
        AutoTimer = 0x83;

#if (PLATFORM_CHIP == INTEL)
        PWSeqStep = 1;
        PowSeqDelay = 0x00;
        SysPowState=SYSTEM_S0_S5;
#else // PLATFORM_CHIP
        pAMDPWRBTNCnt = AMDPWRBTNWK;
#endif // PLATFORM_CHIP
      }
    }
  }
  else
  {
    if( (AutoTimer&0x7F) > 0x01 )
    {
      if( IS_MASK_SET(AutoTimer,b7AutoBootTm) )
      {
        if ( SystemIsS5 )
        { AutoTimer--; }
      }
      else
      { AutoTimer--; }
    }
  }
}

/* ========================================================================
 * Function Name : ChkTimeScale
 * Time count for 1 byte,max 2.55 Second
 * Input : PORT_BYTE_PNTR countadr  Counter pointer
 *          BYTE timeout          Timer scale
 * Output: FLAG TRUE     Timer count to timer scale
 *            FALSE    Timer don't count to timer scale
 * ======================================================================== */
FLAG ChkTimeScale(PORT_BYTE_PNTR countadr, BYTE timeout)
{
#if SW_ChkTimeScale
  if( ( *countadr )++ >= timeout )
  {
    ( *countadr ) = 0;
    return(TRUE);
  }
  return(FALSE);
#endif  // End of SW_ChkTimeScale
}

