/** NeQuick G JRC electron density
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_ELECTRON_DENSITY_H
#define NEQUICK_G_JRC_ELECTRON_DENSITY_H

#include <math.h>

#include "NeQuickG_JRC_iono_profile.h"

/** Returns the electron density given the current height using the calculated ionospheric profile
 *
 * See 2.5.6 Electron density computation:<br>
 * Two different modules have to be used accordingly to the height considered.<br>
 * In particular:<br>
 * if h <= hmF2<br>
 *   - the bottomside electron density has to be computed using #bottom_side<br>
 * if h < hmF2
 *   - the topside electron density has to be computed using #top_side<br>
 *
 * @param[in, out] pProfile ionospheric profile
 * @param[in] height_km height in km
 * @return electron density in m<SUP>-3</SUP>
 */
extern double_t electron_density_get(
  iono_profile_t* const pProfile,
  const double_t height_km);

#endif // NEQUICK_G_JRC_ELECTRON_DENSITY_H
