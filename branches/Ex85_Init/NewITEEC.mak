#--------------------------------------------------------------------
#
#  PURPOSE:
#
#      Makefile for ITE EC code.
#--------------------------------------------------------------------
#
#  2013/09/11       Restructure
#                                                -- Dustin Lee
#  ---
#
DUMMY: ALL

#--------------------------------------------------------------------
# Compiler Flags
#--------------------------------------------------------------------
CDirectives  = LA WL(1) CD OT(9, size) NOAREGS OR INCDIR(.\Code\CORE\INCLUDE\;.\Code\OEM\INCLUDE\;.\Code\CHIP\INCLUDE\)
CDirectives1 = LA WL(1) CD OT(9, size) NOAREGS OR SRC INCDIR(.\Code\CORE\INCLUDE\;.\Code\OEM\INCLUDE\;.\Code\CHIP\INCLUDE\)
CDirectives2 = LA WL(1) CD OT(8, size) NOAREGS OR INCDIR(.\Code\CORE\INCLUDE\;.\Code\OEM\INCLUDE\;.\Code\CHIP\INCLUDE\)
ADirectives  = SET (LA) DEBUG EP

CC           = C51
AS           = A51
Linker       = BL51

#--------------------------------------------------------------------
# Directory Declarations
#--------------------------------------------------------------------
CHIPSRCDIR   = Code\CHIP
CHIPINCDIR   = $(CHIPSRCDIR)\INCLUDE
CORESRCDIR   = Code\CORE
COREINCDIR   = $(CORESRCDIR)\INCLUDE
OEMSRCDIR    = Code\OEM
OEMINCDIR    = $(OEMSRCDIR)\INCLUDE
PROJSRCDIR    = Code\OEM
PROJINCDIR    = $(PROJSRCDIR)\INCLUDE

OBJDIR       = KeilMisc\Obj

#--------------------------------------------------------------------
# [CHIP] Object Files.
#--------------------------------------------------------------------
CHIPOBJS = \
        $(OBJDIR)\CORE_CHIPREGS.OBJ \
        $(OBJDIR)\CORE_ITEString.OBJ

CHIPINC = $(CHIPINCDIR)\*.H

#--------------------------------------------------------------------
# [CORE] Object Files.
#--------------------------------------------------------------------
COREOBJS = \
        $(OBJDIR)\CORE_IRQ.OBJ \
        $(OBJDIR)\CORE_BITADDR.OBJ \
        $(OBJDIR)\CORE_ACPI.OBJ \
        $(OBJDIR)\CORE_Memory.OBJ \
        $(OBJDIR)\CORE_HOSTIF.OBJ \
        $(OBJDIR)\CORE_MAIN.OBJ \
        $(OBJDIR)\CORE_PORT6064.OBJ \
        $(OBJDIR)\CORE_PS2.OBJ \
        $(OBJDIR)\CORE_SCAN.OBJ \
        $(OBJDIR)\CORE_SMBUS.OBJ \
        $(OBJDIR)\CORE_TIMERS.OBJ \
        $(OBJDIR)\CORE_XLT.OBJ \
        $(OBJDIR)\CORE_FLASH.OBJ \
        $(OBJDIR)\CORE_INIT.OBJ \
        $(OBJDIR)\CORE_ASM.OBJ \
        $(OBJDIR)\CORE_GPIO.OBJ \
        $(OBJDIR)\L51_BANK.OBJ \
        $(OBJDIR)\STARTUP.OBJ

COREINC = $(COREINCDIR)\*.H

#--------------------------------------------------------------------
# [OEM] Object Files.
#--------------------------------------------------------------------
OEMOBJS = \
        $(OBJDIR)\OEM_HOOK.OBJ \
        $(OBJDIR)\OEM_GPIO.OBJ \
        $(OBJDIR)\OEM_LED.OBJ \
        $(OBJDIR)\OEM_BEEP.OBJ \
        $(OBJDIR)\OEM_MAIN.OBJ \
        $(OBJDIR)\OEM_SCANTABS.OBJ \
        $(OBJDIR)\OEM_FAN.OBJ \
        $(OBJDIR)\OEM_FANTBL.OBJ \
        $(OBJDIR)\OEM_BATTERY.OBJ \
        $(OBJDIR)\OEM_EVENT.OBJ \
        $(OBJDIR)\OEM_POWERSEQ.OBJ \
        $(OBJDIR)\OEM_POWER.OBJ \
        $(OBJDIR)\OEM_SMBUS.OBJ \
        $(OBJDIR)\OEM_ADC.OBJ \
        $(OBJDIR)\OEM_MMB.OBJ \
        $(OBJDIR)\OEM_ACPI.OBJ \
        $(OBJDIR)\OEM_CIR.OBJ \
        $(OBJDIR)\OEM_FLASH.OBJ \
        $(OBJDIR)\OEM_INIT.OBJ \
        $(OBJDIR)\OEM_HOSTIF.OBJ \
        $(OBJDIR)\OEM_6064.OBJ \
        $(OBJDIR)\OEM_MEMORY.OBJ \
        $(OBJDIR)\OEM_MailBox.OBJ \
        $(OBJDIR)\OEM_Ver.OBJ \
        $(OBJDIR)\OEM_LCD.OBJ \
        $(OBJDIR)\OEM_PORT686C.OBJ \
        $(OBJDIR)\OEM_PECI.OBJ \
        $(OBJDIR)\OEM_PM3.OBJ \
        $(OBJDIR)\OEM_IRQ.OBJ \
        $(OBJDIR)\OEM_PM2.OBJ \
        $(OBJDIR)\OEM_PS2.OBJ \
        $(OBJDIR)\OEM_SPI.OBJ \
        $(OBJDIR)\OEM_LPC.OBJ \
        $(OBJDIR)\OEM_AOU.OBJ \
        $(OBJDIR)\OEM_TIMER.OBJ \
        $(OBJDIR)\OEM_HSPI.OBJ \
        $(OBJDIR)\OEM_ASM.OBJ \
        $(OBJDIR)\OEM_Debug.OBJ \
        $(OBJDIR)\OEM_BANK1_Func.OBJ \
        $(OBJDIR)\OEM_BANK2_Func.OBJ \
        $(OBJDIR)\OEM_EDGE.OBJ \
        $(OBJDIR)\Origa_Ecc.OBJ \
        $(OBJDIR)\Origa_Gf2nMath.OBJ \
        $(OBJDIR)\OEM_RFID.OBJ \
        $(OBJDIR)\OEM_GSNS.OBJ \
        $(OBJDIR)\OEM_CCG2.OBJ \
        $(OBJDIR)\OEM_CCG3.OBJ \
        $(OBJDIR)\OEM_RTS5455.OBJ \
        $(OBJDIR)\OEM_EnhancedPcBeep.OBJ \
        $(OBJDIR)\OEM_EEPROM.OBJ \
        $(OBJDIR)\OEM_VGA_AMD.OBJ \
        $(OBJDIR)\OEM_VGA_NV.OBJ \
	$(OBJDIR)\OEM_TAMPER.OBJ \
	$(OBJDIR)\OEM_ucsi.OBJ \
	$(OBJDIR)\OEM_BMA255.OBJ \
	$(OBJDIR)\OEM_EcCmd.OBJ \
        $(OBJDIR)\OEM_BANK3_Func.OBJ\
        $(OBJDIR)\OEM_BANK4_Func.OBJ\
         $(OBJDIR)\OEM_BANK5_Func.OBJ\
          $(OBJDIR)\OEM_BANK6_Func.OBJ
        

OEMINC = $(OEMINCDIR)\*.H
PROJINC = $(PROJINCDIR)\*.H

.SUFFIXES : .obj .c .a51
.PHONY: all

#--------------------------------------------------------------------
# Inference Rules
#--------------------------------------------------------------------
{$(CHIPSRCDIR)}.c{$(OBJDIR)}.obj:
    $(CC) $< $(CDirectives)
    move $(CHIPSRCDIR)\$(@B).OBJ KeilMisc\Obj
    move $(CHIPSRCDIR)\$(@B).LST KeilMisc\Lst

{$(CORESRCDIR)}.c{$(OBJDIR)}.obj:
    $(CC) $< $(CDirectives)
    move $(CORESRCDIR)\$(@B).OBJ KeilMisc\Obj
    move $(CORESRCDIR)\$(@B).LST KeilMisc\Lst

{$(CORESRCDIR)}.a51{$(OBJDIR)}.obj:
    $(AS) $< $(ADirectives)
    move $(CORESRCDIR)\$(@B).OBJ KeilMisc\Obj
    move $(CORESRCDIR)\$(@B).LST KeilMisc\Lst

{$(OEMSRCDIR)}.c{$(OBJDIR)}.obj:
    $(CC) $< $(CDirectives)
    move $(OEMSRCDIR)\$(@B).OBJ KeilMisc\Obj
    move $(OEMSRCDIR)\$(@B).LST KeilMisc\Lst

{$(PROJSRCDIR)}.c{$(OBJDIR)}.obj:
    $(CC) $< $(CDirectives)
    move $(PROJSRCDIR)\$(@B).OBJ KeilMisc\Obj
    move $(PROJSRCDIR)\$(@B).LST KeilMisc\Lst

#--------------------------------------------------------------------
# Special directives rules
#--------------------------------------------------------------------
$(OBJDIR)\CORE_ASM.OBJ:$(CORESRCDIR)\CORE_ASM.C $(CHIPINC) $(COREINC) $(OEMINC)
  $(CC) $(CORESRCDIR)\CORE_ASM.C $(CDirectives1)
  $(AS) $(CORESRCDIR)\CORE_ASM.SRC $(ADirectives)
  move $(CORESRCDIR)\CORE_ASM.OBJ KeilMisc\Obj
  move $(CORESRCDIR)\CORE_ASM.LST KeilMisc\Lst
  del $(CORESRCDIR)\CORE_ASM.SRC

$(OBJDIR)\OEM_HSPI.OBJ:$(OEMSRCDIR)\OEM_HSPI.C $(CHIPINC) $(COREINC) $(OEMINC)
  $(CC) $(OEMSRCDIR)\OEM_HSPI.C $(CDirectives1)
  $(AS) $(OEMSRCDIR)\OEM_HSPI.SRC $(ADirectives)
  move $(OEMSRCDIR)\OEM_HSPI.OBJ KeilMisc\Obj
  move $(OEMSRCDIR)\OEM_HSPI.LST KeilMisc\Lst
  del $(OEMSRCDIR)\OEM_HSPI.SRC

$(OBJDIR)\OEM_ASM.OBJ:$(OEMSRCDIR)\OEM_ASM.C $(CHIPINC) $(COREINC) $(OEMINC)
  $(CC) $(OEMSRCDIR)\OEM_ASM.C $(CDirectives1)
  $(AS) $(OEMSRCDIR)\OEM_ASM.SRC $(ADirectives)
  move $(OEMSRCDIR)\OEM_ASM.OBJ KeilMisc\Obj
  move $(OEMSRCDIR)\OEM_ASM.LST KeilMisc\Lst
  del $(OEMSRCDIR)\OEM_ASM.SRC

$(OBJDIR)\CORE_FLASH.OBJ:$(CORESRCDIR)\CORE_FLASH.C $(CHIPINC) $(COREINC) $(OEMINC)
  $(CC) $(CORESRCDIR)\CORE_FLASH.C $(CDirectives2)
  move $(CORESRCDIR)\CORE_FLASH.OBJ KeilMisc\Obj
  move $(CORESRCDIR)\CORE_FLASH.LST KeilMisc\Lst

$(OBJDIR)\OEM_FLASH.OBJ:$(OEMSRCDIR)\OEM_FLASH.C $(CHIPINC) $(COREINC) $(OEMINC)
  $(CC) $(OEMSRCDIR)\OEM_FLASH.C $(CDirectives2)
  move $(OEMSRCDIR)\OEM_FLASH.OBJ KeilMisc\Obj
  move $(OEMSRCDIR)\OEM_FLASH.LST KeilMisc\Lst



ALL: ITEEC.b0
#------------------------------------------------------------------
# Hex file to bin file
# Syntax :
#  Hex2bin -s 0x0000 -e b0 ITEEC.H00  # common + bank0 (0x0000 ~ 0xFFFF)
#  Hex2bin -s 0x8000 -e b1 ITEEC.H01  # bank1 (0x10000 ~ 0x17FFF)
#  Hex2bin -s 0x8000 -e b2 ITEEC.H02  # bank2 (0x18000 ~ 0x1FFFF)
#  Hex2bin -s 0x8000 -e b3 ITEEC.H03  # bank3 (0x20000 ~ 0x27FFF)

#  OH51 ITEEC.B00 HEXFILE(ITEEC.H00)  # common + bank0 (0x0000 ~ 0xFFFF)
#  OH51 ITEEC.B01 HEXFILE(ITEEC.H01)  # bank1 (0x10000 ~ 0x17FFF)
#  OH51 ITEEC.B02 HEXFILE(ITEEC.H02)  # bank2 (0x18000 ~ 0x1FFFF)
#  OH51 ITEEC.B03 HEXFILE(ITEEC.H03)  # bank3 (0x20000 ~ 0x27FFF)
#------------------------------------------------------------------
ITEEC.b0: ITEEC.H00
  Hex2bin -s 0x0000 -e b0 ITEEC.H00
  Hex2bin -s 0x8000 -e b1 ITEEC.H01
  Hex2bin -s 0x8000 -e b2 ITEEC.H02
  Hex2bin -s 0x8000 -e b3 ITEEC.H03
  Hex2bin -s 0x8000 -e b4 ITEEC.H04
  Hex2bin -s 0x8000 -e b5 ITEEC.H05
  Hex2bin -s 0x8000 -e b6 ITEEC.H06

ITEEC.H00:ITEEC.B00
  OH51 ITEEC.B00 HEXFILE(ITEEC.H00)
  OH51 ITEEC.B01 HEXFILE(ITEEC.H01)
  OH51 ITEEC.B02 HEXFILE(ITEEC.H02)
  OH51 ITEEC.B03 HEXFILE(ITEEC.H03)
  OH51 ITEEC.B04 HEXFILE(ITEEC.H04)
  OH51 ITEEC.B05 HEXFILE(ITEEC.H05)
  OH51 ITEEC.B06 HEXFILE(ITEEC.H06)

ITEEC.B00:ITEEC.ABS
  OC51 ITEEC.ABS
#------------------------------------------------------------------
# Link all obj fils
#------------------------------------------------------------------

ITEEC.ABS: $(CHIPOBJS) $(COREOBJS) $(OEMOBJS) $(CHIPINC) $(COREINC) $(OEMINC) $(PROJINC)
  $(Linker) @ITEEC.lin
