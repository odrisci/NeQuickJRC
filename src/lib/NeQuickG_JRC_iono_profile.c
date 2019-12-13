/** NeQuickG inospheric profile
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_iono_profile.h"

#include <assert.h>

#include "NeQuickG_JRC_iono_E_layer.h"
#include "NeQuickG_JRC_iono_F1_layer.h"
#include "NeQuickG_JRC_iono_F2_layer.h"
#include "NeQuickG_JRC_math_utils.h"

#define NEQUICK_G_JRC_IONO_PEAK_AMPLITUDE_ITERATION_COUNT (5)

int32_t iono_profile_init(
  iono_profile_t * const pProfile,
  const char* const pCCIR_folder) {

  E_layer_init(&pProfile->E);

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
  assert(pCCIR_folder);
#endif
  return F2_layer_init(
    &pProfile->F2, pCCIR_folder);
}

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
void iono_profile_close(iono_profile_t * const pProfile) {
  F2_layer_close(&pProfile->F2);
}
#endif

static double_t iono_profile_get_amplitude_of_peak(
  const peak_t* const pPeak,
  double_t height_km) {

  double_t thickness_param =
    (pPeak->height_km > height_km) ?
    pPeak->thickness.bottom_km :
    pPeak->thickness.top_km;

  double_t electron_density =
    NeQuickG_exp((height_km - pPeak->height_km) /
                  thickness_param);

  electron_density =
    (pPeak->amplitude * electron_density /
      NeQuickG_square(electron_density + 1.0));

  return NEQUICKG_IONO_LAYER_GET_PEAK_AMPLITUDE(electron_density);
}

static int32_t iono_profile_get_critical_freqs(
  iono_profile_t * const pProfile,
  const NeQuickG_time_t* const pTime,
  const modip_context_t* const pModip,
  const solar_activity_t* const pSolar_activity,
  const position_t * const pCurrent_position) {

  E_layer_get_critical_freq_MHz(
    &pProfile->E,
    pTime,
    pSolar_activity,
    pCurrent_position);

  int32_t ret = F2_layer_get_critical_freq_MHz(
    &pProfile->F2,
    pTime,
    pModip,
    pSolar_activity,
    pCurrent_position);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  F1_layer_get_critical_freq_MHz(
    &pProfile->F1,
    pProfile->E.layer.critical_frequency_MHz,
    pProfile->F2.layer.critical_frequency_MHz);

  return NEQUICK_OK;
}

static void iono_profile_get_peak_heights(
  iono_profile_t * const pProfile) {

  E_layer_get_peak_height(&pProfile->E);

  F2_layer_get_peak_height(
    &pProfile->F2,
    pProfile->E.layer.critical_frequency_MHz);

  F1_layer_get_peak_height(
    &pProfile->F1,
    pProfile->E.layer.peak.height_km,
    pProfile->F2.layer.peak.height_km);
}

static void iono_profile_get_peak_thicknesses(
  iono_profile_t * const pProfile) {

  F2_layer_get_peak_thickness(
    &pProfile->F2);

  F1_layer_get_peak_thickness(
    &pProfile->F1,
    pProfile->E.layer.peak.height_km,
    pProfile->F2.layer.peak.height_km);

  E_layer_get_peak_thickness(
    &pProfile->E,
    pProfile->F1.peak.thickness.bottom_km);
}


static double_t iono_profile_get_peak_amp_substracting_layer(
  const peak_t * const pPeak,
  const peak_t * const pOther_layer_peak) {
  double_t other_layer_contribution =
    iono_profile_get_amplitude_of_peak(pOther_layer_peak, pPeak->height_km);

  return (
    NEQUICKG_IONO_LAYER_GET_PEAK_AMPLITUDE(pPeak->electron_density) -
    other_layer_contribution);
}

#ifndef FTR_UNIT_TEST
static
#endif
void iono_profile_get_peak_amplitudes(
  iono_profile_t * const pProfile) {

  F2_layer_get_peak_amplitude(&pProfile->F2);

  peak_t* const pE_peak = &pProfile->E.layer.peak;
  peak_t* const pF1_peak = &pProfile->F1.peak;
  peak_t* const pF2_peak = &pProfile->F2.layer.peak;

  double_t E_peak_amplitude_substracting_F2 =
    iono_profile_get_peak_amp_substracting_layer(pE_peak, pF2_peak);

  if (NEQUICK_G_F1_LAYER_IS_PEAK_PRESENT(pProfile->F1)) {

    double_t F1_peak_amplitude_substracting_F2 =
      iono_profile_get_peak_amp_substracting_layer(pF1_peak, pF2_peak);

    pE_peak->amplitude =
        NEQUICKG_IONO_LAYER_GET_PEAK_AMPLITUDE(pE_peak->electron_density);

    for (
      size_t i = 0; i < NEQUICK_G_JRC_IONO_PEAK_AMPLITUDE_ITERATION_COUNT; i++) {

      // F1
      {
        double_t amplitude_E_at_F1_height =
        iono_profile_get_amplitude_of_peak(pE_peak, pF1_peak->height_km);

        pF1_peak->amplitude =
          F1_peak_amplitude_substracting_F2 -
          amplitude_E_at_F1_height;

        pF1_peak->amplitude =
          NeQuickG_func_join(
            pF1_peak->amplitude,
            NEQUICK_G_JRC_IONO_F1_LAYER_AMPLITUDE_CONSTANT *
            pF1_peak->electron_density,
            1.0,
            pF1_peak->amplitude -
            (NEQUICK_G_JRC_IONO_F1_LAYER_AMPLITUDE_CONSTANT *
             pF1_peak->electron_density));
      }

      // E
      pE_peak->amplitude =
        E_peak_amplitude_substracting_F2 -
        iono_profile_get_amplitude_of_peak(pF1_peak, pE_peak->height_km);
    }
  } else {
    pF1_peak->amplitude = 0.0;
    pE_peak->amplitude = E_peak_amplitude_substracting_F2;
  }

  pE_peak->amplitude =
    NeQuickG_func_join(
      pE_peak->amplitude,
      NEQUICK_G_JRC_IONO_E_LAYER_AMPLITUDE_CONSTANT_1,
      NEQUICK_G_JRC_IONO_E_LAYER_AMPLITUDE_WITH_TRANSITION_REGION,
      pE_peak->amplitude -
      NEQUICK_G_JRC_IONO_E_LAYER_AMPLITUDE_CONSTANT_2);
}

static void iono_profile_get_peaks(iono_profile_t * const pProfile) {

  iono_profile_get_peak_heights(pProfile);

  iono_profile_get_peak_thicknesses(pProfile);

  iono_profile_get_peak_amplitudes(pProfile);
}

static void iono_profile_get_exosphere_contribution(
  iono_profile_t * const pProfile,
  const NeQuickG_time_t* const pTime,
  const solar_activity_t* const pSolar_activity) {

  F2_layer_exosphere_adjust(
    &pProfile->F2,
    pTime,
    pSolar_activity);
}

int32_t iono_profile_get(
  iono_profile_t * const pProfile,
  const NeQuickG_time_t* const pTime,
  modip_context_t* const pModip,
  const solar_activity_t* const pSolar_activity,
  const position_t * const pCurrent_position) {

  modip_get(pModip, pCurrent_position);

  int32_t ret = iono_profile_get_critical_freqs(
    pProfile,
    pTime,
    pModip,
    pSolar_activity,
    pCurrent_position);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  iono_profile_get_peaks(pProfile);

  iono_profile_get_exosphere_contribution(
    pProfile,
    pTime,
    pSolar_activity);

  return NEQUICK_OK;
}

#undef NEQUICK_G_JRC_IONO_PEAK_AMPLITUDE_ITERATION_COUNT
