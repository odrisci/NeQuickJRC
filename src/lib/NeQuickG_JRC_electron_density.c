/** NeQuick G JRC electron density computation
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_electron_density.h"

#if defined(_MSC_VER)
#include <stdlib.h>
#else
#include "NeQuickG_JRC_macros.h"
#endif

#include "NeQuickG_JRC_math_utils.h"

/** Top side electron density. Constant parameter g Eq. 122 */
#define NEQUICK_G_JRC_ELE_DENSITY_TOP_CONST_1 (0.125)
/** Top side electron density. Constant parameter r Eq. 123 */
#define NEQUICK_G_JRC_ELE_DENSITY_TOP_CONST_2 (100.0)
/** Top side electron density. Threshold value e<SUB>a</SUB> r Eq. 127 */
#define NEQUICK_G_JRC_ELE_DENSITY_TOP_APROXIMATION_EPSILON (1.0e11)
/** Bottom side electron density. Anchor point h Eq. 115, 116 and 120 */
#define NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CORRECTION_ANCHOR_POINT (100.0)
/** Bottom side electron density. Exponential constant 1 Eq. 112, 113 */
#define NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CONST_1 (10.0)
/** Bottom side electron density. Exponential constant 2 Eq. 112, 113 */
#define NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CONST_2 (1.0)
/** Bottom side electron density. Scale Eq. 119, 120 */
#define NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_SCALE (10.0)
/** Bottom side electron density. Threshold. Eq. 114 */
#define NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_EPSILON (25.0)
/** Bottom side electron density. Factor to get electron density in m<SUP>-3</SUP>].Eq. 121 */
#define NEQUICK_G_JRC_ELE_DENSITY_TO_ELECTRON_DENSITY(_N) (1.0e11*_N)

typedef struct bottom_side_st {
  double_t B_param;
  double_t exponential_arg;
  double_t s;
  bool is_exponential_arg_above_threshold;
} bottom_side_t;

typedef struct bottom_side_info_st {
  bottom_side_t F2;
  bottom_side_t F1;
  bottom_side_t E;
} bottom_side_info_t;

static void bottom_side_get_B_params(
  bottom_side_info_t* const pBottom_side_info,
  const iono_profile_t* const pProfile,
  const double_t height_km) {

  pBottom_side_info->F2.B_param =
    pProfile->F2.layer.peak.thickness.bottom_km;

  pBottom_side_info->F1.B_param =
    (height_km > pProfile->F1.peak.height_km) ?
    pProfile->F1.peak.thickness.top_km :
    pProfile->F1.peak.thickness.bottom_km;

  pBottom_side_info->E.B_param =
    (height_km > pProfile->E.layer.peak.height_km) ?
    pProfile->E.layer.peak.thickness.top_km :
    pProfile->E.layer.peak.thickness.bottom_km;
}

static bool is_exponential_arg_above_threshold(double_t exponential_arg) {
  return (
    fabs(exponential_arg) >
    NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_EPSILON);
}

static void bottom_side_get_exp_params(
  bottom_side_info_t* const pBottom_side_info,
  const iono_profile_t* const pProfile,
  const double_t height_km) {

 double_t height_above_F2_peak_km =
    height_km - pProfile->F2.layer.peak.height_km;

 pBottom_side_info->F2.exponential_arg =
   height_above_F2_peak_km / pBottom_side_info->F2.B_param;

 pBottom_side_info->F1.exponential_arg =
   (height_km - pProfile->F1.peak.height_km) / pBottom_side_info->F1.B_param;

 pBottom_side_info->E.exponential_arg =
   (height_km - pProfile->E.layer.peak.height_km) / pBottom_side_info->E.B_param;

 double_t temp =
   exp(NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CONST_1 /
   ((NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CONST_2 *
     fabs(height_above_F2_peak_km) + 1.0)));

  pBottom_side_info->F1.exponential_arg *= temp;
  pBottom_side_info->E.exponential_arg *= temp;

  pBottom_side_info->F2.is_exponential_arg_above_threshold =
    is_exponential_arg_above_threshold(pBottom_side_info->F2.exponential_arg);

  pBottom_side_info->F1.is_exponential_arg_above_threshold =
    is_exponential_arg_above_threshold(pBottom_side_info->F1.exponential_arg);

  pBottom_side_info->E.is_exponential_arg_above_threshold =
    is_exponential_arg_above_threshold(pBottom_side_info->E.exponential_arg);

  pBottom_side_info->F2.exponential_arg =
    exp(pBottom_side_info->F2.exponential_arg);

  pBottom_side_info->F1.exponential_arg =
    exp(pBottom_side_info->F1.exponential_arg);

  pBottom_side_info->E.exponential_arg =
    exp(pBottom_side_info->E.exponential_arg);
}

static double_t get_s_param(
  const double_t peak_amplitude,
  const bottom_side_t* const pInfo) {
  if (pInfo->is_exponential_arg_above_threshold) {
    return 0.0;
  } else {
    return (peak_amplitude * pInfo->exponential_arg /
            NeQuickG_square(pInfo->exponential_arg + 1.0));
  }
}

static double_t get_ds_param(const bottom_side_t* const pInfo) {
  if (pInfo->is_exponential_arg_above_threshold) {
    return 0.0;
  } else {
    return ((1.0 - pInfo->exponential_arg) /
       (1.0 + pInfo->exponential_arg)) /
          pInfo->B_param;
  }
}

static void bottom_side_get_s_params(
  bottom_side_info_t* const pBottom_side_info,
  const iono_profile_t* const pProfile) {

  pBottom_side_info->F2.s =
    get_s_param(
      pProfile->F2.layer.peak.amplitude,
      &pBottom_side_info->F2);

  pBottom_side_info->F1.s =
    get_s_param(
      pProfile->F1.peak.amplitude,
      &pBottom_side_info->F1);

  pBottom_side_info->E.s =
    get_s_param(
      pProfile->E.layer.peak.amplitude,
      &pBottom_side_info->E);
}

static double_t bottom_side_low(
  const bottom_side_info_t* const pBottom_side_info,
  const double_t height_km) {

  double_t F2_ds_param = get_ds_param(&pBottom_side_info->F2);
  double_t F1_ds_param = get_ds_param(&pBottom_side_info->F1);
  double_t E_ds_param = get_ds_param(&pBottom_side_info->E);

  double_t s_sum =
    pBottom_side_info->F2.s +
    pBottom_side_info->F1.s +
    pBottom_side_info->E.s;

  double_t s_ds_sum =
    (pBottom_side_info->F2.s * F2_ds_param) +
    (pBottom_side_info->F1.s * F1_ds_param) +
    (pBottom_side_info->E.s * E_ds_param);

  // chapman parameters BC and Z
  double_t BC = 1.0 -
    ((s_ds_sum / s_sum) * NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_SCALE);

  double_t Z =
    (height_km - NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CORRECTION_ANCHOR_POINT) /
    NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_SCALE;

  return (s_sum * NeQuickG_exp(1 - ((BC * Z) + NeQuickG_exp(-Z))));
}

static double_t bottom_side_high(
  const bottom_side_info_t* const pBottom_side_info) {
  return (
    pBottom_side_info->F2.s +
    pBottom_side_info->F1.s +
    pBottom_side_info->E.s);
}

/** This function calculates electron content at the specified height,
 * in the bottom part of the ionosphere below the F2 peak height.
 * The function sums semi-Epstein Layers with a modification to reduce
 * excessive Ne around F2 peak and 100km.
 *
 * @param pProfile ionosphere profile
 * @param height_km The height at which the electron density is required in km
 *
 * @return electron content at the specified height in m<SUP>-3</SUP>
 */
static double_t bottom_side(
  const iono_profile_t* const pProfile,
  const double_t height_km) {

  bottom_side_info_t bottom_side_info;

  bottom_side_get_B_params(&bottom_side_info, pProfile, height_km);

  bottom_side_get_exp_params(
    &bottom_side_info, pProfile,
    max(NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CORRECTION_ANCHOR_POINT, height_km));

  bottom_side_get_s_params(&bottom_side_info, pProfile);

  double_t electron_density =
    (height_km < NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CORRECTION_ANCHOR_POINT) ?
      bottom_side_low(&bottom_side_info, height_km):
      bottom_side_high(&bottom_side_info);

  return NEQUICK_G_JRC_ELE_DENSITY_TO_ELECTRON_DENSITY(electron_density);
}

/** This function calculates electron content at the specified height,
 * in the top part of the ionosphere above the F2 electron density peak point.
 * The function uses topside expression derived from [Annex A [15]],
 * although the expression in the paper has an error with the brackets.
 *
 * @param pProfile ionosphere profile, maximum Ne (F2 peak)
 * @param height_km The height at which the electron density is required in km
 *
 * @return electron content at the specified height in m<SUP>-3</SUP>
 */
static double_t top_side(
  iono_profile_t* const pProfile,
  const double_t height_km) {

  double_t height_above_F2_peak_km =
    height_km - pProfile->F2.layer.peak.height_km;

  double_t delta_height =
    NEQUICK_G_JRC_ELE_DENSITY_TOP_CONST_1 *
    height_above_F2_peak_km;

  double_t temp =
    NeQuickG_exp(
      height_above_F2_peak_km /
       (pProfile->F2.layer.peak.thickness.top_km *
         (1.0 +
           ((NEQUICK_G_JRC_ELE_DENSITY_TOP_CONST_2 * delta_height) /
              ((NEQUICK_G_JRC_ELE_DENSITY_TOP_CONST_2 *
                pProfile->F2.layer.peak.thickness.top_km) +
                delta_height)))));

  if (temp > NEQUICK_G_JRC_ELE_DENSITY_TOP_APROXIMATION_EPSILON) {
    temp = 1.0 / temp;
  } else {
    temp /= NeQuickG_square(1.0 + temp);
  }

  if (isnan(pProfile->F2.layer.peak.electron_density)) {
    pProfile->F2.layer.peak.electron_density =
      bottom_side(pProfile, pProfile->F2.layer.peak.height_km);
  }
  return (NEQUICKG_IONO_LAYER_GET_PEAK_AMPLITUDE(temp) *
          pProfile->F2.layer.peak.electron_density);
}

double_t electron_density_get(
  iono_profile_t* const pProfile,
  const double_t height_km) {

  if (height_km > pProfile->F2.layer.peak.height_km) {
    return top_side(pProfile, height_km);
  } else {
    return bottom_side(pProfile, height_km);
  }
}

#undef NEQUICK_G_JRC_ELE_DENSITY_TO_ELECTRON_DENSITY

#undef NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_EPSILON
#undef NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CORRECTION_ANCHOR_POINT
#undef NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CONST_1
#undef NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_CONST_2
#undef NEQUICK_G_JRC_ELE_DENSITY_BOTTOM_SCALE

#undef NEQUICK_G_JRC_ELE_DENSITY_TOP_CONST_1
#undef NEQUICK_G_JRC_ELE_DENSITY_TOP_CONST_2
#undef NEQUICK_G_JRC_ELE_DENSITY_TOP_APROXIMATION_EPSILON
