/*-----------------------------------------------------------------------------
 * TITLE: OEM_PS2.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

// ----------------------------------------------------------------------------
// To define "Hook_ECACKMouseDriver" is necessary
// If define "Hook_ECACKMouseDriver" EC firmware need Respond mouse driver 
//  via Hook_ECRespondtoMouseDriver function
// ----------------------------------------------------------------------------
void Hook_ECRespondtoMouseDriver(BYTE mscmd)
{
    
}

// ----------------------------------------------------------------------------
// Hook kernel service_ps2 function
// ----------------------------------------------------------------------------
void Hook_service_ps2(BYTE ps2_channel, BYTE ps2_data)
{

}

// ----------------------------------------------------------------------------
// Disable port 0 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void Hook_DisablePS2Port_0(void)
{
    DisablePS2Port_0();
}

// ----------------------------------------------------------------------------
// Disable port 1 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void Hook_DisablePS2Port_1(void)
{
    DisablePS2Port_1();
}

// ----------------------------------------------------------------------------
// Disable port 2 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void Hook_DisablePS2Port_2(void)
{
    DisablePS2Port_2();
}

// ----------------------------------------------------------------------------
// Enable port 0 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void Hook_EnablePS2Port_0(void)
{
    EnablePS2Port_0();
}

// ----------------------------------------------------------------------------
// Enable port 1 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void Hook_EnablePS2Port_1(void)
{
    EnablePS2Port_1();
}

// ----------------------------------------------------------------------------
// Enable port 0 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void Hook_EnablePS2Port_2(void)
{
    EnablePS2Port_2();
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void Hook_TPOnlyLowLevelFunc(void)
{

}

void SendSynapticsIDSequence()
{
    BYTE Temp;

  Temp=0;
  do {
    Send2PortNWait(0x02, 0xE8, 1);
    Send2PortNWait(0x02, 0x00, 1);
    Temp++;
  }while(Temp<4);

  Send2PortNWait(0x02, 0xE9, 4);
}

void SendElanIDSequence()
{
    Send2PortNWait(0x02, 0xE6, 1);
    Send2PortNWait(0x02, 0xE6, 1);
    Send2PortNWait(0x02, 0xE6, 1);
    Send2PortNWait(0x02, 0xE9, 4);
}

void SendAlpsIDSequence()
{
    Send2PortNWait(0x02, 0xE7, 1);
    Send2PortNWait(0x02, 0xE7, 1);
    Send2PortNWait(0x02, 0xE7, 1);
    Send2PortNWait(0x02, 0xE9, 4);
}

void ReadTPid()
{
    //BYTE Temp;
    SendSynapticsIDSequence();

    if(TPID3 == 0x47)
    {
      // TPID1 should be 0xFA, following 3bytes: 0x??, 0x47, 0x??
      TP_Type = 0x02;    // Synatics TouchPad.
    }
    else
    {
      SendAlpsIDSequence();
      // TPID1 should be 0xFA, following 3bytes: 0x73, 0x03, 0x??
      if ((TPID2 == 0x73) && (TPID3 == 0x03))
      {
          TP_Type = 0x04;  // Alps 3+2BCP.
      }

//         SendElanIDSequence();
//         if(TPID2 == 0x3C)
//         {
//             TP_Type = 0x01;  // ELAN ToucnPad.
//         }
    }
/*
    Temp = Read_Eflash_Byte(0x01,0xB0,0xEF);

    if(Temp != TP_Type)
    {
        eEEPROMBank= 0x00;
        eEEPROMAddrsss = 0xEF;
        eEEPROMData  = TP_Type;
        Update_EEPROMMark();
    }
*/
}