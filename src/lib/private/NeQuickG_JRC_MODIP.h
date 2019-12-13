/** Modified Dip Latitude.<br>
 *
 * The Effective Ionisation Level, Az, is determined from the MODIP using the three
 * ionospheric coefficients.<br>
 *
 * Depending on the severity and general characterisation of ionospheric
 * effects, five regions are defined based on their MODIP
 * (related to geomagnetic field):<br>
 *   - Region 1  60deg <  MODIP <= 90deg
 *   - Region 2  30deg <  MODIP <= 60deg
 *   - Region 3 -30deg <= MODIP <= 30deg
 *   - Region 4 -60deg <= MODIP < -30deg
 *   - Region 5 -90deg <= MODIP < -60deg<br>
 * A table grid of MODIP values versus geographical location is provided
 * together with the NeQuick G model
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_MODIP_H
#define NEQUICK_G_JRC_MODIP_H

#include <math.h>
#include <stdbool.h>

#include "NeQuickG_JRC_coordinates.h"
#include "NeQuickG_JRC_MODIP_grid.h"

/** Modip context */
typedef struct modip_context_st {
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
  /** modip grid context */
  modip_grid_t grid;
#endif //!FTR_MODIP_CCIR_AS_CONSTANTS
  /** calculated modip in degrees */
  double_t modip_degree;
} modip_context_t;

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
/** Modip context initialization
 *
 * @param[in] pContext modip context
 * @param[in] pModip_file modip grid file
 * @return on success NEQUICK_OK
 */
extern int32_t modip_init(
  modip_context_t* const pContext,
  const char* const pModip_file);
#endif //!FTR_MODIP_CCIR_AS_CONSTANTS

/** Get receiver modip by grid interpolation, see 2.5.4.3.
 *
 * @param[in, out] pContext modip context
 * @param[in] pPosition location at which the modip is required
 */
extern void modip_get(
  modip_context_t* const pContext,
  const position_t* const pPosition);

#endif // NEQUICK_G_JRC_MODIP_H
