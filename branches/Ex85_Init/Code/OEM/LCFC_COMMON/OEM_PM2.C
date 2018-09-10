/*----------------------------------------------------------------------------
 * Filename: OEM_PM2.C  For Chipset: ITE.IT85XX
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
// The function of debuging 68 port
//----------------------------------------------------------------------------

#if RTS5400_Smbus
void Data_To_BIOS(BYTE data_byte)
{
	PM2DO = data_byte;
}

BYTE Read_ACPI_table(BYTE MapIndex)
{
	BYTE *Tmp_XPntr;
	Tmp_XPntr= (BYTE *) (u4Cfgspace0+MapIndex);
	return(*Tmp_XPntr);	
}

void Write_ACPI_Table(BYTE MapIndex, BYTE data1)
{
	BYTE *Tmp_XPntr;
	Tmp_XPntr= (BYTE *) (u4Cfgspace0+MapIndex);
	*Tmp_XPntr = data1;
}

void PM2_6CCmd_80(void)
{
	PM2DataCount = 1;  
}

void PM2_6CCmd_80Data(void)
{
	if (PM2DataCount == 1)
	{
		Data_To_BIOS( Read_ACPI_table(PM2Data));
	}
}

void PM2_6CCmd_81(void)
{
	PM2DataCount = 2;  
}

void PM2_6CCmd_81Data(void)
{
	if (PM2DataCount == 2)
	{
     	PM2Data1=PM2Data;  		
	}
	else if(PM2DataCount == 1)
	{
       	Write_ACPI_Table(PM2Data1, PM2Data);
		Hook_ACPICommand();		//alex copy 
 	}
}
#endif
void Hook_68Port(BYTE Data)
{
#if Support_RFID
  BYTE i,j;		
#endif  //Support_RFID  
  switch(Data)
  {
#if  Support_RFID 
	case 0x01:		
	  if( PM2DataCount == 0x01 )
	  {
	  	i = Cmd_01(PM2Data1,PM2Data,&j);
		if( i == TRUE )
			Data2Port(0x68,j);
	  }
	  break;
	case 0x02:		
	  if( PM2DataCount == 0x01 )
	  {
	  	i = Cmd_02(PM2Data2,PM2Data1,PM2Data);
		if( i == TRUE )
			Data2Port(0x68,0);
	  }
	  break;
	case 0x09:		
	  if( PM2DataCount == 0x01 )
	  {
	  	i = Cmd_09(PM2Data,&j);
		if( i == TRUE )
			Data2Port(0x68,j);
	  }
	  break;
    case 0x0A:		
	  if( PM2DataCount == 0x01 )
	  {
	  	Cmd_0A(PM2Data1,PM2Data);
	  }
	  break;
#endif //Support_RFID
	  	
#if (PLATFORM_CHIP == INTEL)
    case 0x45:
       Cmd_45(0x68,PM2Data);
       break;
#endif
	#if RTS5400_Smbus
			case 0x80:
				PM2_6CCmd_80Data();
				break;
			case 0x81:
				PM2_6CCmd_81Data();
				break;
		#endif
    case 0xB1:
      if( PM2DataCount == 0x01 )
      {
        Cmd_B1(PM2Data1,PM2Data);
      }
      break;
    case 0xB7:
      if( PM2DataCount == 0x01 )
      {
        Cmd_B7(0x68,PM2Data);
      }
      break;
  }
}

//----------------------------------------------------------------------------
// The function of debuging 6C port
//----------------------------------------------------------------------------
void Hook_6CPort(BYTE Cmd)
{
  switch(Cmd)
  {
#if Support_RFID
  	case 0x01:		
  	  PM2DataCount=0x02;
	  break;
	case 0x02:		
	  PM2DataCount=0x03;
	  break;
	case 0x09:		
	  PM2DataCount=0x01;
	  break;
	case 0x0A:		
	  PM2DataCount=0x02;
	  break;
#endif //Support_RFID
	#if Support_LenovoAPS
    case 0x10:
      APS_Cmd_10(0x68);
      break;
    case 0x11:
      APS_Cmd_11(0x68);
      break;
    case 0x12:
      APS_Cmd_12(0x68);
      break;
    case 0x13:
      APS_Cmd_13(0x68);
      break;
    case 0x14:
      APS_Cmd_14(0x68);
      break;
    case 0x15:
      APS_Cmd_15(0x68);
      break;
    case 0x16:
      APS_Cmd_16(0x68);
      break;
    case 0x17:
      APS_Cmd_17(0x68);
      break;
    case 0x18:
      APS_Cmd_18(0x68);
      break;
    #endif  // Support_LenovoAPS
	  	  case 0x56:
      Cmd_56(0x68);
      break;
#if (PLATFORM_CHIP == INTEL)	
	case 0x45:
      PM2DataCount=0x01;
      break;
#endif
    #if RTS5400_Smbus  
			case 0x80:
				PM2_6CCmd_80();	// alex 
				break;
			case 0x81:
				PM2_6CCmd_81();	// alex
				break;
		#endif
    case 0xB1:
      PM2DataCount=0x02;
      break;
//[Project][CC20130423][Start] Add EC idle function for 68/6C port.
    case 0xB4:
      Cmd_B4();
      break;
//[Project][CC20130423][End] Add EC idle function for 68/6C port.
    case 0xB7:
      PM2DataCount=0x01;
      break;
    default:
      break;
  }
}

//----------------------------------------------------------------------------
// Process Command/Data received from System via 68/6C interface
//----------------------------------------------------------------------------
void service_pci3(void)
{
  if ( IS_MASK_CLEAR(PM2STS,P_IBF) )
  {
    return;
  }

  if ( PM2STS & P_C_D)  // Command port
  {
    PM2Cmd  = PM2DI;  // Load command
    EnablePMCIBFInt();
    Hook_6CPort(PM2Cmd);
  }
  else          // Data port
  {
    PM2Data2 = PM2Data1;
    PM2Data1 = PM2Data;
    PM2Data = PM2DI;
    if(PM2DataCount==0x00)
    {
      //PM2Data = PM2DI;
      EnablePMCIBFInt();
    }
    else
    {
      //PM2Data = PM2DI;
      EnablePMCIBFInt();
      if (PM2DataCount!=0x00)
      {
        Hook_68Port(PM2Cmd);
        PM2DataCount--;
      }
    }
  }
}


