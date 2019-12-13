/** NeQuickG input data for ray and time
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_input_data.h"

#include <string.h>

#define NEQUICK_G_INPUT_DATA_RECORD_FORMAT \
  "%hhu %2.2f %7.2f %6.2f %7.2f %7.2f %6.2f %9.2f"

void input_data_to_km(
  input_data_t* const pInputData) {
  position_to_km(&pInputData->station_position);
  position_to_km(&pInputData->satellite_position);
}

void input_data_to_meters(
  input_data_t* const pInputData) {
  position_to_meters(&pInputData->station_position);
  position_to_meters(&pInputData->satellite_position);
}

void input_data_to_std_output(
  const input_data_t* const pInputData, bool addNewLine) {
  printf(NEQUICK_G_INPUT_DATA_RECORD_FORMAT,
   pInputData->time.month,
   pInputData->time.utc,
   // ESA uses this longitude for this field
   pInputData->station_position.longitude.degree,
   pInputData->station_position.latitude.degree,
   pInputData->station_position.height,
   // ESA uses this longitude for this field
   pInputData->satellite_position.longitude_org,
   pInputData->satellite_position.latitude.degree,
   pInputData->satellite_position.height);
  if (addNewLine) {
    printf("\n");
  }
}

void input_data_to_output(
  const input_data_t* const pInputData,
  FILE* const pFile,
  const char* const pFormat) {
  fprintf(pFile, pFormat,
   pInputData->time.month,
   pInputData->time.utc,
   // ESA uses this longitude for this field
   pInputData->station_position.longitude.degree,
   pInputData->station_position.latitude.degree,
   pInputData->station_position.height,
   // ESA uses this longitude for this field
   pInputData->satellite_position.longitude_org,
   pInputData->satellite_position.latitude.degree,
   pInputData->satellite_position.height);
}

#undef NEQUICK_G_INPUT_DATA_RECORD_FORMAT
