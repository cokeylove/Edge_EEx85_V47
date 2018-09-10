/*----------------------------------------------------------------------------
 * Filename: OEM_PM3.C  For Chipset: ITE.IT85XX
 *
 * Function: PMC3 example function
 *
 * Author  : Dino
 *
 * Copyright (c) 2010 - , ITE Tech. Inc. All Rights Reserved.
 *
 * You may not present,reproduce,distribute,publish,display,modify,adapt,
 * perform,transmit,broadcast,recite,release,license or otherwise exploit
 * any part of this publication in any form,by any means,without the prior
 * written permission of ITE Tech. Inc.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// Process Command/Data received from System via PMC3 interface
//----------------------------------------------------------------------------
void service_pci4(void)
{
//PCBEEP
	if ( IS_MASK_CLEAR(PM3STS,P_IBF) )
	{
		return;
	}
	
	if ( PM3STS & P_C_D)	// Command port
	{
		PM3Cmd	= PM3DI;  // Load command
		Hook_86Port(PM3Cmd);
		RamDebug(0xDD);
		EnablePMC3IBFInt();
		
	}	
	else
	{
		PM3Data = PM3DI;  // Load command
		
		EnablePMC3IBFInt();
	}
}

//----------------------------------------------------------------------------
// The function of debuging 86 port
//----------------------------------------------------------------------------
void Hook_86Port(BYTE Cmd)
{
	SET_MASK(EnhancedPcBeep_Status,MilePostInterrupt);
	if(IS_MASK_CLEAR(EnhancedPcBeep_Status,BeepActive))
  {
    if (0 != (MSB_MASK & Cmd)){
			SET_MASK(EnhancedPcBeep_Status,BeepActive);
		}
		//SET_MASK(EnhancedPcBeep_Status,BeepBusy);
  	//EnhancedPcBeep_DATA[0] = 0x02;
  	EnhancedPcBeep_DATA[1] = Cmd;
  	//EnhancedPcBeep_ID = 0x01;
  	RamDebug(0xDE);
  }
  
  switch(Cmd)
  {
	
    default:
      break;
  }
}
// 20160913 Build Port 86 - 
//PCBEEP
