/** Effective Ionisation Level (Az)<br>
 *
 * An index that represents solar activity in
 *  the same fashion as F10.7. It is used to drive NeQuick model for a daily
 *  use in Galileo, instead of the monthly mean original behaviour when using
 *  F10.7. It is also expressed in sfu. The Az parameter is a continuous function
 *  of MODIP at the receiver location. Az is expressed by three coefficients:<br>
 *              Az = ai<SUB>0</SUB> + ai<SUB>1</SUB>*MODIP + ai<SUB>2</SUB>*(MODIP)<SUP>2</SUP><br>
 *  The three coefficients, ai0, ai1 and ai2 are transmitted to the users in
 *  the Galileo navigation broadcast message.
 * Note that sfu (solar flux unit) is not a SI unit but can be converted as:
 *  1 sfu = 10<SUP>-22</SUP> W/(m<SUP>2</SUP>*Hz)
 *
 * Note that Az is not updated with MODIP along the ray.
 * Instead, for each ray, Az is fixed to the one corresponding to MODIP at the
 * receiver location.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_SOLAR_ACTIVITY_H
#define NEQUICK_G_JRC_SOLAR_ACTIVITY_H

#include <math.h>
#include <stddef.h>

#include "ITU_R_P_371_8.h"
#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_MODIP.h"

/** 3.3 The operational range for the Effective Ionisation Parameter is between 0
 * and 400 sfu
 */
#define NEQUICK_G_AZ_MIN_VALUE_SFU (0.0)

/** 3.3 The operational range for the Effective Ionisation Parameter is between 0
 * and 400 sfu
 */
#define NEQUICK_G_AZ_MAX_VALUE_SFU (400.0)

/** AZ coefficients */
typedef struct AZ_coefficients_st {
  /** AZ coefficients */
  double_t a_sfu[NEQUICKG_AZ_COEFFICIENTS_COUNT];
} AZ_coefficients_t;

/** solar activity */
typedef struct solar_activity_st {
  /** AZ coefficients */
  AZ_coefficients_t effective_ionisation_level_coeff;
  /** Effective Ionisation Level (Az) in sfu */
  double_t effective_ionisation_level_sfu;
  /** Effective sun spot count */
  double_t effective_sun_spot_count;
} solar_activity_t;

/** Set the AZ coefficients in the context
 *
 * @param[in, out] pSolar_activity solar activity context
 * @param[in] pAi ionospheric coefficients (a<SUB>0</SUB>, a<SUB>1</SUB>, a<SUB>2</SUB>)
 * @param[in] coeff_count number of ionospheric coefficients, must be at least #NEQUICKG_AZ_COEFFICIENTS_COUNT
 */
extern int32_t solar_activity_coefficients_set(
  solar_activity_t* const pSolar_activity,
  const double_t * const pAi,
  size_t coeff_count);

/** Compute the Effective Ionisation Level Az at the given receiver location
 * (having MODIP) as a function of the coefficients (a<SUB>0</SUB>, a<SUB>1</SUB>, a<SUB>2</SUB>) broadcast
 * in the navigation message.<br>
 * Az = ai<SUB>0</SUB> + ai<SUB>1</SUB>*MODIP + ai<SUB>2</SUB>*(MODIP)<SUP>2</SUP>
 * also the effective sun spot count is calculated
 *
 * @param[in, out] pContext solar activity context with ionospheric coefficients (a0, a1, a2)
 *          units: sfu, sfu/deg, sfu/dega<SUP>2</SUP>
 * @param[in] modip_degrees MODIP in degrees
 *
 */
extern void solar_activity_get(
  solar_activity_t* const pContext,
  double_t modip_degrees);

/** Compares two values of the effective Ionisation Level Az using a threshold based comparison approach
 * Threshold used is #NEQUICK_G_AZ_EQUALITY_EPSILON
 *
 * @param[in] Az1 first Az in sfu
 * @param[in] Az2 second Az in sfu
 * @return true if equal
 */
extern bool solar_activity_is_effective_ionisation_level_equal(
  double_t Az1,
  double_t Az2);

/** Compute the Effective Sunspot Number Azr as a function of the
 *  Effective Ionisation Level Az.
 *
 * @param Az_in_sfu Effective Ionisation Level Az in sfu
 *
 * @return Effective Sunspot Number Azr
 */
#define NeQuickG_Az_solar_activity_get_sun_spot_effective_count(Az_in_sfu) \
    get_solar_mean_spot_number(Az_in_sfu)

#ifdef FTR_UNIT_TEST
/** Unit test for Effective Ionisation Level (Az)
 * @param[in] pAi ionospheric coefficients (a<SUB>0</SUB>, a<SUB>1</SUB>, a<SUB>2</SUB>)
 * @param[in] modip_in_degrees MODIP in degrees
 */
extern double_t
solar_activity_get_effective_ionisation_level_in_sfu
  (const AZ_coefficients_t* const pAi,
   double_t modip_in_degrees);
#endif // FTR_UNIT_TEST

#endif // NEQUICK_G_JRC_SOLAR_ACTIVITY_H
