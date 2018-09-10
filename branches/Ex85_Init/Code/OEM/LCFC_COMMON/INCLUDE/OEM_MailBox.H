/*-----------------------------------------------------------------------------
 * TITLE: OEM_MAILBOX.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef  OEM_MailBox_H
#define OEM_MailBox_H

extern void RamDebug(unsigned char dbgcode);
#if Support_COMPAL_MFG_TOOL
extern void ThermalMailBox(void);
#endif
#endif