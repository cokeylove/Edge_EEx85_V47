/*-----------------------------------------------------------------------------
 * TITLE: OEM_INCLUDE.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_INCLUDE_H
#define OEM_INCLUDE_H

//----------------------------------------------------------------------------
// OEM Bank0
//----------------------------------------------------------------------------
#include <OEM_DEFINE.h>
#include <OEM_Project.H>
#include <OEM_LCFC.h>
#include <OEM_GPIO.H>
#include <OEM_INIT.H>
#include <OEM_MAIN.H>
#include <OEM_LED.H>
#include <OEM_BEEP.H>
#include <OEM_FAN.H>
#include <OEM_FANTBL.H>
#include <OEM_BATTERY.H>
#if Thinkpad_Battery_HW_Auth
#include <Origa_DataTypes.h>
#endif
#include <OEM_CCG3.H>
#include <OEM_RTS5455.H>
#include <OEM_MEMORY.H>
#include <OEM_POWERSEQ.H>
#include <OEM_POWER.H>
#include <OEM_EVENT.H>
#include <OEM_SMBUS.H>
#include <OEM_SCANTABS.H>
#include <OEM_ADC.H>
#include <OEM_MMB.H>
#include <OEM_ACPI.H>
#include <OEM_CIR.H>
#include <OEM_FLASH.H>
#include <OEM_HOSTIF.H>
#include <OEM_6064.H>
#include <OEM_MailBox.H>
#include <OEM_Ver.H>
#include <OEM_LCD.H>
#include <OEM_PORT686C.H>
#if (PLATFORM_CHIP == INTEL)
#include <OEM_PECI.H>
#endif // PLATFORM_CHIP
#include <OEM_PM3.H>
#include <OEM_LPC.H>
#include <OEM_AOU.H>
#include <OEM_SPI.H>
#include <OEM_IRQ.H>
#include <OEM_PM2.H>
#include <OEM_PS2.H>
#include <OEM_QEVENT.H>
#include <OEM_EEPROM.H>
#include <OEM_SHUTDOWN_ID.H>
#include <OEM_SPI.H>
#include <OEM_LPC.H>
#include <OEM_EcCmd.H>
#include <OEM_Timer.H>
#include <OEM_HSPI.H>
#include <OEM_ASM.H>
#include <OEM_Debug.H>
#include <OEM_SHA1.H>
#include <OEM_HOOK.H>
#include <OEM_EDGE.H>
#include <OEM_RFID.H>
#include <OEM_GSNS.H>
#include <OEM_CCG2.H>
#include <OEM_Ucsi.H>
#include <OEM_VGA_AMD.H>
#include <OEM_VGA_NV.H>
#include <OEM_TAMPER.H>
#include <OEM_EnhancedPcBeep.H>
#include <Oem_bma255.H>
#include <stdlib.h>

//----------------------------------------------------------------------------
// OEM Bank1
//----------------------------------------------------------------------------
#include <OEM_BANK1_Func.H>

//----------------------------------------------------------------------------
// OEM Bank2
//----------------------------------------------------------------------------
#include <OEM_BANK2_Func.H>
#if Thinkpad_Battery_HW_Auth
#include <Origa_Gf2nMath.h>
#include "Origa_Ecc.h"
#endif
//----------------------------------------------------------------------------
// OEM Ban3
//----------------------------------------------------------------------------
#include <OEM_BANK3_Func.H>
// 8996 +
//----------------------------------------------------------------------------
// OEM Ban4
//----------------------------------------------------------------------------
#include <OEM_BANK4_Func.H>
//----------------------------------------------------------------------------
// OEM Ban5
//----------------------------------------------------------------------------
#include <OEM_BANK5_Func.H>
//----------------------------------------------------------------------------
// OEM Ban6
//----------------------------------------------------------------------------
#include <OEM_BANK6_Func.H>
// 8996 -

#endif
