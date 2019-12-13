/** NeQuick G JRC position coordinates
 *
 *  The model uses WGS-84 ellipsoidal coordinates:
 *  - geodetic latitude
 *  - geodetic longitude
 *  - ellipsoidal height
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_COORDINATES_H
#define NEQUICK_G_JRC_COORDINATES_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "NeQuickG_JRC_geometry.h"

/** unit of longitude used in #position_t is meters. */
#define NEQUICK_G_JRC_HEIGHT_UNITS_METERS (0)
/** unit of longitude used in #position_t is kilometers. */
#define NEQUICK_G_JRC_HEIGHT_UNITS_KM (1)

/** Minimum latitude in degrees. */
#define NEQUICK_G_JRC_LAT_MIN_VALUE_DEGREE (-90.0)
/** Maximum latitude in degrees. */
#define NEQUICK_G_JRC_LAT_MAX_VALUE_DEGREE (90.0)
/** Maximum longitude in degrees */
#define NEQUICK_G_JRC_LONG_MAX_VALUE_DEGREE  (180.0)

/** WGS-84 ellipsoidal coordinates. */
typedef struct NeQuickG_position_st {
  /** &lambda; geodetic longitude
   * A value between 0.0 and 360.0
   */
  angle_t longitude;
  /**  &phi; geodetic latitude*/
  angle_t latitude;
  /** ellipsoidal height*/
  double_t height;
  /** units for height:
   * #NEQUICK_G_JRC_HEIGHT_UNITS_METERS or
   * #NEQUICK_G_JRC_HEIGHT_UNITS_KM
   */
  uint8_t height_units;
  /** radius in km longitude:
   * radius = #NEQUICK_G_EARTH_RADIUS_IN_KM + #position_t.height
   */
  double_t radius_km;
  /** geodetic longitude original value
   *  not restricted to the range in #position_t.longitude
   *  ESA outputs this value so we need to preserve it
   *  to compare results
   */
  double_t longitude_org;
} position_t;

/** Sets the position in the structure using the ranges and units required
 * by the library
 *
 * @param[out] pPosition position coordinates
 * @param[in] longitude_degree geodetic longitude in degrees
 * @param[in] latitude_degree geodetic latitude in degrees
 *  must be in range [#NEQUICK_G_JRC_LAT_MIN_VALUE_DEGREE, #NEQUICK_G_JRC_LAT_MAX_VALUE_DEGREE]
 * @param[in] height ellipsoidal height
 * @param[in] height_units units for ellipsoidal height:
 *  #NEQUICK_G_JRC_HEIGHT_UNITS_METERS or
 *  #NEQUICK_G_JRC_HEIGHT_UNITS_KM
 * @return on success NEQUICK_OK
 */
extern int32_t position_set(
  position_t* const pPosition,
  double_t longitude_degree,
  double_t latitude_degree,
  double_t height, uint_fast8_t height_units);

/** Sets the position in kilometers
 * @param[in, out] pPosition position coordinates
 */
extern void position_to_km(position_t* const pPosition);

/** Sets the position in meters
 * @param[in, out] pPosition position coordinates
 */
extern void position_to_meters(position_t* const pPosition);

/** Checks if the path between two points (e.g. a satellite and a receiver) is vertical<br>
 * F.2.5.2.<br>
 *  if<SUB>1</SUB> point 2 is directly above point 1, |pstP2.dLat - pstP1.dLat| < 10<SUP>-5</SUP> and
 *   |pstP2.dLng - pstP1.dLng| < 10<SUP>-5</SUP>
 * @param[in] p1 Point 1
 * @param[in] p2 Point 2
 * @return true if path is vertical
 */
extern bool position_is_above(
  const position_t* const p1,
  const position_t* const p2);

/** Checks if the height is greater than the radius of the earth #NEQUICK_G_EARTH_RADIUS_IN_KM
 * @param[in] height_km ellipsoidal height in kilometers
 * @return true if the height is greater than the radius of the earth
 */
extern bool position_is_exterior(const double_t height_km);

/** Get the radius from the height i.e. height + NEQUICK_G_EARTH_RADIUS_IN_KM
 * @param[in] height_km ellipsoidal height in kilometers
 * @return radius in km
 */
extern double_t get_radius_from_height(const double_t height_km);

/** Get the height from the radius i.e. radius - NEQUICK_G_EARTH_RADIUS_IN_KM
 * @param[in] radius_km radius in kilometers
 * @return height in km
 */
extern double_t get_height_from_radius(const double_t radius_km);

#endif // NEQUICK_G_JRC_COORDINATES_H
