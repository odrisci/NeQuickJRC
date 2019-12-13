/**
 * NeQuickG Modified Dip Latitude Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_MODIP_test.h"

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_macros.h"
#include "NeQuickG_JRC_MODIP.h"

#define NEQUICK_G_JRC_MODIP_TEST_VECTORS_COUNT (8)
#define NEQUICK_G_JRC_MODIP_EPSILON (1e-5)

typedef struct NeQuickG_modip_test_st {
  position_t station_position;
  double_t modip_degree_expected;
} NeQuickG_modip_test_t;

static const NeQuickG_modip_test_t
  NeQuickG_modip_test_vector[NEQUICK_G_JRC_MODIP_TEST_VECTORS_COUNT] =
{
  {
    {
      {297.65954, 0.0, 0.0, 0.0}, {82.49429, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    76.28407
  },
  {
    {
      {307.19404, 0.0, 0.0, 0.0}, {5.25218, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    19.52877
  },
  {
    {
      {355.75034, 0.0, 0.0, 0.0}, {40.42916, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    47.85769
  },
  {
    {
      {40.19439, 0.0, 0.0, 0.0}, {-2.99591, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    -23.31631
  },
  {
    {
      {166.66933, 0.0, 0.0, 0.0}, {-77.83835, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    -71.81130
  },
  {
    {
      {141.13283, 0.0, 0.0, 0.0}, {39.13517, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    46.48742
  },
  {
    {
      {204.54366, 0.0, 0.0, 0.0}, {19.80135, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    33.05457
  },
  {
    {
      {115.88525, 0.0, 0.0, 0.0}, {-31.80197, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    -51.37982
  }
};

static bool NeQuickG_modip_test_new(void) {
  modip_context_t context;

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
  char modip_grid_file[] = "./../../modip/modip2001_wrapped.asc";
  if (modip_init(&context, modip_grid_file) != NEQUICK_OK) {
    return false;
  }
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

  for (size_t i = 0x00; i < NEQUICK_G_JRC_MODIP_TEST_VECTORS_COUNT; i++) {
    modip_get(
      &context, &NeQuickG_modip_test_vector[i].station_position);

    if (!THRESHOLD_COMPARE(
        context.modip_degree,
        NeQuickG_modip_test_vector[i].modip_degree_expected,
        NEQUICK_G_JRC_MODIP_EPSILON)) {
      LOG_ERROR("MODIP value is not the expected.");
      return false;
    }
  }
  return true;
}

bool NeQuickG_modip_test(void) {
  return NeQuickG_modip_test_new();
}

#undef NEQUICK_G_JRC_MODIP_TEST_VECTORS_COUNT
#undef NEQUICK_G_JRC_MODIP_EPSILON
