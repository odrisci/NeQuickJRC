/**
 * NeQuickG Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @defgroup NeQuickG_JRC_UT NeQuickG_JRC_UT_lib [NeQuickG_JRC_UT]
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_UT_H
#define NEQUICK_G_JRC_UT_H

#include <stdbool.h>

#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define NeQuickG_unit_test(pModip_file, pCCIR_folder) \
  NeQuickG_unit_test()
#endif // FTR_MODIP_CCIR_AS_CONSTANTS

extern bool NeQuickG_unit_test(
  const char* const pModip_file,
  const char* const pCCIR_folder);

#endif // NEQUICK_G_JRC_UT_H
