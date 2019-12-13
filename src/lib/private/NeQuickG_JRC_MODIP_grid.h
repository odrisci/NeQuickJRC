/** Modified Dip Latitude grid.<br>
 *
 * A table grid of MODIP values versus geographical location is provided
 *  together with the NeQuick G mode.<br>
 *
 * The MODIP grid allows to estimate MODIP [deg] at a given point
 * (latitude, longitude) by interpolation.
 * This grid is used within NeQuick to compute MODIP at a given point
 * interpolating with the 4x4-points grid surrounding the desired element.
 * The MODIP grid file contains the values of MODIP (expressed in degrees) on a geocentric
 * grid from 90&deg;S to 90&deg;N with a 5-degree step in latitude and from
 * 180&deg;W to 180&deg;E with a 10-degree in longitude.
 * For computational purposes, it is wrapped around including as first column
 * the values of 170&deg;E (i.e. 190&deg;W) and in the last column the values of 170&deg;W
 * (i.e. 190&deg;E), also there is an extra first and last rows phased 180 degrees
 * in longitude to wrap the poles around.<BR>
 *
 * The MODIP table grid file used by NeQuick is calculated using the IGRF model
 * for the Earth’s magnetic field. Strictly speaking, the coordinates derived
 * from such a model are defined as Corrected Geomagnetic Coordinates (CGM):
 * coordinates relative to Earth’s magnetic field when this is not
 * approximated from a dipole but instead calculated from a model, usually the
 * International Geomagnetic Reference Field (IGRF) model.
 * Typical geomagnetic coordinates are those derived from a dipole
 * approximation of the Earth’s magnetic field. In this sense, parameters that
 * depend on dipole latitude, such as the magnetic dip I or MODIP,
 * were defined based on geomagnetic coordinates and not CGM.
 * Thereby, when NeQuick applies equations for I and MODIP using CGM
 * coordinates instead of the dipole geomagnetic coordinates, those concepts
 * should be referred as Corrected Magnetic Dip (I') and
 * Corrected Modified Dip Latitude (MODIP') respectively.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_MODIP_GRID_H
#define NEQUICK_G_JRC_MODIP_GRID_H

#include <math.h>
#include <stdbool.h>

#include "NeQuickG_JRC_coordinates.h"

/** Number of grid points for the latitude */
#define NEQUICK_G_JRC_MODIP_GRID_LAT_POINTS_COUNT (39)

/** Number of grid points for the longitude */
#define NEQUICK_G_JRC_MODIP_GRID_LONG_POINTS_COUNT (39)

/** Size of longitude step in Modip grid. */
#define NEQUICK_G_JRC_MODIP_GRID_LONG_STEP_DEGREE (10)

/** Size of latitude step in Modip grid. */
#define NEQUICK_G_JRC_MODIP_GRID_LAT_STEP_DEGREE (5)

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
/** Modip grid loaded from file */
typedef struct modip_grid_st {
  /** array of pointsL the grid */
  double_t corrected_modip_degree \
    [NEQUICK_G_JRC_MODIP_GRID_LAT_POINTS_COUNT] \
    [NEQUICK_G_JRC_MODIP_GRID_LONG_POINTS_COUNT];
  /** loaded from file i.e. is valid? */
  bool is_loaded;
} modip_grid_t;

/** Modip grid initialisation.
 * The grid is loaded from file into the structure
 * @param[in] pModip_file path to the modip grid file
 * @param[out] pGrid modip grid loaded from file
 */
extern int32_t modip_grid_init(
  const char* const pModip_file,
  modip_grid_t* const pGrid);
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && !defined(_doxygen)
#define modip_grid_interpolate(pGrid, pPosition, pHandle) \
  modip_grid_interpolate(pPosition, pHandle)
#endif

/** Modip grid interpolation, See 2.5.4.3.
 *
 * @param[in] pGrid modip grid context
 * @param[in] pPosition location at which the modip is required
 * @param[out] pModip_degree modip in degrees
 */
extern void modip_grid_interpolate(
  const modip_grid_t* const pGrid,
  const position_t* const pPosition,
  double_t *pModip_degree);

#endif // NEQUICK_G_JRC_MODIP_GRID_H
