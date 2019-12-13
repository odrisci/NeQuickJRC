/**
 * NeQuickG test program input data from standard input
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_main
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_INPUT_DATA_STD_STREAM_H
#define NEQUICK_G_JRC_INPUT_DATA_STD_STREAM_H

#include "NeQuickG_JRC.h"

#include <errno.h>
#include <math.h>
#include <stdbool.h>

typedef struct NeQuickG_input_data_std_stream_st {
  char** ppArgv;
  int argc;
  int offset;
  NeQuickG_handle nequick;
  double_t TEC_expected;
  bool eof;
} NeQuickG_input_data_std_stream_t;

extern void NeQuickG_input_data_std_stream_init(
  NeQuickG_input_data_std_stream_t * const,
  int argc,
  char** ppArgv,
  int offset,
  const NeQuickG_handle);

extern void NeQuickG_input_data_std_stream_get_coef(
  NeQuickG_input_data_std_stream_t* const);

extern bool NeQuickG_input_data_std_stream_get_next(
  NeQuickG_input_data_std_stream_t* const);

#endif // NEQUICK_G_JRC_INPUT_DATA_STD_STREAM_H
