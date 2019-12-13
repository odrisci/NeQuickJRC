/** NeQuickG ray.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_ray.h"

#include <math.h>

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_coordinates.h"
#include "NeQuickG_JRC_error.h"
#include "NeQuickG_JRC_geometry.h"
#include "NeQuickG_JRC_math_utils.h"

#define NEQUICKG_JRC_RAY_ZENITH_ANGLE_MAX_DEGREE (90.0)
#define NEQUICKG_JRC_RAY_IS_VERTICAL_PERIGEE_EPSILON_KM (0.1)
#define NEQUICKG_JRC_RAY_IS_AT_POLE_EPSILON (1.0e-10)

static angle_restricted_t get_longitude_delta_angle(
  const position_t* const pStation_position,
  const position_t* const pSatellite_position) {

  double_t longitude_delta_rad =
    (pSatellite_position->longitude.rad -
     pStation_position->longitude.rad);

  angle_restricted_t longitude_delta;
  angle_restricted_set(&longitude_delta, longitude_delta_rad);
  return longitude_delta;
}

static angle_restricted_t get_delta_angle(
  const position_t* const pStation_position,
  const position_t* const pSatellite_position,
  const angle_restricted_t* const pLongitude_delta) {

  angle_restricted_t delta;

  delta.cos =
    (pStation_position->latitude.sin *
     pSatellite_position->latitude.sin) +
    (pStation_position->latitude.cos *
      pSatellite_position->latitude.cos *
      pLongitude_delta->cos);

  delta.sin = get_sin_from_cos(delta.cos);

  return delta;
}

static angle_restricted_t get_sigma_angle(
  const position_t* const pStation_position,
  const position_t* const pSatellite_position,
  const angle_restricted_t* const pLongitude_delta,
  const angle_restricted_t* const pDelta) {

  angle_restricted_t sigma;
  sigma.sin =
    (pLongitude_delta->sin * pSatellite_position->latitude.cos) /
    pDelta->sin;

  sigma.cos =
    ((pSatellite_position->latitude.sin -
     (pDelta->cos * pStation_position->latitude.sin)) / pDelta->sin) /
    pStation_position->latitude.cos;

  return sigma;
}

static angle_t get_zenith_angle(
  const position_t* const pStation_position,
  const position_t* const pSatellite_position,
  const angle_restricted_t* const pDelta) {

  angle_t zenith;

  zenith.rad = atan2(pDelta->sin, pDelta->cos - (
    pStation_position->radius_km /
    pSatellite_position->radius_km));

  zenith.sin = sin(zenith.rad);
  zenith.degree = NEQUICKG_JRC_RAD_TO_DEGREE(zenith.rad);

  return zenith;
}

static angle_restricted_t get_delta_p_angle(const angle_t* const pZenith) {

  double_t delta_p_angle_rad =
    NEQUICKG_JRC_DEGREE_TO_RAD(90.0) - pZenith->rad;

  angle_restricted_t delta_p;
  angle_restricted_set(&delta_p, delta_p_angle_rad);
  return delta_p;
}

/** Calculate ray perigee latitude */
static angle_t get_ray_latitude(
  const position_t* const pStation_position,
  const angle_restricted_t* const pSigma,
  const angle_restricted_t* const pDelta_p) {

  angle_t latitude;
  latitude.sin =
    (pStation_position->latitude.sin * pDelta_p->cos) -
    (pStation_position->latitude.cos * pDelta_p->sin * pSigma->cos);

  angle_set_fom_sin(&latitude);
  return latitude;
}

/** Calculate ray perigee longitude */
static angle_t get_ray_longitude(
  const position_t* const pStation_position,
  const angle_restricted_t* const pSigma,
  const angle_restricted_t* const pDelta_p,
  const angle_t* const pRay_latitude) {

  angle_t longitude;

  double_t sin_lamp = (-pSigma->sin * pDelta_p->sin) / pRay_latitude->cos;

  double_t cos_lamp = (
    (pDelta_p->cos - (pStation_position->latitude.sin * pRay_latitude->sin)) /
     pStation_position->latitude.cos) / pRay_latitude->cos;

  longitude.rad =
    atan2(sin_lamp, cos_lamp) + pStation_position->longitude.rad;
  longitude.degree = NEQUICKG_JRC_RAD_TO_DEGREE(longitude.rad);

  return longitude;
}

static int32_t ray_get_radius(
  ray_context_t* const pRay,
  const position_t* const pStation_position,
  const angle_t* const pZenith_angle) {

  pRay->slant.perigee_radius_km = pStation_position->radius_km * pZenith_angle->sin;

  // See F.2.5.1. NeQuick internal function NeqGetRayProperties
  // if1 invalid ray, i.e. |pdZeta| > 90.0 and pstRay.dR < Re
  if ((fabs(pZenith_angle->rad) >
      NEQUICKG_JRC_DEGREE_TO_RAD(NEQUICKG_JRC_RAY_ZENITH_ANGLE_MAX_DEGREE))
      &&
      !position_is_exterior(pRay->slant.perigee_radius_km)) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_RAY,
      NEQUICK_ERROR_CODE_BAD_RAY,
      "the ray is not correct, angle(rad) = %lf, slant perigee radius (km) = %lf",
       pZenith_angle->rad, pRay->slant.perigee_radius_km);
  }

  pRay->is_vertical =
    (pRay->slant.perigee_radius_km <
      NEQUICKG_JRC_RAY_IS_VERTICAL_PERIGEE_EPSILON_KM);

  return NEQUICK_OK;
}

static bool is_at_poles(const ray_context_t* const pRay) {
  return (
    fabs(
      fabs(pRay->latitude.degree) - NEQUICK_G_JRC_LAT_MAX_VALUE_DEGREE) <
        NEQUICKG_JRC_RAY_IS_AT_POLE_EPSILON);
}

static void get_azimuth(ray_context_t* const pRay) {

  // Check if latitude of lower end point is +-90 degrees.
  // it would cause divide by zero error later on
  if (is_at_poles(pRay)) {
    pRay->slant.azimuth.sin = 0.0;
    pRay->slant.azimuth.cos =
      (pRay->latitude.degree > 0) ?
      -1.0 : 1.0;
  } else {
    // Calculate difference in longitude of ray end points
    double_t delta_rad =
      (pRay->satellite_position.longitude.rad -
       pRay->longitude.rad);

    // Calculate sine and cosine of angular distance between ends of ray (psi)
    angle_restricted_t psi_angle;
    {
      psi_angle.cos =
         (pRay->latitude.sin * pRay->satellite_position.latitude.sin) +
         pRay->latitude.cos * pRay->satellite_position.latitude.cos *
         cos(delta_rad);
      psi_angle.sin = get_sin_from_cos(psi_angle.cos);
    }

    pRay->slant.azimuth.sin =
      (pRay->satellite_position.latitude.cos * sin(delta_rad)) /
      psi_angle.sin;

    pRay->slant.azimuth.cos =
      (pRay->satellite_position.latitude.sin -
      (pRay->latitude.sin * psi_angle.cos)) /
        (psi_angle.sin * pRay->latitude.cos);
  }
}

/** Set the ray latitude and longitude to be the same as receiver
 *  Set satellite longitude to be exactly the same as station longitude
 */
static void on_vertical_ray(ray_context_t* const pRay) {

  pRay->satellite_position.longitude = pRay->receiver_position.longitude;

  pRay->latitude = pRay->receiver_position.latitude;
  pRay->longitude = pRay->receiver_position.longitude;
}

static int32_t on_slant_ray(ray_context_t* const pRay) {

  int32_t ret;

  angle_restricted_t longitude_delta =
    get_longitude_delta_angle(
      &pRay->receiver_position,
      &pRay->satellite_position);

  angle_restricted_t delta =
    get_delta_angle(
      &pRay->receiver_position,
      &pRay->satellite_position,
      &longitude_delta);

  // zenith angle of satellite, seen from receiver
  angle_t zenith_angle =
    get_zenith_angle(
      &pRay->receiver_position,
      &pRay->satellite_position,
      &delta);

  ret = ray_get_radius(pRay, &pRay->receiver_position, &zenith_angle);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  angle_restricted_t sigma =
    get_sigma_angle(
      &pRay->receiver_position,
      &pRay->satellite_position,
      &longitude_delta,
      &delta);

  angle_restricted_t delta_p = get_delta_p_angle(&zenith_angle);

  pRay->latitude = get_ray_latitude
    (&pRay->receiver_position, &sigma, &delta_p);

  pRay->longitude = get_ray_longitude
    (&pRay->receiver_position, &sigma, &delta_p, &pRay->latitude);

  if (!pRay->is_vertical) {
    {
      // Replace sine and cosine of receiver end point
      // using ray perigee latitude for receiver
      pRay->receiver_position.latitude.sin = pRay->latitude.sin;
      pRay->receiver_position.latitude.cos = pRay->latitude.cos;
    }
    get_azimuth(pRay);

    pRay->slant.receiver_distance_km =
    get_slant_distance(pRay, pRay->receiver_position.radius_km);

    pRay->slant.satellite_distance_km =
      get_slant_distance(pRay, pRay->satellite_position.radius_km);
  }
  return NEQUICK_OK;
}

double_t get_slant_distance(
  const ray_context_t* const pRay,
  double_t radius_km) {
  double_t temp =
    NeQuickG_square(radius_km) -
    NeQuickG_square(pRay->slant.perigee_radius_km);
  return sqrt(fabs(temp));
}

int32_t ray_get(
  ray_context_t* const pRay,
  const position_t * const pStation_position,
  const position_t * const pSatellite_position) {

  pRay->receiver_position = *pStation_position;
  pRay->satellite_position = *pSatellite_position;

  pRay->is_vertical =
    position_is_above(pStation_position, pSatellite_position);

  // Check if ray is vertical
  if (pRay->is_vertical) {

    on_vertical_ray(pRay);
    return NEQUICK_OK;

  } else {

    return on_slant_ray(pRay);

  }
}

#undef NEQUICKG_JRC_RAY_IS_AT_POLE_EPSILON
#undef NEQUICKG_JRC_RAY_IS_VERTICAL_PERIGEE_EPSILON_KM
#undef NEQUICKG_JRC_RAY_ZENITH_ANGLE_MAX_DEGREE
