/** NeQuickG solar auxiliary routines
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_SOLAR_H
#define NEQUICK_G_JRC_SOLAR_H

#include <math.h>

#include "NeQuickG_JRC_coordinates.h"
#include "NeQuickG_JRC_geometry.h"
#include "NeQuickG_JRC_time.h"

/** Get solar declination &delta;<SUB>sun</SUB>, see 2.5.4.6.
 * The Sun's declination at any given moment is calculated by:
 * sin(declination_sun) = sin(-23.44)sin(ecliptic_long)
 * where ecliptic_long is the ecliptic longitude.
 *
 * @param[in] pTime month and Universal Time
 * @return sin(&delta;<SUB>sun</SUB>), cos(&delta;<SUB>sun</SUB>)
 */
extern angle_restricted_t
solar_get_declination(const NeQuickG_time_t* const pTime);

/** Get solar effective zenith angle in degrees, see 2.5.4.8, 2.5.4.7
 *
 * @param[in] pPosition latitude in degrees
 * @param[in] pTime local time in hours
 * @param[in] pSolar_declination sin(&delta;<SUB>sun</SUB>), cos(&delta;<SUB>sun</SUB>)
 * @return solar effective zenith angle &chi;<SUB>eff</SUB> in degrees
 */
extern double_t solar_get_effective_zenith_angle(
  const position_t* const pPosition,
  const NeQuickG_time_t * const pTime,
  const angle_restricted_t* const pSolar_declination);

/** Get solar longitude in radians Eq. 49
 * @param[in] pTime Universal Time
 * @return solar longitude in radians
 */
extern double_t solar_get_longitude(
  const NeQuickG_time_t* const pTime);

#endif // NEQUICK_G_JRC_SOLAR_H
