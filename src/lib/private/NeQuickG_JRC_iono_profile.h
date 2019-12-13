/** NeQuickG inospheric profile
 *
 * NeQuick is a profiler that makes use of three profile anchor points:
 *  - E layer peak (at a fixed height of 120 km),
 *  - F1 peak,
 *  - F2 peak<br>
 * where E, F1 and F2 are different layers of the ionosphere, as previously introduced.
 * To model the anchor points the model employs ionosonde parameters
 *  - foE, foF1, foF2 (critical frequencies) and
 *  - M(3000)F2 (transmission factor).
 *
 * The model is constituted by two major components:
 *  - The bottom side model for the height region below the peak of the F2-layer, which consists on the
 * superposition of three Epstein layers which peak at the anchor points.
 * This is a modified version of the Di Giovanni-Radicella model based on the ionospheric characteristics
 * foE, foF1, foF2 and M(3000)F2.
 * For foE derivation, a modified formulation of that due to John Titheridge is selected and foF1 is selected
 * as being equal to 1.4*foE during daytime and zero during night-time, respectively.
 * For the calculation of foF2 and M(3000)F2, the CCIR maps (provided as ccirXX.asc files) are used.
 *
 *  - The topside model for the height region above the F2-layer peak.
 * The topside of NeQuick is a semi-Epstein layer with a height dependent thickness parameter B
 * through a new parameter H. A correction factor adjusts vertical TEC values to take into account
 *  exosphere electron density in a simple manner.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_IONO_PROFILE_H
#define NEQUICK_G_JRC_IONO_PROFILE_H

#include "NeQuickG_JRC_iono_E_layer.h"
#include "NeQuickG_JRC_iono_F2_layer.h"
#include "NeQuickG_JRC_iono_profile_types.h"
#include "NeQuickG_JRC_MODIP.h"
#include "NeQuickG_JRC_solar_activity.h"
#include "NeQuickG_JRC_time.h"

/** From critical frequency, calculates the associated electron density.
 * See Eqs. 36, 48, 39 and 77, see F.2.9.4
 */
#define NEQUICK_G_JRC_FREQ_TO_NE_FACTOR_D (0.124)

/** From critical frequency, calculates the associated electron density, using
 *  N[m<SUP>-3</SUP>] = 0.124 * f[MHz]<SUP>2</SUP>
 *
 * @param[in] _crit_freq_MHz The peak plasma frequency for the layer
 * @return The calculated electron density (in 10<SUP>11</SUP> m<SUP>-3</SUP>)
 */
#define NEQUICK_G_JRC_IONO_GET_e_DENSITY(_crit_freq_MHz) \
  (NEQUICK_G_JRC_FREQ_TO_NE_FACTOR_D * _crit_freq_MHz * _crit_freq_MHz)

/** Get peak amplitude from the electron density Eqs. 90, 92, 93, 94 and 96
 *
 * @param[in] _e_density the electron density in 10<SUP>11</SUP> m<SUP>-3</SUP>
 * @return the peak amplitude in 10<SUP>11</SUP> m<SUP>-3</SUP>
 */
#define NEQUICKG_IONO_LAYER_GET_PEAK_AMPLITUDE(_e_density) \
 (_e_density * 4.0)

/** NeQuickG inospheric profile context */
typedef struct iono_profile_st {
  /** E layer context */
  E_layer_t E;
  /** F1 layer context */
  layer_t F1;
  /** F2 layer context */
  F2_layer_t F2;
} iono_profile_t;

#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && !defined(_doxygen)
#define iono_profile_init(pContext, pCCIR_folder) \
  iono_profile_init(pContext)
#endif

/** Initializes the ionospheric profile context
 * @param[out] pContext ionospheric profile context
 * @param[in] pCCIR_folder CCIR folder
 */
extern int32_t iono_profile_init(
  iono_profile_t* const pContext,
  const char* const pCCIR_folder);

#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && !defined(_doxygen)
#define iono_profile_close(pContext)
#else
/** Free resources allocated by the ionospheric profile context
 * @param[in, out] pContext ionospheric profile context
 */
extern void iono_profile_close(iono_profile_t* const pContext);
#endif

/** Get the ionospheric profile
 *
 * @param[out] pContext ionospheric profile context
 * @param[in] pTime indication of the time at which the profile is required
 * @param[in] pModip MODIP context
 * @param[in] pSolar_activity solar activity
 * @param[in] pCurrent_position location at which the profile is required
 */
extern int32_t iono_profile_get(
  iono_profile_t* const pContext,
  const NeQuickG_time_t* const pTime,
  modip_context_t* const pModip,
  const solar_activity_t* const pSolar_activity,
  const position_t * const pCurrent_position);

#ifdef FTR_UNIT_TEST
/** Unit test for ionospheric profile
 * @param[in, out] pContext ionospheric profile context
 */
extern void iono_profile_get_peak_amplitudes(
  iono_profile_t* const pContext);
#endif

#endif // NEQUICK_G_JRC_IONO_PROFILE_H
