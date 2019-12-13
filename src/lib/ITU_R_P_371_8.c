/**
 * Sunspot algorithm recommended by ITU.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "ITU_R_P_371_8.h"

double_t get_solar_mean_spot_number
  (double_t solar_12_month_running_mean_of_2800_MHZ_noise_flux) {
  return (sqrt(167273.0 +
      (solar_12_month_running_mean_of_2800_MHZ_noise_flux -
      ITU_R_P_371_8_LOWER_SOLAR_FLUX_IN_SFU)*1123.6
    ) - 408.99);
}
