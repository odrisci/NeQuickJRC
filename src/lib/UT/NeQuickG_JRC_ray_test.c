/**
 * NeQuickG Ray properties Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_ray_test.h"

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_coordinates.h"
#include "NeQuickG_JRC_macros.h"
#include "NeQuickG_JRC_ray.h"

#define NEQUICK_G_JRC_RAY_TEST_VECTORS_COUNT (1)
#define NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON (1e-5)

typedef struct NeQuickG_ray_test_st {
  double_t station_lat;
  double_t station_long;
  double_t station_height;
  double_t satellite_lat;
  double_t satellite_long;
  double_t satellite_height;
  ray_context_t ray_expected;
} NeQuickG_ray_test_t;

static const NeQuickG_ray_test_t
  NeQuickG_ray_test_vector[NEQUICK_G_JRC_RAY_TEST_VECTORS_COUNT] =
{
  {
    82.494293510, 297.659539798, 0.078107446,
    54.445029416, 241.529931024, 20370.730845002,
    {
      {
        {297.659539798, 0.0, 0.0, 0.0}, {82.494293510, 0.0, 0.688995129, 0.724765970}, 0.078107446, NEQUICK_G_JRC_HEIGHT_UNITS_KM, 0.0, 0.0
      },
      {
        {241.529931024, 0.0, 0.0, 0.0}, {54.445029416, 0.0, 0.813558007, 0.581483765}, 20370.730845002, NEQUICK_G_JRC_HEIGHT_UNITS_KM, 0.0, 0.0
      },
      {405.289045373, 0.0, 0.0, 0.0},
      {43.550617197, 0.0, 0.0, 0.0},
      false,
      {
        {-0.164640718, 0.986353605},
        4169.486317342, 0.0, 0.0
      }
    }
  },
};

static bool NeQuickG_ray_test_impl(void) {

  for (size_t i = 0x00; i < NEQUICK_G_JRC_RAY_TEST_VECTORS_COUNT; i++) {

    position_t station_position;
    position_t satellite_position;

    if (position_set(
      &station_position,
      NeQuickG_ray_test_vector[i].station_long,
      NeQuickG_ray_test_vector[i].station_lat,
      NeQuickG_ray_test_vector[i].station_height,
      NEQUICK_G_JRC_HEIGHT_UNITS_KM) != NEQUICK_OK) {
      return false;
    }

    if (position_set(
      &satellite_position,
      NeQuickG_ray_test_vector[i].satellite_long,
      NeQuickG_ray_test_vector[i].satellite_lat,
      NeQuickG_ray_test_vector[i].satellite_height,
      NEQUICK_G_JRC_HEIGHT_UNITS_KM) != NEQUICK_OK) {
      return false;
    }

    ray_context_t context;
    if (ray_get(
      &context,
      &station_position,
      &satellite_position) != NEQUICK_OK) {
      return false;
    }

    const ray_context_t* const pExpected_context =
      &NeQuickG_ray_test_vector[i].ray_expected;

    if (context.is_vertical != pExpected_context->is_vertical) {
      LOG_ERROR("Rey verticality test error.");
      return false;
    }

    if (!THRESHOLD_COMPARE(
        context.slant.perigee_radius_km,
        pExpected_context->slant.perigee_radius_km,
        NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
      LOG_ERROR("Rey radius perigee is not the expected.");
      return false;
    }

    // Check azimuth
    {
      if (!THRESHOLD_COMPARE(
          context.slant.azimuth.sin,
          pExpected_context->slant.azimuth.sin,
          NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
        LOG_ERROR("Azimuth sin is not the expected.");
        return false;
      }

      if (!THRESHOLD_COMPARE(
          context.slant.azimuth.cos,
          pExpected_context->slant.azimuth.cos,
          NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
        LOG_ERROR("Azimuth cos is not the expected.");
        return false;
      }
    }

    // Check longitude, latitude
    {
      if (!THRESHOLD_COMPARE(
          context.latitude.degree,
          pExpected_context->latitude.degree,
          NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
        LOG_ERROR("Ray latitude is not the expected.");
        return false;
      }
      if (!THRESHOLD_COMPARE(
          context.longitude.degree,
          pExpected_context->longitude.degree,
          NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
        LOG_ERROR("Ray longitude is not the expected.");
        return false;
      }
    }

    // Check receiver
    {
      if (!THRESHOLD_COMPARE(
          context.receiver_position.latitude.degree,
          pExpected_context->receiver_position.latitude.degree,
          NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
        LOG_ERROR("Receiver latitude is not the expected.");
        return false;
      }

      if (!THRESHOLD_COMPARE(
          context.receiver_position.latitude.sin,
          pExpected_context->receiver_position.latitude.sin,
          NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
        LOG_ERROR("Receiver latitude sinus is not the expected.");
        return false;
      }

      if (!THRESHOLD_COMPARE(
          context.receiver_position.latitude.cos,
          pExpected_context->receiver_position.latitude.cos,
          NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
        LOG_ERROR("Receiver latitude cosinus is not the expected.");
        return false;
      }

      if (!THRESHOLD_COMPARE(
          context.receiver_position.longitude.degree,
          pExpected_context->receiver_position.longitude.degree,
          NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
        LOG_ERROR("Receiver longitude is not the expected.");
        return false;
      }

      if (!THRESHOLD_COMPARE(
          context.receiver_position.height,
          pExpected_context->receiver_position.height,
          NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
        LOG_ERROR("Receiver height is not the expected.");
        return false;
      }

      // Check satellite
      {
        if (!THRESHOLD_COMPARE(
            context.satellite_position.latitude.degree,
            pExpected_context->satellite_position.latitude.degree,
            NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
          LOG_ERROR("satellite latitude is not the expected.");
          return false;
        }

        if (!THRESHOLD_COMPARE(
            context.satellite_position.latitude.sin,
            pExpected_context->satellite_position.latitude.sin,
            NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
          LOG_ERROR("satellite latitude sinus is not the expected.");
          return false;
        }

        if (!THRESHOLD_COMPARE(
            context.satellite_position.latitude.cos,
            pExpected_context->satellite_position.latitude.cos,
            NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
          LOG_ERROR("satellite latitude cosinus is not the expected.");
          return false;
        }

        if (!THRESHOLD_COMPARE(
            context.satellite_position.longitude.degree,
            pExpected_context->satellite_position.longitude.degree,
            NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
          LOG_ERROR("satellite longitude is not the expected.");
          return false;
        }

        if (!THRESHOLD_COMPARE(
            context.satellite_position.height,
            pExpected_context->satellite_position.height,
            NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON)) {
          LOG_ERROR("satellite height is not the expected.");
          return false;
        }
      }
    }
  }
  return true;
}

bool NeQuickG_ray_test(void) {
  return NeQuickG_ray_test_impl();
}

#undef NEQUICK_G_JRC_RAY_TEST_VECTORS_COUNT
#undef NEQUICK_G_JRC_RAY_TEST_RESULT_EPSILON
