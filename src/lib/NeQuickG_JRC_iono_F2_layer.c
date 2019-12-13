/** NeQuickG F2 layer profile
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_iono_F2_layer.h"

#include <assert.h>
#include <stdlib.h>

#include "NeQuickG_JRC_geometry.h"
#include "NeQuickG_JRC_iono_E_layer.h"
#include "NeQuickG_JRC_iono_F2_layer_fourier_coefficients.h"
#include "NeQuickG_JRC_iono_profile.h"
#include "NeQuickG_JRC_macros.h"
#include "NeQuickG_JRC_math_utils.h"

#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_0_LEGRENDE_GRADE (12)
#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_1_LEGRENDE_GRADE (12)
#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_2_LEGRENDE_GRADE (9)
#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_3_LEGRENDE_GRADE (5)
#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_4_LEGRENDE_GRADE (2)
#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_5_LEGRENDE_GRADE (1)
#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_6_LEGRENDE_GRADE (1)
#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_7_LEGRENDE_GRADE (1)
#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_8_LEGRENDE_GRADE (1)
#define NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_COUNT (9)

#define NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_0_LEGRENDE_GRADE (7)
#define NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_1_LEGRENDE_GRADE (8)
#define NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_2_LEGRENDE_GRADE (6)
#define NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_3_LEGRENDE_GRADE (3)
#define NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_4_LEGRENDE_GRADE (2)
#define NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_5_LEGRENDE_GRADE (1)
#define NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_6_LEGRENDE_GRADE (1)
#define NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_COUNT (7)

#define NEQUICKG_JRC_IONO_F2_LAYER_MODIP_COEFF_COUNT \
  (max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_0_LEGRENDE_GRADE, \
    max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_1_LEGRENDE_GRADE, \
      max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_2_LEGRENDE_GRADE, \
        max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_3_LEGRENDE_GRADE, \
          max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_4_LEGRENDE_GRADE, \
            max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_5_LEGRENDE_GRADE, \
              max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_6_LEGRENDE_GRADE, \
                max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_7_LEGRENDE_GRADE, \
                  max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_8_LEGRENDE_GRADE, \
                    max(NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_0_LEGRENDE_GRADE, \
                      max(NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_1_LEGRENDE_GRADE, \
                        max(NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_2_LEGRENDE_GRADE, \
                          max(NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_3_LEGRENDE_GRADE, \
                            max(NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_4_LEGRENDE_GRADE, \
                              max(NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_5_LEGRENDE_GRADE, \
                                NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_6_LEGRENDE_GRADE \
                              ) \
                            ) \
                          ) \
                        ) \
                      ) \
                    ) \
                  ) \
                ) \
              ) \
            ) \
          ) \
        ) \
      ) \
    ) \
  ))

#define NEQUICKG_JRC_IONO_F2_LAYER_LONG_COEFF_COUNT \
  (max(NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_COUNT, \
    NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_COUNT) - 1)

#if ((NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_0_LEGRENDE_GRADE + \
    2 * ( \
      NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_1_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_2_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_3_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_4_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_5_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_6_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_7_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_8_LEGRENDE_GRADE )) \
    != ITU_F2_COEFF_MAX_DEGREE)
  #error "Please, review  \
NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_N_LEGRENDE_GRADE array"
#endif

#if ((NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_0_LEGRENDE_GRADE + \
    2 * ( \
      NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_1_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_2_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_3_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_4_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_5_LEGRENDE_GRADE + \
      NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_6_LEGRENDE_GRADE)) \
    != ITU_FM3_COEFF_MAX_DEGREE)
  #error "Please, review  \
NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_N_LEGRENDE_GRADE array"
#endif

/** Modip legendre coefficients are set to zero
 * if their absolute value is found to be 1.0e-30
 *
 */
#define NEQUICKG_JRC_IONO_F2_LAYER_MODIP_COEFF_NEGLIGIBLE_VALUE (1.0e-30)

/** Transmission factor must be at least 1.0
 * otherwise a square root could be calculated
 * with a negative operand.
 * TBD. (undocumented feature and value)
 */
#define NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_MINIMUM_VALUE (1.0)

#define NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_1 (1490.0)
#define NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_2 (0.0196)
#define NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_3 (1.2967)
#define NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_4 (-0.012)
#define NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_5 (0.253)
#define NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_6 (1.215)
#define NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_7 (-176.0)
#define NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_JOIN_CONST (20.0)
#define NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_JOIN_CLIP_CONST (1.75)

#define NEQUICK_G_JRC_IONO_F2_LAYER_THICKNESS_BOTTOM_FACTOR (0.385)

#define NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_1 (0.01)
#define NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_2 (-3.467)
#define NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_3 (0.857)
#define NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_4 (2.02)

#define NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_1 (6.705)
#define NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_2 (-0.014)
#define NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_3 (-0.008)
#define NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_4 (-7.77)
#define NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_5 (0.097)
#define NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_6 (0.153)
#define NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_7 (2.0)
#define NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_8 (8.0)
#define NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_TRANSITION_REGION (1.0)

/* Constants used to calculate the F2 topside thickness */
#define NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_1 (150.0)
#define NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_2 (100.0)
#define NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_3 (0.041163)
#define NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_4 (0.183981)
#define NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_5 (1.424472)

static const size_t NeQuickG_iono_F2_layer_crit_f_legrendre_grades
  [NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_COUNT] = {
  NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_0_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_1_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_2_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_3_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_4_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_5_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_6_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_7_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_8_LEGRENDE_GRADE};

static const size_t NeQuickG_iono_F2_layer_trans_factor_grades[
  NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_COUNT] = {
  NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_0_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_1_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_2_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_3_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_4_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_5_LEGRENDE_GRADE,
  NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_6_LEGRENDE_GRADE};

static void get_legrende_coeff_for_modip(
  const double_t modip_degree,
  double_t* const pCoeff) {

  pCoeff[0] = 1.0;
  // to satisfy lint
  size_t grade = (NEQUICKG_JRC_IONO_F2_LAYER_MODIP_COEFF_COUNT - 1);
  NeQuickG_get_power_array(
    grade,
    sin(NEQUICKG_JRC_DEGREE_TO_RAD(modip_degree)),
    NEQUICKG_JRC_IONO_F2_LAYER_MODIP_COEFF_NEGLIGIBLE_VALUE,
    &pCoeff[1]);
}

static void get_legrende_coeff_for_longitude(
  const angle_t* const pLongitude,
  double_t* const pCoeff_sinus,
  double_t* const pCoeff_cosinus) {

  pCoeff_sinus[0] = pLongitude->sin;
  pCoeff_cosinus[0] = pLongitude->cos;

  double_t n_long = 2.0 * pLongitude->rad;

  // to satisfy lint
  size_t coeff_count = NEQUICKG_JRC_IONO_F2_LAYER_LONG_COEFF_COUNT;
  for (
    size_t i = 1;
    i < coeff_count;
    i++) {
    pCoeff_sinus[i] = sin(n_long);
    pCoeff_cosinus[i] = cos(n_long);
    n_long += pLongitude->rad;
  }
}

static double_t legendre_expansion(
  const double_t* const pFourier_coeff,
  const size_t* const pLegendre_degrees_per_order,
  size_t legendre_degrees_per_order_size,
  const double_t* const pModip_coeff,
  const double_t* const pLong_coeff_sinus,
  const double_t* const pLong_coeff_cosinus,
  double_t cos_lat) {

  double_t parameter = 0.0;

  size_t degree_index;

  // Order 0 term
  for (
    degree_index = 0;
    degree_index < pLegendre_degrees_per_order[0];
    degree_index++) {
    parameter +=
      pFourier_coeff[degree_index]*
      pModip_coeff[degree_index];
  }

  double_t lat_coeff = cos_lat;

  // Order i + 1 term
  for (
    size_t i = 1;
    i < legendre_degrees_per_order_size;
    i++) {

    for (
      size_t j = 0;
      j < pLegendre_degrees_per_order[i];
      j++) {
      parameter +=
        (pModip_coeff[j] * lat_coeff *
        ((pFourier_coeff[degree_index] * pLong_coeff_cosinus[i - 1]) +
        (pFourier_coeff[degree_index + 1] * pLong_coeff_sinus[i - 1])));
      degree_index += 2;
    }

    lat_coeff *= cos_lat;
  }
  return parameter;
}

static double_t get_transmission_factor(
  const Fm3_fourier_coefficient_array_t Cm3,
  const double_t* const pModip_coeff,
  const double_t* const pLong_coeff_sinus,
  const double_t* const pLong_coeff_cosinus,
  double_t cos_lat) {

  // to satisfy lint
  size_t order_count = NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_COUNT;
  double_t transmission_factor = legendre_expansion(
    Cm3,
    NeQuickG_iono_F2_layer_trans_factor_grades,
    order_count,
    pModip_coeff,
    pLong_coeff_sinus,
    pLong_coeff_cosinus,
    cos_lat);

  return max(
    transmission_factor,
    NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_MINIMUM_VALUE);
}

int32_t F2_layer_init(
  F2_layer_t* const pLayer,
  const char* const pCCIR_folder) {

  return F2_layer_fourier_coefficients_init(
    &pLayer->coef, pCCIR_folder);
}

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
void F2_layer_close(F2_layer_t* const pLayer) {
  F2_layer_fourier_coefficients_close(&pLayer->coef);
}
#endif

int32_t F2_layer_get_critical_freq_MHz(
  F2_layer_t * const pF2,
  const NeQuickG_time_t* const pTime,
  const modip_context_t* const pModip,
  const solar_activity_t* const pSolar_activity,
  const position_t * const pCurrent_position) {

  int32_t ret = F2_layer_fourier_coefficients_get(
    &pF2->coef,
    pTime,
    pSolar_activity);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  //lint -e{506} NEQUICKG_JRC_IONO_F2_LAYER_MODIP_COEFF_COUNT is not constant, as lint means
  double_t modip_coeff[NEQUICKG_JRC_IONO_F2_LAYER_MODIP_COEFF_COUNT];
  get_legrende_coeff_for_modip(pModip->modip_degree, modip_coeff);

  //lint -e{506} NEQUICKG_JRC_IONO_F2_LAYER_LONG_COEFF_COUNT is not constant, as lint means
  double_t long_coeff_sinus[NEQUICKG_JRC_IONO_F2_LAYER_LONG_COEFF_COUNT];

  //lint -e{506} NEQUICKG_JRC_IONO_F2_LAYER_LONG_COEFF_COUNT is not constant, as lint means
  double_t long_coeff_cosinus[NEQUICKG_JRC_IONO_F2_LAYER_LONG_COEFF_COUNT];

  get_legrende_coeff_for_longitude(
    &pCurrent_position->longitude,
    long_coeff_sinus,
    long_coeff_cosinus);

  // to satisfy lint
  size_t order_count = NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_COUNT;
  pF2->layer.critical_frequency_MHz = legendre_expansion(
    pF2->coef.fourier.CF2,
    NeQuickG_iono_F2_layer_crit_f_legrendre_grades,
    order_count,
    modip_coeff,
    long_coeff_sinus,
    long_coeff_cosinus,
    pCurrent_position->latitude.cos);

  pF2->layer.peak.electron_density =
    NEQUICK_G_JRC_IONO_GET_e_DENSITY(
      pF2->layer.critical_frequency_MHz);

  pF2->trans_factor = get_transmission_factor(
    pF2->coef.fourier.Cm3,
    modip_coeff,
    long_coeff_sinus,
    long_coeff_cosinus,
    pCurrent_position->latitude.cos);

  return NEQUICK_OK;
}

static double_t get_E_Term_for_peak_height(
  const double_t critical_freq_F2_layer_MHz,
  const double_t critical_freq_E_layer_MHz) {

  // soft clipped for r < 1.75, using NeQuickG_func_join
  double_t E_term =
    critical_freq_F2_layer_MHz /
    critical_freq_E_layer_MHz;

  E_term = NeQuickG_func_join(
    E_term, NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_JOIN_CLIP_CONST,
    NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_JOIN_CONST,
    E_term - NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_JOIN_CLIP_CONST);

  // no divide by zero check needed as r2 > ~1.75
  assert(E_term > NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_6);

  return (NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_5 /
           (E_term - NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_6));
}

void F2_layer_get_peak_height(
  F2_layer_t* const pF2,
  const double_t critical_freq_E_layer_MHz) {

  double_t transmission_factor_squared =
    pF2->trans_factor * pF2->trans_factor;

  // no divide by zero check needed as previous clipping
  // ensures M3000 >= 1
  assert(transmission_factor_squared > 1.0);

  double_t numerator =
    NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_1 *
    pF2->trans_factor *
    sqrt(
      ((NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_2 *
        transmission_factor_squared) + 1.0) /
      ((NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_3 *
        transmission_factor_squared) - 1.0));

  double_t denominator =
    pF2->trans_factor +
    NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_4;

  if (NEQUICK_G_JRC_IONO_E_LAYER_INFLUENCES_F2_PEAK_HEIGHT(
        critical_freq_E_layer_MHz)) {

    denominator += get_E_Term_for_peak_height(
      pF2->layer.critical_frequency_MHz,
      critical_freq_E_layer_MHz);
  }

  pF2->layer.peak.height_km =
    (numerator / denominator) +
    NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_7;
}

void F2_layer_get_peak_thickness(F2_layer_t* const pF2) {
  pF2->layer.peak.thickness.top_km = INFINITY;
  pF2->layer.peak.thickness.bottom_km =
    NEQUICK_G_JRC_IONO_F2_LAYER_THICKNESS_BOTTOM_FACTOR *
    pF2->layer.peak.electron_density;

  /* Calculate gradient of rho(e) at base of F2 layer (10^9 m^-3 km^-1)
   * re: Mosert de Gonzalez and Radicella, 1990, Adv. Space. Res., 10, 17 */
  double_t grad =
    NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_1 *
    exp(NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_2 +
       (NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_3 *
         log(pF2->layer.critical_frequency_MHz *
             pF2->layer.critical_frequency_MHz)) +
       (NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_4 *
         log(pF2->trans_factor))
       );

  pF2->layer.peak.thickness.bottom_km /= grad;
}

void F2_layer_get_peak_amplitude(F2_layer_t* const pF2) {
  pF2->layer.peak.amplitude =
    NEQUICKG_IONO_LAYER_GET_PEAK_AMPLITUDE(
      pF2->layer.peak.electron_density);
}

static double_t F2_layer_get_shape_factor(
  const F2_layer_t* const pF2,
  const NeQuickG_time_t* const pTime,
  const solar_activity_t* const pSolar_activity) {

  double_t shape_factor;

  if ((pTime->month > NEQUICK_G_JRC_MONTH_MARCH) &&
      (pTime->month < NEQUICK_G_JRC_MONTH_OCTOBER)) {

    shape_factor =
      NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_1 +
      (NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_2 *
       pSolar_activity->effective_sun_spot_count) +
      (NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_3 *
       pF2->layer.peak.height_km);

  } else {

    shape_factor =
      NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_4 +
      (NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_5 *
        NeQuickG_square(pF2->layer.peak.height_km /
                        pF2->layer.peak.thickness.bottom_km)) +
      (NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_6 *
       pF2->layer.peak.electron_density);
  }

  shape_factor =
    NeQuickG_func_join(
      shape_factor,
      NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_7,
      NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_TRANSITION_REGION,
      shape_factor - NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_7);

  shape_factor =
    NeQuickG_func_join(
      NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_8,
      shape_factor,
      NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_TRANSITION_REGION,
      shape_factor - NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_8);

  return shape_factor;
}

void F2_layer_exosphere_adjust(
  F2_layer_t* const pF2,
  const NeQuickG_time_t* const pTime,
  const solar_activity_t* const pSolar_activity) {

  double_t shape_factor =
    F2_layer_get_shape_factor(pF2, pTime, pSolar_activity);

  pF2->layer.peak.thickness.top_km =
    shape_factor * pF2->layer.peak.thickness.bottom_km;

  double_t auxiliary_param_x =
    (pF2->layer.peak.thickness.top_km -
     NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_1) /
      NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_2;

  double_t auxiliary_param_v =
    (((NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_3 * auxiliary_param_x) -
       NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_4) * auxiliary_param_x) +
       NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_5;

  pF2->layer.peak.thickness.top_km /= auxiliary_param_v;

  // Set to Not a Number.
  // It must be recalculated using the
  // electron density module
  pF2->layer.peak.electron_density = NAN;
}

#undef NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_1
#undef NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_2
#undef NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_3
#undef NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_4
#undef NEQUICKG_JRC_IONO_F2_THICKNESS_TOP_CONST_5

#undef NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_TRANSITION_REGION
#undef NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_1
#undef NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_2
#undef NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_3
#undef NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_4
#undef NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_5
#undef NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_6
#undef NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_7
#undef NEQUICKG_JRC_IONO_F2_SHAPE_FACTOR_CONST_8

#undef NEQUICKG_JRC_IONO_F2_LAYER_LONG_COEFF_COUNT

#undef NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_1
#undef NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_2
#undef NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_3
#undef NEQUICKG_JRC_IONO_F2_e_DENSITY_GRAD_CONST_4

#undef NEQUICK_G_JRC_IONO_F2_LAYER_THICKNESS_BOTTOM_FACTOR

#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_0_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_1_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_2_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_3_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_4_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_5_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_6_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_7_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_8_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_CRITICAL_FREQ_ORDER_COUNT

#undef NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_0_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_1_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_2_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_3_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_4_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_5_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_6_LEGRENDE_GRADE
#undef NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_ORDER_COUNT

#undef NEQUICKG_JRC_IONO_F2_LAYER_MODIP_COEFF_COUNT
#undef NEQUICKG_JRC_IONO_F2_LAYER_MODIP_COEFF_NEGLIGIBLE_VALUE

#undef NEQUICKG_JRC_IONO_F2_TRANS_FACTOR_MINIMUM_VALUE

#undef NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_1
#undef NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_2
#undef NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_3
#undef NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_4
#undef NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_5
#undef NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_6
#undef NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_CONST_7
#undef NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_JOIN_CONST
#undef NEQUICKG_JRC_IONO_F2_MAX_e_DENSITY_DUDENEY_JOIN_CLIP_CONST
