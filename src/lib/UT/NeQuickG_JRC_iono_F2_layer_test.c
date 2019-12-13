/**
 * NeQuickG F2 layer Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_iono_F2_layer_test.h"

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_iono_F2_layer.h"
#include "NeQuickG_JRC_macros.h"

#define NEQUICKG_IONO_F2_LAYER_TEST_VECTORS_COUNT (3)
#define NEQUICKG_IONO_F2_LAYER_TEST_HEIGHT_VECTORS_COUNT (1)
#define NEQUICKG_IONO_F2_LAYER_TEST_EXOSPHERE_VECTORS_COUNT (1)
#define NEQUICKG_IONO_F2_LAYER_TEST_CRIT_F_THESHOLD (1e-5)
#define NEQUICKG_IONO_F2_LAYER_TEST_TRANS_FACTOR_THESHOLD (1e-5)

typedef struct NeQuickG_F2_layer_test_st {
  NeQuickG_time_t time;
  double_t Azr;
  double_t longitude_degree;
  double_t latitude_degree;
  double_t critical_frequency_MHz;
  double_t trans_factor;
} NeQuickG_F2_layer_test_t;

typedef struct NeQuickG_F2_layer_test_height_st {
  double_t trans_factor;
  double_t critical_frequency_MHz;
  double_t critical_frequency_MHz_E;
  double_t height;
} NeQuickG_F2_layer_test_height_t;

typedef struct NeQuickG_F2_layer_test_exosphere_st {
  double_t electron_density;
  double_t height;
  double_t bottom_km;
  NeQuickG_time_t time;
  double_t top_km_expected;
} NeQuickG_F2_layer_test_exosphere_t;

static const NeQuickG_F2_layer_test_t
NeQuickG_F2_layer_test_vector[NEQUICKG_IONO_F2_LAYER_TEST_VECTORS_COUNT] = {
  {
    {0.0, 4},
    186.328060298,
    297.438314733,
    82.481346266,
    6.575612801,
    2.366982653
  },
  {
    {2.666667, 4},
    186.328060298,
    378.105699987,
    74.653105616,
    6.289174069,
    2.375445167
  },
  {
    {10.500000, 4},
    203.036737976,
    31.022679098,
    -8.649092730,
    16.638422783,
    2.244593590
  },
};

static const NeQuickG_F2_layer_test_height_t
NeQuickG_F2_layer_test_height_vector[NEQUICKG_IONO_F2_LAYER_TEST_HEIGHT_VECTORS_COUNT] = {
  {
    2.366982653,
    6.575612801,
    2.409694059,
    412.551921084
  },
};

static const NeQuickG_F2_layer_test_exosphere_t
NeQuickG_F2_layer_test_exosphere_vector[NEQUICKG_IONO_F2_LAYER_TEST_EXOSPHERE_VECTORS_COUNT] = {
  {
    5.361596779,
    412.551921084,
    45.987213389,
    {0.0, 4},
    50.657704983
  },
};


static bool NeQuickG_iono_F2_layer_test_freq(void) {

  F2_layer_t F2;

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
  const char CCIR_folder[] = {"./../../ccir/"};
#endif

  if (F2_layer_init(&F2, CCIR_folder) != NEQUICK_OK) {
    return false;
  }

  for (
    size_t i = 0;
    i < NEQUICKG_IONO_F2_LAYER_TEST_VECTORS_COUNT; i++) {

    input_data_t input_data;
    input_data.time = NeQuickG_F2_layer_test_vector[i].time;

    solar_activity_t solar_activity;
    solar_activity.effective_sun_spot_count = NeQuickG_F2_layer_test_vector[i].Azr;

    position_t current_position;
    if (position_set(&current_position,
      NeQuickG_F2_layer_test_vector[i].longitude_degree,
      NeQuickG_F2_layer_test_vector[i].latitude_degree,
      0.0, NEQUICK_G_JRC_HEIGHT_UNITS_KM) != NEQUICK_OK) {
      return false;
    }

    modip_context_t modip;
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
    char modip_grid_file[] = "./../../modip/modip2001_wrapped.asc";
    if (modip_init(&modip, modip_grid_file) != NEQUICK_OK) {
      return false;
    }
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS
    modip_get(&modip, &current_position);

    if (F2_layer_get_critical_freq_MHz(
        &F2,
        &input_data.time,
        &modip,
        &solar_activity,
        &current_position) != NEQUICK_OK) {
      return false;
    }

    if (!THRESHOLD_COMPARE(
          F2.layer.critical_frequency_MHz,
          NeQuickG_F2_layer_test_vector[i].critical_frequency_MHz,
          NEQUICKG_IONO_F2_LAYER_TEST_CRIT_F_THESHOLD)) {
      LOG_ERROR("F2 critical frequency is not the expected.");
      F2_layer_close(&F2);
      return false;
    }

    if (!THRESHOLD_COMPARE(
          F2.trans_factor,
          NeQuickG_F2_layer_test_vector[i].trans_factor,
          NEQUICKG_IONO_F2_LAYER_TEST_TRANS_FACTOR_THESHOLD)) {
      LOG_ERROR("F2 transmission factor is not the expected.");
      F2_layer_close(&F2);
      return false;
    }
  }
  F2_layer_close(&F2);
  return true;
}

static bool NeQuickG_iono_F2_layer_test_height(void) {
  for (
    size_t i = 0;
    i < NEQUICKG_IONO_F2_LAYER_TEST_HEIGHT_VECTORS_COUNT; i++) {

    F2_layer_t F2;
    F2.trans_factor = NeQuickG_F2_layer_test_height_vector[i].trans_factor;
    F2.layer.critical_frequency_MHz =
      NeQuickG_F2_layer_test_height_vector[i].critical_frequency_MHz;

    F2_layer_get_peak_height(
      &F2,
      NeQuickG_F2_layer_test_height_vector[i].critical_frequency_MHz_E);

    if (!THRESHOLD_COMPARE(
          F2.layer.peak.height_km,
          NeQuickG_F2_layer_test_height_vector[i].height,
          NEQUICKG_IONO_F2_LAYER_TEST_TRANS_FACTOR_THESHOLD)) {
      LOG_ERROR("F2 peak height is not the expected.");
      return false;
    }
  }
  return true;
}

static bool NeQuickG_iono_F2_layer_test_exosphere(void) {
  for (
    size_t i = 0;
    i < NEQUICKG_IONO_F2_LAYER_TEST_EXOSPHERE_VECTORS_COUNT; i++) {

    F2_layer_t F2;
    F2.layer.peak.electron_density =
      NeQuickG_F2_layer_test_exosphere_vector[i].electron_density;

    F2.layer.peak.height_km =
      NeQuickG_F2_layer_test_exosphere_vector[i].height;

    F2.layer.peak.thickness.bottom_km =
      NeQuickG_F2_layer_test_exosphere_vector[i].bottom_km;

    solar_activity_t solar_activity;
    solar_activity.effective_sun_spot_count = NeQuickG_F2_layer_test_vector[i].Azr;

    F2_layer_exosphere_adjust(&F2,
      &NeQuickG_F2_layer_test_exosphere_vector[i].time,
      &solar_activity);

    if (!THRESHOLD_COMPARE(
          F2.layer.peak.thickness.top_km,
          NeQuickG_F2_layer_test_exosphere_vector[i].top_km_expected,
          NEQUICKG_IONO_F2_LAYER_TEST_TRANS_FACTOR_THESHOLD)) {
      LOG_ERROR("F2 thickness top is not the expected.");
      return false;
    }
  }
  return true;
}

bool NeQuickG_iono_F2_layer_test(void) {
  bool ret = true;
  if (!NeQuickG_iono_F2_layer_test_freq()) {
    ret = false;
  }
  if (!NeQuickG_iono_F2_layer_test_height()) {
    ret = false;
  }
  if (!NeQuickG_iono_F2_layer_test_exosphere()) {
    ret = false;
  }
  return ret;
}

#undef NEQUICKG_IONO_F2_LAYER_TEST_EXOSPHERE_VECTORS_COUNT
#undef NEQUICKG_IONO_F2_LAYER_TEST_HEIGHT_VECTORS_COUNT
#undef NEQUICKG_IONO_F2_LAYER_TEST_TRANS_FACTOR_THESHOLD
#undef NEQUICKG_IONO_F2_LAYER_TEST_VECTORS_COUNT
