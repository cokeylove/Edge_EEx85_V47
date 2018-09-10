/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * FILE NAME: OCTYPES.H
 *
 * Copyright (c) Compal Corporation 2004.
 * All rights reserved.
 *
 * DESCRIPTION:Contains data type definitions for common.
 *
 * REVISION HISTORY (Started March, 2004 by Kelly Lin)
 *
 * 2004,3 Initial code.
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#ifndef OCTYPES_H
#define OCTYPES_H

/* ========================================================================
 * Define a name for a bit/byte/word.
 * ======================================================================== */
typedef unsigned char  FLAG;
typedef unsigned char  BYTE;
typedef unsigned short  WORD;
typedef unsigned long  DWORD;

/* ========================================================================
 * Define function pointer.
 * ======================================================================== */
//Define a pointer to a function thatreturn nothing and  take nothing
typedef void (*FunPtr_V_V)(void);
//Define a pointer to a function that return nothing and take a argument of WORD
typedef void (*FunPtr_V_W)(WORD);
//Define a pointer to a function that return nothing and take a argument of BYTE
typedef void (*FunPtr_V_B)(BYTE);

//Define a pointer to a function thatreturn return a WORD and take nothing
typedef WORD (*FunPtr_W_V)(void);
//Define a pointer to a function that return a WORD and take a argument of WORD
typedef WORD (*FunPtr_W_W)(WORD);
//Define a pointer to a function that return a WORD and take a argument of BTYE
typedef WORD (*FunPtr_W_B)(BYTE);

//Define a pointer to a function thatreturn return a BYTE and take nothing
typedef BYTE (*FunPtr_B_V)(void);
//Define a pointer to a function that return a BYTE and take a argument of WORD
typedef BYTE (*FunPtr_B_W)(WORD);
//Define a pointer to a function that return a BYTE and take a argument of BTYE
typedef BYTE (*FunPtr_B_B)(BYTE);

/* ========================================================================
 * Define a pointer structure for a function.
 * ======================================================================== */
typedef BYTE    *PORT_BYTE_PNTR;
typedef WORD    *PORT_WORD_PNTR;

#define PORT_BYTE *(PORT_BYTE_PNTR)
#define PORT_WORD *(PORT_WORD_PNTR)

#endif /* ifndef OCTYPES_H */
