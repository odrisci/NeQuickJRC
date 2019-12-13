/** NeQuickG inospheric profile types
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_IONO_PROFILE_TYPES_H
#define NEQUICK_G_JRC_IONO_PROFILE_TYPES_H

#include <math.h>
#include <stdbool.h>

#include "NeQuickG_JRC_coordinates.h"

/** Thickness parameters: top and bottom */
typedef struct thickness_st {
  /** top [km] */
  double_t top_km;
  /** bottom [km] */
  double_t bottom_km;
} thickness_t;

/** Peak layer */
typedef struct peak_st {
  /** Amplitude for the Epstein function [in 10<SUP>11</SUP> m<SUP>-3</SUP>] */
  double_t amplitude;
  /** Height of the electron density peak [km]*/
  double_t height_km;
  /** Thickness parameters */
  thickness_t thickness;
  /** electron density [in 10<SUP>11</SUP> m<SUP>-3</SUP>] */
  double_t electron_density;
} peak_t;

/** Generic ionospheric layer profile context */
typedef struct layer_st {
  /** Critical frequency [MHz] */
  double_t critical_frequency_MHz;
  /** Peak layer */
  peak_t peak;
} layer_t;

#endif // NEQUICK_G_JRC_IONO_PROFILE_TYPES_H
