/**
 * CCIR coefficients for the foF2 and M(3000)F2 models as preloaded constants.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_CCIR.h"

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_error.h"
#include "NeQuickG_JRC_time.h"

int32_t CCIR_get_ionosonde_F2(
  uint_fast8_t month,
  F2_coefficient_array_t** ppF2) {

  switch (month) {
  case NEQUICK_G_JRC_MONTH_JANUARY:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir11_F2;
    break;
  case NEQUICK_G_JRC_MONTH_FEBRUARY:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir12_F2;
    break;
  case NEQUICK_G_JRC_MONTH_MARCH:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir13_F2;
    break;
  case NEQUICK_G_JRC_MONTH_APRIL:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir14_F2;
    break;
  case NEQUICK_G_JRC_MONTH_MAY:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir15_F2;
    break;
  case NEQUICK_G_JRC_MONTH_JUNE:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir16_F2;
    break;
  case NEQUICK_G_JRC_MONTH_JULY:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir17_F2;
    break;
  case NEQUICK_G_JRC_MONTH_AUGUST:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir18_F2;
    break;
  case NEQUICK_G_JRC_MONTH_SEPTEMBER:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir19_F2;
    break;
  case NEQUICK_G_JRC_MONTH_OCTOBER:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir20_F2;
    break;
  case NEQUICK_G_JRC_MONTH_NOVEMBER:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir21_F2;
    break;
  case NEQUICK_G_JRC_MONTH_DECEMBER:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppF2 = (F2_coefficient_array_t*)&g_NeQuickG_ccir22_F2;
    break;
  default:
    *ppF2 = NULL;
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_INPUT_DATA,
      NEQUICK_ERROR_CODE_BAD_MONTH,
      "Invalid month %u", month);
  }

  return NEQUICK_OK;
}

int32_t CCIR_get_ionosonde_Fm3(
  uint_fast8_t month,
  Fm3_coefficient_array_t** ppFm3) {

  switch (month) {
  case NEQUICK_G_JRC_MONTH_JANUARY:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir11_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_FEBRUARY:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir12_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_MARCH:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir13_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_APRIL:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir14_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_MAY:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir15_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_JUNE:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir16_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_JULY:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir17_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_AUGUST:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir18_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_SEPTEMBER:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir19_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_OCTOBER:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir20_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_NOVEMBER:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir21_Fm3;
    break;
  case NEQUICK_G_JRC_MONTH_DECEMBER:
    //lint -e{545, 740} & is suspicious to lint because g_NeQuickG_ccir11_F2 is an array, but that's okay.
    *ppFm3 = (Fm3_coefficient_array_t*)&g_NeQuickG_ccir22_Fm3;
    break;
  default:
    *ppFm3 = NULL;
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_INPUT_DATA,
      NEQUICK_INPUT_DATA_BAD_MONTH,
      "Invalid month %u", month);
  }

  return NEQUICK_OK;
}
