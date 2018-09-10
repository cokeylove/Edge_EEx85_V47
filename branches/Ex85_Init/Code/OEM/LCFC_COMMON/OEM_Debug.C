/*-----------------------------------------------------------------------------
 * TITLE: OEM_Debug.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

void Ec2iWrite (
  BYTE  RegOffset,
  BYTE  RegValue
)
{
  SET_MASK(LSIOHA,LKCFG);
  SET_MASK(IBMAE,CFGAE);
  SET_MASK(IBCTL,CSAE);

  //
  // Set offset
  //
  IHIOA = 0;
  IHD = RegOffset;
  while(IS_MASK_SET(IBCTL,CWIB));

  //
  // Write data
  //
  IHIOA = 1;
  IHD = RegValue;
  while(IS_MASK_SET(IBCTL,CWIB));

 	CLEAR_MASK(LSIOHA,LKCFG);
  CLEAR_MASK(IBMAE,CFGAE);
  CLEAR_MASK(IBCTL,CSAE);
}

#if 0
void Ec2iWrite (
  BYTE  RegOffset,
  BYTE  RegValue
)
{
  BYTE  IbmaeTemp;

  SET_MASK(LSIOHA,LKCFG);

  //
  // Following IT8586 spec chapter 7.15.5 to program EC2I write
  //

  // 1. Make sure CRIB and CWIB equals '0'.
  while (IS_MASK_SET(IBCTL,CWIB) && IS_MASK_SET(IBCTL,CWIB));

  // 2. Write IHIOA reg.
  IHIOA = RegOffset; // Set Indirect Host I/O Address Register

  // 3. Write IHD reg.
  IHD = RegValue;

  // 4. Write IBMAE reg. (PNPCFG Register EC Access Enable)
  IbmaeTemp = IBMAE;
  IBMAE = CFGAE;

  // 5. Write CSAE bit to 1.
  SET_MASK(IBCTL,CSAE);

  // 6. CWIB = 0?
  while (IS_MASK_SET(IBCTL,CWIB));

  // 7. Clear IBMAE reg
  IBMAE = IbmaeTemp;

  // 8. Clear CSAE bit to 0
  CLEAR_MASK(IBCTL,CSAE);

  //
  // -- Write Completed --
  //
  CLEAR_MASK(LSIOHA,LKCFG);
}
#endif

BYTE Ec2iRead (
  BYTE  RegOffset
)
{
  BYTE  RegValue;

  SET_MASK(LSIOHA,LKCFG);
  SET_MASK(IBMAE,CFGAE);
  SET_MASK(IBCTL,CSAE);

  //
  // Set offset
  //
  IHIOA = 0;
  IHD = RegOffset;
  while(IS_MASK_SET(IBCTL,CWIB));

  //
  // Read data
  //
  IHIOA = 1;
  SET_MASK(IBCTL,CRIB);
  while(IS_MASK_SET(IBCTL,CRIB));
  RegValue = IHD;

 	CLEAR_MASK(LSIOHA,LKCFG);
  CLEAR_MASK(IBMAE,CFGAE);
  CLEAR_MASK(IBCTL,CSAE);

  return RegValue;
}

#if 0
BYTE Ec2iRead (
  BYTE  RegOffset
)
{
  BYTE  IbmaeTemp;
  BYTE  RegValue;

  SET_MASK(LSIOHA,LKCFG);
  //
  // Following IT8586 spec chapter 7.15.5 to program EC2I read
  //

  // 1. Make sure CRIB and CWIB equals '0'.
  while (IS_MASK_SET(IBCTL,CRIB) && IS_MASK_SET(IBCTL,CWIB));

  // 2. Write IHIOA reg
  IHIOA = RegOffset;              // Set Indirect Host I/O Address Register

  // 3. Write IBMAE reg (PNPCFG Register EC Access Enable)
  IbmaeTemp = IBMAE;
  IBMAE = CFGAE;

  // 4. Write CRIB bit to 1, Write CSAE bit to 1.
  SET_MASK(IBCTL,CRIB);
  SET_MASK(IBCTL,CSAE);

  // 5. CRIB = 0?
  while (IS_MASK_SET(IBCTL,CRIB));

  // 6. Read IHD reg
  RegValue = IHD;

  // 7. Clear IBMAE reg
  IBMAE = IbmaeTemp;

  // 8. Clear CSAE bit to 0
  CLEAR_MASK(IBCTL,CSAE);

  //
  // -- Read Completed --
  //
  CLEAR_MASK(LSIOHA,LKCFG);

  return RegValue;
}
#endif

void DumpLpcReg (
  unsigned char Ldn
)
{
  unsigned short  RegIndex;
  unsigned char   LpcValue;

  //
  // Select LDN
  //
  Ec2iWrite (0x07, Ldn);
  printf ("\n---------------         ");
  switch (Ldn)
  {
    case 0x01: // UART1
      printf ("UART1   ");
      break;

    case 0x02: // UART2
      printf ("UART2   ");
      break;

    case 0x04: // SWUC
      printf ("SWUC    ");
      break;

    case 0x05: // KBC_MS
      printf ("KBC_MS");
      break;

    case 0x06: // KBC_KB
      printf ("KBC_KB  ");
      break;

    case 0x0a: // CIR
      printf ("CIR         ");
      break;

    case 0x0f: // SMFI
      printf ("SMFI      ");
      break;

    case 0x10: // BRAM
      printf ("BRAM    ");
      break;

    case 0x11: // PMC1
      printf ("PMC1     ");
      break;

    case 0x12: // PMC2
      printf ("PMC2     ");
      break;

    case 0x13: // SPI
      printf ("SPI           ");
      break;

    case 0x14: // PECI
      printf ("PECI        ");
      break;

    case 0x17: // PMC3
      printf ("PMC3     ");
      break;

    case 0x18: // PMC4
      printf ("PMC4     ");
      break;

    case 0x19: // PMC5
      printf ("PMC5     ");
      break;

    default:
      break;

  }

  //
  // Header
  //
  printf ("       ----------------\n");
  printf ("\t00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n\n");

  for (RegIndex = 0; RegIndex <=0xFF; RegIndex++) {
    //
    // Address label
    //
    if ((RegIndex & 0x0F) == 0x00) {
      printf ("%02X      ", RegIndex);
    }

    //
    // Register
    //
    LpcValue = Ec2iRead (RegIndex);
    printf ("%02bx ", LpcValue);
    if ((RegIndex & 0x0F) == 0x0F) {
      printf ("\n");
    }
  }
}



void DumpReg (
  unsigned short RegBase
)
{
  unsigned short ContentIndex;

  //
  // Header
  //
  printf ("\n----------------------- 0x%04X ------------------------\n", (RegBase & BASE_MASK));
  printf ("\t00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n\n");

  //
  // Content
  //
  for (ContentIndex = 0; ContentIndex <= 0xFF; ContentIndex++) {
    //
    // Address label
    //
    if ((ContentIndex & 0x0F) == 0x00) {
      printf ("%04X    ", (RegBase + ContentIndex & LINE_MASK));
    }

    //
    // Register
    //
    printf ("%02bx ", (* (XBYTE * )( (RegBase & BASE_MASK) + ContentIndex)));

    if ((ContentIndex & 0x0F) == 0x0F) {
      printf ("\n");
    }
  }
}

void DumpAllReg (
  void
)
{
  unsigned short DumpAddress;
  //
  // Use UART to print out all EC registers.
  // Following D2EC's dump log format.
  //
  for (DumpAddress = 0x1000; DumpAddress <= 0x2900; DumpAddress += 0x100) {
    DumpReg (DumpAddress);
  }

  DumpReg (0x2A00);

  for (DumpAddress = 0x2C00; DumpAddress <= 0x3000; DumpAddress += 0x100) {
    DumpReg (DumpAddress);
  }

  DumpReg (0x8000);
  DumpReg (0xC000);

  for (DumpAddress = 0x0000; DumpAddress <= 0x0F00; DumpAddress += 0x100) {
    DumpReg (DumpAddress);
  }

  DumpLpcReg (0x01);  // UART1
  DumpLpcReg (0x02);  // UART2
  DumpLpcReg (0x04);  // SWUC
  DumpLpcReg (0x05);  // KBC_MS
  DumpLpcReg (0x06);  // KBC_KB
  DumpLpcReg (0x0A);  // CIR
  DumpLpcReg (0x0F);  // SMFI
  DumpLpcReg (0x10);  // RTCT
  DumpLpcReg (0x11);  // PMC1
  DumpLpcReg (0x12);  // PMC2
  DumpLpcReg (0x13);  // SSPI
  DumpLpcReg (0x14);  // PECI
  DumpLpcReg (0x17);  // PMC3
  DumpLpcReg (0x18);  // PMC4
  DumpLpcReg (0x19);  // PMC5
  printf("\nDone!\n");
}