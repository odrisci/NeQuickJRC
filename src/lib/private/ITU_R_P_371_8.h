/**
 *  Sunspot algorithm recommended by ITU.
 *
 *  <h3>Spec(s):</h3>
 *    - RECOMMENDATION ITU-R P.371-8. Choice of indices for long-term ionospheric predictions
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef ITU_R_P_371_8_H
#define ITU_R_P_371_8_H

#include <math.h>

/** The lowest Solar Flux value in average conditions that NeQuick is expected to operate on.
 * It corresponds to a Sun Spot Number of 0. Eq. 17<br>
 * This value is considered adequate when no other solution is available, while still being able
 * to correct for a significant contribution of the ionospheric delay error.
 * unit is sfu (solar flux unit) where 1 sfu = 10<SUP>-22</SUP> W/(m<SUP>2</SUP>*Hz)
 */
#define ITU_R_P_371_8_LOWER_SOLAR_FLUX_IN_SFU (63.7)

/** An empirical formula to convert the smoothed relative sunspot
 * number into monthly averaged F10.7 index is provided by the
 * Radiocommunication sector of the International Telecommunication Union
 * (ITU-R) as follow (Leitinger et al., 2005):
 * F10.7 = 63.7 + (0.728 + 0.00089R12)R12
 * and wise versa:
 * R12 = (1672.73 + (F10.7 − 63.7)1123.6)<SUP>0.5</SUP> − 408.99.
 *
 * @param[in] solar_12_month_running_mean_of_2800_MHZ_noise_flux_in_sfu solar flux
 *        expressed in units of  10<SUP>-22</SUP> W/(m<SUP>2</SUP>*Hz).
 *
 * @return solar 12 month mean spot number
 */
extern double_t get_solar_mean_spot_number
  (double_t solar_12_month_running_mean_of_2800_MHZ_noise_flux_in_sfu);

#endif // ITU_R_P_371_8_H
