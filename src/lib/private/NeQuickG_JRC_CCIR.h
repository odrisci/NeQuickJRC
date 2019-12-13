/**
 * CCIR coefficients for the foF2 and M(3000)F2 models as preloaded constants.<br>
 *
 * The usage of the CCIR as preloaded constants instead of files is a customer requirement.
 * The modules are included with the compilation option FTR_MODIP_CCIR_AS_CONSTANTS=1<br>
 *
 * These are Coefficients for the foF2 and M(3000)F2 models
 * recommended by the Comite Consultatif International des Radiocommunications (CCIR).
 * They are numerical grid maps which describe the regular variation of the ionosphere.
 * They are used to derive other variables such as critical frequencies and transmission factors.
 * In NeQuick, foF2 and M(3000)F2 are derived from R12 and the CCIR files, see [Annex A].
 *
 * TBD: date for coeff?
 *
 * These coefficients are stored in the ccirXX.asc files and include the spherical harmonic coefficients
 * representing the development of monthly median foF2 and M(3000)F2 all over the world.
 * The coefficients correspond to low (Sun Spot Number=0) and high (Sun Spot Number=100) solar activity conditions.
 * For other Sun Spot Number activity the coefficients must be interpolated (or extrapolated) to obtain the values
 * corresponding to the required solar activity.
 *
 * The CCIR file naming convention is ccirXX.asc where each XX means month + 10.
 * This convention has been followed for the definiton of the constants.
 *
 * Each file ccirXX.asc contains 2858 values sequentially organized as follows:
 * {f2 1,1,1, f2 1,1,2, ..., f2 1,1,13,
 *  f2 1,2,1, f2 1,2,2, ..., f2 1,2,13,
 *  ...,
 *  f2 1,76,1, f2 1,76,2, ..., f2 1,76,13,
 *  f2 2,1,1, f2 2,1,2, ..., f2 2,1,13,
 *  f2 2,2,1, f2 2,2,2, ..., f2 2,2,13,
 *  ...,
 *  f2 2,76,1, f2 2,76,2, ..., f2 2,76,13,
 *  fm3 1,1,1, fm3 1,1,2, ..., fm3 1,1,9,
 *  fm3 1,2,1, fm3 1,2,2, ..., fm3 1,2,9,
 *  ...,
 *  fm3 1,49,1, fm3 1,49,2, ..., fm3 1,49,9,
 *  fm3 2,1,1,  fm3 2,1,2, ..., fm3 2,1,9,
 *  fm3 2,2,1, fm3 2,2,2, ..., fm3 2,2,9,
 *  ...,
 *  fm3 2,49,1, fm3 2,49,2, ..., fm3 2,49,9}
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_CCIR_H
#define NEQUICK_G_JRC_CCIR_H

#include <stdint.h>

#include "NeQuickG_JRC_iono_F2_layer_fourier_coefficients.h"

/** F2 coefficient array for January*/
extern const F2_coefficient_array_t g_NeQuickG_ccir11_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for January*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir11_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for February*/
extern const F2_coefficient_array_t g_NeQuickG_ccir12_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for February*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir12_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for February*/
extern const F2_coefficient_array_t g_NeQuickG_ccir13_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for February*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir13_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for April*/
extern const F2_coefficient_array_t g_NeQuickG_ccir14_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for April*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir14_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for May*/
extern const F2_coefficient_array_t g_NeQuickG_ccir15_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for May*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir15_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for June*/
extern const F2_coefficient_array_t g_NeQuickG_ccir16_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for June*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir16_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for July*/
extern const F2_coefficient_array_t g_NeQuickG_ccir17_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for July*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir17_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for August*/
extern const F2_coefficient_array_t g_NeQuickG_ccir18_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for August*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir18_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for September*/
extern const F2_coefficient_array_t g_NeQuickG_ccir19_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for September*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir19_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for October*/
extern const F2_coefficient_array_t g_NeQuickG_ccir20_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for October*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir20_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for November*/
extern const F2_coefficient_array_t g_NeQuickG_ccir21_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for November*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir21_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** F2 coefficient array for December*/
extern const F2_coefficient_array_t g_NeQuickG_ccir22_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
/** Fm3 coefficient array for December*/
extern const Fm3_coefficient_array_t g_NeQuickG_ccir22_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];

/** Get the F2 coefficients for a given month
 * @param[in] month valid range is [@ref NEQUICK_G_JRC_MONTH_JANUARY - @ref NEQUICK_G_JRC_MONTH_DECEMBER]
 * @param[out] ppF2 F2 Coefficients
 * @return on success NEQUICK_OK
 */
extern int32_t CCIR_get_ionosonde_F2(
  uint_fast8_t month,
  F2_coefficient_array_t** ppF2);

/** Get the Fm3 coefficients for a given month
 * @param[in] month valid range is [@ref NEQUICK_G_JRC_MONTH_JANUARY - @ref NEQUICK_G_JRC_MONTH_DECEMBER]
 * @param[out] ppFm3 Fm3 Coefficients
 * @return on success NEQUICK_OK
 */
extern int32_t CCIR_get_ionosonde_Fm3(
  uint_fast8_t month,
  Fm3_coefficient_array_t** ppFm3);

#endif // NEQUICK_G_JRC_CCIR_H
