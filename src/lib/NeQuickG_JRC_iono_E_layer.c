/** NeQuickG E layer profile
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_iono_E_layer.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "NeQuickG_JRC_geometry.h"
#include "NeQuickG_JRC_iono_profile.h"
#include "NeQuickG_JRC_macros.h"
#include "NeQuickG_JRC_math_utils.h"
#include "NeQuickG_JRC_solar.h"

#define NEQUICK_G_JRC_IONO_E_LAYER_LAT_FACTOR (0.3)
#define NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_1 (1.112)
#define NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_2 (0.019)
#define NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_3 (0.3)
#define NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_4 (0.49)
/** The E layer maximum density height hmE [km] is defined as a constant, see 2.5.5.4 Eq 78 */
#define NEQUICK_G_JRC_IONO_E_LAYER_MAX_e_DENSITY_HEIGHT_KM (120)
#define NEQUICK_G_JRC_IONO_E_LAYER_BOTTOM_KM (5.0)
#define NEQUICK_G_JRC_IONO_E_LAYER_MIN_TOP_KM (7.0)

/**
 * Define the seas parameter as a function of the month of the year as follows:
 * If mth = 1,2,11,12 then seas = -1
 * If mth = 3,4,9,10 then seas = 0
 * If mth = 5,6,7,8 then seas = 1
 */
//lint -esym(533, get_seasonal_parameter) should return a value
// there is an exception in the default case
// but lint cannot handle it
static double_t get_seasonal_parameter(
  const NeQuickG_time_t * const pTime) {
  switch (pTime->month) {
  case NEQUICK_G_JRC_MONTH_JANUARY:
  case NEQUICK_G_JRC_MONTH_FEBRUARY:
  case NEQUICK_G_JRC_MONTH_NOVEMBER:
  case NEQUICK_G_JRC_MONTH_DECEMBER:
    return -1.0;
  case NEQUICK_G_JRC_MONTH_MARCH:
  case NEQUICK_G_JRC_MONTH_APRIL:
  case NEQUICK_G_JRC_MONTH_SEPTEMBER:
  case NEQUICK_G_JRC_MONTH_OCTOBER:
    return 0.0;
  case NEQUICK_G_JRC_MONTH_MAY:
  case NEQUICK_G_JRC_MONTH_JUNE:
  case NEQUICK_G_JRC_MONTH_JULY:
  case NEQUICK_G_JRC_MONTH_AUGUST:
    return 1.0;
  default:
    // This was validated in set_time
    //lint -e{506} this is intententionally a constant value boolean
    assert(false);
    //lint -e{527} unreachable code is ok
    return 0.0;
  }
}

static double_t get_lat_parameter(
  const position_t * const pPosition,
  const NeQuickG_time_t * const pTime) {

  // To compute the E layer critical frequency foE [MHz]
  // at a given location,
  // in addition to the effective solar zenith angle
  // a season dependent parameter has to be computed.
  double_t season_parameter = get_seasonal_parameter(pTime);

  // Introduce the latitudinal dependence.
  double_t ee =
    NeQuickG_exp(NEQUICK_G_JRC_IONO_E_LAYER_LAT_FACTOR *
                 pPosition->latitude.degree);

  return ((season_parameter*(ee - 1.0)) / (ee + 1.0));
}

void E_layer_get_critical_freq_MHz(
  E_layer_t* const pLayer,
  const NeQuickG_time_t* const pTime,
  const solar_activity_t* const pSolar_activity,
  const position_t* const pPosition) {

  double_t solar_effective_angle_degree;
  {
    if (!pLayer->is_solar_declination_valid) {
      pLayer->solar_declination = solar_get_declination(pTime);
    }

    solar_effective_angle_degree =
      solar_get_effective_zenith_angle(
        pPosition, pTime, &pLayer->solar_declination);
  }

  double_t parameter = get_lat_parameter(pPosition, pTime);

  double_t critical_freq =
    (NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_1 -
     NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_2*parameter) *
     sqrt(sqrt(pSolar_activity->effective_ionisation_level_sfu));

  critical_freq *=
    NeQuickG_exp(
      log(
        cos(NEQUICKG_JRC_DEGREE_TO_RAD(solar_effective_angle_degree)))
          *NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_3);

  pLayer->layer.critical_frequency_MHz =
    sqrt(critical_freq*critical_freq + NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_4);

  pLayer->layer.peak.electron_density =
    NEQUICK_G_JRC_IONO_GET_e_DENSITY(pLayer->layer.critical_frequency_MHz);
}

void E_layer_get_peak_height(E_layer_t* const pLayer) {
  pLayer->layer.peak.height_km =
    NEQUICK_G_JRC_IONO_E_LAYER_MAX_e_DENSITY_HEIGHT_KM;
}

void E_layer_get_peak_thickness(
  E_layer_t* const pE,
  const double_t F1_peak_thickness_bottom_km) {

  pE->layer.peak.thickness.top_km =
    max(F1_peak_thickness_bottom_km, NEQUICK_G_JRC_IONO_E_LAYER_MIN_TOP_KM);

  pE->layer.peak.thickness.bottom_km = NEQUICK_G_JRC_IONO_E_LAYER_BOTTOM_KM;
}

void E_layer_init(E_layer_t* const pE) {
  pE->is_solar_declination_valid = false;
}

#undef NEQUICK_G_JRC_IONO_E_LAYER_BOTTOM_KM
#undef NEQUICK_G_JRC_IONO_E_LAYER_MIN_TOP_KM
#undef NEQUICK_G_JRC_IONO_E_LAYER_MAX_e_DENSITY_HEIGHT_KM
#undef NEQUICK_G_JRC_IONO_E_LAYER_LAT_FACTOR
#undef NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_1
#undef NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_2
#undef NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_3
#undef NEQUICK_G_JRC_IONO_E_LAYER_CONSTANT_4
