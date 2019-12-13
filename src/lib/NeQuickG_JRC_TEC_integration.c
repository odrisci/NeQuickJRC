/** NeQuickG Total Electron Content (TEC) integration routine
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_TEC_integration.h"

#include <assert.h>
#include <stdlib.h>

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_Gauss_Kronrod_integration.h"
#include "NeQuickG_JRC_macros.h"
#include "NeQuickG_JRC_ray_vertical.h"

#define NEQUICK_G_JRC_INTEGRATION_FIRST_POINT_KM (1000.0)
#define NEQUICK_G_JRC_INTEGRATION_SECOND_POINT_KM (2000.0)

/** Default integration tolerance for Kronrod G7-K15 integration method below 1000 km
 * See F.2.3.3.
 */
#define NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_BELOW_FIRST_POINT (0.001)
/** Default integration tolerance for Kronrod G7-K15 integration method above 1000 km
 * See F.2.3.3.
 */
#define NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_ABOVE_FIRST_POINT (0.01)

#define IS_SATELLITE_BELOW_FIRST_POINT(_p) \
  (_p->ray.satellite_position.height <= \
  NEQUICK_G_JRC_INTEGRATION_FIRST_POINT_KM)

#define IS_SATELLITE_BELOW_SECOND_POINT(_p) \
  (_p->ray.satellite_position.height <= \
  NEQUICK_G_JRC_INTEGRATION_SECOND_POINT_KM)

#define IS_RECEIVER_ABOVE_FIRST_POINT(_p) \
  (_p->ray.receiver_position.height >= \
   NEQUICK_G_JRC_INTEGRATION_FIRST_POINT_KM)

#define IS_RECEIVER_ABOVE_SECOND_POINT(_p) \
  (_p->ray.receiver_position.height >= \
   NEQUICK_G_JRC_INTEGRATION_SECOND_POINT_KM)

/** Kronrod G7-K15 integration maximum recursion level */
#define NEQUICK_G_JRC_RECURSION_LIMIT_MAX (50)

static double_t get_point_height(
  const NeQuickG_context_t* const pContext,
  double_t height_km) {
  if (pContext->ray.is_vertical) {
    return height_km;
  } else {
     return get_slant_distance(
      &pContext->ray,
      get_radius_from_height(height_km));
  }
}

static double_t get_point_zero_height(
  const NeQuickG_context_t* const pContext) {
  double_t temp = max(0.0, pContext->input_data.station_position.height);
  return get_point_height(pContext, temp);
}

static int32_t Gauss_Kronrod_integrate_impl(
  gauss_kronrod_context_t* const pContext,
  NeQuickG_context_t* const pNequick_Context,
  const double_t point_1_height_km,
  const double_t point_2_height_km,
  double_t* const pTEC) {

  pContext->recursion_level = 0;
  pContext->recursion_max = NEQUICK_G_JRC_RECURSION_LIMIT_MAX;

  return Gauss_Kronrod_integrate(
    pContext,
    pNequick_Context,
    point_1_height_km,
    point_2_height_km,
    pTEC);
}

static int32_t both_below_first_integration_point(
  NeQuickG_context_t* const pContext,
  double_t* const pTEC) {

  gauss_kronrod_context_t gauss_kronrod_context;
  gauss_kronrod_context.tolerance =
   NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_BELOW_FIRST_POINT;

  double_t zero_point_height_km = get_point_zero_height(pContext);

  double_t second_point_height_km =
    pContext->ray.is_vertical ?
      pContext->ray.satellite_position.height :
      pContext->ray.slant.satellite_distance_km;

  // integrate straight between P0 and P2 for TEC.
  // P0 is used because P1 could be less than zero
  return Gauss_Kronrod_integrate_impl(
    &gauss_kronrod_context,
    pContext,
    zero_point_height_km,
    second_point_height_km,
    pTEC);
}

static int32_t between_integration_points(
  NeQuickG_context_t* const pContext,
  double_t* const pTEC) {

  gauss_kronrod_context_t gauss_kronrod_context;
  gauss_kronrod_context.tolerance =
   NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_BELOW_FIRST_POINT;

  double_t first_point_height_km =
    pContext->ray.is_vertical ?
      pContext->ray.receiver_position.height :
      pContext->ray.slant.receiver_distance_km;

  double_t second_point_height_km =
    pContext->ray.is_vertical ?
      pContext->ray.satellite_position.height :
      pContext->ray.slant.satellite_distance_km;

  // P1 and P2 are between the two integration breakpoints
  // integrate straight between P1 and P2
  return Gauss_Kronrod_integrate_impl(
    &gauss_kronrod_context,
    pContext,
    first_point_height_km,
    second_point_height_km,
    pTEC);
}

static int32_t path_crosses_first_integration_point(
  NeQuickG_context_t* const pContext,
  double_t* const pTEC) {

  int32_t ret;
  // Path between P1 and P2 crosses the first integration breakpoint
  // integrate between P0 and the first breakpoint and from the
  // first breakpoint to P2.
  // P0 is used because P1 could be less than zero.

  double_t total_electron_content;

  gauss_kronrod_context_t gauss_kronrod_context;

  double_t first_integration_point_height_km =
    get_point_height(pContext, NEQUICK_G_JRC_INTEGRATION_FIRST_POINT_KM);

  {
    double_t zero_point_height_km = get_point_zero_height(pContext);

    gauss_kronrod_context.tolerance =
      NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_BELOW_FIRST_POINT;

     ret =
      Gauss_Kronrod_integrate_impl(
        &gauss_kronrod_context,
        pContext,
        zero_point_height_km,
        first_integration_point_height_km,
        &total_electron_content);
     if (ret != NEQUICK_OK) {
       return ret;
     }
  }

  {
    double_t second_point_height_km =
    pContext->ray.is_vertical ?
      pContext->ray.satellite_position.height :
      pContext->ray.slant.satellite_distance_km;

    gauss_kronrod_context.tolerance =
      NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_ABOVE_FIRST_POINT;

      double_t total_electron_content_;
      ret =
        Gauss_Kronrod_integrate_impl(
          &gauss_kronrod_context,
          pContext,
          first_integration_point_height_km,
          second_point_height_km,
          &total_electron_content_);
      if (ret != NEQUICK_OK) {
        return ret;
      }

      total_electron_content += total_electron_content_;
  }

  *pTEC = total_electron_content;
  return NEQUICK_OK;
}

static int32_t path_crosses_second_integration_point(
  NeQuickG_context_t* const pContext,
  double_t* const pTEC) {

  int32_t ret;

  // Path between P1 and P2 crosses the second integration
  // breakpoint so integrate between P1 and the second breakpoint
  // and from the second breakpoint to P2

  double_t total_electron_content;

  gauss_kronrod_context_t gauss_kronrod_context;
  gauss_kronrod_context.tolerance =
      NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_ABOVE_FIRST_POINT;

  double_t second_integration_point_height_km =
    get_point_height(pContext, NEQUICK_G_JRC_INTEGRATION_SECOND_POINT_KM);

  {
    double_t first_point_height_km =
    pContext->ray.is_vertical ?
      pContext->ray.receiver_position.height :
      pContext->ray.slant.receiver_distance_km;

     ret =
      Gauss_Kronrod_integrate_impl(
        &gauss_kronrod_context,
        pContext,
        first_point_height_km,
        second_integration_point_height_km,
        &total_electron_content);
      if (ret != NEQUICK_OK) {
        return ret;
      }
  }

  {
    double_t second_point_height_km =
    pContext->ray.is_vertical ?
      pContext->ray.satellite_position.height :
      pContext->ray.slant.satellite_distance_km;

      double_t total_electron_content_;
      ret =
        Gauss_Kronrod_integrate_impl(
          &gauss_kronrod_context,
          pContext,
          second_integration_point_height_km,
          second_point_height_km,
          &total_electron_content_);
      if (ret != NEQUICK_OK) {
        return ret;
      }
      total_electron_content += total_electron_content_;
   }

  *pTEC = total_electron_content;
  return NEQUICK_OK;
}

static int32_t both_above_second_integration_point(
  NeQuickG_context_t* const pContext,
  double_t* const pTEC) {

  // P1 and P2 are both above the second integration breakpoint so
  // integrate straight between P1 and P2

  double_t first_point_height_km =
    pContext->ray.is_vertical ?
      pContext->ray.receiver_position.height :
      pContext->ray.slant.receiver_distance_km;

  double_t second_point_height_km =
    pContext->ray.is_vertical ?
      pContext->ray.satellite_position.height :
      pContext->ray.slant.satellite_distance_km;

  gauss_kronrod_context_t gauss_kronrod_context;
  gauss_kronrod_context.tolerance =
   NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_ABOVE_FIRST_POINT;

  return Gauss_Kronrod_integrate_impl(
    &gauss_kronrod_context,
    pContext,
    first_point_height_km,
    second_point_height_km,
    pTEC);
}

static int32_t path_crosses_both_integration_points(
  NeQuickG_context_t* const pContext,
  double_t* const pTEC) {

  int32_t ret;

  // Path between P1 and P2 crosses both integration breakpoints
  // so integrate between P0 and the first breakpoint, between the
  // two breakpoints and from the second breakpoint to P2.
  // P0 is used because P1 could be less than zero.

  gauss_kronrod_context_t gauss_kronrod_context;
  double_t total_electron_content;

  double_t first_integration_point_height_km =
      get_point_height(pContext, NEQUICK_G_JRC_INTEGRATION_FIRST_POINT_KM);
  {
    double_t zero_point_height_km = get_point_zero_height(pContext);

    gauss_kronrod_context.tolerance =
      NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_BELOW_FIRST_POINT;

     ret =
      Gauss_Kronrod_integrate_impl(
        &gauss_kronrod_context,
        pContext,
        zero_point_height_km,
        first_integration_point_height_km,
        &total_electron_content);
     if (ret != NEQUICK_OK) {
        return ret;
     }
  }

  gauss_kronrod_context.tolerance =
      NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_ABOVE_FIRST_POINT;

  double_t second_integration_point_height_km =
   get_point_height(pContext, NEQUICK_G_JRC_INTEGRATION_SECOND_POINT_KM);

  {
    double_t total_electron_content_;
     ret =
      Gauss_Kronrod_integrate_impl(
        &gauss_kronrod_context,
        pContext,
        first_integration_point_height_km,
        second_integration_point_height_km,
        &total_electron_content_);
     if (ret != NEQUICK_OK) {
        return ret;
     }
     total_electron_content += total_electron_content_;
  }

  {
    double_t second_point_height_km =
    pContext->ray.is_vertical ?
      pContext->ray.satellite_position.height :
      pContext->ray.slant.satellite_distance_km;

    double_t total_electron_content_;
    ret =
      Gauss_Kronrod_integrate_impl(
        &gauss_kronrod_context,
        pContext,
        second_integration_point_height_km,
        second_point_height_km,
        &total_electron_content_);
    if (ret != NEQUICK_OK) {
        return ret;
    }
    total_electron_content += total_electron_content_;
  }

  *pTEC = total_electron_content;
  return NEQUICK_OK;
}

int32_t NeQuickG_integrate(
  NeQuickG_context_t* const pContext,
  double_t* const pTEC) {

  int32_t ret;
  *pTEC = 0.0;

  if (pContext->ray.is_vertical) {
    ret = ray_vertical_get_profile(pContext);
    if (ret != NEQUICK_OK) {
      return ret;
    }
  }

  if (IS_SATELLITE_BELOW_FIRST_POINT(pContext)) {
    return both_below_first_integration_point(pContext, pTEC);
  }

  if (IS_SATELLITE_BELOW_SECOND_POINT(pContext)) {
    return IS_RECEIVER_ABOVE_FIRST_POINT(pContext) ?
      between_integration_points(pContext, pTEC) :
      path_crosses_first_integration_point(pContext, pTEC);
  }

  if (IS_RECEIVER_ABOVE_SECOND_POINT(pContext)) {
    return both_above_second_integration_point(pContext, pTEC);
  }

  if (IS_RECEIVER_ABOVE_FIRST_POINT(pContext)) {
    return path_crosses_second_integration_point(pContext, pTEC);
  }

  return path_crosses_both_integration_points(pContext, pTEC);
}

#undef NEQUICK_G_JRC_RECURSION_LIMIT_MAX
#undef IS_SATELLITE_BELOW_FIRST_POINT
#undef IS_SATELLITE_BELOW_SECOND_POINT
#undef IS_RECEIVER_ABOVE_FIRST_POINT
#undef IS_RECEIVER_ABOVE_SECOND_POINT
#undef NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_BELOW_FIRST_POINT
#undef NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_ABOVE_FIRST_POINT
#undef NEQUICK_G_JRC_INTEGRATION_FIRST_POINT_KM
#undef NEQUICK_G_JRC_INTEGRATION_SECOND_POINT_KM
