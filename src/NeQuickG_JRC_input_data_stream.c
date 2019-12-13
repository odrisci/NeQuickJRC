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
#include "NeQuickG_JRC_input_data_stream.h"

#include <string.h>

#include "NeQuickG_JRC_exception.h"

#define NEQUICK_G_INPUT_DATA_RECORD_FORMAT \
"%hhu %lf %lf %lf %lf %lf %lf %lf"

#define NEQUICK_G_OUTPUT_DATA_RECORD_FORMAT \
"%3d %12.5f %12.5f %12.5f %16.5f %12.5f %12.5f %16.5f"

#define NEQUICK_G_INPUT_DATA_FIELD_COUNT (\
  NEQUICKG_TIME_PARAMETERS_COUNT + \
  NEQUICKG_WGS_84_ELLIPSOIDAL_COORDINATES_COUNT*2)

static void NeQuickG_input_data_stream_get_solar_activity_coefficients(
  const NeQuickG_input_data_stream_t * const pContext,
  const NeQuickG_handle handle) {
  double_t az[NEQUICKG_AZ_COEFFICIENTS_COUNT];
  // first line is az coefficients
  int ret = fscanf(pContext->pInputFile,
    "%lf %lf %lf",
    &az[0x00],
    &az[0x01],
    &az[0x02]);
  if (ret != NEQUICKG_AZ_COEFFICIENTS_COUNT) {
    NEQUICK_EXCEPTION_THROW(
      NEQUICK_ERROR_SRC_INPUT_DATA,
      NEQUICK_INPUT_DATA_BAD_COEFF,
      "Az coefficients error, cannot be scanned with fscanf. Expected 3 coefficients");
  }
  // to satisfy lint
  size_t coef_count = NEQUICKG_AZ_COEFFICIENTS_COUNT;
  if (NeQuickG.set_solar_activity_coefficients(
        handle,
        az, coef_count) != NEQUICK_OK) {
    NEQUICK_EXCEPTION_THROW(
      NEQUICK_ERROR_SRC_INPUT_DATA,
      NEQUICK_INPUT_DATA_BAD_COEFF,
      "Az coefficients error");
  }
}

void NeQuickG_input_data_stream_init(
  NeQuickG_input_data_stream_t * const pStream) {
  pStream->pInputFile = NULL;
  pStream->TEC_expected = NAN;
  pStream->is_TEC_expected_field_present = true;
  pStream->az_in_header = true;
  pStream->read = true;
  pStream->is_modip_field_present = false;
  pStream->modip = NAN;
}

void NeQuickG_input_data_stream_open(
  NeQuickG_input_data_stream_t * const pContext,
  const char * const pFileName,
  const NeQuickG_handle handle) {
  {
    if (pContext->read) {
      pContext->pInputFile = fopen(pFileName, "r");
    } else {
      pContext->pInputFile = fopen(pFileName, "w");
    }

    if (!pContext->pInputFile) {
      NEQUICK_EXCEPTION_THROW(
        NEQUICK_ERROR_SRC_INPUT_DATA,
        NEQUICK_INPUT_DATA_FILE_REASON,
        "Cannot open input data file %s", pFileName);
    }
  }

  if (pContext->az_in_header) {
    NeQuickG_input_data_stream_get_solar_activity_coefficients(
      pContext, handle);
  }

  pContext->nequick = handle;
}

void NeQuickG_input_data_stream_close(
  const NeQuickG_input_data_stream_t * const pContext) {
  if (pContext->pInputFile) {
    fclose(pContext->pInputFile);
  }
}

static void NeQuickG_input_data_stream_get_TEC_expected(
  NeQuickG_input_data_stream_t* const pContext) {

  double_t TEC_expected;

  int ret = fscanf(pContext->pInputFile,
    "%lf\n",
    &TEC_expected);

  if (ret != 1) {
    NEQUICK_EXCEPTION_THROW(
      NEQUICK_ERROR_SRC_INPUT_DATA,
      NEQUICK_INPUT_DATA_BAD_DATA,
      "fscanf error. Expected end of line.");
  }

  pContext->TEC_expected = TEC_expected;

}

static bool NeQuickG_input_data_stream_get_next_from_file(
  NeQuickG_input_data_stream_t* const pContext) {
  if (!pContext->pInputFile ||
      feof(pContext->pInputFile)) {
    return false;
  }

  {
    uint8_t month;
    double_t UTC;

    double_t station_longitude_degree;
    double_t station_latitude_degree;
    double_t station_height;

    double_t satellite_longitude_degree;
    double_t satellite_latitude_degree;
    double_t satellite_height;

    int ret = fscanf(pContext->pInputFile,
      NEQUICK_G_INPUT_DATA_RECORD_FORMAT,
      &month,
      &UTC,
      &station_longitude_degree,
      &station_latitude_degree,
      &station_height,
      &satellite_longitude_degree,
      &satellite_latitude_degree,
      &satellite_height);
    if (ret != NEQUICK_G_INPUT_DATA_FIELD_COUNT) {
      NEQUICK_EXCEPTION_THROW(
        NEQUICK_ERROR_SRC_INPUT_DATA,
        NEQUICK_INPUT_DATA_BAD_DATA,
        "Error scanning input data with fscanf");
    }

    {
      if (NeQuickG.set_time(pContext->nequick, month, UTC) != NEQUICK_OK) {
        NEQUICK_EXCEPTION_THROW(
        NEQUICK_ERROR_SRC_INPUT_DATA,
        NEQUICK_INPUT_DATA_BAD_DATA,
        "Error setting time");
      }
      if (NeQuickG.set_receiver_position(
          pContext->nequick,
          station_longitude_degree,
          station_latitude_degree,
          station_height) != NEQUICK_OK) {
        NEQUICK_EXCEPTION_THROW(
          NEQUICK_ERROR_SRC_INPUT_DATA,
          NEQUICK_INPUT_DATA_BAD_DATA,
          "Error setting station position");
      }
      if (NeQuickG.set_satellite_position(
          pContext->nequick,
          satellite_longitude_degree,
          satellite_latitude_degree,
          satellite_height) != NEQUICK_OK) {
        NEQUICK_EXCEPTION_THROW(
          NEQUICK_ERROR_SRC_INPUT_DATA,
          NEQUICK_INPUT_DATA_BAD_DATA,
          "Error setting satellite position");
      }
    }

    if (pContext->is_TEC_expected_field_present) {
      NeQuickG_input_data_stream_get_TEC_expected(pContext);
    } else {
      int ret_ = fscanf(pContext->pInputFile, "\n");
       if ((ret_ < 0) && (ret_ != EOF)) {
         NEQUICK_EXCEPTION_THROW(
          NEQUICK_ERROR_SRC_INPUT_DATA,
          NEQUICK_INPUT_DATA_BAD_DATA,
          "Error scanning input data with fscanf, expected end of line, return %i.", ret_);
       }
    }

    return true;
  }
}

bool NeQuickG_input_data_stream_get_next(
  NeQuickG_input_data_stream_t* const pContext) {
  return NeQuickG_input_data_stream_get_next_from_file(pContext);
}

void NeQuickG_input_data_stream_put(
  const NeQuickG_input_data_stream_t* const pStream) {

  NeQuickG.input_data_to_output(
    pStream->nequick,
    pStream->pInputFile,
    NEQUICK_G_OUTPUT_DATA_RECORD_FORMAT);

  if (pStream->is_modip_field_present) {
    fprintf(pStream->pInputFile, " %12.5f",
      pStream->modip);
  }
  if (pStream->is_TEC_expected_field_present) {
    fprintf(pStream->pInputFile, " %12.5f",
      pStream->TEC_expected);
  }
  fprintf(pStream->pInputFile, "\n");
}

#undef NEQUICK_G_INPUT_DATA_RECORD_FORMAT
#undef NEQUICK_G_OUTPUT_DATA_RECORD_FORMAT
