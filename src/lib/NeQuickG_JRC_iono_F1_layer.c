/** NeQuickG F1 layer profile
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_iono_F1_layer.h"

#include "NeQuickG_JRC_iono_profile.h"
#include "NeQuickG_JRC_math_utils.h"

#define NEQUICK_G_JRC_IONO_F1_LAYER_TITHERIDGE_FACTOR (1.4)
#define NEQUICK_G_JRC_IONO_F1_LAYER_JOIN_FRACTIONAL_1 (1000.0)
#define NEQUICK_G_JRC_IONO_F1_LAYER_JOIN_FRACTIONAL_2 (60.0)
#define NEQUICK_G_JRC_IONO_F1_LAYER_HEIGHT_CONSTANT (0.85)
#define NEQUICK_G_JRC_IONO_F1_FREQUENCY_LOWER_LIMIT_MHZ (1.0e-6)

#define NEQUICK_G_JRC_IONO_F1_LAYER_THICKNESS_TOP_FACTOR (0.3)
#define NEQUICK_G_JRC_IONO_F1_LAYER_THICKNESS_BOTTOM_FACTOR (0.5)

void F1_layer_get_critical_freq_MHz(
  layer_t* const pLayer,
  const double_t critical_freq_E_layer_MHz,
  const double_t critical_freq_F2_layer_MHz) {

  double_t critical_freq_F1_layer_MHz =
    NeQuickG_func_join(
      NEQUICK_G_JRC_IONO_F1_LAYER_TITHERIDGE_FACTOR *
       critical_freq_E_layer_MHz,
      0.0,
      NEQUICK_G_JRC_IONO_F1_LAYER_JOIN_FRACTIONAL_1,
      critical_freq_E_layer_MHz - 2);

  critical_freq_F1_layer_MHz =
    NeQuickG_func_join(
      0.0,
      critical_freq_F1_layer_MHz,
      NEQUICK_G_JRC_IONO_F1_LAYER_JOIN_FRACTIONAL_1,
      critical_freq_E_layer_MHz - critical_freq_F1_layer_MHz);

  critical_freq_F1_layer_MHz =
    NeQuickG_func_join(
      critical_freq_F1_layer_MHz,
      NEQUICK_G_JRC_IONO_F1_LAYER_HEIGHT_CONSTANT *
        critical_freq_F1_layer_MHz,
      NEQUICK_G_JRC_IONO_F1_LAYER_JOIN_FRACTIONAL_2,
      (NEQUICK_G_JRC_IONO_F1_LAYER_HEIGHT_CONSTANT *
        critical_freq_F2_layer_MHz) -
      critical_freq_F1_layer_MHz);

  pLayer->critical_frequency_MHz =
    (critical_freq_F1_layer_MHz <
      NEQUICK_G_JRC_IONO_F1_FREQUENCY_LOWER_LIMIT_MHZ) ?
    0x00 :
    critical_freq_F1_layer_MHz;

  pLayer->peak.electron_density =
    NEQUICK_G_JRC_IONO_GET_e_DENSITY(pLayer->critical_frequency_MHz);
}

void F1_layer_get_peak_height(
  layer_t* const pLayer,
  const double_t E_peak_height_km,
  const double_t F2_peak_height_km) {
  pLayer->peak.height_km =
    (E_peak_height_km + F2_peak_height_km) / 2.0;
}

void F1_layer_get_peak_thickness(
  layer_t* const pF1,
  const double_t E_peak_height_km,
  const double_t F2_peak_height_km) {

  pF1->peak.thickness.top_km =
    NEQUICK_G_JRC_IONO_F1_LAYER_THICKNESS_TOP_FACTOR *
    (F2_peak_height_km - pF1->peak.height_km);

  pF1->peak.thickness.bottom_km =
    NEQUICK_G_JRC_IONO_F1_LAYER_THICKNESS_BOTTOM_FACTOR *
    (pF1->peak.height_km - E_peak_height_km);
}

#undef NEQUICK_G_JRC_IONO_F1_LAYER_THICKNESS_TOP_FACTOR
#undef NEQUICK_G_JRC_IONO_F1_LAYER_THICKNESS_BOTTOM_FACTOR

#undef NEQUICK_G_JRC_IONO_F1_LAYER_TITHERIDGE_FACTOR
#undef NEQUICK_G_JRC_IONO_F1_LAYER_JOIN_FRACTIONAL_1
#undef NEQUICK_G_JRC_IONO_F1_LAYER_JOIN_FRACTIONAL_2
#undef NEQUICK_G_JRC_IONO_F1_LAYER_HEIGHT_CONSTANT
