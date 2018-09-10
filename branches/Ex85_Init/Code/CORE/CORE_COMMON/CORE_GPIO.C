/*-----------------------------------------------------------------------------
 * TITLE: CORE_GPIO.C
 *
 * Author : Dino
 *
 * Copyright (c) ITE INC. All Rights Reserved.
 *
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>
// 8996 +
void gpio_fw_default_voltage_selection(void)
{
	
	
    /* GCR19 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPB5_1P8V__
    SET_MASK(GCR19, GPB5VS);
    #else
    CLEAR_MASK(GCR19, GPB5VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPB6_1P8V__
    SET_MASK(GCR19, GPB6VS);
    #else
    CLEAR_MASK(GCR19, GPB6VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPC1_1P8V__
    SET_MASK(GCR19, GPC1VS);
    #else
    CLEAR_MASK(GCR19, GPC1VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPC2_1P8V__
    SET_MASK(GCR19, GPC2VS);
    #else
    CLEAR_MASK(GCR19, GPC2VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPC7_1P8V__
    SET_MASK(GCR19, GPC7VS);
    #else
    CLEAR_MASK(GCR19, GPC7VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPD0_1P8V__
    SET_MASK(GCR19, GPD0VS);
    #else
    CLEAR_MASK(GCR19, GPD0VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPD1_1P8V__
    SET_MASK(GCR19, GPD1VS);
    #else
    CLEAR_MASK(GCR19, GPD1VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPD2_1P8V__
    SET_MASK(GCR19, GPD2VS);
    #else
    CLEAR_MASK(GCR19, GPD2VS);
    #endif

    /* GCR20 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPD3_1P8V__
    SET_MASK(GCR20, GPD3VS);
    #else
    CLEAR_MASK(GCR20, GPD3VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPD4_1P8V__
    SET_MASK(GCR20, GPD4VS);
    #else
    CLEAR_MASK(GCR20, GPD4VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPE0_1P8V__
    SET_MASK(GCR20, GPE0VS);
    #else
    CLEAR_MASK(GCR20, GPE0VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPE6_1P8V__
    SET_MASK(GCR20, GPE6VS);
    #else
    CLEAR_MASK(GCR20, GPE6VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPE7_1P8V__
    SET_MASK(GCR20, GPE7VS);
    #else
    CLEAR_MASK(GCR20, GPE7VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF2_1P8V__
    SET_MASK(GCR20, GPF2VS);
    #else
    CLEAR_MASK(GCR20, GPF2VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF3_1P8V__
    SET_MASK(GCR20, GPF3VS);
    #else
    CLEAR_MASK(GCR20, GPF3VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF4_1P8V__
    SET_MASK(GCR20, GPF4VS);
    #else
    CLEAR_MASK(GCR20, GPF4VS);
    #endif

    /* GCR21 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPF5_1P8V__
    SET_MASK(GCR21, GPF5VS);
    #else
    CLEAR_MASK(GCR21, GPF5VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF6_1P8V__
    SET_MASK(GCR21, GPF6VS);
    #else
    CLEAR_MASK(GCR21, GPF6VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF7_1P8V__
    SET_MASK(GCR21, GPF7VS);
    #else
    CLEAR_MASK(GCR21, GPF7VS);
    #endif

	#ifdef __DEF_VOLTAGE_SELECTION_GPG1_1P8V__
    SET_MASK(GCR21, GPG1VS);
    #else
    CLEAR_MASK(GCR21, GPG1VS);
    #endif

	#ifdef __DEF_VOLTAGE_SELECTION_GPG6_1P8V__
    SET_MASK(GCR21, GPG6VS);
    #else
    CLEAR_MASK(GCR21, GPG6VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPH0_1P8V__
    SET_MASK(GCR21, GPH0VS);
    #else
    CLEAR_MASK(GCR21, GPH0VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPH1_1P8V__
    SET_MASK(GCR21, GPH1VS);
    #else
    CLEAR_MASK(GCR21, GPH1VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPH2_1P8V__
    SET_MASK(GCR21, GPH2VS);
    #else
    CLEAR_MASK(GCR21, GPH2VS);
    #endif

    /* GCR27 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPH0_1P05V__
    SET_MASK(GCR27, V1P05GPH0VS);
    #else
    CLEAR_MASK(GCR27, V1P05GPH0VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPE7_1P05V__
    SET_MASK(GCR27, V1P05GPE7VS);
    #else
    CLEAR_MASK(GCR27, V1P05GPE7VS);
    #endif

	#ifdef __DEF_VOLTAGE_SELECTION_GPE6_1P05V__
    SET_MASK(GCR27, V1P05GPE6VS);
    #else
    CLEAR_MASK(GCR27, V1P05GPE6VS);
    #endif

	#ifdef __DEF_VOLTAGE_SELECTION_GPE0_1P05V__
    SET_MASK(GCR27, V1P05GPE0VS);
    #else
    CLEAR_MASK(GCR27, V1P05GPE0VS);
    #endif

	#ifdef __DEF_VOLTAGE_SELECTION_GPD4_1P05V__
    SET_MASK(GCR27, V1P05GPD4VS);
    #else
    CLEAR_MASK(GCR27, V1P05GPD4VS);
    #endif

	 #ifdef __DEF_VOLTAGE_SELECTION_GPD1_1P05V__
    SET_MASK(GCR27, V1P05GPD1VS);
    #else
    CLEAR_MASK(GCR27, V1P05GPD1VS);
    #endif

	#ifdef __DEF_VOLTAGE_SELECTION_GPD0_1P05V__
    SET_MASK(GCR27, V1P05GPD0VS);
    #else
    CLEAR_MASK(GCR27, V1P05GPD0VS);
    #endif

	#ifdef __DEF_VOLTAGE_SELECTION_GPB5_1P05V__
    SET_MASK(GCR27, V1P05GPB5VS);
    #else
    CLEAR_MASK(GCR27, V1P05GPB5VS);
    #endif
}

void gpio_fw_default_current_selection(void)
{
	 /* GCR24 */
    #ifdef __DEF_CURRENT_SELECTION_GPA7_2MA__
    SET_MASK(GCR24, GPA7CS);
    #else
    CLEAR_MASK(GCR24, GPA7CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPA6_2MA__
    SET_MASK(GCR24, GPA6CS);
    #else
    CLEAR_MASK(GCR24, GPA6CS);
    #endif

	 #ifdef __DEF_CURRENT_SELECTION_GPA5_2MA__
    SET_MASK(GCR24, GPA5CS);
    #else
    CLEAR_MASK(GCR24, GPA5CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPA4_2MA__
    SET_MASK(GCR24, GPA4CS);
    #else
    CLEAR_MASK(GCR24, GPA4CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPA3_2MA__
    SET_MASK(GCR24, GPA3CS);
    #else
    CLEAR_MASK(GCR24, GPA3CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPA2_2MA__
    SET_MASK(GCR24, GPA2CS);
    #else
    CLEAR_MASK(GCR24, GPA2CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPA1_2MA__
    SET_MASK(GCR24, GPA1CS);
    #else
    CLEAR_MASK(GCR24, GPA1CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPA0_2MA__
    SET_MASK(GCR24, GPA0CS);
    #else
    CLEAR_MASK(GCR24, GPA0CS);
    #endif

	/* GCR25 */
    #ifdef __DEF_CURRENT_SELECTION_GPF0_2MA__
    SET_MASK(GCR25, GPF0CS);
    #else
    CLEAR_MASK(GCR25, GPF0CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPE3_2MA__
    SET_MASK(GCR25, GPE3CS);
    #else
    CLEAR_MASK(GCR25, GPE3CS);
    #endif

	 #ifdef __DEF_CURRENT_SELECTION_GPE2_2MA__
    SET_MASK(GCR25, GPE2CS);
    #else
    CLEAR_MASK(GCR25, GPE2CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPE1_2MA__
    SET_MASK(GCR25, GPE1CS);
    #else
    CLEAR_MASK(GCR25, GPE1CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPD5_2MA__
    SET_MASK(GCR25, GPD5CS);
    #else
    CLEAR_MASK(GCR25, GPD5CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPC5_2MA__
    SET_MASK(GCR25, GPC5CS);
    #else
    CLEAR_MASK(GCR25, GPC5CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPC3_2MA__
    SET_MASK(GCR25, GPC3CS);
    #else
    CLEAR_MASK(GCR25, GPC3CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPB2_2MA__
    SET_MASK(GCR25, GPB2CS);
    #else
    CLEAR_MASK(GCR25, GPB2CS);
    #endif
	
	/* GCR26 */
    #ifdef __DEF_CURRENT_SELECTION_GPH6_2MA__
    SET_MASK(GCR26, GPH6CS);
    #else
    CLEAR_MASK(GCR26, GPH6CS);
    #endif

    #ifdef __DEF_CURRENT_SELECTION_GPH5_2MA__
    SET_MASK(GCR26, GPH5CS);
    #else
    CLEAR_MASK(GCR26, GPH5CS);
    #endif

	#ifdef __DEF_CURRENT_SELECTION_GPH4_2MA__
    SET_MASK(GCR26, GPH4CS);
    #else
    CLEAR_MASK(GCR26, GPH4CS);
    #endif

    #ifdef __DEF_CURRENT_SELECTION_GPH3_2MA__
    SET_MASK(GCR26, GPH3CS);
    #else
    CLEAR_MASK(GCR26, GPH3CS);
    #endif

   #ifdef __DEF_CURRENT_SELECTION_GPF1_2MA__
   SET_MASK(GCR26, GPF1CS);
   #else
   CLEAR_MASK(GCR26, GPF1CS);
   #endif

}
// 8996 -

