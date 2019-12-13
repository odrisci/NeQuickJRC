/** TEC calculation along rays using Kronrod G<SUB>7</SUB>-K<SUB>15</SUB> adaptive quadrature method.
 *
 * See:
 *  - 2.5.8.3. Alternative computational efficient TEC integration method
 *  - F.2.6.1. NeQuick internal function NeqIntegrate
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_GAUSS_KRONROD_INTEGRATION_H
#define NEQUICK_G_JRC_GAUSS_KRONROD_INTEGRATION_H

#include <math.h>

#include "NeQuickG_JRC_context.h"

/** Kronrod G<SUB>7</SUB>-K<SUB>15</SUB> context */
typedef struct NeQuickG_gauss_kronrod_context_st {
  /** &epsilon; Integration tolerance for Kronrod G7-K15 integration method.
   * Increasing tolerance increases integration speed at the expense of accuracy.
   */
  double_t tolerance;
  /** Current level of integration recursion */
  size_t recursion_level;
  /** recursion limit */
  size_t recursion_max;
} gauss_kronrod_context_t;

/** Integration function for calculating TEC along rays using
 * Kronrod G<SUB>7</SUB>-K<SUB>15</SUB> adaptive quadrature method.
 * This method involves sampling values at 15 points and calculating
 * the integration from them.
 * At the same time it misses out half of the points to see what difference
 * it makes and therefore the likely error contained in the result,
 * before deciding whether to accept the result,
 * or to split the portion into two and try again in order to improve accuracy.
 * Note that this method is recursive but has appropriate safeguards in the form of the
 * recursion limit passed in from configuration.
 * See F.2.6.1.
 *
 * Default integration tolerance for Kronrod G7-K15 integration method below 1000 km is
 *  #NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_BELOW_FIRST_POINT
 *
 * Default integration tolerance for Kronrod G7-K15 integration method above 1000 km is
 *  #NEQUICK_G_JRC_INTEGRATION_KRONROD_TOLERANCE_ABOVE_FIRST_POINT
 *
 * @param[in, out] pGauss_kronrod Kronrod G<SUB>7</SUB>-K<SUB>15</SUB> context
 * @param[in, out] pContext NeQuick-G context
 * @param[in] point_1_height_km Height of point 1 in km
 * @param[in] point_2_height_km Height of point 2 in km
 * @param[out] pTEC TEC value, to get TECU divide by 10<SUP>13</SUP>
 *
 * @return on success NEQUICK_OK
 */
extern int32_t Gauss_Kronrod_integrate(
  gauss_kronrod_context_t* const pGauss_kronrod,
  NeQuickG_context_t* const pContext,
  const double_t point_1_height_km,
  const double_t point_2_height_km,
  double_t* const pTEC);

#endif // NEQUICK_G_JRC_GAUSS_KRONROD_INTEGRATION_H
