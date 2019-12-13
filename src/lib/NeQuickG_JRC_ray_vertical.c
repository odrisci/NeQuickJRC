/** NeQuickG slant vertical ray
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_ray_vertical.h"

#include "NeQuickG_JRC_electron_density.h"

static position_t get_current_position(const ray_context_t* const pRay) {
  position_t current_position;

  current_position.latitude = pRay->receiver_position.latitude;
  current_position.longitude = pRay->receiver_position.longitude;

  // not used for profile...
  current_position.height = pRay->satellite_position.height;
  current_position.height_units = pRay->satellite_position.height_units;

  return current_position;
}

int32_t ray_vertical_get_profile(NeQuickG_context_t* const pContext) {

  position_t current_position = get_current_position(&pContext->ray);

  return iono_profile_get(
    &pContext->profile,
    &pContext->input_data.time,
    &pContext->modip,
    &pContext->solar_activity,
    &current_position);
}

double_t ray_vertical_get_electron_density(
  NeQuickG_context_t* const pContext,
  const double_t height_km) {
  return electron_density_get(&pContext->profile, height_km);
}
