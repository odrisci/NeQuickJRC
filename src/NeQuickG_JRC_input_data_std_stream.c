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
#include "NeQuickG_JRC_input_data_std_stream.h"

#include <stdlib.h>

#include "NeQuickG_JRC_exception.h"

void NeQuickG_input_data_std_stream_init(
  NeQuickG_input_data_std_stream_t * const pStream,
  int argc,
  char** ppArgv,
  int offset,
  const NeQuickG_handle handle) {

  pStream->nequick = handle;
  pStream->ppArgv = ppArgv;
  pStream->argc = argc;
  pStream->offset = offset;
  pStream->TEC_expected = NAN;
  pStream->eof = false;
}

void NeQuickG_input_data_std_stream_get_coef(
  NeQuickG_input_data_std_stream_t* const pStream) {

  if (pStream->argc <
     (pStream->offset +
      NEQUICKG_AZ_COEFFICIENTS_COUNT)) {
    NeQuick_exception_throw(
      NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_COUNT);
  }

  double_t coef[NEQUICKG_AZ_COEFFICIENTS_COUNT];
  {
    coef[0] = strtod(pStream->ppArgv[pStream->offset++], NULL);
    coef[1] = strtod(pStream->ppArgv[pStream->offset++], NULL);
    coef[2] = strtod(pStream->ppArgv[pStream->offset++], NULL);
  }

  if (errno != 0) {
    NeQuick_exception_throw(
      NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_FORMAT);
  }

  // to satisfy lint
  size_t coef_count = NEQUICKG_AZ_COEFFICIENTS_COUNT;
  if (NeQuickG.set_solar_activity_coefficients(
        pStream->nequick,
        coef, coef_count)  != NEQUICK_OK) {
    NEQUICK_EXCEPTION_THROW(
      NEQUICK_ERROR_SRC_INPUT_DATA,
      NEQUICK_INPUT_DATA_BAD_COEFF,
      "Az coefficients error")
  }
}


static void NeQuickG_input_data_std_stream_get_time(
  NeQuickG_input_data_std_stream_t* const pStream) {

  if (pStream->argc <
     (pStream->offset +
      NEQUICKG_TIME_PARAMETERS_COUNT)) {
    NeQuick_exception_throw(NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_COUNT);
  }

  uint8_t month;
  double_t UTC;
  {
    month = (uint8_t)atoi(pStream->ppArgv[pStream->offset++]);
    UTC = strtod(pStream->ppArgv[pStream->offset++], NULL);
  }

  if (errno != 0) {
    NeQuick_exception_throw(NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_FORMAT);
  }

  if (NeQuickG.set_time(pStream->nequick, month, UTC) != NEQUICK_OK) {
    NeQuick_exception_throw(NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_FORMAT);
  }
}

static void NeQuickG_input_data_std_stream_get_position(
  NeQuickG_input_data_std_stream_t* const pStream,
  double_t* const pLongitude_degree,
  double_t* const pLatitude_degree,
  double_t* const pHeight_meters) {

  if (pStream->argc <
     (pStream->offset +
      NEQUICKG_WGS_84_ELLIPSOIDAL_COORDINATES_COUNT)) {
    NeQuick_exception_throw(NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_COUNT);
  }

  {
    *pLongitude_degree = strtod(pStream->ppArgv[pStream->offset++], NULL);
    *pLatitude_degree = strtod(pStream->ppArgv[pStream->offset++], NULL);
    *pHeight_meters = strtod(pStream->ppArgv[pStream->offset++], NULL);
  }

  if (errno != 0) {
    NeQuick_exception_throw(NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_FORMAT);
  }

}

static void NeQuickG_input_data_std_stream_get_station_position(
  NeQuickG_input_data_std_stream_t* const pStream) {

  double_t longitude_degree;
  double_t latitude_degree;
  double_t height_meters;

  NeQuickG_input_data_std_stream_get_position(
    pStream,
    &longitude_degree,
    &latitude_degree,
    &height_meters);

  if (NeQuickG.set_receiver_position(
    pStream->nequick,
    longitude_degree,
    latitude_degree,
    height_meters) != NEQUICK_OK) {
    NeQuick_exception_throw(NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_FORMAT);
  }
}

static void NeQuickG_input_data_std_stream_get_satellite_position(
  NeQuickG_input_data_std_stream_t* const pStream) {

  double_t longitude_degree;
  double_t latitude_degree;
  double_t height_meters;

  NeQuickG_input_data_std_stream_get_position(
    pStream,
    &longitude_degree,
    &latitude_degree,
    &height_meters);

  if (NeQuickG.set_satellite_position(
    pStream->nequick,
    longitude_degree,
    latitude_degree,
    height_meters) != NEQUICK_OK) {
    NeQuick_exception_throw(NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_FORMAT);
  }
}

bool NeQuickG_input_data_std_stream_get_next(
  NeQuickG_input_data_std_stream_t* const pStream) {

  if (pStream->eof) {
    return false;
  }

  NeQuickG_input_data_std_stream_get_coef(pStream);
  NeQuickG_input_data_std_stream_get_time(pStream);
  NeQuickG_input_data_std_stream_get_station_position(pStream);
  NeQuickG_input_data_std_stream_get_satellite_position(pStream);

  pStream->eof = true;
  return true;
}
