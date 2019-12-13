/** NeQuickG F1 layer profile
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_IONO_F1_LAYER_H
#define NEQUICK_G_JRC_IONO_F1_LAYER_H

#include <math.h>

#include "NeQuickG_JRC_iono_profile_types.h"

/** Constant used to calculate F1 layer amplitude. Eq 95 */
#define NEQUICK_G_JRC_IONO_F1_LAYER_AMPLITUDE_CONSTANT (0.8)

/** F1 peak is present, see Eq. 01
 * See F.2.9.5. NeQuick internal function NeqCalcF2PeakHeight
 *
 *@param[in] _F1 F1 layer profile context
 */
#define NEQUICK_G_F1_LAYER_IS_PEAK_PRESENT(_F1) \
 (_F1.critical_frequency_MHz >= 0.5)

/**  Get F1 layer critical frequency f0F1 in MHz, see 2.5.5.1 and F.2.9.1
 *
 * the initial formulation of the model was:
 *  foF1 is taken to be 1.4*foE
 *  (Titheridge's Formula, Leitinger et al., 1999) during
 *  daytime and is set zero during nighttime.
 * but in some cases strong gradients and strange structures appeared
 * in E and F1-layer heights.
 * So the formulation of f0F1 was changed to foF1 =
 *  - 0            if foE < 2
 *  - 1.4f0E       if foE >= 2 and 1.4*foE <= 0.85*foF2
 *  - 0.85*1.4foE  if 1.4*foE > 0.85*foF2
 *
 * if foF1 < 10<SUP>-6</SUP> then foF1 = 0
 *
 * the original formulation was 1.4*f0E for all daytime conditions,
 * now 0.85*1.4foE if 1.4*foE > 0.85*foF2.
 *
 * See Leitinger et al.
 * "An improved bottomside for the ionospheric electron density model NeQuick",
 * Annals of Geophysics, 48(3), p. 525-534, 2005.
 *
 * The implementation takes into account the need to ensure continuity
 * and derivability over the full range of f0F1,
 * by making use of NeQuickG_func_join
 *
 * @param[in, out] pF1 F1 layer profile context
 * @param[in] critical_freq_E_layer_MHz E layer critical frequency foE (MHz)
 * @param[in] critical_freq_F2_layer_MHz F2 layer critical frequency foF2 (MHz)
 *
 * @return The F1 layer critical frequency foF1 (MHz)
 *
 */
extern void F1_layer_get_critical_freq_MHz(
  layer_t* const pF1,
  const double_t critical_freq_E_layer_MHz,
  const double_t critical_freq_F2_layer_MHz);

/** Get F1 layer maximum density height hmF1 [km] see 2.5.5.5.
 *
 * @param[in, out] pF1 F1 layer profile context
 * @param[in] E_peak_height_km E layer maximum density height
 * @param[in] F2_peak_height_km F2 layer maximum density height
 */
extern void F1_layer_get_peak_height(
  layer_t* const pF1,
  const double_t E_peak_height_km,
  const double_t F2_peak_height_km);

/** Get F1 layer peak thickness see 2.5.5.7, Eq 86 and Eq. 87
 *
 * @param[in, out] pF1 F1 layer profile context
 * @param[in] E_peak_height_km E layer maximum density height
 * @param[in] F2_peak_height_km F2 layer maximum density height
 */
extern void F1_layer_get_peak_thickness(
  layer_t* const pF1,
  const double_t E_peak_height_km,
  const double_t F2_peak_height_km);

#endif // NEQUICK_G_JRC_IONO_F1_LAYER_H
