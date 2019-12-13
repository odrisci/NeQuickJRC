/**
 * NeQuick-G public interface implementation
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 *
 */
#include "NeQuickG_JRC.h"

#include <assert.h>
#include <stdlib.h>

#include "NeQuickG_JRC_context.h"
#include "NeQuickG_JRC_error.h"
#include "NeQuickG_JRC_TEC_integration.h"
#include "NeQuickG_JRC_ray.h"
#ifdef FTR_UNIT_TEST
#include "NeQuickG_JRC_UT.h"
#endif

/** Sets the modip in the context using the station position
 *
 * input data is converted to km if necessary
 *
 * @param[in, out] pContext NeQuick context
 */
static void get_modip_impl(
  NeQuickG_context_t* const pContext) {
  input_data_to_km(&pContext->input_data);
  modip_get(&pContext->modip, &pContext->input_data.station_position);
}

/** Returns the modip in degrees
 * @see {@link get_modip_impl}
 * @param[in] handle NeQuick handle
 * @return modip in degrees
 */
static double_t get_modip_interface(NeQuickG_handle handle) {
  NeQuickG_context_t* pContext = (NeQuickG_context_t*)(handle);
  get_modip_impl(pContext);
  return pContext->modip.modip_degree;
}

/** Sets the solar activity in the context using the modip
 * @see {@link get_modip_impl}
 * @see {@link solar_activity_get}
 *
 * @param[in, out] pContext NeQuick context
 */
static void get_solar_activity(
  NeQuickG_context_t* const pContext) {
  get_modip_impl(pContext);
  solar_activity_get(&pContext->solar_activity, pContext->modip.modip_degree);
}

/** Checks if the handle is valid
 * @param[in] handle NeQuick handle
 * @return on success NEQUICK_OK
 */
static int32_t check_handle(NeQuickG_chandle handle) {
  if (handle == NEQUICKG_INVALID_HANDLE) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_INPUT_DATA,
      NEQUICK_HANDLE_NULL,
      "NULL handle");
  }
  return NEQUICK_OK;
}

/** {@ref NeQuickG_library.init} */
static int32_t init(
  const char* const pModip_file,
  const char* const pCCIR_folder,
  NeQuickG_handle* const pHandle) {

  *pHandle = NEQUICKG_INVALID_HANDLE;

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
  if (!pModip_file) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_MODIP,
      NEQUICK_ERROR_CODE_BAD_MODIP_GRID_FILE,
      "Modip file. null pointer exception.");
  }

  if (!pCCIR_folder) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_F2_LAYER,
      NEQUICK_ERROR_CODE_BAD_CCIR_FILE,
      "CCIR folder. null pointer exception.");
  }
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

  *pHandle = malloc(sizeof(NeQuickG_context_t));
  if (*pHandle == NEQUICKG_INVALID_HANDLE) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_MEMORY,
      NEQUICK_ERROR_CODE_NO_RESOURCES,
      "Not enough resources to allocate NeQuick context on the heap");
  }

  NeQuickG_context_t* pContext = (NeQuickG_context_t*)(*pHandle);
  assert(pContext);

  int32_t ret;

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
  ret = modip_init(&pContext->modip, pModip_file);
  if (ret != NEQUICK_OK) {
    free(*pHandle);
    *pHandle = NEQUICKG_INVALID_HANDLE;
    return ret;
  }
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

  ret = iono_profile_init(&pContext->profile, pCCIR_folder);
  if (ret != NEQUICK_OK) {
    free(*pHandle);
    *pHandle = NEQUICKG_INVALID_HANDLE;
    return ret;
  }

  return NEQUICK_OK;
}

/** {@ref NeQuickG_library.close} */
static void close(NeQuickG_handle handle) {
  if (handle != NEQUICKG_INVALID_HANDLE) {

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
    NeQuickG_context_t* pContext = (NeQuickG_context_t*)(handle);
    iono_profile_close(&pContext->profile);
#endif

    free(handle);
  }
}

/** {@ref NeQuickG_library.set_solar_activity_coefficients} */
static int32_t set_solar_activity_coefficients(
  const NeQuickG_handle handle,
  const double_t * const pCoeff, size_t coeff_count) {

  int32_t ret = check_handle(handle);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  return solar_activity_coefficients_set(
    &((NeQuickG_context_t*)handle)->solar_activity,
    pCoeff, coeff_count);
}

/** {@ref NeQuickG_library.get_total_electron_content} */
static int32_t get_total_electron_content(
  const NeQuickG_handle handle,
  double_t* const pTotal_electron_content) {

  *pTotal_electron_content = 0.0;

  int32_t ret = check_handle(handle);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  NeQuickG_context_t* pContext = (NeQuickG_context_t*)(handle);

  get_solar_activity(pContext);

  ret = ray_get(
    &pContext->ray,
    &pContext->input_data.station_position,
    &pContext->input_data.satellite_position);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  ret = NeQuickG_integrate(pContext, pTotal_electron_content);
  if (ret == NEQUICK_OK) {
    // Eq. 151, Eq. 202 for the correction factor
    *pTotal_electron_content = (*pTotal_electron_content / 1.0E13);
  }
  return ret;
}

/** {@ref NeQuickG_library.set_time} */
static int32_t set_time(
  const NeQuickG_handle handle,
  const uint8_t month,
  const double_t UTC) {

  int32_t ret = check_handle(handle);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  NeQuickG_context_t* pContext = (NeQuickG_context_t*)(handle);
  ret = NeQuickG_time_set(&pContext->input_data.time, month, UTC);

  pContext->profile.E.is_solar_declination_valid = false;

  return ret;
}

/** {@ref NeQuickG_library.set_station_position} */
static int32_t set_station_position(
  const NeQuickG_handle handle,
  const double_t longitude_degree,
  const double_t latitude_degree,
  const double_t height_meters) {

  int32_t ret = check_handle(handle);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  NeQuickG_context_t* pContext = (NeQuickG_context_t*)(handle);
  return position_set(
    &pContext->input_data.station_position,
    longitude_degree,
    latitude_degree,
    height_meters,
    NEQUICK_G_JRC_HEIGHT_UNITS_METERS);
}

/** {@ref NeQuickG_library.set_satellite_position} */
static int32_t set_satellite_position(
  const NeQuickG_handle handle,
  const double_t longitude_degree,
  const double_t latitude_degree,
  const double_t height_meters) {

  int32_t ret = check_handle(handle);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  NeQuickG_context_t* pContext = (NeQuickG_context_t*)(handle);
  return position_set(
    &pContext->input_data.satellite_position,
    longitude_degree,
    latitude_degree,
    height_meters,
    NEQUICK_G_JRC_HEIGHT_UNITS_METERS);
}

/** {@ref NeQuickG_library.input_data_to_std_output} */
static void input_data_to_std_output_impl(NeQuickG_chandle handle) {
  if (handle == NEQUICKG_INVALID_HANDLE) {
    return;
  }

  input_data_t input_data =
    ((const NeQuickG_context_t* const)handle)->input_data;
  input_data_to_meters(&input_data);
  bool add_new_line = false; // to satisfy lint
  input_data_to_std_output(&input_data, add_new_line);
}

/** {@ref NeQuickG_library.input_data_to_output} */
static void input_data_to_output_impl(
  NeQuickG_chandle handle,
  FILE* const pFile,
  const char* const pFormat) {

  if (handle == NEQUICKG_INVALID_HANDLE) {
    return;
  }

  input_data_t input_data =
    ((const NeQuickG_context_t* const)handle)->input_data;
  input_data_to_meters(&input_data);
  input_data_to_output(&input_data, pFile, pFormat);
}

#ifdef FTR_UNIT_TEST
#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define unit_test_impl(pModip_file, pCCIR_folder) \
  unit_test_impl()
#endif // FTR_MODIP_CCIR_AS_CONSTANTS

/** {@ref NeQuickG_library.unit_test} */
static bool unit_test_impl(
  const char* const pModip_file,
  const char* const pCCIR_folder) {
  //lint -e{746} not made in the presence of a prototype
  //due to FTR_MODIP_CCIR_AS_CONSTANTS trick
  return NeQuickG_unit_test(pModip_file, pCCIR_folder);
}
#endif // FTR_UNIT_TEST

const struct NeQuickG_library NeQuickG = {
  //lint -e{123} macro with arguments is intended
  .init = init,
  .close = close,
  .set_solar_activity_coefficients = set_solar_activity_coefficients,
  .set_time = set_time,
  .set_receiver_position = set_station_position,
  .set_satellite_position = set_satellite_position,
  .get_modip = get_modip_interface,
  .get_total_electron_content = get_total_electron_content,
  .input_data_to_std_output = input_data_to_std_output_impl,
  .input_data_to_output = input_data_to_output_impl,
#ifdef FTR_UNIT_TEST
  //lint -e{123} macro with arguments is intended
  .unit_test = unit_test_impl,
#endif
};
