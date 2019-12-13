/** NeQuickG E layer profile
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_IONO_E_LAYER_H
#define NEQUICK_G_JRC_IONO_E_LAYER_H

#include "NeQuickG_JRC_coordinates.h"
#include "NeQuickG_JRC_iono_profile_types.h"
#include "NeQuickG_JRC_solar_activity.h"
#include "NeQuickG_JRC_time.h"

/** E layer critical frequency is non-zero.
 * See F.2.9.5. NeQuick internal function NeqCalcF2PeakHeight
 */
#define NEQUICK_G_JRC_IONO_E_LAYER_CRITICAL_FREQUENCY_NEGLIGIBLE (1.0e-30)

/** E layer critical frequency is non-zero.
 * See F.2.9.5. NeQuick internal function NeqCalcF2PeakHeight
 *
 *@param _crit_freq_MHz E layer critical frequency in MHz
 */
#define NEQUICK_G_JRC_IONO_E_LAYER_INFLUENCES_F2_PEAK_HEIGHT(_crit_freq_MHz) \
(_crit_freq_MHz >=  \
  NEQUICK_G_JRC_IONO_E_LAYER_CRITICAL_FREQUENCY_NEGLIGIBLE)

/** Constant used to calculate E layer amplitude. Eq 98 */
#define NEQUICK_G_JRC_IONO_E_LAYER_AMPLITUDE_CONSTANT_1 (5.0e-2)
/** Constant used to calculate E layer amplitude. Eq 98 */
#define NEQUICK_G_JRC_IONO_E_LAYER_AMPLITUDE_WITH_TRANSITION_REGION (60.0)
/** Constant used to calculate E layer amplitude. Eq 98 */
#define NEQUICK_G_JRC_IONO_E_LAYER_AMPLITUDE_CONSTANT_2 (5.0e-3)

/** E layer profile context */
typedef struct E_layer_st {
  /** Sinus and cosinus of the solar declination &delta;*/
  angle_restricted_t solar_declination;
  /** generic ionospheric layer profile context */
  layer_t layer;
  /** is the soldar declination field valid? */
  bool is_solar_declination_valid;
} E_layer_t;

/** Initialize E layer profile context
 * @param[out] pE E layer profile context
 */
extern void E_layer_init(E_layer_t* const pE);

/** Get E layer critical frequency f0E in MHz, see 2.5.5.1.
 *
 * @param[in, out] pE E layer profile context
 * @param[in] pTime indication of the time at which the f0E is required
 * @param[in] pSolar_activity solar activity
 * @param[in] pPosition location at which the f0E is required
 */
extern void E_layer_get_critical_freq_MHz(
  E_layer_t* const pE,
  const NeQuickG_time_t* const pTime,
  const solar_activity_t* const pSolar_activity,
  const position_t* const pPosition);

/** Get E layer maximum density height hmE [km] see 2.5.5.4.
 * @param[in, out] pE E layer profile context
 */
extern void E_layer_get_peak_height(E_layer_t* const pE);

/** Get E layer peak thickness see 2.5.5.7, Eq 88 and Eq. 89
 * @param[in, out] pE E layer profile context
 * @param[in] F1_peak_thickness_bottom_km F1 layer thickness bottom parameter
 */
extern void E_layer_get_peak_thickness(
  E_layer_t* const pE,
  const double_t F1_peak_thickness_bottom_km);

#endif // NEQUICK_G_JRC_IONO_E_LAYER_H
