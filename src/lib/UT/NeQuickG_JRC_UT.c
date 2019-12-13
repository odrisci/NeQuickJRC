/**
 * NeQuickG Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_UT.h"

#include "NeQuickG_JRC_API_test.h"
#include "NeQuickG_JRC_Az_test.h"
#include "NeQuickG_JRC_iono_E_layer_test.h"
#include "NeQuickG_JRC_iono_F1_layer_test.h"
#include "NeQuickG_JRC_iono_F2_layer_fourier_coefficients_test.h"
#include "NeQuickG_JRC_iono_F2_layer_test.h"
#include "NeQuickG_JRC_iono_layer_amplitudes_test.h"
#include "NeQuickG_JRC_iono_layer_thickness_test.h"
#include "NeQuickG_JRC_MODIP_test.h"
#include "NeQuickG_JRC_ray_test.h"
#include "NeQuickG_JRC_solar_test.h"

#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && (defined(__GNUC__) || defined(__GNUG__))
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmissing-prototypes"
#endif
bool NeQuickG_unit_test(
  const char* const pModip_file,
  const char* const pCCIR_folder) {

  bool ret = true;

  if (!NeQuickG_API_test(pModip_file, pCCIR_folder)) {
    ret = false;
  }
  if (!NeQuickG_modip_test()) {
    ret = false;
  }
  if (!NeQuickG_Az_test()) {
    ret = false;
  }
  if (!NeQuickG_solar_test()) {
    ret = false;
  }
  if (!NeQuickG_iono_E_layer_test()) {
    ret = false;
  }
  if (!NeQuickG_iono_F1_layer_test()) {
    ret = false;
  }
  if (!ITU_F2_layer_coefficients_test()) {
    ret = false;
  }
  if (!NeQuickG_iono_F2_layer_test()) {
    ret = false;
  }
  if (!NeQuickG_iono_layer_thickness_test()) {
    ret = false;
  }
  if (!NeQuickG_iono_layer_amplitudes_test()) {
    ret = false;
  }
  if (!NeQuickG_ray_test()) {
    ret = false;
  }
  return ret;
}
#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && (defined(__GNUC__) || defined(__GNUG__))
  #pragma GCC diagnostic pop
#endif
