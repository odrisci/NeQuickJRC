/**
 * NeQuickG Az Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_Az_test.h"

#include "NeQuickG_JRC_macros.h"
#include "NeQuickG_JRC_MODIP.h"
#include "NeQuickG_JRC_solar_activity.h"

#define NEQUICK_G_JRC_AZ_TEST_VECTORS_COUNT (8*3)
#define NEQUICK_G_JRC_AZ_EPSILON (1e-6)
#define NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY {236.831641, -0.39362878, 0.00402826613}
#define NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY {121.129893, 0.351254133, 0.0134635348}
#define NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY {2.580271, 0.127628236, 0.0252748384}

typedef struct NeQuickG_Az_test_st {
  AZ_coefficients_t az_coefficients;
  double_t modip;
  double_t Az_expected;
} NeQuickG_Az_test_t;

static const NeQuickG_Az_test_t
  NeQuickG_Az_test_vector[NEQUICK_G_JRC_AZ_TEST_VECTORS_COUNT] =
{
  {
    {NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY},
    76.284073, 230.245562
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY},
    76.284073, 226.272795
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY},
    76.284073, 159.397123
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY},
    19.528774, 230.680826
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY},
    19.528774, 133.124084
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY},
    19.528774, 14.711835
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY},
    47.857688, 227.219650
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY},
    47.857688, 168.776422
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY},
    47.857688, 66.576699
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY},
    -23.316310, 248.199580
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY},
    -23.316310, 120.259398
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY},
    -23.316310, 13.345125
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY},
    -71.811295, 285.871846
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY},
    -71.811295, 165.335471
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY},
    -71.811295, 123.753978
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY},
    46.487417, 227.238261
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY},
    46.487417, 166.554565
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY},
    46.487417, 63.134324
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY},
    33.054571, 228.221713
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY},
    33.054571, 147.450769
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY},
    33.054571, 34.414374
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY},
    -51.379818, 267.690378
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY},
    -51.379818, 138.624713
  },
  {
    {NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY},
    -51.379818, 62.745440
  }
};

static bool NeQuickG_Az_test_new(void) {
  for (size_t i = 0x00; i < NEQUICK_G_JRC_AZ_TEST_VECTORS_COUNT; i++) {
    double_t az =
      solar_activity_get_effective_ionisation_level_in_sfu(
        &NeQuickG_Az_test_vector[i].az_coefficients,
        NeQuickG_Az_test_vector[i].modip);
    if (!THRESHOLD_COMPARE(
        az,
        NeQuickG_Az_test_vector[i].Az_expected,
        NEQUICK_G_JRC_AZ_EPSILON)) {
      LOG_ERROR("Az value is not the expected.");
      return false;
    }
  }
  return true;
}

bool NeQuickG_Az_test(void) {
  return NeQuickG_Az_test_new();
}

#undef NEQUICK_G_JRC_AZ_COEFFS_LOW_ACTIVITY
#undef NEQUICK_G_JRC_AZ_COEFFS_MEDIUM_ACTIVITY
#undef NEQUICK_G_JRC_AZ_COEFFS_HIGH_ACTIVITY
#undef NEQUICK_G_JRC_AZ_TEST_VECTORS_COUNT
#undef NEQUICK_G_JRC_AZ_EPSILON
