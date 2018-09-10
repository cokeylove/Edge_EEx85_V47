/*----------------------------------------------------------------------------
 * Filename: OEM_SMBus.H  For Chipset: ITE.IT85XX
 *
 * Author  : Dino
 *
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved.
 *
 * You may not present,reproduce,distribute,publish,display,modify,adapt,
 * perform,transmit,broadcast,recite,release,license or otherwise exploit
 * any part of this publication in any form,by any means,without the prior
 * written permission of ITE Tech. Inc.
 *---------------------------------------------------------------------------*/

#ifndef OEM_SMBUS_H
#define OEM_SMBUS_H
/*
*********************************************************
*     #define Constants
*********************************************************
*/

/*
*         SMBus Status code definition
*/

#define SMB_STS_STATUS_OK              0       //SMBus OK
#define SMB_STS_STATUS_CMD_ACC_DENIED  0x12    //SMBus Device Command Aceess
#define SMB_STS_STATUS_DATA_ACC_DENIED 0x17    //SMBus Device Access Denied

/*
*         SMBus Protocol definition
*/

#define SMB_PROTO_WRITE_QUICK 0x02
#define SMB_PROTO_READ_QUICK  0x03
#define SMB_PROTO_SEND_BYTE   0x04
#define SMB_PROTO_RCV_BYTE    0x05
#define SMB_PROTO_WRITE_BYTE  0x06
#define SMB_PROTO_READ_BYTE   0x07
#define SMB_PROTO_WRITE_WORD  0x08
#define SMB_PROTO_READ_WORD   0x09
#define SMB_PROTO_WRITE_BLOCK 0x0A
#define SMB_PROTO_READ_BLOCK  0x0B
#define SMB_PROTO_PROC_CALL   0x0C
//#define ;MB_PROTO_            0x0D
#define SMB_PROTO_WRITE_BYTES 0x0E
#define SMB_PROTO_READ_BYTES  0x0F
extern void Hook_GetSMBusReadBlockByteCount(BYTE *rblk_bc);

#endif