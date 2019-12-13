/** NeQuickG slant ray electron density
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_ray_slant.h"

#include "NeQuickG_JRC_electron_density.h"
#include "NeQuickG_JRC_geometry.h"
#include "NeQuickG_JRC_math_utils.h"

static angle_restricted_t get_delta_angle(
  const ray_context_t* const pRay,
  const double_t height_km) {

  angle_restricted_t delta;

  double_t tangent_delta = height_km / pRay->slant.perigee_radius_km;
  delta.cos = 1.0 / sqrt(1.0 + NeQuickG_square(tangent_delta));
  delta.sin = tangent_delta * delta.cos;

  return delta;
}

static angle_t get_lambda_s_p_angle(
  const ray_context_t* const pRay,
  const angle_restricted_t* pDelta,
  const angle_t* pCurrent_position_latitude) {

  angle_t delta_lambda_s_p;

  delta_lambda_s_p.sin =
      pDelta->sin *
      pRay->slant.azimuth.sin *
      pRay->receiver_position.latitude.cos;

  delta_lambda_s_p.cos =
    pDelta->cos -
    (pRay->receiver_position.latitude.sin *
     pCurrent_position_latitude->sin);

  delta_lambda_s_p.rad =
    atan2(delta_lambda_s_p.sin,
          delta_lambda_s_p.cos);

  return delta_lambda_s_p;
}

static position_t get_current_position(
  const ray_context_t* const pRay,
  const double_t height_km) {
  position_t current_position;

  current_position.radius_km =
    sqrt(NeQuickG_square(height_km) + NeQuickG_square(pRay->slant.perigee_radius_km));

  current_position.height =
    get_height_from_radius(current_position.radius_km);

  angle_restricted_t delta = get_delta_angle(pRay, height_km);

  // latitude
  {
    current_position.latitude.sin =
      (pRay->receiver_position.latitude.sin * delta.cos) +
      (pRay->receiver_position.latitude.cos * delta.sin * pRay->slant.azimuth.cos);

    angle_set_fom_sin(&current_position.latitude);
  }

  // longitude
  {
    angle_t delta_lambda_s_p =
      get_lambda_s_p_angle(
        pRay,
        &delta,
        &current_position.latitude);

    angle_set_rad(
      &current_position.longitude,
      delta_lambda_s_p.rad + pRay->longitude.rad);
  }

  return current_position;
}

int32_t ray_slant_get_electron_density(
  NeQuickG_context_t* const pContext,
  const double_t height_km,
  double_t* const pElectron_density) {

  // Adjust position information for current position along ray
  position_t current_position =
    get_current_position(&pContext->ray, height_km);

  // recalculate ionosphere information now that the latitude and longitude have
  // changed
  int32_t ret = iono_profile_get(
    &pContext->profile,
    &pContext->input_data.time,
    &pContext->modip,
    &pContext->solar_activity,
    &current_position);

  if (ret != NEQUICK_OK) {
    return ret;
  }

  *pElectron_density = electron_density_get(
    &pContext->profile,
    current_position.height);

  return NEQUICK_OK;
}
