/** NeQuickG F2 layer profile
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_IONO_F2_LAYER_H
#define NEQUICK_G_JRC_IONO_F2_LAYER_H

#include <math.h>

#include "NeQuickG_JRC_iono_F2_layer_fourier_coefficients.h"
#include "NeQuickG_JRC_input_data.h"
#include "NeQuickG_JRC_iono_profile_types.h"
#include "NeQuickG_JRC_MODIP.h"
#include "NeQuickG_JRC_solar_activity.h"

/** F2 layer profile context */
typedef struct F2_layer_st {
  /** F2 layer fourier coefficients context */
  F2_layer_fourier_coeff_context_t coef;
  /** generic ionospheric layer profile context */
  layer_t layer;
  /** Transmission factor M(3000)F2
   * The ratio of the maximum usable frequency at
   *  a distance of 3000 km to the F2 layer critical frequency.
   */
  double_t trans_factor;
} F2_layer_t;

#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && !defined(_doxygen)
#define F2_layer_init(pContext, pCCIR_folder) \
  F2_layer_init(pContext)
#endif

/** Initialize F2 layer profile context
 * @param[out] pContext F2 layer profile context
 * @param[in] pCCIR_folder CCIR folder
 */
extern int32_t F2_layer_init(
  F2_layer_t* const pContext,
  const char* const pCCIR_folder);

#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && !defined(_doxygen)
#define F2_layer_close(pContext)
#else
/** Free resources allocated by the F2 layer profile context
 * @param[in, out] pContext F2 layer profile context
 */
extern void F2_layer_close(
  F2_layer_t* const pContext);
#endif

/** Get F2 layer critical frequency f0F2 in MHz and Transmission factor M(3000)F2, see 2.5.5.3.
 * For the calculation of foF2 and M(3000)F2, the CCIR maps are used.
 *
 * @param[in, out] pContext F2 layer profile context
 * @param[in] pTime indication of the time at which the f0F2 is required
 * @param[in] pModip MODIP context
 * @param[in] pSolar_activity solar activity
 * @param[in] pCurrent_position location at which the f0F2 is required
 */
extern int32_t F2_layer_get_critical_freq_MHz(
  F2_layer_t* const pContext,
  const NeQuickG_time_t* const pTime,
  const modip_context_t* const pModip,
  const solar_activity_t* const pSolar_activity,
  const position_t* const pCurrent_position);

/** Get F2 layer maximum density height hmF2 [km] from
 * foE, foF2 and M3000. It is based on the method of
 * Dudeney, 1983, J. Atmos. Terr.Phys., 45, 629-640.
 * but modified such that the ratio foF2/foE is clipped at 1.75.
 * Note that the clipping is soft, the 1st derivative is continuous
 * but note the clipped value can be slightly below 1.75 at the join
 * (but note analysis indicate > 1.73).
 * Also, Dudeney uses a figure of 1470 rather than 1490 in the
 * numerator of hmF2 and a figure of 1.296 rather than
 * 1.2967 in the denominator of MF.
 * See 2.5.5.6. hmF2
 *
 * @param[in, out] pContext F2 layer profile context
 * @param[in] critical_freq_E_layer_MHz E layer critical frequency f0E in MHz
 */
extern void F2_layer_get_peak_height(
  F2_layer_t* const pContext,
  const double_t critical_freq_E_layer_MHz);

/** Get F2 layer peak thickness see 2.5.5.7, Eq 85
 *
 * @param[in, out] pContext F2 layer profile context
 */
extern void F2_layer_get_peak_thickness(F2_layer_t* const pContext);

/** Get F2 layer peak amplitude see 2.5.5.8, Eq 90
 *
 * @param[in, out] pContext F2 layer profile context
 */
extern void F2_layer_get_peak_amplitude(F2_layer_t* const pContext);

/** Compute the topside thickness parameter H0 [km] see 2.5.5.11
 *
 * The topside of NeQuick is a semi-Epstein layer with a height dependent
 * thickness parameter B through a new parameter H.
 * A correction factor adjusts vertical TEC values to take into account
 * exosphere electron density in a simple manner.
 *
 * @param[in, out] pContext F2 layer profile context
 * @param[in] pTime indication of the time at which the H0 is required
 * @param[in] pSolar_activity solar activity
 */
extern void F2_layer_exosphere_adjust(
  F2_layer_t* const pContext,
  const NeQuickG_time_t* const pTime,
  const solar_activity_t* const pSolar_activity);

#endif // NEQUICK_G_JRC_IONO_F2_LAYER_H
