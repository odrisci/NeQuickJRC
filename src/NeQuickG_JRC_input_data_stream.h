/**
 * NeQuickG test program input data stream
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_main
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_INPUT_DATA_STREAM_H
#define NEQUICK_G_JRC_INPUT_DATA_STREAM_H

#include "NeQuickG_JRC.h"

#include <math.h>
#include <stdio.h>

typedef struct NeQuickG_input_data_stream_st {
  FILE *pInputFile;
  NeQuickG_handle nequick;
  double_t TEC_expected;
  double_t modip;
  bool az_in_header;
  bool is_TEC_expected_field_present;
  bool is_modip_field_present;
  bool read;
} NeQuickG_input_data_stream_t;

extern void NeQuickG_input_data_stream_init(
  NeQuickG_input_data_stream_t * const);

extern void NeQuickG_input_data_stream_open(
  NeQuickG_input_data_stream_t * const,
  const char * const pFileName,
  const NeQuickG_handle);

extern void NeQuickG_input_data_stream_close(
  const NeQuickG_input_data_stream_t * const);

extern bool NeQuickG_input_data_stream_get_next(
  NeQuickG_input_data_stream_t* const);

extern void NeQuickG_input_data_stream_put(
  const NeQuickG_input_data_stream_t* const);


#endif // NEQUICK_G_JRC_INPUT_DATA_STREAM_H
