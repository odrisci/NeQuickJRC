/**
 * NeQuickG peak layer Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_iono_layer_amplitudes_test.h"

#include <math.h>

#include "NeQuickG_JRC_iono_E_layer.h"
#include "NeQuickG_JRC_iono_F1_layer.h"
#include "NeQuickG_JRC_iono_F2_layer.h"
#include "NeQuickG_JRC_iono_profile.h"
#include "NeQuickG_JRC_macros.h"

#define NEQUICKG_IONO_LAYER_AMPLITUDES_TEST_VECTORS_COUNT (2)
#define NEQUICKG_IONO_LAYER_AMPLITUDES_TEST_VECTORS_THESHOLD (1e-5)

typedef struct NeQuickG_layer_amplitude_test_st {
  double_t F2_electron_density;
  double_t F2_peak_height;
  double_t F2_peak_thickness_bottom;
  double_t F1_critical_frequency_MHz;
  double_t F1_electron_density;
  double_t F1_peak_height;
  double_t F1_peak_thickness_bottom;
  double_t E_electron_density;
  double_t E_peak_height;
  double_t E_peak_thickness_top;
  double_t F2_expected;
  double_t F1_expected;
  double_t E_expected;
} NeQuickG_layer_amplitude_test_t;

static const NeQuickG_layer_amplitude_test_t
NeQuickG_layer_amplitude_test_vector[NEQUICKG_IONO_LAYER_AMPLITUDES_TEST_VECTORS_COUNT] = {
  {
    5.361596779,
    412.551921084,
    45.987213389,

    3.373571683,
    1.411242252,
    266.275960542,
    73.137980271,

    0.720021557,
    120.000000000,
    73.137980271,

    21.446387117,
    1.310172197,
    2.182239112,
  },
  {
    5.107156754,
    429.080085583,
    46.507547598,

    0.000000000,
    0.000000000,
    274.540042792,
    77.270021396,

    0.466645104,
    120.000000000,
    77.270021396,

    20.428627016,
    0.000000000,
    1.760670936
  }
};

static bool NeQuickG_iono_layer_amplitudes_test_impl(void) {
  for (
    size_t i = 0;
    i < NEQUICKG_IONO_LAYER_AMPLITUDES_TEST_VECTORS_COUNT; i++) {

    F2_layer_t F2;
    {
      F2.layer.peak.electron_density =
        NeQuickG_layer_amplitude_test_vector[i].F2_electron_density;
      F2.layer.peak.height_km =
        NeQuickG_layer_amplitude_test_vector[i].F2_peak_height;
      F2.layer.peak.thickness.bottom_km =
        NeQuickG_layer_amplitude_test_vector[i].F2_peak_thickness_bottom;
    }

    layer_t F1;
    {
      F1.critical_frequency_MHz =
        NeQuickG_layer_amplitude_test_vector[i].F1_critical_frequency_MHz;
      F1.peak.electron_density =
        NeQuickG_layer_amplitude_test_vector[i].F1_electron_density;
      F1.peak.height_km =
        NeQuickG_layer_amplitude_test_vector[i].F1_peak_height;
      F1.peak.thickness.bottom_km =
        NeQuickG_layer_amplitude_test_vector[i].F1_peak_thickness_bottom;
    }

    E_layer_t E;
    {
      E.layer.peak.electron_density =
        NeQuickG_layer_amplitude_test_vector[i].E_electron_density;
      E.layer.peak.height_km =
        NeQuickG_layer_amplitude_test_vector[i].E_peak_height;
      E.layer.peak.thickness.top_km =
        NeQuickG_layer_amplitude_test_vector[i].E_peak_thickness_top;
    }

    iono_profile_t profile;
    profile.E = E;
    profile.F1 = F1;
    profile.F2 = F2;

    iono_profile_get_peak_amplitudes(&profile);

    if (!THRESHOLD_COMPARE(
        profile.F2.layer.peak.amplitude,
        NeQuickG_layer_amplitude_test_vector[i].F2_expected,
        NEQUICKG_IONO_LAYER_AMPLITUDES_TEST_VECTORS_THESHOLD)) {
      LOG_ERROR("F2 peak amplitude is not the expected.");
      return false;
    }

    if (!THRESHOLD_COMPARE(
        profile.F1.peak.amplitude,
        NeQuickG_layer_amplitude_test_vector[i].F1_expected,
        NEQUICKG_IONO_LAYER_AMPLITUDES_TEST_VECTORS_THESHOLD)) {
      LOG_ERROR("F1 peak amplitude is not the expected.");
      return false;
    }

    if (!THRESHOLD_COMPARE(
        profile.E.layer.peak.amplitude,
        NeQuickG_layer_amplitude_test_vector[i].E_expected,
        NEQUICKG_IONO_LAYER_AMPLITUDES_TEST_VECTORS_THESHOLD)) {
      LOG_ERROR("E peak amplitude is not the expected.");
      return false;
    }
  }
  return true;
}

bool NeQuickG_iono_layer_amplitudes_test(void) {
  return NeQuickG_iono_layer_amplitudes_test_impl();
}

#undef NEQUICKG_IONO_LAYER_AMPLITUDES_TEST_VECTORS_THESHOLD
#undef NEQUICKG_IONO_LAYER_AMPLITUDES_TEST_VECTORS_COUNT
