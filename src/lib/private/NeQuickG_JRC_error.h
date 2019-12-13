/** NeQuick G JRC error codes
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_ERROR_H
#define NEQUICK_G_JRC_ERROR_H

/** The source of the error code is the memory. */
#define NEQUICK_ERROR_SRC_MEMORY (-1)
/** The source of the error code is the input data. */
#define NEQUICK_ERROR_SRC_INPUT_DATA (-2)
/** The source of the error code is the modip module. */
#define NEQUICK_ERROR_SRC_MODIP (-3)
/** The source of the error code is the F2 layer. */
#define NEQUICK_ERROR_SRC_F2_LAYER (-4)
/** The source of the error code is the solar activity module. */
#define NEQUICK_ERROR_SRC_SOLAR_ACTIVITY (-5)
/** The source of the error code is the ray module. */
#define NEQUICK_ERROR_SRC_RAY (-6)

/** Error code: no resources available. */
#define NEQUICK_ERROR_CODE_NO_RESOURCES (1)

/** Error code: month parameter is not correct.
 * valid range is [@ref NEQUICK_G_JRC_MONTH_JANUARY - @ref NEQUICK_G_JRC_MONTH_DECEMBER]
 */
#define NEQUICK_ERROR_CODE_BAD_MONTH (2)
/** Error code: UTC time is not correct.
 * valid range is [0.0 - @ref NEQUICK_G_JRC_TIME_DAY_IN_HOURS]
 */
#define NEQUICK_ERROR_CODE_BAD_UTC (3)

/** Error code: Latitude is not correct.
 * valid range is [@ref NEQUICK_G_JRC_LAT_MIN_VALUE_DEGREE - @ref NEQUICK_G_JRC_LAT_MAX_VALUE_DEGREE]
 */
#define NEQUICK_ERROR_CODE_BAD_LATITUDE (4)

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
/** Error code: cannot open modip file. */
#define NEQUICK_ERROR_CODE_BAD_MODIP_GRID_FILE (5)
/** Error code: bad modip file structure. */
#define NEQUICK_ERROR_CODE_BAD_MODIP_STRUCTURE (6)
/** Error code: cannot open CCIR file. */
#define NEQUICK_ERROR_CODE_BAD_CCIR_FILE (7)
/** Error code: bad CCIR file structure. */
#define NEQUICK_ERROR_CODE_BAD_CCIR_STRUCTURE (8)
#endif

/** Error code: bad AZ coefficients */
#define NEQUICK_ERROR_CODE_BAD_AZ_COEFF (9)

/** Error code: bad ray, see F.2.5.1. NeQuick internal function NeqGetRayProperties*/
#define NEQUICK_ERROR_CODE_BAD_RAY (10)

/** Error code: invalid Nequick handle */
#define NEQUICK_HANDLE_NULL (11)

/** Log an error in the standard error.
 * @param[in] error_src error source
 * @param[in] error_code error code
 * @param[in] ... printf format string + additional arguments
 */
#define NEQUICK_ERROR_LOG(error_src, error_code, ...) \
  fprintf(stderr, "Nequick error. File = %s, line = %i\n", __FILE__, __LINE__); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \

/** Log an error in the standard error and return the error code
 * @param[in] error_src error source
 * @param[in] error_code error code
 * @param[in] ... printf format string + additional arguments
 */
//lint -emacro(717, NEQUICK_ERROR_RETURN) while (0) intentional to force ';'
#define NEQUICK_ERROR_RETURN(error_src, error_code, ...) \
do { \
  NEQUICK_ERROR_LOG(error_src, error_code, __VA_ARGS__); \
  return (error_src); \
} while (false)

#endif // NEQUICK_G_JRC_ERROR_H
