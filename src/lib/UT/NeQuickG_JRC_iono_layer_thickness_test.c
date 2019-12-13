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
#include "NeQuickG_JRC_iono_layer_thickness_test.h"

#include <math.h>

#include "NeQuickG_JRC_iono_E_layer.h"
#include "NeQuickG_JRC_iono_F1_layer.h"
#include "NeQuickG_JRC_iono_F2_layer.h"
#include "NeQuickG_JRC_macros.h"

#define NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_COUNT (1)
#define NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_THESHOLD (1e-5)

typedef struct NeQuickG_layer_thickness_test_st {
  double_t trans_factor;
  double_t F2_critical_frequency_MHz;
  double_t F2_electron_density;
  double_t F2_peak_height;
  thickness_t F2_expected;
  thickness_t F1_expected;
  thickness_t E_expected;
} NeQuickG_layer_thickness_test_t;

static const NeQuickG_layer_thickness_test_t
NeQuickG_layer_thickness_test_vector[NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_COUNT] = {
  {
    2.366982653,
    6.5756128,
    5.361596779,
    412.551921084,
    {0.0, 45.987213389},
    {43.882788163, 73.137980271},
    {73.137980271, 5.000000000},
  },
};

static bool NeQuickG_iono_layer_thickness_test_impl(void) {
  for (
    size_t i = 0;
    i < NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_COUNT; i++) {

    {
      F2_layer_t F2;
      F2.trans_factor = NeQuickG_layer_thickness_test_vector[i].trans_factor;
      F2.layer.critical_frequency_MHz =
        NeQuickG_layer_thickness_test_vector[i].F2_critical_frequency_MHz;
      F2.layer.peak.electron_density =
        NeQuickG_layer_thickness_test_vector[i].F2_electron_density;
      F2.layer.peak.height_km =
        NeQuickG_layer_thickness_test_vector[i].F2_peak_height;

      F2_layer_get_peak_thickness(
        &F2);

      if (!THRESHOLD_COMPARE(
          F2.layer.peak.thickness.bottom_km,
          NeQuickG_layer_thickness_test_vector[i].F2_expected.bottom_km,
          NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_THESHOLD)) {
        LOG_ERROR("F2 peak B2Bot is not the expected.");
        return false;
      }
    }

    layer_t F1;
    {
      E_layer_t E;
      E_layer_init(&E);

      E_layer_get_peak_height(&E);

      F1_layer_get_peak_height(
        &F1,
        E.layer.peak.height_km,
        NeQuickG_layer_thickness_test_vector[i].F2_peak_height);

      F1_layer_get_peak_thickness(
        &F1,
        E.layer.peak.height_km,
        NeQuickG_layer_thickness_test_vector[i].F2_peak_height);
      if (!THRESHOLD_COMPARE(
          F1.peak.thickness.bottom_km,
          NeQuickG_layer_thickness_test_vector[i].F1_expected.bottom_km,
          NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_THESHOLD)) {
        LOG_ERROR("F1 peak B1Bot is not the expected.");
        return false;
      }
      if (!THRESHOLD_COMPARE(
          F1.peak.thickness.top_km,
          NeQuickG_layer_thickness_test_vector[i].F1_expected.top_km,
          NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_THESHOLD)) {
        LOG_ERROR("F1 peak B1Top is not the expected.");
        return false;
      }
    }

    {
      E_layer_t E;
      E_layer_init(&E);
      E_layer_get_peak_thickness(
        &E,
        F1.peak.thickness.bottom_km);
      if (!THRESHOLD_COMPARE(
          E.layer.peak.thickness.bottom_km,
          NeQuickG_layer_thickness_test_vector[i].E_expected.bottom_km,
          NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_THESHOLD)) {
        LOG_ERROR("E peak BEBot is not the expected.");
        return false;
      }
      if (!THRESHOLD_COMPARE(
          E.layer.peak.thickness.top_km,
          NeQuickG_layer_thickness_test_vector[i].E_expected.top_km,
          NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_THESHOLD)) {
        LOG_ERROR("E peak BETop is not the expected.");
        return false;
      }
    }
  }
  return true;
}

bool NeQuickG_iono_layer_thickness_test(void) {
  return NeQuickG_iono_layer_thickness_test_impl();
}

#undef NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_THESHOLD
#undef NEQUICKG_IONO_LAYER_THICKNESS_TEST_VECTORS_COUNT
