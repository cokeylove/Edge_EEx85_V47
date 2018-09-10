/*----------------------------------------------------------------------------
 * Filename: OEM_Ver.H  For Chipset: ITE.IT85XX
 *
 * Function: Project version
 *
 *---------------------------------------------------------------------------*/

#ifndef OEM_Ver_H
#define OEM_Ver_H

#define ProjectID0L   0x13  // Proect ID  (low byte)
#define ProjectID0H   0x00  // Proect ID  (high byte)
#define REV0_BYTE0    0x01  // Major number from 0 to 7
#define REV0_BYTE1    0x27  // 2nd version from 1 to 99
#define REV0_BYTE2    0x00  // 'A'   // Type of KB-BIOS from 'A' to 'Z'
#define REV0_BYTE3    0x00  // Verify byte from 0 to 15, 0 is ROM other is T01-T15
#define REV0_BYTE4    0x47 // Series number of EC F/W release

#define CHIP_TYPE0    0x0C  // 8996 - Chip type
#define tModuName0    "EEx85" // for module name

//Cmd_52_A0   report LCFC project name
#define OEM_PROJECT1 "EE485"    //14"
#define OEM_PROJECT2 "EE585"    //15"

//HTETXXWW
#define OEM_BuildID1 'R'
#define OEM_BuildID2 '0'
#define OEM_BuildID3 'U'
#define OEM_BuildID4 'H'
#define OEM_BuildID5 'T'
#define OEM_BuildID6 (REV0_BYTE4 >> 4)+0x30
#define OEM_BuildID7 (REV0_BYTE4&0x0F)+0x30
#define OEM_BuildID8 'W'



#endif