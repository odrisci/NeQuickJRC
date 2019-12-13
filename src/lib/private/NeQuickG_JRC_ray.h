/** NeQuickG ray types and routines<br>
 *
 * The ray is straight line passing through receiver and satellite.<br>
 * To compute the slant TEC along a straight line between a point P1 and a point P2,
 * the NeQuick electron density N has to be evaluated on a point P defined by the coordinates
 * {h, &phi;, &lambda;} along the ray-path.
 *
 * For computational efficiency, a ray is vertical if the latitude and the longitude of P1 and P2 are close to each other
 * (if ray perigee radius r<SUB>p</SUB> < 0.1 km), otherwise is a slant ray
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_RAY_H
#define NEQUICK_G_JRC_RAY_H

#include <stdbool.h>

#include "NeQuickG_JRC_coordinates.h"

/** Slant ray
 * perigeee is the point on ray closest to centre of Earth.
 */
typedef struct NeQuickG_ray_slant_info_st {
  /** azimuth &sigma; of satellite as seen from ray-perigee P<SUB>p</SUB> */
  angle_restricted_t azimuth;
  /** ray perigee radius r<SUB>p</SUB> in km */
  double_t perigee_radius_km;
  /** Distance from receiver to ray perigee in km */
  double_t receiver_distance_km;
  /** Distance from satellite to ray perigee in km */
  double_t satellite_distance_km;
} NeQuickG_ray_slant_info_t;

/** Ray context */
typedef struct NeQuickG_ray_context_st {
  /** Receiver position */
  position_t receiver_position;
  /** Satellite position */
  position_t satellite_position;
  /** ray perigee longitude &lambda;<SUB>p</SUB>*/
  angle_t longitude;
  /** ray perigee latitude &phi;<SUB>p</SUB> */
  angle_t latitude;
  /** is the ray vertical? */
  bool is_vertical;
  /** slant ray context */
  NeQuickG_ray_slant_info_t slant;
} ray_context_t;

/** This function obtains the properties of the ray and checks if it is a valid ray.
 * See F.2.5.2.
 *
 * @param[in, out] pContext ray context
 * @param[in] pStation_position Positional information for receiver
 * @param[in] pSatellite_position Positional information for satellite
 *
 * @return on success NEQUICK_OK
 */
extern int32_t ray_get(
  ray_context_t* const pContext,
  const position_t * const pStation_position,
  const position_t * const pSatellite_position);

/** Retusn slant distance from a point to the perigee radius.
 * See F.2.3.1.
 *
 * @param[in, out] pContext ray context
 * @param[in] radius_km point radius in km
 *
 * @return slant distance in km
 */
extern double_t get_slant_distance(
  const ray_context_t* const pContext,
  double_t radius_km);

#endif // NEQUICK_G_JRC_RAY_H
