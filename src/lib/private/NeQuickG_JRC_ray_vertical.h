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
#ifndef NEQUICK_G_JRC_RAY_VERTICAL_H
#define NEQUICK_G_JRC_RAY_VERTICAL_H

#include <math.h>

#include "NeQuickG_JRC_context.h"

/**  Recalculates ionosphere information for a vertical ray
 * - Adjusts position information for current position along ray
 * - Recalculates ionosphere information for new latitude and longitude (#iono_profile_get)
 *
 * @param[in, out] pContext NeQuick context
 *
 * @return on success NEQUICK_OK
 */
extern int32_t ray_vertical_get_profile(NeQuickG_context_t* const pContext);

/** This function returns electron density at a specified point along a vertical ray.
 * See F.2.7.1.<br>
 * - Gets the electron density with a call to #electron_density_get
 *
 * @param[in, out] pContext NeQuick context
 * @param[in] height_km Height of point in km
 *
 * @return Maximum N<SUB>e</SUB> in electrons/m<SUP>3</SUP>
 */
extern double_t ray_vertical_get_electron_density(
  NeQuickG_context_t* const pContext,
  const double_t height_km);

#endif // NEQUICK_G_JRC_RAY_VERTICAL_H
