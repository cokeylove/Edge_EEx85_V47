/*----------------------------------------------------------------------------
 * Filename: OEM_Ver.C  For Chipset: ITE.IT85XX
 *
 * Function: Project version
 *
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//----------------------------------------------------------------------------
// Project Information
//----------------------------------------------------------------------------
const unsigned char code CurBuildDate[] =
{
  __DATE__,
};

const unsigned char code CurBuildTime[] =
{
  __TIME__,
};

const unsigned char code ProjectInfoI[] =
{
  ProjectID0L,
  ProjectID0H,
  REV0_BYTE0,
  REV0_BYTE1,
  REV0_BYTE2,
  REV0_BYTE3,
  REV0_BYTE4,

  CHIP_TYPE0,
  0x00,
  tModuName0,  // formodule name
};

const unsigned char code ProjectInfoII[] =
{
  ProjectID0L,
  ProjectID0H,
  REV0_BYTE0,
  REV0_BYTE1,
  REV0_BYTE2,
  REV0_BYTE3,
  REV0_BYTE4,

  CHIP_TYPE0,
  0x00,
  tModuName0,  // formodule name
};
