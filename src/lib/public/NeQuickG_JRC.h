/**
 *  Ionospheric Correction Algorithm for Galileo Single Frequency Users.
 *
 *  Example implementation of the algorithm to compute ionospheric corrections based
 *  on the broadcast coefficients in the navigation message for Galileo single-frequency users.<br>
 *  The term Galileo is used to refer to the Global Navigation Satellite System (GNSS) established under
 *  the European GNSS programme.
 *
 *  <h3>Spec(s):</h3>
 *    - European GNSS (Galileo) Open Service. Ionospheric Correction Algorithm for Galileo Single Frequency Users, 1.2 September 2016
 *
 *  <h3>Hints for Usage:</h3>
 *    - Initialize the library (#NeQuickG_library.init) using the
 *      CCIR files and MODIP file provided with the library
 *
 *    - Set inputs:<br>
 *       - solar activity coefficients (#NeQuickG_library.set_solar_activity_coefficients)<br>
 *       - time (#NeQuickG_library.set_time)<br>
 *       - receiver position (#NeQuickG_library.set_receiver_position)<br>
 *       - satellite position (#NeQuickG_library.set_satellite_position)<br>
 *
 *    - Get the Slant Total Electron Content (STEC) in TECU (#NeQuickG_library.get_total_electron_content)
 *
 *    - Free resources (#NeQuickG_library.close)
 *
 * The STEC integration routine used in this implementation is the method based on
 * Kronrod G<SUB>7</SUB>-K<SUB>15</SUB> adaptive quadrature with the default tolerances defined in the specification.
 * See section F.2.6.<br>
 * The maximum recursion level for the integration routine is #NEQUICK_G_JRC_RECURSION_LIMIT_MAX<br>
 *
 * When the compilation flag FTR_MODIP_CCIR_AS_CONSTANTS is set to 1 the CCIR grid and the MODIP files are
 *  preloaded as constants in the library (no need for external files):
 *  - MODIP matrix available at its first release dated on year 2001 and calculated at a height of 300 km.
 *
 * @mainpage NeQuick-G (Galileo)
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @bug No known bugs.
 * @defgroup NeQuickG_JRC NeQuickG_JRC_lib [NeQuickG_JRC]
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_H
#define NEQUICK_G_JRC_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/** This is NequickG JRC version 0.1*/
#define NEQUICKG_VERSION (0.1)

/** Invalid handle */
#define NEQUICKG_INVALID_HANDLE (NULL)

/** Effective Ionisation Level uses 3rd order parameter. */
#define NEQUICKG_AZ_COEFFICIENTS_COUNT (3)

/** Number of WGS-84 ellipsoidal coordinates:
 * latitude, longitude and height
 */
#define NEQUICKG_WGS_84_ELLIPSOIDAL_COORDINATES_COUNT (3)

/** Number of time parameters:
 * month and UTC
 */
#define NEQUICKG_TIME_PARAMETERS_COUNT (2)

/** NeQuick success */
#define NEQUICK_OK 0

/** NequickG JRC handle */
typedef void* NeQuickG_handle;

/** NequickG JRC constant handle */
typedef const void* const NeQuickG_chandle;

/** NequickG JRC API */
struct NeQuickG_library {
  /** NequickG JRC library initialization
   *
   * the library context is allocated in the heap<br>
   * the modip file is loaded in the library context
   *
   * @param[in] pModip_file a file with the table grid of MODIP values
   *  versus geographical location provided together with the NeQuick G.
   *
   * @param[in] pCCIR_directory directory where the coefficients for the foF2 and M(3000)F2 models
   * recommended by the Comite Consultatif International des Radiocommunications (CCIR)
   * can be found.
   * These coefficients are the spherical harmonic coefficients representing the
   * development of monthly median foF2 and M(3000)F2 all over the world.
   * The coefficients correspond to low (R12 = 0) and high (R12 = 100) solar
   * activity conditions.
   * Each file ccirXX.asc contains 2858 values sequentially organized as follows:
   * {f2 1,1,1, f2 1,1,2, ..., f2 1,1,13,
   *  f2 1,2,1, f2 1,2,2, ..., f2 1,2,13,
   *  ...,
   *  f2 1,76,1, f2 1,76,2, ..., f2 1,76,13,
   *  f2 2,1,1, f2 2,1,2, ..., f2 2,1,13,
   *  f2 2,2,1, f2 2,2,2, ..., f2 2,2,13,
   *  ...,
   *  f2 2,76,1, f2 2,76,2, ..., f2 2,76,13,
   *  fm3 1,1,1, fm3 1,1,2, ..., fm3 1,1,9,
   *  fm3 1,2,1, fm3 1,2,2, ..., fm3 1,2,9,
   *  ...,
   *  fm3 1,49,1, fm3 1,49,2, ..., fm3 1,49,9,
   *  fm3 2,1,1,  fm3 2,1,2, ..., fm3 2,1,9,
   *  fm3 2,2,1, fm3 2,2,2, ..., fm3 2,2,9,
   *  ...,
   *  fm3 2,49,1, fm3 2,49,2, ..., fm3 2,49,9}
   * The coefficients of a ccirXX.asc file have to be stored in two 3-D arrays,
   *  F2 and Fm3,
   *
   * @param[out] pHandle on success contains a valid NequickG JRC handle,
   *  on error is set to NEQUICKG_INVALID_HANDLE
   *
   * @return on success NEQUICK_OK
   */
#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define init(pModip_file, pCCIR_directory, pHandle) \
  init(pHandle)
#endif

   int32_t (*init)(
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
    const char* const pModip_file,
    const char* const pCCIR_directory,
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS
    NeQuickG_handle* const pHandle);

  /** NequickG JRC library uninitialization.
   *  Free resources allocated by init.
   *
   * @param[in] NeQuickG_handle NequickG JRC handle
   */
  void (*close)(const NeQuickG_handle);

  /** Sets solar activity coefficients
   *  The three coefficients, ai<SUB>0</SUB>,  ai<SUB>1</SUB> and  ai<SUB>2</SUB> are transmitted to the users in
   *  the Galileo navigation broadcast message.
   *  They are used to calculate the Effective Ionisation Level (Az),
   *  an index that represents solar activity.
   *  The Az parameter is a continuous function
   *  of MODIP at the receiver location and Az is expressed by the three coefficients as
   *   Az = ai<SUB>0</SUB> + ai<SUB>1</SUB>*MODIP + ai<SUB>2</SUB>*(MODIP)<SUP>2</SUP>
   *  units used:
   *  ai<SUB>0</SUB> : sfu, ai<SUB>1</SUB> : sfu/degree and ai<SUB>2</SUB> : sfu/deg<SUP>2</SUP>
   *   sfu (solar flux unit) is not a SI unit but can be converted as:
   *   1 sfu = 10<SUP>-22</SUP> W/(m<SUP>2</SUP>*Hz)
   *
   * @param[in] NeQuickG_handle NequickG JRC handle
   * @param[in] pAz array with the solar activity coefficients
   *         ai0 Effective Ionisation Level 1st order parameter
   *         ai1 Effective Ionisation Level 2nd order parameter
   *         ai2 Effective Ionisation Level 3rd order parameter
   * @param[in] az_size Size of array pAz
   *
   * @return on success NEQUICK_OK
   */
  int32_t (*set_solar_activity_coefficients)(
    const NeQuickG_handle,
    const double_t* const pAz,
    size_t az_size);

  /** Sets time
   *
   * @param[in] NeQuickG_handle NequickG JRC handle
   * @param[in] month numerical value January = 1, ..., December = 12
   * @param[in] UTC UT time (hours) range accepted [0, 24]
   *
   * @return on success NEQUICK_OK
   */
  int32_t (*set_time)(
    const NeQuickG_handle,
    const uint8_t month,
    const double_t UTC);

  /** Sets the receiver position using WGS-84 ellipsoidal coordinates
   *
   * @param[in] NeQuickG_handle NequickG JRC handle
   * @param[in] longitude_degree Geodetic longitude from receiver (degrees)
   * @param[in] height_meters Geodetic latitude from receiver (degrees) range accepted [-90, +90]
   * @param[in] height_meters Geodetic height from receiver (m)
   *
   * @return on success NEQUICK_OK
   */
  int32_t (*set_receiver_position)(
    const NeQuickG_handle,
    const double_t longitude_degree,
    const double_t latitude_degree,
    const double_t height_meters);

  /** Sets the satellite position using WGS-84 ellipsoidal coordinates
   *
   * @param[in] NeQuickG_handle NequickG JRC handle
   * @param[in] longitude_degree Geodetic longitude from receiver (degrees)
   * @param[in] latitude_degree Geodetic latitude from receiver (degrees) range accepted [-90, +90]
   * @param[in] height_meters Geodetic height from receiver (m)
   *
   * @return on success NEQUICK_OK
   */
  int32_t (*set_satellite_position)(
    const NeQuickG_handle,
    const double_t longitude_degree,
    const double_t latitude_degree,
    const double_t height_meters);

  /** Gets the receiver MODIP.
   * Needs a previous call to set_receiver_position.
   *
   * @param[in] NeQuickG_handle NequickG JRC handle
   *
   * @return MODIP in degrees
   */
  double_t (*get_modip)(const NeQuickG_handle);

  /** Gets the Slant Total Electron Content in TECU.
   *  1 TECU equals 10<SUP>16</SUP> electrons/m<SUP>2</SUP>
   *
   * for each satellite-receiver link<br>
   *  Obtain receiver MODIP<SUB>U</SUB> using &phi;<SUB>i</SUB>, lambda;<SUB>i</SUB><br>
   *  Obtain Effective Ionisation Level Az<SUB>U</SUB> with MODIP<SUB>U</SUB> and broadcast coefficients ai<SUB>0</SUB>, ai<SUB>1</SUB> and ai<SUB>2</SUB><br>
   *  Call NeQuick G STEC integration routine (#NeQuickG_integrate) for path (x,y,z)<SUB>i</SUB> to (x,y,z)<SUB>j</SUB>
   *
   * @param[in] NeQuickG_handle NequickG JRC handle
   * @param[out] TEC ouput parameter total electron content in TECU (10<SUP>16</SUP> electrons/m<SUP>2</SUP>)
   *
   * @return on success NEQUICK_OK
   */
  int32_t (*get_total_electron_content)(
    const NeQuickG_handle,
    double_t* const TEC);

  /** Input data set to std output
   *
   * @param[in] NeQuickG_chandle NequickG JRC handle
   */
  void (*input_data_to_std_output)(
    NeQuickG_chandle);

  /** Input data set to file
   *
   * @param[in] NeQuickG_chandle NequickG JRC handle
   * @param[in] pFile pointer to a valid FILE structure
   * @param[in] pFormat the fprintf format
   */
  void (*input_data_to_output)(
    NeQuickG_chandle,
    FILE* const pFile,
    const char* const pFormat);

#ifdef FTR_UNIT_TEST
#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#define unit_test(pModip_file, pCCIR_directory) unit_test()
#endif
  /** Execute the unit tests
   *
   * @param[in] pModip_file a file with the table grid of MODIP values
   * @param[in] pCCIR_directory directory where the coefficients for the foF2 and M(3000)F2 models
   *        can be found
   *
   * @return true on success, false otherwise
   */
  bool (*unit_test)(
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
    const char* const pModip_file,
    const char* const pCCIR_directory
#endif //!FTR_MODIP_CCIR_AS_CONSTANTS
    );
#endif
};

/** NequickG JRC library interface. */
extern const struct NeQuickG_library NeQuickG;

#endif // NEQUICK_G_JRC_H
