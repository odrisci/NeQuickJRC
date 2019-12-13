/**
 * NeQuickG Ray Solar module Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_solar_test.h"

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_macros.h"
#include "NeQuickG_JRC_solar.h"

#define NEQUICK_G_JRC_SOLAR_TEST_VECTORS_COUNT (5)
#define NEQUICK_G_JRC_SOLAR_EFFETIVE_ZENITH_ANGLE_EPSILON (1e-5)

typedef struct NeQuickG_solar_effective_zenith_angle_test_st {
  position_t position;
  NeQuickG_time_t time;
  double_t angle_degree_expected;
} NeQuickG_solar_effective_zenith_angle_test_t;

static const NeQuickG_solar_effective_zenith_angle_test_t
  NeQuickG_solar_test_vector[NEQUICK_G_JRC_SOLAR_TEST_VECTORS_COUNT] =
{
  {
    {
      {297.438315, 0.0, 0.0, 0.0}, {82.481346, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    {
      0.000000, 4
    },
    82.978526
  },
  {
    {
      {139.790965, 0.0, 0.0, 0.0}, {26.595660, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    {
      0.833333, 4
    },
    30.664744
  },
  {
    {
      {298.035128, 0.0, 0.0, 0.0}, {82.566581, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    {
      12.0, 4
    },
    76.209610
  },
  {
    {
      {260.236348, 0.0, 0.0, 0.0}, {35.803032, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    {
      23.833333, 4
    },
    74.148769
  },
   {
    {
      {114.989502, 0.0, 0.0, 0.0}, {-50.092818, 0.0, 0.0, 0.0}, 0.0, 0, 0.0, 0.0
    },
    {
      23.833333, 4
    },
    83.933196
  },
};

static bool NeQuickG_solar_test_impl(void) {

  for (size_t i = 0x00; i < NEQUICK_G_JRC_SOLAR_TEST_VECTORS_COUNT; i++) {

    position_t pos;
    if (position_set(&pos,
      NeQuickG_solar_test_vector[i].position.longitude.degree,
      NeQuickG_solar_test_vector[i].position.latitude.degree,
      0.0, NEQUICK_G_JRC_HEIGHT_UNITS_KM) != NEQUICK_OK) {
      return false;
    }

    angle_restricted_t solar_declination =
      solar_get_declination(&NeQuickG_solar_test_vector[i].time);

    double_t angle =
      solar_get_effective_zenith_angle(
        &pos,
        &NeQuickG_solar_test_vector[i].time,
        &solar_declination);

    if (!THRESHOLD_COMPARE(
        angle,
        NeQuickG_solar_test_vector[i].angle_degree_expected,
        NEQUICK_G_JRC_SOLAR_EFFETIVE_ZENITH_ANGLE_EPSILON)) {
      LOG_ERROR("Solar effective zenith angle is not the expected.");
      return false;
    }
  }
  return true;
}

bool NeQuickG_solar_test(void) {
  return NeQuickG_solar_test_impl();
}

#undef NEQUICK_G_JRC_SOLAR_EFFETIVE_ZENITH_ANGLE_EPSILON
#undef NEQUICK_G_JRC_SOLAR_TEST_VECTORS_COUNT
