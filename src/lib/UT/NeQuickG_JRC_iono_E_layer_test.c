/**
 * NeQuickG E layer Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_iono_E_layer_test.h"

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_iono_E_layer.h"
#include "NeQuickG_JRC_macros.h"
#include "NeQuickG_JRC_MODIP.h"
#include "NeQuickG_JRC_solar.h"
#include "NeQuickG_JRC_solar_activity.h"

#define NEQUICK_G_JRC_IONO_E_LAYER_TEST_VECTORS_COUNT (3)
#define NEQUICK_G_JRC_IONO_E_LAYER_CRIT_FREQ_EPSILON (1e-5)

// We need AZ because we generate the test vectors using
// the ESA version where the calculation is done during the integration
// so Az cannot be calculated with the position given below.
// this position is not the initial given in the input data.
typedef struct NeQuickG_iono_E_layer_test_st {
  double_t Az;
  position_t position;
  NeQuickG_time_t time;
  double_t freq_expected;
} NeQuickG_iono_E_layer_test_t;

static const NeQuickG_iono_E_layer_test_t
  NeQuickG_iono_E_layer_test_vector[NEQUICK_G_JRC_IONO_E_LAYER_TEST_VECTORS_COUNT] =
{
  {
    230.245562,
    {
      {297.438315, 0.0, 0.0, 0.0}, {82.481346, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    {
      0.0, 4
    },
    2.409694
  },
  {
    248.199580,
    {
      {40.173216, 0.0, 0.0, 0.0}, {-3.023059, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    {
      1.0, 4
    },
    0.726159
  },
  {
    285.871846,
    {
      {137.301902, 0.0, 0.0, 0.0}, {-82.090099, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    {
      11.0, 4
    },
    0.971728
  },
};

static bool NeQuickG_iono_E_layer_new(void) {

  for (size_t i = 0x00; i < NEQUICK_G_JRC_IONO_E_LAYER_TEST_VECTORS_COUNT; i++) {

    position_t pos;
    if (position_set(&pos,
      NeQuickG_iono_E_layer_test_vector[i].position.longitude.degree,
      NeQuickG_iono_E_layer_test_vector[i].position.latitude.degree,
      0.0, NEQUICK_G_JRC_HEIGHT_UNITS_KM) != NEQUICK_OK) {
      return false;
    }

    solar_activity_t solar_activity;
    solar_activity.effective_ionisation_level_sfu =
      NeQuickG_iono_E_layer_test_vector[i].Az;

    E_layer_t E;
    E_layer_init(&E);
      E_layer_get_critical_freq_MHz(
        &E,
        &NeQuickG_iono_E_layer_test_vector[i].time,
        &solar_activity,
        &pos);

    if (!THRESHOLD_COMPARE(
        E.layer.critical_frequency_MHz,
        NeQuickG_iono_E_layer_test_vector[i].freq_expected,
        NEQUICK_G_JRC_IONO_E_LAYER_CRIT_FREQ_EPSILON)) {
      LOG_ERROR("E layer. Critical frequency is not the expected.");
      return false;
    }
  }
  return true;
}

bool NeQuickG_iono_E_layer_test(void) {
  return NeQuickG_iono_E_layer_new();
}

#undef NEQUICK_G_JRC_IONO_E_LAYER_CRIT_FREQ_EPSILON
#undef NEQUICK_G_JRC_IONO_E_LAYER_TEST_VECTORS_COUNT
