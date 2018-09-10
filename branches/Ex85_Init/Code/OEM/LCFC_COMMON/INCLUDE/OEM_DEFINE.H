/*
--------------------------------------------------------------------
  Filename: OEM_DEFINE.H

  Purpose : LCFC define

  Author  : Ivan Chen 2016/03/14
--------------------------------------------------------------------
*/

#ifndef OEM_DEFINE_H
#define OEM_DEFINE_H
//------------------------------------------------------------------
//Typc-C chip
#define CCG2   1
#define CCG3   2
#define TPS65982  3
#define RTS5455   4

//charger IC chip
#define BQ24780S 0
#define BQ25700  1

//Battery low votlage shutdown protect rule
#define LowVot_LGNotLG  0
#define LowVot_3S4S     1

//platform chip
#define INTEL           1
#define AMD             0
#define NVidia          2

//ITE Chip
#define IT8586     0x8586  //chip id
#define IT8986     0x8986  //chip id
#define IT8996     0x8996  //chip id

//Keyboard 
#define CS13KB     0    //CS13 keyboard define 
#define CS16KB     1    //CS16 keyboard define 
#define Matrixtable1 1    //EE design follow up original E series design
#define Matrixtable2 2    //EE design follow up keyboard design
//------------------------------------------------------------------
#endif  //OEM_DEFINE_H