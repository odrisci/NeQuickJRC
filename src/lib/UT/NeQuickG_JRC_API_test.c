/**
 * NeQuickG API Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_API_test.h"

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_macros.h"

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
static bool test_NULL_modip_file(
  const char* const pCCIR_directory) {

  char *pModip_file = NULL;
  NeQuickG_handle nequick;
  (void)NeQuickG.init(pModip_file, pCCIR_directory, &nequick);
  return (nequick == NEQUICKG_INVALID_HANDLE);
}

static bool test_NULL_CCIR_dir(
  const char* const pModip_file) {

  char *pCCIR_directory = NULL;
  NeQuickG_handle nequick;
  (void)NeQuickG.init(pModip_file, pCCIR_directory, &nequick);
  return (nequick == NEQUICKG_INVALID_HANDLE);
}
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define test_bad_month(pModip_file, pCCIR_directory) \
  test_bad_month()
#endif

static bool test_bad_month(
  const char* const pModip_file,
  const char* const pCCIR_directory) {

  NeQuickG_handle nequick;
  int32_t ret_init = NeQuickG.init(pModip_file, pCCIR_directory, &nequick);
  if (ret_init != NEQUICK_OK) {
    return false;
  }

  bool ret = true;

  if (NeQuickG.set_time(nequick, 0, 0.0) == NEQUICK_OK) {
    ret = false;
  }

  if (NeQuickG.set_time(nequick, 13, 0.0) == NEQUICK_OK) {
    ret = false;
  }
  NeQuickG.close(nequick);

  return ret;
}

#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define test_bad_UTC(pModip_file, pCCIR_directory) \
  test_bad_UTC()
#endif

static bool test_bad_UTC(
  const char* const pModip_file,
  const char* const pCCIR_directory) {

  NeQuickG_handle nequick;
  int32_t ret_init = NeQuickG.init(pModip_file, pCCIR_directory, &nequick);
  if (ret_init != NEQUICK_OK) {
    return false;
  }

  bool ret = true;

  if (NeQuickG.set_time(nequick, 1, -1.0) == NEQUICK_OK) {
    ret = false;
  }

  if (NeQuickG.set_time(nequick, 13, 25.0) == NEQUICK_OK) {
    ret = false;
  }
  NeQuickG.close(nequick);

  return ret;
}

#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define test_bad_latitude(pModip_file, pCCIR_directory) \
  test_bad_latitude()
#endif

static bool test_bad_latitude(
  const char* const pModip_file,
  const char* const pCCIR_directory) {

  NeQuickG_handle nequick;
  int32_t ret_init = NeQuickG.init(pModip_file, pCCIR_directory, &nequick);
  if (ret_init != NEQUICK_OK) {
    return false;
  }

  bool ret = true;

  if (NeQuickG.set_receiver_position(nequick, 0.0, -91.0, 0.0) == NEQUICK_OK) {
    ret = false;
  }

  if (NeQuickG.set_receiver_position(nequick, 0.0, 91.0, 0.0) == NEQUICK_OK) {
    ret = false;
  }

  NeQuickG.close(nequick);

  return ret;
}

#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define test_bad_coefficients(pModip_file, pCCIR_directory) \
  test_bad_coefficients()
#endif

static bool test_bad_coefficients(
  const char* const pModip_file,
  const char* const pCCIR_directory) {

  NeQuickG_handle nequick;
  int32_t ret_init = NeQuickG.init(pModip_file, pCCIR_directory, &nequick);
  if (ret_init != NEQUICK_OK) {
    return false;
  }

  bool ret = true;

  if (NeQuickG.set_solar_activity_coefficients(nequick, NULL, (size_t)3)
      == NEQUICK_OK) {
    ret = false;
  }

  double_t az[2] = {0.0, 0.0};
  if (NeQuickG.set_solar_activity_coefficients(nequick, az, (size_t)2)
      == NEQUICK_OK) {
    ret = false;
  }

  NeQuickG.close(nequick);

  return ret;
}

#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define test_receiver_satellite_same_position(pModip_file, pCCIR_directory) \
  test_receiver_satellite_same_position()
#endif

// check that the TEC returned is close to 0
static bool test_receiver_satellite_same_position(
  const char* const pModip_file,
  const char* const pCCIR_directory) {

  NeQuickG_handle nequick;
  int32_t ret_init = NeQuickG.init(pModip_file, pCCIR_directory, &nequick);
  if (ret_init != NEQUICK_OK) {
    return false;
  }

  bool ret = true;

  double_t az[NEQUICKG_AZ_COEFFICIENTS_COUNT] = {0.0, 0.0, 0.0};
  if (NeQuickG.set_solar_activity_coefficients(
    nequick, az, (size_t)NEQUICKG_AZ_COEFFICIENTS_COUNT) != NEQUICK_OK) {
    ret = false;
  }

  uint8_t month = 1;
  double_t UTC = 0.0;
  if (NeQuickG.set_time(nequick, month, UTC) != NEQUICK_OK) {
    ret = false;
  }

  {
    double_t latitude = 0.0;
    double_t longitude = 0.0;
    double_t height = 0.0;
    if (NeQuickG.set_receiver_position(
        nequick, longitude, latitude, height) != NEQUICK_OK) {
      ret = false;
    }
  }

  {
    double_t latitude = 0.0;
    double_t longitude = 0.0;
    double_t height = 0.0;
    if (NeQuickG.set_satellite_position(
        nequick, longitude, latitude, height) != NEQUICK_OK) {
      ret = false;
    }
  }

  double_t TEC;
  if (NeQuickG.get_total_electron_content(nequick, &TEC) != NEQUICK_OK) {
    ret = false;
  }

  if (!THRESHOLD_COMPARE_TO_ZERO(TEC, 1.0e-10)) {
    ret = false;
  }

  NeQuickG.close(nequick);
  return ret;
}

#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define test_bad_ray(pModip_file, pCCIR_directory) \
  test_bad_ray()
#endif

static bool test_bad_ray(
  const char* const pModip_file,
  const char* const pCCIR_directory) {

  NeQuickG_handle nequick;
  int32_t ret_init = NeQuickG.init(pModip_file, pCCIR_directory, &nequick);
  if (ret_init != NEQUICK_OK) {
    return false;
  }

  bool ret = true;

  double_t az[NEQUICKG_AZ_COEFFICIENTS_COUNT] = {0.0, 0.0, 0.0};
  if (NeQuickG.set_solar_activity_coefficients(
    nequick, az, (size_t)NEQUICKG_AZ_COEFFICIENTS_COUNT) != NEQUICK_OK) {
    ret = false;
  }

  uint8_t month = 1;
  double_t UTC = 0.0;
  if (NeQuickG.set_time(nequick, month, UTC) != NEQUICK_OK) {
    ret = false;
  }

  {
    double_t latitude = 0.0;
    double_t longitude = 0.0;
    double_t height = -3000000.0;
    if (NeQuickG.set_receiver_position(
        nequick, longitude, latitude, height) != NEQUICK_OK) {
      ret = false;
    }
  }

  {
    double_t latitude = 90.0;
    double_t longitude = 0.0;
    double_t height = 2000000.0;
    if (NeQuickG.set_satellite_position(
        nequick, longitude, latitude, height) != NEQUICK_OK) {
      ret = false;
    }
  }

  double_t TEC;
  if (NeQuickG.get_total_electron_content(nequick, &TEC) == NEQUICK_OK) {
    ret = false;
  }

  NeQuickG.close(nequick);
  return ret;
}


#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && (defined(__GNUC__) || defined(__GNUG__))
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmissing-prototypes"
#endif
bool NeQuickG_API_test(
  const char* const pModip_file,
  const char* const pCCIR_folder) {
  bool ret = true;

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
  if (!test_NULL_modip_file(pCCIR_folder)) {
    ret = false;
  }

  if (!test_NULL_CCIR_dir(pModip_file)) {
    ret = false;
  }
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

  if (!test_bad_month(pModip_file, pCCIR_folder)) {
    ret = false;
  }

  if (!test_bad_UTC(pModip_file, pCCIR_folder)) {
    ret = false;
  }

  if (!test_bad_latitude(pModip_file, pCCIR_folder)) {
    ret = false;
  }

  if (!test_bad_coefficients(pModip_file, pCCIR_folder)) {
    ret = false;
  }

  if (!test_receiver_satellite_same_position(pModip_file, pCCIR_folder)) {
    ret = false;
  }

  if (!test_bad_ray(pModip_file, pCCIR_folder)) {
    ret = false;
  }

  return ret;
}
#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && (defined(__GNUC__) || defined(__GNUG__))
  #pragma GCC diagnostic pop
#endif
