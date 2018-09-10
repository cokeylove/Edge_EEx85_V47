/*-----------------------------------------------------------------------------
 * TITLE: OEM_HOOK.C
 *
 * Author : ITE
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//
// Fan Table Struct
//

/*****************************************************************************/
//  Delta FAN Control. (Updated by Kenobi_Intel_FAN_Control_Table_ver03_20160704)
/*****************************************************************************/
//-----------------------------------------------------------------------------
const FanControlstruct code DeltaFanCtlTableST_OS_100_14[] = 
{
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {48,   42,    30,     30},  // 1
    {51,   45,    34,     44},  // 2
    {54,   48,    38,     52},  // 3
    {57,   50,    41,     58},  // 4
    {59,   52,    44,     63},  // 5
    {73,   56,    48,     72}   // 6 
};

/*const FanControlstruct code DeltaFanCtlTable_OS_105_14[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},  // 0
    {55,   40,    28,     41},  // 1
    {60,   45,    32,     50},  // 2
    {70,   55,    35,     54},  // 3
    {75,   65,    37,     57},  // 4
    {85,   70,    40,     63}   // 5
};

const FanControlstruct code DeltaVGAFanCtlTable_OS_105_14[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},  // 0
    {70,   50,    35,     54},  // 1
    {75,   65,    37,     57},  // 2
    {85,   70,    40,     63},  // 3
    {93,   78,    40,     63}   // 4
};

const FanControlstruct code DeltaFanTable_DOS_105_14[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},  // 0
    {55,   40,    28,     41},  // 1
    {60,   45,    32,     50},  // 2
    {70,   55,    35,     54},  // 3
    {75,   65,    37,     57},  // 4
    {80,   70,    40,     63},  // 5
    {90,   70,    40,     100}  // 6
};*/
const FanControlstruct code DeltaFanCtlTable_OS_100_14[]=
{//  On    Off    RPM     PWM    Step
   {70,   00,    00,     00},  // 0
    {85,   65,    30,     30},  // 1
    {90,   75,    38,     52},  // 2
    {95,   81,    48,     63},  // 3
    {99,   85,    48,     72},  // 4
    //{90,   70,    48,     72},  // 5
};

const FanControlstruct code DeltaVGAFanCtlTable_OS_100_14[]=
{//  On    Off    RPM     PWM    Step
     {50,   00,    00,     00},  // 0
    {57,   40,    30,     30},  // 1
    {65,   45,    34,     44},  // 2
    {65,   50,    38,     52},  // 3
    {85,   60,    44,     63},  // 4
    {90,   70,    48,     72},  // 5
};

const FanControlstruct code DeltaFanTable_DOS_100_14[]=
{//  On    Off    RPM     PWM    Step
    {70,   00,    00,     00},  // 0
    {85,   65,    30,     30},  // 1
    {90,   75,    38,     52},  // 2
    {95,   81,    48,     63},  // 3
    {99,   85,    48,     72},  // 4
    //{90,   70,    48,     72},  // 5
};

/*****************************************************************************/
//  Delta THR Control.
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct code DeltaCPUTHRTable_OS_105_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
    {90,    104,   75,        85,          85,       102,      105}  // 0
};

const CPUTHRstruct code DeltaVGATHRTable_OS_105_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
    {85,    100,   78,        85,          80,          93,        105}  // 0
};

const CPUTHRstruct code DeltaCPUTHRTable_DOS_105_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    90,   0,         0,          0,      0,     105}  // 0
};
const CPUTHRstruct code DeltaCPUTHRTable_OS_100_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
   {85,    95,  80,        90,         75,     95,     100}  // 0 // 0
};

const CPUTHRstruct code DeltaVGATHRTable_OS_100_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
    {72,    86,   67,        80,          70,          85,        105}  // 0
};

const CPUTHRstruct code DeltaCPUTHRTable_DOS_100_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    95,   0,         0,          0,      0,     100}  // 0
};
//-----------------------------------------------------------------------------
//  Delta FAN Control. (Updated by Skywalker_Intel_FAN_Control_Table_ver04_20160722)
//-----------------------------------------------------------------------------
const FanControlstruct code DeltaFanCtlTableST_OS_100_15[] = 
{
//  On    Off    RPM     PWM    Step
	   {45,   00,    00,     00},  // 0
    {48,   42,    30,     30},  // 1
    {51,   45,    34,     44},  // 2
    {54,   48,    38,     52},  // 3
    {57,   50,    41,     58},  // 4
    {59,   52,    44,     63},  // 5
    {73,   56,    48,     72}   // 6 

};

/*const FanControlstruct code DeltaFanCtlTable_OS_105_15[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},  // 0
    {55,   40,    25,     37},  // 1
    {60,   45,    30,     46},  // 2
    {70,   55,    32,     52},  // 3
    {75,   65,    34,     55},  // 4
    {85,   70,    37,     61}   // 5
};

const FanControlstruct code DeltaVGAFanCtlTable_OS_105_15[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},  // 0
    {70,   50,    32,     52},  // 1
    {75,   65,    34,     55},  // 2
    {85,   70,    37,     61},  // 3
    {93,   78,    37,     61}   // 4
};

const FanControlstruct code DeltaFanTable_DOS_105_15[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},  // 0
    {55,   40,    25,     37},  // 1
    {60,   45,    30,     46},  // 2
    {70,   55,    32,     52},  // 3
    {75,   65,    34,     55},  // 4
    {80,   70,    37,     61},  // 5
    {90,   75,    48,     100}  // 6
};*/
const FanControlstruct code DeltaFanCtlTable_OS_100_15[]=
{//  On    Off    RPM     PWM    Step
   {70,   00,    00,     00},  // 0
    {85,   65,    30,     30},  // 1
    {90,   75,    38,     52},  // 2
    {95,   81,    48,     63},  // 3
    {99,   85,    48,     72},  // 4
    //{90,   70,    48,     72},  // 5
};

const FanControlstruct code DeltaVGAFanCtlTable_OS_100_15[]=
{//  On    Off    RPM     PWM    Step
    {70,   00,    00,     00},  // 0
    {85,   65,    30,     30},  // 1
    {90,   75,    38,     50},  // 2
    {95,   81,    47,     60},  // 3
    {99,   85,    47,     60},  // 4
    //{90,   70,    48,     72},  // 5
};

const FanControlstruct code DeltaFanTable_DOS_100_15[]=
{//  On    Off    RPM     PWM    Step
  {70,   00,    00,     00},  // 0
    {85,   65,    30,     30},  // 1
    {90,   75,    38,     52},  // 2
    {95,   81,    48,     63},  // 3
    {99,   85,    48,     72},  // 4
    //{90,   70,    48,     72},  // 5

};
/*****************************************************************************/
//  Delta THR Control.
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct code DeltaCPUTHRTable_OS_105_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
    {90,    104,   75,        85,          85,       102,      105}  // 0
};

const CPUTHRstruct code DeltaVGATHRTable_OS_105_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
    {85,    100,   78,        85,          80,          93,        105}  // 0
};

const CPUTHRstruct code DeltaCPUTHRTable_DOS_105_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    90,   0,         0,          0,      0,     105}  // 0
};
const CPUTHRstruct code DeltaCPUTHRTable_OS_100_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
  {85,    95,  80,        90,         75,     95,     100}  // 0  // 0
};

const CPUTHRstruct code DeltaVGATHRTable_OS_100_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
    {85,    100,   78,        85,          80,          93,        105}  // 0
};

const CPUTHRstruct code DeltaCPUTHRTable_DOS_100_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    95,   0,         0,          0,      0,     100}  // 0
};
//-----------------------------------------------------------------------------
/*****************************************************************************/
//  SUN FAN Control. (Updated by Lonestar_Gordon_FAN_Control_Table_PWM_Sunon_v03_0716)
/*****************************************************************************/
//-----------------------------------------------------------------------------
const FanControlstruct code SUNFanCtlTableST_OS_100_14[] = 
{
	
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {48,   42,    29,     30},  // 1
    {51,   45,    32,     44},  // 2
    {54,   48,    36,     58},  // 3
    {57,   50,    38,     63},  // 4
    {59,   52,    41,     72},  // 5
    {73,   56,    45,     72},  // 5
};

/*const FanControlstruct code SUNFanCtlTable_OS_105_14[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},  // 0
    {55,   40,    28,     32},  // 1
    {60,   45,    30,     34},  // 2
    {70,   55,    35,     44},  // 3
    {75,   65,    37,     48},  // 4
    {85,   70,    40,     52}   // 5
};

const FanControlstruct code SUNVGAFanCtlTable_OS_105_14[]=
{//  On    Off    RPM     PWM    Step
  {  55,   00,    00,     00},   // 0
  {  70,   50,    35,     44},   // 1
  {  75,   65,    37,     48},   // 2
  {  85,   70,    40,     52},   // 3
  {  93,   78,    40,     52}    // 4
};

const FanControlstruct code SUNFanTable_DOS_105_14[]=
{//  On    Off    RPM     PWM    Step
  {  50,   00,    00,     00},   // 0
  {  55,   40,    28,     32},   // 1
  {  60,   45,    30,     34},   // 2
  {  70,   55,    35,     44},   // 3
  {  75,   65,    37,     48},   // 4
  {  80,   70,    40,     52},   // 5
  {  90,   70,    54,     100}   // 6
};*/
const FanControlstruct code SUNFanCtlTable_OS_100_14[]=
{//  On    Off    RPM     PWM    Step
  {  70,   00,    00,     00},   // 0
  {  85,   65,    29,     30},   // 1
  {  90,   75,    36,     52},   // 2
  {  95,   81,    45,     63},   // 3
  //{  75,   65,    41,     63},   // 4
  //{  90,   70,    45,     72}    // 5
};

const FanControlstruct code SUNVGAFanCtlTable_OS_100_14[]=
{//  On    Off    RPM     PWM    Step
 {  50,   00,    00,     00},   // 0
  {  57,   40,    29,     30},   // 1
  {  65,   45,    32,     44},   // 2
  {  70,   50,    36,     52},   // 3
  {  85,   60,    41,     63},   // 4
  {  90,   70,    45,     72}    // 5
};

const FanControlstruct code SUNFanTable_DOS_100_14[]=
{//  On    Off    RPM     PWM    Step
  {  70,   00,    00,     00},   // 0
  {  85,   65,    29,     30},   // 1
  {  90,   75,    36,     52},   // 2
  {  95,   81,    45,     63},   // 3
  //{  75,   65,    41,     63},   // 4
  //{  90,   70,    45,     72}    // 5
};

/*****************************************************************************/
//  SUN THR Control.
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct code SUNCPUTHRTable_OS_105_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
  { 90,    104,    75,      85,    80,    102,      105}  // 0
};

const CPUTHRstruct code SUNVGATHRTable_OS_105_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
  { 85,   100,   78,        85,       80,     93,        105}  // 0
};

const CPUTHRstruct code SUNCPUTHRTable_DOS_105_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
  { 85,     90,    0,     0,      0,      0,     105}  // 0
};
const CPUTHRstruct code SUNCPUTHRTable_OS_100_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
  {85,    95,  80,        90,         75,     95,     100}  // 0  // 0
};

const CPUTHRstruct code SUNVGATHRTable_OS_100_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
  { 85,    100,   78,      85,     80,          93,        105}  // 0
};

const CPUTHRstruct code SUNCPUTHRTable_DOS_100_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
   { 85,    95,    75,      89,     85,     97,      100}  // 0
};
//-----------------------------------------------------------------------------
const FanControlstruct code SUNFanCtlTableST_OS_100_15[] = 
{
	
//  On    Off    RPM     PWM    Step
   {45,   00,    00,     00},  // 0
    {48,   42,    29,     30},  // 1
    {51,   45,    32,     44},  // 2
    {54,   48,    36,     58},  // 3
    {57,   50,    38,     63},  // 4
    {59,   52,    41,     72},  // 5
    {73,   56,    45,     72},  // 5
};

/*const FanControlstruct code SUNFanCtlTable_OS_105_15[]=
{//  On    Off    RPM     PWM    Step
  {  50,   00,    00,     00},   // 0
  {  55,   40,    27,     30},   // 1
  {  60,   45,    30,     35},   // 2
  {  70,   55,    32,     39},   // 3
  {  75,   65,    34,     43},   // 4
  {  85,   70,    37,     49}    // 5
};

const FanControlstruct code SUNVGAFanCtlTable_OS_105_15[]=
{//  On    Off    RPM     PWM    Step
  {  55,   00,    00,     00},   // 0
  {  70,   50,    32,     39},   // 1
  {  75,   65,    34,     43},   // 2
  {  85,   70,    37,     49},   // 3
  {  93,   78,    37,     49}    // 4
};

const FanControlstruct code SUNFanTable_DOS_105_15[]=
{//  On    Off    RPM     PWM    Step
  {  50,   00,    00,     00},   // 0
  {  55,   40,    27,     30},   // 1
  {  60,   45,    30,     35},   // 2
  {  70,   55,    32,     39},   // 3
  {  75,   65,    34,     43},   // 4
  {  80,   70,    37,     49},   // 5
  {  90,   70,    55,     100}   // 6
};*/
const FanControlstruct code SUNFanCtlTable_OS_100_15[]=
{//  On    Off    RPM     PWM    Step
	{  70,	 00,	00, 	00},   // 0
	{  85,	 65,	29, 	30},   // 1
	{  90,	 75,	36, 	52},   // 2
	{  95,	 81,	45, 	63},   // 3
	//{  75,   65,	  41,	  63},	 // 4
	//{  90,   70,	  45,	  72}	 // 5

};

const FanControlstruct code SUNVGAFanCtlTable_OS_100_15[]=
{//  On    Off    RPM     PWM    Step
	{  70,	 00,	00, 	00},   // 0
	 {	85,   65,	 29,	 30},	// 1
	 {	90,   75,	 32,	 44},	// 2
	 {	95,   81,	 36,	 52},	// 3

};

const FanControlstruct code SUNFanTable_DOS_100_15[]=
{//  On    Off    RPM     PWM    Step
	  {  70,   00,    00,     00},   // 0
  {  85,   65,    29,     30},   // 1
  {  90,   75,    36,     52},   // 2
  {  95,   81,    45,     63},   // 3
  //{  75,   65,    41,     63},   // 4
  //{  90,   70,    45,     72}    // 5

};

/*****************************************************************************/
//  SUN THR Control.
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct code SUNCPUTHRTable_OS_105_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
  { 90,    104,    75,      85,    85,    102,      105}  // 0
};

const CPUTHRstruct code SUNVGATHRTable_OS_105_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
  { 85,   100,   78,        85,     80,          93,        105}  // 0
};

const CPUTHRstruct code SUNCPUTHRTable_DOS_105_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
  { 85,     90,    0,         0,          0,      0,     105}  // 0
};
const CPUTHRstruct code SUNCPUTHRTable_OS_100_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
    { 85,    95,    75,      89,     85,     97,      100}  // 0
};

const CPUTHRstruct code SUNVGATHRTable_OS_100_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
  { 85,   100,   78,       85,     80,          93,        105}  // 0
};

const CPUTHRstruct code SUNCPUTHRTable_DOS_100_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
  { 85,     95,    0,         0,          0,      0,     100}  // 0
};
//-----------------------------------------------------------------------------


/*****************************************************************************/ 
//  AVC FAN Control. (Updated by Kenobi_Intel_FAN_Control_Table_ver03_20160704)
/*****************************************************************************/
//-----------------------------------------------------------------------------
const FanControlstruct code AVCFanCtlTableST_OS_100_14[] = 
{
	
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {48,   42,    30,     30},  // 1
    {51,   45,    34,     44},  // 2
    {54,   48,    38,     52},  // 3
    {57,   50,    40,     58},  // 4
    {59,   52,    43,     63},  // 5
    {73,   56,    47,     72},   // 6 
};

/*const FanControlstruct code AVCFanCtlTable_OS_105_14[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},   // 0
    {55,   40,    28,     38},   // 1
    {60,   45,    32,     47},   // 2
    {70,   55,    35,     53},   // 3
    {75,   65,    37,     58},   // 4
    {85,   70,    39,     64}    // 5
};

const FanControlstruct code AVCVGAFanCtlTable_OS_105_14[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},   // 0
    {70,   50,    35,     53},   // 1
    {75,   65,    37,     58},   // 2
    {85,   70,    39,     64},   // 3
    {93,   78,    39,     64}    // 4
};

const FanControlstruct code AVCFanTable_DOS_105_14[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},   // 0
    {55,   40,    28,     38},   // 1
    {60,   45,    32,     47},   // 2
    {70,   55,    35,     53},   // 3
    {75,   65,    37,     58},   // 4
    {80,   70,    39,     64},   // 5
    {90,   75,    43,     100}   // 6
};*/
const FanControlstruct code AVCFanCtlTable_OS_100_14[]=
{//  On    Off    RPM     PWM    Step
   {70,   00,    00,     00},  // 0
    {85,   65,    30,     30},  // 1
    {90,   75,    38,     50},  // 2
    {95,   81,    47,     60},  // 3
    {99,   85,    47,     60},  // 4
    //{90,   70,    48,     72},  // 5

};

const FanControlstruct code AVCVGAFanCtlTable_OS_100_14[]=
{//  On    Off    RPM     PWM    Step
   {  50,   00,    00,     00},   // 0
  {  57,   40,    30,     30},   // 1
  {  65,   45,    34,     44},   // 2
  {  70,   50,    38,     52},   // 3
  {  85,   60,    43,     63},   // 4
  {  90,   70,    47,     72},    // 5
};

const FanControlstruct code AVCFanTable_DOS_100_14[]=
{//  On    Off    RPM     PWM    Step
   {70,   00,    00,     00},  // 0
    {85,   65,    30,     30},  // 1
    {90,   75,    38,     50},  // 2
    {95,   81,    47,     60},  // 3
    {99,   85,    47,     60},  // 4
    //{90,   70,    48,     72},  // 5

};

/*****************************************************************************/
//  AVC THR Control.
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct code AVCCPUTHRTable_OS_105_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     TCCoff    TCCon    Shut  Step
    {90,    104,   75,        85,           85,       102,      105}  // 0
};

const CPUTHRstruct code AVCVGATHRTable_OS_105_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     PMAX_D2_off  PMAX_D2_on  Shut  Step
    {85,    100,   78,        85,           80,          93,        105}  // 0
};

const CPUTHRstruct code AVCCPUTHRTable_DOS_105_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    90,   0,         0,          0,      0,     105}  // 0
};
const CPUTHRstruct code AVCCPUTHRTable_OS_100_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     TCCoff    TCCon    Shut  Step
    {85,    95,   75,        89,           75,       94,      100}  // 0
};

const CPUTHRstruct code AVCVGATHRTable_OS_100_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     PMAX_D2_off  PMAX_D2_on  Shut  Step
    {85,    100,   78,        90,           80,          93,        105}  // 0
};

const CPUTHRstruct code AVCCPUTHRTable_DOS_100_14[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
   {85,    95,   75,        89,           75,       94,      100}  // 0
};
//-----------------------------------------------------------------------------
//  AVC FAN Control. (Updated by Skywalker_Intel_FAN_Control_Table_ver04_20160722)
//-----------------------------------------------------------------------------
const FanControlstruct code AVCFanCtlTableST_OS_100_15[] = 
{
	
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {48,   42,    30,     30},  // 1
    {51,   45,    34,     44},  // 2
    {54,   48,    38,     52},  // 3
    {57,   50,    40,     58},  // 4
    {59,   52,    43,     63},  // 5
    {73,   56,    47,     72},   // 6 
};

/*const FanControlstruct code AVCFanCtlTable_OS_105_15[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},   // 0
    {55,   40,    25,     35},   // 1
    {60,   45,    29,     44},   // 2
    {70,   55,    31,     51},   // 3
    {75,   65,    33,     57},   // 4
    {85,   70,    35,     60}    // 5
};

const FanControlstruct code AVCVGAFanCtlTable_OS_105_15[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},   // 0
    {70,   50,    31,     51},   // 1
    {75,   65,    33,     57},   // 2
    {85,   70,    35,     60},   // 3
    {93,   78,    35,     60}    // 4
};

const FanControlstruct code AVCFanTable_DOS_105_15[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},   // 0
    {55,   40,    25,     35},   // 1
    {60,   45,    29,     44},   // 2
    {70,   55,    31,     51},   // 3
    {75,   65,    33,     57},   // 4
    {80,   70,    35,     60},   // 5
    {90,   75,    41,     100}   // 6
};*/
const FanControlstruct code AVCFanCtlTable_OS_100_15[]=
{//  On    Off    RPM     PWM    Step
      {70,   00,    00,     00},  // 0
    {85,   65,    30,     30},  // 1
    {90,   75,    38,     50},  // 2
    {95,   81,    47,     60},  // 3
    {99,   85,    47,     60},  // 4
    //{90,   70,    48,     72},  // 5

};

const FanControlstruct code AVCVGAFanCtlTable_OS_100_15[]=
{//  On    Off    RPM     PWM    Step
     {  50,   00,    00,     00},   // 0
  {  57,   40,    30,     30},   // 1
  {  65,   45,    34,     44},   // 2
  {  70,   50,    38,     52},   // 3
  {  85,   60,    43,     63},   // 4
  {  90,   70,    47,     72},    // 5
};

const FanControlstruct code AVCFanTable_DOS_100_15[]=
{//  On    Off    RPM     PWM    Step
    {70,   00,    00,     00},  // 0
    {85,   65,    30,     30},  // 1
    {90,   75,    38,     50},  // 2
    {95,   81,    47,     60},  // 3
    {99,   85,    47,     60},  // 4
    //{90,   70,    48,     72},  // 5

};

/*****************************************************************************/
//  AVC THR Control.
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct code AVCCPUTHRTable_OS_105_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     TCCoff    TCCon    Shut  Step
    {90,    104,   75,        85,           85,       102,      105}  // 0
};

const CPUTHRstruct code AVCVGATHRTable_OS_105_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     PMAX_D2_off  PMAX_D2_on  Shut  Step
    {85,    100,   78,        85,           80,          93,        105}  // 0
};

const CPUTHRstruct code AVCCPUTHRTable_DOS_105_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    90,   0,         0,          0,      0,     105}  // 0
};

const CPUTHRstruct code AVCCPUTHRTable_OS_100_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     TCCoff    TCCon    Shut  Step
    {85,    95,   75,        89,           75,       94,      100}  // 0
};

const CPUTHRstruct code AVCVGATHRTable_OS_100_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     PMAX_D2_off  PMAX_D2_on  Shut  Step
    {85,    100,   78,        85,           80,          93,        105}  // 0
};

const CPUTHRstruct code AVCCPUTHRTable_DOS_100_15[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
   {85,    95,   75,        89,           75,       94,      100}  // 0
};
//-----------------------------------------------------------------------------
/*const FanTableList code TjMax105_14[3] = {
  {
  	DeltaFanCtlTable_ST_OS_100_14,
	sizeof (DeltaFanCtlTable_ST_OS_100_14) / sizeof (FanControlstruct),
    DeltaFanCtlTable_OS_105_14,
	sizeof (DeltaFanCtlTable_OS_105_14) / sizeof (FanControlstruct),
    DeltaVGAFanCtlTable_OS_105_14,
	sizeof (DeltaVGAFanCtlTable_OS_105_14) / sizeof (FanControlstruct),
    DeltaFanTable_DOS_105_14,
    sizeof (DeltaFanTable_DOS_105_14) / sizeof (FanControlstruct),
    DeltaCPUTHRTable_OS_105_14,
    DeltaVGATHRTable_OS_105_14,
    DeltaCPUTHRTable_DOS_105_14
  },
  {
  SUNFanCtlTable_ST_OS_100_14,
	sizeof (SUNFanCtlTable_ST_OS_100_14) / sizeof (FanControlstruct),
    SUNFanCtlTable_OS_105_14,
	sizeof (SUNFanCtlTable_OS_105_14) / sizeof (FanControlstruct),
    SUNVGAFanCtlTable_OS_105_14,
	sizeof (SUNVGAFanCtlTable_OS_105_14) / sizeof (FanControlstruct),
    SUNFanTable_DOS_105_14,
	sizeof (SUNFanTable_DOS_105_14) / sizeof (FanControlstruct),
	SUNCPUTHRTable_OS_105_14,
    SUNVGATHRTable_OS_105_14,
    SUNCPUTHRTable_DOS_105_14
  },
  {
  AVCFanCtlTable_ST_OS_100_14,
	sizeof (AVCFanCtlTable_ST_OS_100_14) / sizeof (FanControlstruct),
    AVCFanCtlTable_OS_105_14,
	sizeof (AVCFanCtlTable_OS_105_14) / sizeof (FanControlstruct),
    AVCVGAFanCtlTable_OS_105_14,
    sizeof (AVCVGAFanCtlTable_OS_105_14) / sizeof (FanControlstruct),
    AVCFanTable_DOS_105_14,
    sizeof (AVCFanTable_DOS_105_14) / sizeof (FanControlstruct),
    AVCCPUTHRTable_OS_105_14,
    AVCVGATHRTable_OS_105_14,
    AVCCPUTHRTable_DOS_105_14
  },
};*/
//-----------------------------------------------------------------------------
/*const FanTableList code TjMax105_15[3] = {
  {
  	DeltaFanCtlTable_ST_OS_100_15,
	sizeof (DeltaFanCtlTable_ST_OS_100_15) / sizeof (FanControlstruct),
    DeltaFanCtlTable_OS_105_15,
	sizeof (DeltaFanCtlTable_OS_105_15) / sizeof (FanControlstruct),
    DeltaVGAFanCtlTable_OS_105_15,
    sizeof (DeltaVGAFanCtlTable_OS_105_15) / sizeof (FanControlstruct),
    DeltaFanTable_DOS_105_15,
    sizeof (DeltaFanTable_DOS_105_15) / sizeof (FanControlstruct),
    DeltaCPUTHRTable_OS_105_15,
    DeltaVGATHRTable_OS_105_15,
    DeltaCPUTHRTable_DOS_105_15
  },
  {SUNFanCtlTable_ST_OS_100_15,
	sizeof (SUNFanCtlTable_ST_OS_100_15) / sizeof (FanControlstruct),
    SUNFanCtlTable_OS_105_15,
	sizeof (SUNFanCtlTable_OS_105_15) / sizeof (FanControlstruct),
    SUNVGAFanCtlTable_OS_105_15,
    sizeof (SUNVGAFanCtlTable_OS_105_15) / sizeof (FanControlstruct),
    SUNFanTable_DOS_105_15,
    sizeof (SUNFanTable_DOS_105_15) / sizeof (FanControlstruct),
    SUNCPUTHRTable_OS_105_15,
    SUNVGATHRTable_OS_105_15,
    SUNCPUTHRTable_DOS_105_15
  },
  {
  AVCFanCtlTable_ST_OS_100_15,
	sizeof (AVCFanCtlTable_ST_OS_100_15) / sizeof (FanControlstruct),
    AVCFanCtlTable_OS_105_15,
	sizeof (AVCFanCtlTable_OS_105_15) / sizeof (FanControlstruct),
    AVCVGAFanCtlTable_OS_105_15,
    sizeof (AVCVGAFanCtlTable_OS_105_15) / sizeof (FanControlstruct),
    AVCFanTable_DOS_105_15,
    sizeof (AVCFanTable_DOS_105_15) / sizeof (FanControlstruct),
    AVCCPUTHRTable_OS_105_15,
    AVCVGATHRTable_OS_105_15,
    AVCCPUTHRTable_DOS_105_15
  },
};*/

//-----------------------------------------------------------------------------
const FanTableList code TjMax100_14[3] = {
  {
  	DeltaFanCtlTableST_OS_100_14,
	sizeof (DeltaFanCtlTableST_OS_100_14) / sizeof (FanControlstruct),
    DeltaFanCtlTable_OS_100_14,
	sizeof (DeltaFanCtlTable_OS_100_14) / sizeof (FanControlstruct),
    DeltaVGAFanCtlTable_OS_100_14,
    sizeof (DeltaVGAFanCtlTable_OS_100_14) / sizeof (FanControlstruct),
    DeltaFanTable_DOS_100_14,
    sizeof (DeltaFanTable_DOS_100_14) / sizeof (FanControlstruct),
    DeltaCPUTHRTable_OS_100_14,
    DeltaVGATHRTable_OS_100_14,
    DeltaCPUTHRTable_DOS_100_14
  },
  {
   	SUNFanCtlTableST_OS_100_14,
	sizeof (SUNFanCtlTableST_OS_100_14) / sizeof (FanControlstruct),
    SUNFanCtlTable_OS_100_14,
	sizeof (SUNFanCtlTable_OS_100_14) / sizeof (FanControlstruct),
    SUNVGAFanCtlTable_OS_100_14,
    sizeof (SUNVGAFanCtlTable_OS_100_14) / sizeof (FanControlstruct),
    SUNFanTable_DOS_100_14,
    sizeof (SUNFanTable_DOS_100_14) / sizeof (FanControlstruct),
    SUNCPUTHRTable_OS_100_14,
    SUNVGATHRTable_OS_100_14,
    SUNCPUTHRTable_DOS_100_14
  },
  {
  	AVCFanCtlTableST_OS_100_14,
	sizeof (AVCFanCtlTableST_OS_100_14) / sizeof (FanControlstruct),
    AVCFanCtlTable_OS_100_14,
	sizeof (AVCFanCtlTable_OS_100_14) / sizeof (FanControlstruct),
    AVCVGAFanCtlTable_OS_100_14,
    sizeof (AVCVGAFanCtlTable_OS_100_14) / sizeof (FanControlstruct),
    AVCFanTable_DOS_100_14,
    sizeof (AVCFanTable_DOS_100_14) / sizeof (FanControlstruct),
    AVCCPUTHRTable_OS_100_14,
    AVCVGATHRTable_OS_100_14,
    AVCCPUTHRTable_DOS_100_14
  },
};
//-----------------------------------------------------------------------------
const FanTableList code TjMax100_15[3] = {
  {
  	DeltaFanCtlTableST_OS_100_15,
	sizeof (DeltaFanCtlTableST_OS_100_15) / sizeof (FanControlstruct),
    DeltaFanCtlTable_OS_100_15,
	sizeof (DeltaFanCtlTable_OS_100_15) / sizeof (FanControlstruct),
    DeltaVGAFanCtlTable_OS_100_15,
    sizeof (DeltaVGAFanCtlTable_OS_100_15) / sizeof (FanControlstruct),
    DeltaFanTable_DOS_100_15,
    sizeof (DeltaFanTable_DOS_100_15) / sizeof (FanControlstruct),
    DeltaCPUTHRTable_OS_100_15,
    DeltaVGATHRTable_OS_100_15,
    DeltaCPUTHRTable_DOS_100_15
  },
  {
  SUNFanCtlTableST_OS_100_15,
	sizeof (SUNFanCtlTableST_OS_100_15) / sizeof (FanControlstruct),
    SUNFanCtlTable_OS_100_15,
	sizeof (SUNFanCtlTable_OS_100_15) / sizeof (FanControlstruct),
    SUNVGAFanCtlTable_OS_100_15,
    sizeof (SUNVGAFanCtlTable_OS_100_15) / sizeof (FanControlstruct),
    SUNFanTable_DOS_100_15,
    sizeof (SUNFanTable_DOS_100_15) / sizeof (FanControlstruct),
    SUNCPUTHRTable_OS_100_15,
    SUNVGATHRTable_OS_100_15,
    SUNCPUTHRTable_DOS_100_15
  },
  {
  AVCFanCtlTableST_OS_100_15,
	sizeof (AVCFanCtlTableST_OS_100_15) / sizeof (FanControlstruct),
    AVCFanCtlTable_OS_100_15,
	sizeof (AVCFanCtlTable_OS_100_15) / sizeof (FanControlstruct),
    AVCVGAFanCtlTable_OS_100_15,
    sizeof (AVCVGAFanCtlTable_OS_100_15) / sizeof (FanControlstruct),
    AVCFanTable_DOS_100_15,
    sizeof (AVCFanTable_DOS_100_15) / sizeof (FanControlstruct),
    AVCCPUTHRTable_OS_100_15,
    AVCVGATHRTable_OS_100_15,
    AVCCPUTHRTable_DOS_100_15
  },
};

FanTableList code * data FanTblPtr;

void oem_FanTblPtrHook (void)
{
  BYTE MB_FAN_TJ_ID;
  // Sample1 --
  // MBID
  // Notice: If MBID has not be written on product line, it's default value would be '0xFF'.
  //         Whice means 15" will be selected.
  
  MB_FAN_TJ_ID =0x00;
  MB_FAN_TJ_ID |= (uMBID & BIT6);
  MB_FAN_TJ_ID |= (FAN_ID_TYPE <<4);
  MB_FAN_TJ_ID |= (CPU_TYPE & (BIT0|BIT1));
  
	switch(MB_FAN_TJ_ID & 0xCF)
	{
	  case 0x43:   // Tj105 & 15" 
			//FanTblPtr = TjMax105_15;			  
			break;
	  case 0x42:   // Tj100 & 15"
	  case 0x41: 	// Tj90 & 15"
	  case 0x40: 	// Tj85 & 15"
			FanTblPtr = TjMax100_15;
			break;  

	  case 0x03: 	// Tj105 & 14" 
            //FanTblPtr = TjMax105_14;			
			break;
	  case 0x02: 	// Tj100 & 14"
	  case 0x01:	  // Tj90 & 14"
	  case 0x00:	  // Tj85 & 14"
      default:
			FanTblPtr = TjMax100_14;
		    break;	    
  }

	switch(MB_FAN_TJ_ID)
	{
	  case 0x63:   // AVC Tj105 & 15" 
			FAN_PCH_Max = FAN_PCH_Max_AVC105_15;
			RPM_PCH_Max = RPM_PCH_Max_AVC105_15;				
			break;
	  case 0x62:  // AVC Tj100 & 15"
	  case 0x61:  // AVC Tj90 & 15"
	  case 0x60:  // AVC Tj85 & 15"
			FAN_PCH_Max = FAN_PCH_Max_AVC100_15;
			RPM_PCH_Max = RPM_PCH_Max_AVC100_15;
			break;	  
	  case 0x53:	 // Sunon Tj105 & 15" 
			FAN_PCH_Max = FAN_PCH_Max_SUN105_15;
			RPM_PCH_Max = RPM_PCH_Max_SUN105_15;			  
			break;
	  case 0x52:	// Sunon Tj100 & 15"
	  case 0x51:	// Sunon Tj90 & 15"
	  case 0x50:	// Sunon Tj85 & 15"
			FAN_PCH_Max = FAN_PCH_Max_SUN100_15;
			RPM_PCH_Max = RPM_PCH_Max_SUN100_15;
			break;
	  case 0x43:   // Delta Tj105 & 15" 
		  FAN_PCH_Max = FAN_PCH_Max_Delta105_15;
			RPM_PCH_Max = RPM_PCH_Max_Delta105_15;			  
			break;
	  case 0x42:  // Delta Tj100 & 15"
	  case 0x41: 	// Delta Tj90 & 15"
	  case 0x40: 	// Delta Tj85 & 15"
		  FAN_PCH_Max = FAN_PCH_Max_Delta100_15;
		  RPM_PCH_Max = RPM_PCH_Max_Delta100_15;
			break;

	  case 0x23:   // AVC Tj105 & 14" 
			FAN_PCH_Max = FAN_PCH_Max_AVC105_14;
			RPM_PCH_Max = RPM_PCH_Max_AVC105_14;				
			break;
	  case 0x22:  // AVC Tj100 & 14"
	  case 0x21:  // AVC Tj90 & 14"
	  case 0x20:  // AVC Tj85 & 14"
			FAN_PCH_Max = FAN_PCH_Max_AVC100_14;
			RPM_PCH_Max = RPM_PCH_Max_AVC100_14;
			break;	  
	  case 0x13:	 // Sunon Tj105 & 14" 
			FAN_PCH_Max = FAN_PCH_Max_SUN105_14;
			RPM_PCH_Max = RPM_PCH_Max_SUN105_14;			  
			break;
	  case 0x12:	// Sunon Tj100 & 14"
	  case 0x11:	// Sunon Tj90 & 14"
	  case 0x10:	// Sunon Tj85 & 14"
			FAN_PCH_Max = FAN_PCH_Max_SUN100_14;
			RPM_PCH_Max = RPM_PCH_Max_SUN100_14;
			break;
	  case 0x03: 	// Tj105 & 14" 
		  FAN_PCH_Max = FAN_PCH_Max_Delta105_14;
			RPM_PCH_Max = RPM_PCH_Max_Delta105_14;			
			break;
	  case 0x02: 	// Tj100 & 14"
	  case 0x01:	  // Tj90 & 14"
	  case 0x00:	  // Tj85 & 14"
    default:
		  FAN_PCH_Max = FAN_PCH_Max_Delta100_14;
		  RPM_PCH_Max = RPM_PCH_Max_Delta100_14;
		  break;	    
  }
}
