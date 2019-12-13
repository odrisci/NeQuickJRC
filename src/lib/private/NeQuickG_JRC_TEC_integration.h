/** NeQuickG Total Electron Content (TEC) integration routine<br>
 *
 * the Total Electron Content (TEC) is defined as the electron density in a cross-section of 1m<SUP>2</SUP>,
 * integrated along a slant (or vertical) path between two points (e.g. a satellite and a receiver);
 * it is expressed in TEC units (TECU) where 1 TECU equals 10<SUP>16</SUP> electrons/m<SUP>2</SUP>
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_TEC_INTEGRATION_H
#define NEQUICK_G_JRC_TEC_INTEGRATION_H

#include <math.h>

#include "NeQuickG_JRC_context.h"

/** This function checks whether the ray is vertical or slant,
 * and where the start and end points are located in regards to
 * the different integration points, before passing the appropriate
 * information to the integration function. See F.2.3.3.<br>
 *
 * For each integration point in the path the routine gets the
 * electron density with the effective inosization level, time of
 * day and month. Then, integrates STEC for all points in the path.<br>
 *
 * The STEC integration routine used in this implementation is the method based on
 * Kronrod G<SUB>7</SUB>-K<SUB>15</SUB> adaptive quadrature with the default tolerances defined in the specification.
 * See section F.2.6.<br>
 * The maximum recursion level for the integration routine is #NEQUICK_G_JRC_RECURSION_LIMIT_MAX<br>
 *
 * @param[in, out] pContext Nequick context
 * @param[out] pTEC Total electron content in km/m<SUP>3</SUP>
 * @return on success NEQUICK_OK
 */
extern int32_t NeQuickG_integrate(
  NeQuickG_context_t* const pContext,
  double_t* const pTEC);

#endif // NEQUICK_G_JRC_TEC_INTEGRATION_H
