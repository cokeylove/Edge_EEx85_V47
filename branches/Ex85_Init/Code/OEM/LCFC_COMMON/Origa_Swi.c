
#include "Origa_TargetPlatform.h"
#include "Origa_Swi.h"

/* *** global definitions *** */
ULONG g_ulBaudrate =                     SWI_BAUDRATE_DEF;

COUNTER g_cntBaudLow =                   System_CyclesToLoops(System_SwiBaudDelayCycles(SWI_BAUDRATE_DEF));
COUNTER g_cntBaudHigh =              3u * System_CyclesToLoops(System_SwiBaudDelayCycles(SWI_BAUDRATE_DEF));
COUNTER g_cntBaudStop =              6u * System_CyclesToLoops(System_SwiBaudDelayCycles(SWI_BAUDRATE_DEF));

COUNTER g_cntResponseTimeOut =      11u * System_CyclesToResponseTimeOut(System_SwiBaudDelayCycles(SWI_BAUDRATE_DEF));
COUNTER g_cntReceiveTimeOut =       11u * System_CyclesToReceiveTimeOut(System_SwiBaudDelayCycles(SWI_BAUDRATE_DEF));

ULONG g_ulImmediateIntTimeOut =     11u * System_CyclesToImmedIntTimeOut(System_SwiBaudDelayCycles(SWI_BAUDRATE_DEF));
/* g_culIrqTimeout is never changed */
const ULONG g_culIrqTimeout =             System_CyclesIrqTimeOut(System_MicrosToCycles(200000)); /* 200ms */



/* ****************************************************************************
   name:      System_IdleDelay()

   function:  create delay times required for low-level signal shaping.
              the idle time is done by performing a given number of
              software loops.
              NOTE: the correct values have to be tweaked for every target
                    host processor!
              NOTE: please take a loop at the assembly code output of the
                    compiler. some compilers may remove the loop completely,
                    if a too agressive optimization level is selected!

   input:     -
   output:    -

   return:    -

   date:      v1.01; 2009-06-04: replaced ULONG ul_Loops by adaptive parameter.
                                 COUNTER cnt_Loops
   ************************************************************************* */
void System_IdleDelay( volatile COUNTER cnt_Loops )
{
  for( ; cnt_Loops != 0u; cnt_Loops-- )
  {
    /* NOTE: add code, if further delay required */
  }
}



/* ****************************************************************************
   name:      Swi_GetBaudrate()

   function:  get current selected baudrate in bps.

   input:     -
   output:    ULONG

   return:    used baudrate.

   date:      .
   ************************************************************************* */
/*
ULONG Swi_GetBaudrate( void )
{
  return g_ulBaudrate;
}
*/


/* ****************************************************************************
   name:      Swi_SetBaudrate()

   function:  change baudrate that has to be used for ORIGA communication.

   input:     IN: ul_Baudrate
                new baudrate in bits per second.
   output:    bool

   return:    true, if new setting has been applied.
              false, if requested setting was out of spec.

   date:      v1.01; 2009-06-03: g_ulWaitForIRQ and g_cntBaudSysCycles removed.
                                 16bit counter support added.
   ************************************************************************* */
/*
BOOL Swi_SetBaudrate( ULONG ul_Baudrate )
{

  ULONG ulSysCycles;
  ULONG ulRespIntTimeOut;
  ULONG ulReceiveTimeOut;
  ULONG ulImmIntTimeOut;


  // check for spec limits
  if( (ul_Baudrate > SWI_BAUDRATE_MAX) || (ul_Baudrate < SWI_BAUDRATE_MIN) )
  {
    return FALSE;
  }


  // get base values. apply changes after checks passed!
  ulSysCycles = System_CyclesToLoops( System_SwiBaudDelayCycles( g_ulBaudrate ) );
  ulRespIntTimeOut = 11u * System_CyclesToResponseTimeOut(System_SwiBaudDelayCycles( g_ulBaudrate ));
  ulReceiveTimeOut = 11u * System_CyclesToResponseTimeOut(System_SwiBaudDelayCycles( g_ulBaudrate ));
  ulImmIntTimeOut  = 11u * System_CyclesToImmedIntTimeOut(System_SwiBaudDelayCycles( g_ulBaudrate ));

#ifdef COUNTER16BIT
  //check, if counters can house new settings
  if( ((ulSysCycles * 5u) > UWORD_MAX) || ( ulRespIntTimeOut > UWORD_MAX ) || ( ulImmIntTimeOut > UWORD_MAX ) )
  {
    return FALSE;
  }
#elif defined COUNTER8BIT
  // check, if counters can house new settings
  if( ((ulSysCycles * 5u) > UBYTE_MAX) || ( ulRespIntTimeOut > UBYTE_MAX ) || ( ulImmIntTimeOut > UBYTE_MAX ) )
  {
    return FALSE;
  }
#endif

  //apply new baudrate and calculate required runtime data, after checks done //
  g_ulBaudrate = ul_Baudrate;

  g_cntBaudLow =  (COUNTER)ulSysCycles;
  g_cntBaudHigh = (COUNTER)(3u * ulSysCycles);
  g_cntBaudStop = (COUNTER)(5u * ulSysCycles);

  g_cntResponseTimeOut = (COUNTER)ulRespIntTimeOut;
  g_cntReceiveTimeOut = (COUNTER)ulReceiveTimeOut;

  g_ulImmediateIntTimeOut = (COUNTER)ulImmIntTimeOut;

  // all done!
  return TRUE;

}

*/

/* ****************************************************************************
   name:      Swi_ReadActualSpace()

   function:  read one byte from actual addressed space.

   input:     OUT: * ubp_Data
                pointer to UBYTE to store read value into.
   output:    bool

   return:    true, if read was ok.
              false, if read errors detected.

   date:      2011-07-22; v1.03: function added.
   ************************************************************************* */
BOOL Swi_ReadActualSpace( UBYTE * ubp_Data )
{

  BOOL bResult;
  U_SWI_WORD uReceivedWord;


  /* read out data */
  bResult = Swi_ReceiveRawWord( &uReceivedWord );
  if( bResult == TRUE )
  {
    *ubp_Data = (UBYTE)(uReceivedWord.sSwiAbstact.SWI_DATA);
  }

  /* show result */
  return bResult;

}



/* ****************************************************************************
   name:      Swi_ReadRegisterSpace()

   function:  read one byte from the ORIGA register space.

   input:     IN: uw_Address
                address within register space to read byte from.
              OUT: * ubp_Data
                pointer to UBYTE to store read value into.
   output:    bool

   return:    true, if read was ok.
              false, if read errors detected.

   date:      .
   ************************************************************************* */
BOOL Swi_ReadRegisterSpace( UWORD uw_Address, UBYTE * ubp_Data )
{

  BOOL bResult;
  U_SWI_WORD uReceivedWord;


  /* set burst length to one byte */
  Swi_SendRawWordNoIrq(SWI_BC, SWI_RBL0 );
  /* select register set */
  Swi_SendRawWordNoIrq(SWI_BC, SWI_EREG );

  /* send address to read from */
  Swi_SendRawWordNoIrq(SWI_ERA, ((uw_Address >> 8u) & 0xFFu) );
  Swi_SendRawWordNoIrq(SWI_RRA, ( uw_Address        & 0xFFu) );

  /* read out data */
  bResult = Swi_ReceiveRawWord( &uReceivedWord );
  if( bResult == TRUE )
  {
    *ubp_Data = (UBYTE)(uReceivedWord.sSwiAbstact.SWI_DATA);
  }

  /* show result */
  return bResult;

}



/* ****************************************************************************
   name:      Swi_WriteRegisterSpace()

   function:  write new content to ORIGA register address space.
              the function can also handle IRQ events that may be triggered
              after doing a register write access.

   input:     IN: uw_Address
                address within ORIGA register space to change the content.
              IN: ub_Data
                new data to store at the address pointed to by uw_Address
              IN: ub_BitSelect
                AND-Mask for doing selective change of register content.
                only bits set to '1' at ub_BitSelect are changed.
                NOTE: ub_BitSelect == 0xFF will alter all bits
              IN: b_WaitForInterrupt
                wait until SWI IRQ is detected.
              IN: b_ImmediateInterrupt
                wait until SWI IRQ is detected directly after command is sent.
              OUT: * bp_IrqDetected
                pointer to bool to save detection of SWI IRQ.
                NOTE: id b_WaitForInterrupt == FALSE and
                      b_ImmediateInterrupt == FALSE, then the pointer can
                      be set to NULL.
   output:    bool

   return:    true, if write was ok.
              false, if errors detected.

   date:      v0.93; 2009-03-31: fix of bit select mechanism.
                     2009-05-25: Swi_SendRawWord return treated as void.
   ************************************************************************* */
BOOL Swi_WriteRegisterSpace(UWORD uw_Address, UBYTE ub_Data, UBYTE ub_BitSelect, BOOL b_WaitForInterrupt, BOOL b_ImmediateInterrupt, BOOL * bp_IrqDetected )
{

  BOOL bIntOccured;
  UBYTE ubReadData = 0x00u;


  /* ub_BitSelect == 0x00 makes no sense! */
  if( ub_BitSelect == 0x00u )
  {
    return FALSE;
  }

  /* read out current data, if bit masking used */
  if( ub_BitSelect != 0xFFu )
  {
    if( Swi_ReadRegisterSpace( uw_Address, &ubReadData ) == FALSE)
    {
      return FALSE;
    }
    ubReadData &= ~ub_BitSelect;
    ub_Data = ubReadData | (ub_Data & ub_BitSelect);
  }

  /* set burst lenght is 1 byte */
  Swi_SendRawWordNoIrq(SWI_BC, SWI_RBL0 );
  /*select device register set */
  Swi_SendRawWordNoIrq(SWI_BC, SWI_EREG );

  Swi_SendRawWordNoIrq(SWI_ERA, ((uw_Address >> 8u) & 0xFFu) );
  Swi_SendRawWordNoIrq(SWI_WRA, ( uw_Address        & 0xFFu) );

  Swi_SendRawWord( SWI_WD, ub_Data, b_WaitForInterrupt, b_ImmediateInterrupt, &bIntOccured );
  if( (b_WaitForInterrupt == TRUE) || (b_ImmediateInterrupt == TRUE) )
  {
    *bp_IrqDetected = bIntOccured;
  }

  /* all done */
  return TRUE;

}



/* ****************************************************************************
   name:      Swi_WriteRegisterSpaceNoIrq()

   function:  write new content to ORIGA register address space.

   input:     IN: uw_Address
                address within ORIGA register space to change the content.
              IN: ub_Data
                new data to store at the address pointed to by uw_Address
              IN: ub_BitSelect
                AND-Mask for doing selective change of register content.
                only bits set to '1' at ub_BitSelect are changed.
                NOTE: ub_BitSelect == 0xFF will alter all bits
   output:    bool

   return:    true, if write was ok.
              false, if errors detected.

   date:      .
   ************************************************************************* */
BOOL Swi_WriteRegisterSpaceNoIrq( UWORD uw_Address, UBYTE ub_Data, UBYTE ub_BitSelect )
{
  return Swi_WriteRegisterSpace( uw_Address, ub_Data, ub_BitSelect, FALSE, FALSE, NULL );
}



/* ****************************************************************************
   name:      Swi_ReadConfigSpace()

   function:  read one byte from the ORIGA config space.

   input:     IN: uw_Address
                address within config space to read one byte from.
              OUT: * ubp_Data
                pointer to UBYTE buffer to store read data into.
   output:    bool

   return:    true, if read was ok.
              false, if read errors detected.

   date:      .
   ************************************************************************* */
BOOL Swi_ReadConfigSpace( UWORD uw_Address, UBYTE * ubp_Data )
{

  BOOL bResult;
  U_SWI_WORD u_ReceivedWord;


  /* burst lenght is 1 byte */
  Swi_SendRawWordNoIrq(SWI_BC, SWI_RBL0 );
  /* select device config set */
  Swi_SendRawWordNoIrq(SWI_BC, SWI_ECFG );

  Swi_SendRawWordNoIrq(SWI_ERA, ((uw_Address >> 8u ) & 0xFFu) );
  Swi_SendRawWordNoIrq(SWI_RRA, ( uw_Address         & 0xFFu) );

  bResult = Swi_ReceiveRawWord( &u_ReceivedWord );
  if( bResult == TRUE )
  {
    *ubp_Data = (UBYTE)(u_ReceivedWord.sSwiAbstact.SWI_DATA);
  }

  /* show result */
  return bResult;

}



/* ****************************************************************************
   name:      Swi_WriteConfigSpace()

   function:  write new content to ORIGA config address space.

   input:     IN: uw_Address
                address within ORIGA config space to change the content.
              IN: ub_Data
                new data to store at the address pointed to by uw_Address.
              IN: ub_BitSelect
                AND-Mask for doing selective change of register content.
                only bits set to '1' at ub_BitSelect are changed.
                NOTE: ub_BitSelect == 0xFF will alter all bits.
   output:    bool

   return:    true, if write was ok.
              false, if errors detected.

   date:      .
   ************************************************************************* */
BOOL Swi_WriteConfigSpace( UWORD uw_Address, UBYTE ub_Data, UBYTE ub_BitSelect )
{

  UBYTE ubReadData = 0u;


  /* ub_BitSelect == 0x00 makes no sense! */
  if( ub_BitSelect == 0x00u )
  {
    return FALSE;
  }

  /* read out current data, if bit masking used */
  if(ub_BitSelect != 0xFFu)
  {
    if( Swi_ReadRegisterSpace(uw_Address, &ubReadData) == FALSE )
    {
      return FALSE;
    }
    ubReadData &= ~ub_BitSelect;
    ub_Data = ubReadData | (ub_Data & ub_BitSelect);
  }

  /* burst lenght is 1 byte */
  Swi_SendRawWordNoIrq(SWI_BC, SWI_RBL0 );
  /* select device config set */
  Swi_SendRawWordNoIrq(SWI_BC, SWI_ECFG );

  Swi_SendRawWordNoIrq(SWI_ERA, ((uw_Address >> 8u) & 0xFFu) );
  Swi_SendRawWordNoIrq(SWI_WRA, ( uw_Address        & 0xFFu) );

  Swi_SendRawWordNoIrq(SWI_WD, ub_Data );

  /* all done */
  return TRUE;

}



/* ****************************************************************************
   name:      Swi_SearchPuid()

   function:  find unique ID of attached system.
              NOTE: this function only supports one attached system. for
                    ORGA multi UID handling please ask your AE.

   input:     IN: ub_BitsToSearch
                length of the UID in bits.
              OUT: * stp_DetectedPuid
                pointer to structure that will hold the detected UID.
   output:    BOOL

   return:    true, if read was ok.
              false, if read errors detected.

   date:      v0.93; 2009-05-25: Swi_SendRawWord return treated as void.
   ************************************************************************* */
/*
BOOL Swi_SearchPuid( UBYTE ub_BitsToSearch, S_ORIGA_PUID * stp_DetectedPuid )
{

  BOOL bFound_0;
  BOOL bFound_1;
  UBYTE ubBitIndex = 0u;
  UBYTE ubByteIndex;
  UBYTE * ubpBytes = (UBYTE*)stp_DetectedPuid;


  // do parameter checks
  if( ( ub_BitsToSearch == 0u ) || ( ub_BitsToSearch > 96u ) )
  {
    return FALSE;
  }

  // do bus reset first
  Swi_Reset();
  // start ID search
  Swi_SendRawWordNoIrq( SWI_BC, SWI_DISS );


  // search loop
  for( ; ub_BitsToSearch != 0u; ub_BitsToSearch-- )
  {
    ubByteIndex = ubBitIndex >> 3u;
    ubpBytes[ubByteIndex] = ubpBytes[ubByteIndex] << 1u;

    // take care that all two probes did perform correct
    Swi_SendRawWord( SWI_BC, SWI_DIP0, TRUE, TRUE, &bFound_0 );
    Swi_SendRawWord( SWI_BC, SWI_DIP1, TRUE, TRUE, &bFound_1 );

    // check result and exit on error!
    if( (bFound_1 == FALSE) && (bFound_0 == TRUE) )
    {
      ubpBytes[ubByteIndex] &= 0xFEu;
      Swi_SendRawWordNoIrq( SWI_BC, SWI_DIE0 );
    }
    else if( (bFound_1 == TRUE) && (bFound_0 == FALSE) )
    {
      ubpBytes[ubByteIndex] |= 0x01u;
      Swi_SendRawWordNoIrq( SWI_BC, SWI_DIE1 );
    }
    else
    {
      // if nothing detected then break!
      Swi_AbortIrq();
      return FALSE;
    }

    // check next bit
    ubBitIndex++;
  }

  // all done
  return TRUE;

}
*/

/* ****************************************************************************
   name:      Swi_SelectByPuid()

   function:  select device by applying a given device UID.
              after UID selection a device address can be set for easier
              device handling.

   input:     IN: ub_BitsToExecute
                number of bits to execute.
              IN: * stp_DeviceToSelect
                pointer to UID structure to hold the UID to use.
              IN: uw_AssignDevAdr
                device address that has to be assigned after UID selection.
                NOTE: assignment is only done, if uw_AssignDevAdr != 0.
   output:    bool

   return:    'true', if selection process passed.
              'false', if selection process failed.

   date:      v0.93; 2009-05-25: Swi_SendRawWord return treated as void.
   ************************************************************************* */
/*
BOOL Swi_SelectByPuid( UBYTE ub_BitsToExecute, S_ORIGA_PUID * stp_DeviceToSelect, UWORD uw_AssignDevAdr )
{

  BOOL bFound_0;
  BOOL bFound_1;

  UBYTE ubByteIndex;
  UBYTE ubBitIndex = 0;

  BOOL bRefBit;
  UBYTE * ubpRefBytes = (UBYTE*)stp_DeviceToSelect;


  // execute bus reset
  Swi_Reset();

  if( (ub_BitsToExecute == 0u) || (ub_BitsToExecute > 96u) )
  {
    return FALSE;
  }

  // start uid search
  Swi_SendRawWordNoIrq( SWI_BC, SWI_DISS );

  // do bit select loop
  for( ; ub_BitsToExecute != 0u; ub_BitsToExecute-- )
  {

    ubByteIndex = ubBitIndex >> 3u;
    bRefBit = (ubpRefBytes[ubByteIndex] & (1u << ( 7u - (ubBitIndex & 0x07u) )));

    if( bRefBit == 0u )
    {
      Swi_SendRawWord( SWI_BC, SWI_DIP0, TRUE, TRUE, &bFound_0 );
      if( bFound_0 == TRUE )
       {
          Swi_SendRawWordNoIrq( SWI_BC, SWI_DIE0 );
        }
      else
      {
        return FALSE;
      }
    }
    else
    {
      Swi_SendRawWord( SWI_BC, SWI_DIP1, TRUE, TRUE, &bFound_1 );
      if( bFound_1 == TRUE )
       {
          Swi_SendRawWordNoIrq( SWI_BC, SWI_DIE1 );
        }
      else
      {
        return FALSE;
      }
    }

    ubBitIndex++;
  }

  // check, if device assignment requested
  if( uw_AssignDevAdr != 0u )
  {
    // function returns always true, if mask is set to 0xFF
    (void)Swi_WriteConfigSpace(SWI_DADR0,  (uw_AssignDevAdr        & 0xFFu), 0xFFu );
    (void)Swi_WriteConfigSpace(SWI_DADR1, ((uw_AssignDevAdr >> 8u) & 0xFFu), 0xFFu );
  }

  // all done
  return TRUE;

}
*/


/* ****************************************************************************
   name:      Swi_SelectByAddress()

   function:  select device by device address.

   input:     IN: uw_DeviceAddress
                device address.
   output:    -

   return:    -

   date:      v0.93; 2009-05-25: Swi_SelectByAddress return treated as void.
   ************************************************************************* */
void Swi_SelectByAddress(UWORD uw_DeviceAddress)
{
  Swi_SendRawWordNoIrq( SWI_EDA, ( (UBYTE)((uw_DeviceAddress >> 8u) & 0xFFu)) );
  Swi_SendRawWordNoIrq( SWI_SDA, ( (UBYTE)( uw_DeviceAddress        & 0xFFu)) );
}



/* ****************************************************************************
   name:      Swi_PowerDown()

   function:  do hardware power down by pushing SWI to low for minimum of
              200us.

   input:     -
   output:    -

   return:    -

   date:      v1.00; 2011-07-21: alternate pin handling removed.
              v1.01; 2011-07-21: do limit check!
   ************************************************************************* */
void Swi_PowerDown( void )
{

  System_SetSwiPort( 0u );

#ifdef COUNTER16BIT
  #if( (System_CyclesToLoops( System_MicrosToCycles(200u))) > UWORD_MAX )
  #error "ERROR: 200us counter loop setup does not fit into 16bit counter!"
  #endif
#elif defined COUNTER8BIT
  #if( (System_CyclesToLoops( System_MicrosToCycles(200u))) > UBYTE_MAX )
  #error "ERROR: 200us counter loop setup does not fit into 8bit counter!"
  #endif
#endif
  /* SWI has to be held at low for at least 200us! */
  System_IdleDelay( System_CyclesToLoops( System_MicrosToCycles(200u)) );

}



/* ****************************************************************************
   name:      Swi_PowerUp()

   function:  start power up sequence by setting SWI to high for at least
              10ms.

   input:     -
   output:    -

   return:    -

   date:      v1.00; 2011-07-21: alternate pin handling removed.
              v1.01; 2011-07-21: do limit check!
   ************************************************************************* */
void Swi_PowerUp( void )
{

  UBYTE ubDelayCount;


  System_SetSwiPort( 1u );

#ifdef COUNTER16BIT
  #if( (System_CyclesToLoops( System_MicrosToCycles(1000u))) > UWORD_MAX )
  #error "ERROR: 1ms counter loop setup does not fit into 16bit counter!"
  #endif
#elif defined COUNTER8BIT
  #if( (System_CyclesToLoops( System_MicrosToCycles(1000u))) > UBYTE_MAX )
  #error "ERROR: 1ms counter loop setup does not fit into 8bit counter!"
  #endif
#endif
  /* wait for 10ms! */
  for( ubDelayCount = 0u; ubDelayCount < 10u; ubDelayCount++ )
  {
    System_IdleDelay( System_CyclesToLoops( System_MicrosToCycles(1000u)) );
  }

}



/* ****************************************************************************
   name:      Swi_Reset()

   function:  execute bus reset command.

   input:     -
   output:    -

   return:    -

   date:      .
   ************************************************************************* */
void Swi_Reset( void )
{
  Swi_SendRawWordNoIrq( SWI_BC, SWI_BRES );
}



/* ****************************************************************************
   name:      Swi_TreatInvertFlag()

   function:  count number of used 3TAUs (ones) and invert data, if number of
              ones is greater than six. invert flag and data if required.

   input:     IN/OUT: * up_DataWord
                pointer to U_SWI_WORD that has to hold valid SWI stream data.
                regarding the number of detected ones, the invert flag and data
                is inverted.
   output:    -

   return:    -

   date:      .
   ************************************************************************* */
void Swi_TreatInvertFlag( U_SWI_WORD * up_DataWord )
{

  UBYTE ubBits;
  UBYTE ubCount = 0u;

  UWORD uwTarget = up_DataWord->uwWord;

  /* count '1'es */
  for( ubBits = 0u; ubBits < 16u; ubBits++)
  {
    ubCount += (UBYTE)(uwTarget) & 1u;
    uwTarget >>= 1u;
  }

  /* check, if invert required */
  if( ubCount > 6u )
  {
    up_DataWord->sSwiInvRelevant.SWI_DATA = ~up_DataWord->sSwiInvRelevant.SWI_DATA;
    up_DataWord->sSwiInvRelevant.SWI_INV = 1u;
  }

}



/* ****************************************************************************
   name:      Swi_AbortIrq()

   function:  send SWI IRQ to active finish IRQ mode on SWI line.

   input:     -
   output:    -

   return:    -

   date:      v1.00; 2011-07-21: alternate pin handling removed.
   ************************************************************************** */
/*
void Swi_AbortIrq( void )
{

  System_SetSwiPort( 0u );

  System_IdleDelay( g_cntBaudLow );

  System_SetSwiPort( 1u );

}

*/

/* ****************************************************************************
   name:      Swi_WaitForIrq()

   function:  wait for SWI IRQ to happen on SWI line.

   input:     OUT: * bp_IrqDetected
                is set to 'true', is IRQ was detected, else is set to 'false'.
              IN: b_Immediate
                if 'true', then poll period for IRQ checking is set to immediate
                time slice, else the long slice timing is used.
   output:    -

   return:    -

   date:      v0.93; 2009-05-25: changed function return from 'BOOL' to 'void'.
              v1.00; 2011-07-21: alternate pin handling removed.
   ************************************************************************* */
/*
void Swi_WaitForIrq( BOOL* bp_IrqDetected, BOOL b_Immediate)
{

  BOOL bResult = FALSE;
  volatile ULONG ulTimeOut;


  ulTimeOut = g_culIrqTimeout;
  if( b_Immediate == TRUE )
  {
    ulTimeOut = g_ulImmediateIntTimeOut;
  }

  // disable host IRQs
  System_DisableIrqs();

  *bp_IrqDetected = FALSE;

  // wait for IRQ to happen
  while( ulTimeOut != 0u )
  {
    if(System_GetSwiPort() == 0u)
    {
      bResult = TRUE;
      *bp_IrqDetected = TRUE;
      break;
    }
    ulTimeOut--;
  }


  // if no IRQ detected then host has to exit IRQ state
  if( bResult == FALSE )
  {
    Swi_AbortIrq();
  }


  // enable host IRQs
  System_EnableIrqs();

}

*/

/* ****************************************************************************
   name:      Swi_SendRawWord()

   function:  translate requested code and data payload to SWI bit stream
              after translation send out SWI stream.

   input:     IN: ub_Code
                SWI command to send-
              IN: ub_Data
                SWI command extension or payload data to send.
              IN: b_WaitForInterrupt
                wait until SWI IRQ is detected.
              IN: b_ImmediateInterrupt
                wait until SWI IRQ is detected directly after command is sent.
              OUT: * bp_IrqDetected
                pointer to bool to save detection of SWI IRQ.
                NOTE: if b_WaitForInterrupt == FALSE and
                      b_ImmediateInterrupt == FALSE, then the pointer can
                      be set to NULL.
   output:    -

   return:    -

   date:      v0.93; 2009-03-31: treat return results.
                     2009-05-25: return value changed from 'BOOL' to 'void'.
              v1.00; 2011-07-21: alternate pin handling removed.
                     2011-07-21: Swi_SendRawWord() send STOP first to have no
                                 issue with following IRQ or read, due to CPU
                                 latency.
              v1.02; 2011-07-21: not needed code before IRQ handling removed.
   ************************************************************************* */
void Swi_SendRawWord( UBYTE ub_Code, UBYTE ub_Data, BOOL b_WaitForInterrupt, BOOL b_ImmediateInterrupt, BOOL * bp_IrqDetected )
{
//  b_ImmediateInterrupt = b_ImmediateInterrupt;
//  *bp_IrqDetected = *bp_IrqDetected;
  BOOL bInterruptOccured = FALSE;
  U_SWI_WORD uSwiWord;
  S_SWI_WORDBITS * sSwiWordBits = &uSwiWord.sSwiBits;


  /* setup required data */
  uSwiWord.uwWord = 0u;
  uSwiWord.sSwiAbstact.SWI_CODE = ub_Code;
  uSwiWord.sSwiAbstact.SWI_DATA = ub_Data;
  Swi_TreatInvertFlag(&uSwiWord);

  /* do not disturb access */
  System_DisableIrqs();


  /* Send a STOP singal first to have time to receive either IRQ or data! */
  System_SetSwiPort(1u);
  System_IdleDelay( g_cntBaudStop );
  /* Send BCF */
  System_SetSwiPort(0u);
  (sSwiWordBits->SWI_BCF == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send _BCF */
  System_SetSwiPort(1u);
  (sSwiWordBits->SWI_BCFQ == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT9 */
  System_SetSwiPort(0u);
  (sSwiWordBits->SWI_D9 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT8 */
  System_SetSwiPort(1u);
  (sSwiWordBits->SWI_D8 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT7 */
  System_SetSwiPort(0u);
  (sSwiWordBits->SWI_D7 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT6 */
  System_SetSwiPort(1u);
  (sSwiWordBits->SWI_D6 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT5 */
  System_SetSwiPort(0u);
  (sSwiWordBits->SWI_D5 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT4 */
  System_SetSwiPort(1u);
  (sSwiWordBits->SWI_D4 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT3 */
  System_SetSwiPort(0u);
  (sSwiWordBits->SWI_D3 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT2 */
  System_SetSwiPort(1u);
  (sSwiWordBits->SWI_D2 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT1 */
  System_SetSwiPort(0u);
  (sSwiWordBits->SWI_D1 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send BIT0 */
  System_SetSwiPort(1u);
  (sSwiWordBits->SWI_D0 == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);
  /* Send Inversion bit */
  System_SetSwiPort(0u);
  (sSwiWordBits->SWI_INV == 1u) ? System_IdleDelay(g_cntBaudHigh): System_IdleDelay(g_cntBaudLow);

  /* Set SWI high */
  /* Note: As STOP was done at begin of function, CPU has 5TAU left for IRQ detection or read access! */
  System_SetSwiPort(1);


  /* Check for IRQ event, if requested */
  //Don't use WaitForInterrupt
  /*
  if( b_WaitForInterrupt == TRUE )
  {
    // wait for interrupt
    Swi_WaitForIrq( &bInterruptOccured, b_ImmediateInterrupt );
  }

*/

  // enable host IRQs
  System_EnableIrqs();

  /* store IRQ data, if requested */
 /*
  if( (b_WaitForInterrupt == TRUE) || (b_ImmediateInterrupt == TRUE) )
  {
   *bp_IrqDetected = bInterruptOccured;
  }
 */
}



/* ****************************************************************************
   name:      Swi_SendRawWordNoIrq()

   function:  Send out SWI-Word without waiting for IRQ response.

   input:     IN: ub_Code:
                SWI command to send
              IN: ub_Data:
                SWI 2nd part of command or payload, if data is sent.
   output:    -

   return:    -

   date:      v0.93; 2009-03-31: function "returns" void.
   ************************************************************************* */
void Swi_SendRawWordNoIrq(UBYTE ub_Code, UBYTE ub_Data )
{
  Swi_SendRawWord( ub_Code, ub_Data, FALSE, FALSE, NULL );
}



/* ****************************************************************************
   name:      Swi_ReceiveRawWord()

   function:  receice data via SWI interface.
              measure time between high and low phases and adjust to detected
              high low phase timing. depending on calculated threshold the
              bits of the detected SWI signal are put together to one SWI
              bit stream result.

   input:     OUT: * up_Word
                pointer to union to hold detected SWI bit stream.
   output:    BOOL

   return:    true, if read was ok.
              false, if read errors detected.

   date:      2011-07-21; v1.00: enable IRQs in case of function return, due to
                                 detected error.
   ************************************************************************* */
BOOL Swi_ReceiveRawWord( U_SWI_WORD * up_Word )
{

  BOOL bPreviousSwiState;
  UBYTE ubIndex = 12u;
  UBYTE ubBitsToCapture;

  COUNTER cntTimeOut = g_cntResponseTimeOut;

  COUNTER cntTimes[13];
  COUNTER cntMaxTime = 0u;
  COUNTER cntMinTime = ~cntMaxTime;
  COUNTER cntCount;
  COUNTER cntThreshold;


  /* disable host IRQs */
  System_DisableIrqs();

  while( (System_GetSwiPort() == 1u) && (cntTimeOut != 0u) )
  {
    cntTimeOut--;
  }
  /* exit with fail, if timeout criteria triggered */
  if( cntTimeOut == 0u )
  {
    /* enable host IRQs before return! */
    System_EnableIrqs();
    return FALSE;
  }
  /* get port state */
  bPreviousSwiState = System_GetSwiPort();

  /* measure time of high and low phases */
  for( ubBitsToCapture = 13u; ubBitsToCapture != 0u; ubBitsToCapture-- )
  {
    cntCount = 0u;
    while( System_GetSwiPort() == bPreviousSwiState )
    {
      cntCount++;
      if( cntCount > g_cntReceiveTimeOut )
      {
        /* enable host IRQs before return! */
        System_EnableIrqs();
        return FALSE;
      }
    }
    cntTimes[ubIndex] = cntCount;
    ubIndex--;

    bPreviousSwiState = System_GetSwiPort();
  }


  /* after SWI accesses host IRQs can be enabled */
  System_EnableIrqs();

  /* evaluate detected results */
  for( ubIndex = 12u; ubIndex != 0u; ubIndex-- )
  {
    cntCount = cntTimes[ubIndex];
    if( cntCount < cntMinTime)
    {
      cntMinTime = cntCount;
    }
    else if(cntCount > cntMaxTime)
    {
      cntMaxTime = cntCount;
    }
    else
    {
    /*  no change required */
    }
  }

  /* calculate threshold */
  cntThreshold = ((cntMaxTime - cntMinTime) >> 1u);
  cntThreshold += cntMinTime;

  /* get bits according threshold */
  up_Word->sSwiBits.SWI_BCF =  (cntTimes[12] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_BCFQ = (cntTimes[11] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D9 =   (cntTimes[10] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D8 =   (cntTimes[ 9] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D7 =   (cntTimes[ 8] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D6 =   (cntTimes[ 7] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D5 =   (cntTimes[ 6] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D4 =   (cntTimes[ 5] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D3 =   (cntTimes[ 4] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D2 =   (cntTimes[ 3] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D1 =   (cntTimes[ 2] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_D0 =   (cntTimes[ 1] > cntThreshold) ? 1u : 0u;
  up_Word->sSwiBits.SWI_INV =  (cntTimes[ 0] > cntThreshold) ? 1u : 0u;

  /* check, if payload had to be inverted to to invert flag */
  if( up_Word->sSwiBits.SWI_INV == 1u )
  {
    up_Word->sSwiBits.SWI_INV = 0u;
    up_Word->sSwiInvRelevant.SWI_DATA = ~up_Word->sSwiInvRelevant.SWI_DATA;
  }

  /* check that SWI_BCF is not equal to SWI_BCFQ */
  if(up_Word->sSwiBits.SWI_BCF == up_Word->sSwiBits.SWI_BCFQ)
  {
    return FALSE;
  }

  /* all done well */
  return TRUE;

}



