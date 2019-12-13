/** Effective Ionisation Level (Az)
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_solar_activity.h"

#include <assert.h>
#include <stdbool.h>

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_error.h"
#include "NeQuickG_JRC_macros.h"

#define NEQUICK_G_IONO_COEFF_IS_ZERO(_coeff) (THRESHOLD_COMPARE_TO_ZERO(_coeff, 1.0e-7))
#define NEQUICK_G_AZ_EQUALITY_EPSILON (1.0e-10)

/** 3.1. This default value represents the lowest Solar Flux value in average
 *  conditions that NeQuick is expected to operate on.
 *  In terms of the analytical expression relating 12-month running Sun
 *  Spot Number and Solar Flux, it corresponds to a Sun Spot Number of 0.
 *  This value is considered adequate when no other solution is available,
 *  while still being able to correct for a significant contribution of
 *  the ionospheric delay error.
 */
#define NEQUICK_G_AZ_DEFAULT_VALUE_SFU (ITU_R_P_371_8_LOWER_SOLAR_FLUX_IN_SFU)

/**
 * 3.1. Zero-valued coefficients and default Effective Ionisation Level
 *   When all the Effective Ionisation Level ionospheric broadcast
 *   coefficients are set to zero:
 *    ai0 = ai1 = ai2 = 0
 *   the coefficients shall NOT be used for single-frequency ionospheric
 *   correction.
 */
static bool are_valid(const AZ_coefficients_t* const pCoeff) {
  for (size_t i = 0; i < NEQUICKG_AZ_COEFFICIENTS_COUNT; i++) {
    if (!NEQUICK_G_IONO_COEFF_IS_ZERO(pCoeff->a_sfu[i])) {
      return true;
    }
  }
  return false;
}

/**
 * 3.3 Effective Ionisation Level boundaries:
 * Due to the fact that the Effective Ionisation Level is represented simply
 * by a second-order degree polynomial as a function of MODIP, it may
 * exceptionally happen that for some MODIP values, the Effective Ionisation
 * Parameter becomes out of range.
 * The operational range for the Effective Ionisation Parameter is between 0
 * and 400 sfu and the following condition for out of range values should be
 * used:
 * if AZ < 0 ==> Az = 0
 * if AZ > 400 ==> Az = 400
 */
static void check_boundaries(
  double_t* const pAz) {
  if (*pAz < NEQUICK_G_AZ_MIN_VALUE_SFU) {
    *pAz = NEQUICK_G_AZ_MIN_VALUE_SFU;
  } else if (*pAz > NEQUICK_G_AZ_MAX_VALUE_SFU) {
    *pAz = NEQUICK_G_AZ_MAX_VALUE_SFU;
  }
}

/**
 * Calculate Effective Ionisation Level:
 * Az = ai0 + ai1.mu + ai2*mu^2
 */
static double_t Az_calc(
  const AZ_coefficients_t* const pCoeff,
  double_t modip) {
  double_t Az = 0.0;
  double_t term = 1.0;
  for (size_t i = 0; i < NEQUICKG_AZ_COEFFICIENTS_COUNT; i++) {
    Az += pCoeff->a_sfu[i]*term;
    term *= modip;
  }
  return Az;
}

#ifndef FTR_UNIT_TEST
static
#endif
double_t
solar_activity_get_effective_ionisation_level_in_sfu(
  const AZ_coefficients_t* const pCoeff,
  double_t modip) {
  if (!are_valid(pCoeff)) {
    /**
     *   In those cases, a default value shall be used for correction in
     *   the receiver:
     *   ai0 = NEQUICK_G_AZ_DEFAULT_VALUE; ai1 = ai2 = 0
     */
    return NEQUICK_G_AZ_DEFAULT_VALUE_SFU;
  }

  {
    double_t Az = Az_calc(pCoeff, modip);
    check_boundaries(&Az);
    return Az;
  }
}

bool solar_activity_is_effective_ionisation_level_equal(
  double_t op1,
  double_t op2) {
  return THRESHOLD_COMPARE(op1, op2,
          NEQUICK_G_AZ_EQUALITY_EPSILON);
}

int32_t solar_activity_coefficients_set (
  solar_activity_t* const pContext,
  const double_t * const pCoeff,
  size_t coeff_count) {
  if ((pCoeff == NULL) ||
      (coeff_count < NEQUICKG_AZ_COEFFICIENTS_COUNT)) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_SOLAR_ACTIVITY,
      NEQUICK_ERROR_CODE_BAD_AZ_COEFF,
      "Az coefficients not valid (null pointer or fewer than 3 coefficients)");
  }
  // to satisfy lint
  assert(pCoeff);
  pContext->effective_ionisation_level_coeff.a_sfu[0] = pCoeff[0];
  pContext->effective_ionisation_level_coeff.a_sfu[1] = pCoeff[1];
  pContext->effective_ionisation_level_coeff.a_sfu[2] = pCoeff[2];
  return NEQUICK_OK;
}

void solar_activity_get(
  solar_activity_t* const pContext,
  double_t modip_degree) {

  pContext->effective_ionisation_level_sfu =
    solar_activity_get_effective_ionisation_level_in_sfu(
    &pContext->effective_ionisation_level_coeff,
    modip_degree);

  pContext->effective_sun_spot_count =
    NeQuickG_Az_solar_activity_get_sun_spot_effective_count(
      pContext->effective_ionisation_level_sfu);
}

#undef NEQUICK_G_AZ_EQUALITY_EPSILON
#undef NEQUICK_G_AZ_DEFAULT_VALUE_SFU
#undef NEQUICK_G_IONO_COEFF_IS_ZERO
