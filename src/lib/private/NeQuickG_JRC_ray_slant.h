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
#ifndef NEQUICK_G_JRC_RAY_SLANT_H
#define NEQUICK_G_JRC_RAY_SLANT_H

#include <math.h>

#include "NeQuickG_JRC_context.h"

/** This function returns electron density at the specified point along a slanted ray.
 * See F.2.8.1.<br>
 * - Adjusts position information for current position along ray
 * - Recalculates ionosphere information for new latitude and longitude (#iono_profile_get)
 * - Gets the electron density with a call to #electron_density_get
 *
 * @param[in, out] pContext NeQuick context
 * @param[in] height_km Distance of the point along the ray in km
 * @param[out] pElectron_density Maximum N<SUB>e</SUB> in electrons/m<SUP>3</SUP>
 *
 * @return on success NEQUICK_OK
 */
extern int32_t ray_slant_get_electron_density(
  NeQuickG_context_t* const pContext,
  const double_t height_km,
  double_t* const pElectron_density);

#endif // NEQUICK_G_JRC_RAY_SLANT_H
