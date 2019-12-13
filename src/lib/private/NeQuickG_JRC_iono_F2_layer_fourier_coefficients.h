/** Coefficients for the foF2 and M(3000)F2 models recommended by the
 *  Comité Consultatif International des Radiocommunications (CCIR).
 *
 * ITU-R P.2297-0 (06/2013)
 * P Series Radiowave propagation
 * Electron density models and data for transionospheric radio propagation.
 *
 *  - foF2 is the F2-peak plasma frequency, which is related to the F2-peak
 *  density NmF2 by NmF2/m<SUP>-3</SUP> = 1.24 * 10^10 (foF2/MHz)<SUP>2</SUP>
 *  - M(3000)F2 = (MUF(3000)/foF2) is a propagation factor closely related to
 *  the height of the F2-peak (see Bilitza et al., 1979).
 *  MUF(3000) is the highest frequency that, refracted in the ionosphere,
 *  can be received at a distance of 3000 km.
 *  Both parameters foF2 and M(3000)F2 are routinely scaled from ionograms.
 *  The CCIR maps are based on monthly median values obtained by the worldwide
 *  network of ionosondes (about 150 stations) during the years 1954 to 1958,
 *  altogether about 10,000 station-months of data.
 *  Following a numerical mapping procedure developed by Jones and Gallet
 *  (1962, 1965), each station data set is first represented by a Fourier time
 *  series (in Universal Time), and then a worldwide development in a special
 *  form of Legendre functions (in geodetic latitude, longitude, and modified
 *  dip latitude) is applied for each Fourier coefficient.
 *  Coefficients sets are provided for high and low solar activity.
 *  The whole CCIR model consists of (988 + 441) * 2 * 12 = 34,296
 *  coefficients.
 *
 * These coefficients are the spherical harmonic coefficients representing the
 * development of monthly median foF2 and M(3000)F2 all over the world.
 * The coefficients correspond to low (R12 = 0) and high (R12 = 100) solar
 * activity conditions. Therefore they have to be interpolated (or
 * extrapolated) to obtain the set of coefficients corresponding to the
 * required solar activity.
 * Each file ccirXX.asc contains 2858 values sequentially organized as follows:<br>
 * <CENTER>
 * {f2 1,1,1, f2 1,1,2, ..., f2 1,1,13,<br>
 *  f2 1,2,1, f2 1,2,2, ..., f2 1,2,13,<br>
 *  ...,<br>
 *  f2 1,76,1, f2 1,76,2, ..., f2 1,76,13,<br>
 *  f2 2,1,1, f2 2,1,2, ..., f2 2,1,13,<br>
 *  f2 2,2,1, f2 2,2,2, ..., f2 2,2,13,<br>
 *  ...,<br>
 *  f2 2,76,1, f2 2,76,2, ..., f2 2,76,13,<br>
 *  <br>
 *  fm3 1,1,1, fm3 1,1,2, ..., fm3 1,1,9,<br>
 *  fm3 1,2,1, fm3 1,2,2, ..., fm3 1,2,9,<br>
 *  ...,<br>
 *  fm3 1,49,1, fm3 1,49,2, ..., fm3 1,49,9,<br>
 *  fm3 2,1,1,  fm3 2,1,2, ..., fm3 2,1,9,<br>
 *  fm3 2,2,1, fm3 2,2,2, ..., fm3 2,2,9,<br>
 *  ...,<br>
 *  fm3 2,49,1, fm3 2,49,2, ..., fm3 2,49,9}<br>
 * </CENTER>
 *
 * The coefficients of a ccirXX.asc file have to be stored in two 3-D arrays,
 *  F2 and Fm3,
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_IONO_F2_LAYER_COEFFICIENTS_H
#define NEQUICK_G_JRC_IONO_F2_LAYER_COEFFICIENTS_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "NeQuickG_JRC_solar_activity.h"
#include "NeQuickG_JRC_time.h"

/** 2 solar activity conditions: low and high. */
#define ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT (2)
/** index for low solar activity condition. */
#define ITU_F2_LAYER_COEFF_LOW_SOLAR_ACTIVITY_IDX (0)
/** index for high solar activity condition. */
#define ITU_F2_LAYER_COEFF_HIGH_SOLAR_ACTIVITY_IDX (1)

/** F2 coefficient max. degree. */
#define ITU_F2_COEFF_MAX_DEGREE (76)
/** F2 coefficient max. order. */
#define ITU_F2_COEFF_MAX_ORDER (13)

/** FM3 coefficient max. degree. */
#define ITU_FM3_COEFF_MAX_DEGREE (49)
/** FM3 coefficient max. order. */
#define ITU_FM3_COEFF_MAX_ORDER (9)

/** Type for the F2 coefficients array */
typedef double_t F2_coefficient_array_t
  [ITU_F2_COEFF_MAX_DEGREE][ITU_F2_COEFF_MAX_ORDER];

/** Type for the Fm3 coefficients array */
typedef double_t Fm3_coefficient_array_t
  [ITU_FM3_COEFF_MAX_DEGREE][ITU_FM3_COEFF_MAX_ORDER];

/** Type for the F2 fourier coefficients array */
typedef double_t F2_fourier_coefficient_array_t
  [ITU_F2_COEFF_MAX_DEGREE];

/** Type for the Fm3 fourier coefficients array */
typedef double_t Fm3_fourier_coefficient_array_t
  [ITU_FM3_COEFF_MAX_DEGREE];

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
/** Spherical harmonic coefficients representing the
 *  development of monthly median foF2 and M(3000)F2 all over the world.
 *  The coefficients correspond to low (R12 = 0) and high (R12 = 100) solar
 *  activity conditions.
 */
typedef struct F2_layer_ionosonde_coefficients_st {
  /** CCIR folder */
  char* pFolder;
  /** Spherical harmonic coefficients for F2 loaded from the CCIR maps*/
  F2_coefficient_array_t F2
    [ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
  /** Spherical harmonic coefficients for M(3000)F2  loaded from the CCIR maps*/
  Fm3_coefficient_array_t Fm3
    [ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT];
} F2_layer_ionosonde_coefficients_t;
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

/** Interpolated/extrapolated coefficients for the foF2 and M(3000)F2
 * for the solar activity.
 */
typedef struct F2_layer_interpolated_coefficients_st {
  /** Interpolated/extrapolated coefficients for foF2 */
  F2_coefficient_array_t F2;
  /** Interpolated/extrapolated coefficients for M(3000)F2 */
  Fm3_coefficient_array_t Fm3;
  /** Effective sun spot number */
  double_t effective_sun_spot_count;
  bool valid;
} F2_layer_interpolated_coefficients_t;

/** Fourier coefficients for the foF2 and M(3000)F2 */
typedef struct F2_fourier_coefficients_st {
  /** Fourier coefficients for foF2 */
  F2_fourier_coefficient_array_t CF2;
  /** Fourier coefficients for M(3000)F2 */
  Fm3_fourier_coefficient_array_t Cm3;
  /** Are the Fourier coefficients valid or must be recalculated? */
  bool valid;
} F2_fourier_coefficients_t;

/** F2 Fourier coefficients context */
typedef struct F2_layer_fourier_coeff_context_st {
  /** indication of the time at which the Fourier coefficients are required */
  NeQuickG_time_t time;
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
  /** Spherical harmonic coefficients from the CCIR maps */
  F2_layer_ionosonde_coefficients_t ionosonde;
#endif
  /** Interpolated/extrapolated coefficients for the foF2 and M(3000)F2
   * taking inti account the solar activity
   */
  F2_layer_interpolated_coefficients_t interpolated;
  /** F2 Fourier coefficients for the foF2 and M(3000)F2*/
  F2_fourier_coefficients_t fourier;
} F2_layer_fourier_coeff_context_t;

#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && !defined(_doxygen)
#define F2_layer_fourier_coefficients_init(pContext, pCCIR_folder) \
  F2_layer_fourier_coefficients_init(pContext)
#endif

/** Initialize the Coefficients for the foF2 and M(3000)F2.
 *
 * @param[out] pContext F2 Fourier coefficients context
 * @param[in] pCCIR_folder CCIR folder
 */
extern int32_t F2_layer_fourier_coefficients_init(
  F2_layer_fourier_coeff_context_t* const pContext,
  const char* const pCCIR_folder);

#if defined(FTR_MODIP_CCIR_AS_CONSTANTS) && !defined(_doxygen)
#define F2_layer_fourier_coefficients_close(pContext)
#else
/** Free resources allocated bt the F2 Fourier coefficients context
 * @param[in, out] pContext F2 Fourier coefficients context
 */
extern void F2_layer_fourier_coefficients_close(
  F2_layer_fourier_coeff_context_t* const pContext);
#endif

#ifdef FTR_UNIT_TEST
/** Unit test for CF2 and Cm3
 * @param[in, out] pContext F2 Fourier coefficients context
 * @param[in] pTime time context
 * @param[in] Azr Effective sun spot number
 */
extern int32_t F2_layer_fourier_coefficients_interpolate(
  F2_layer_fourier_coeff_context_t* const pContext,
  const NeQuickG_time_t * const pTime,
  const double_t Azr);
#endif // FTR_UNIT_TEST

/** Compute CF2, the array of interpolated coefficients for
 * foF2 and Cm3, the array of interpolated coefficients for
 * M(3000)F2.
 * See 2.5.5.3.2.
 *
 * @param[in, out] pContext F2 Fourier coefficients context
 * @param[in] pTime time context
 * @param[in] pSolar_activity solar activity
 */
extern int32_t F2_layer_fourier_coefficients_get(
  F2_layer_fourier_coeff_context_t* const pContext,
  const NeQuickG_time_t * const pTime,
  const solar_activity_t* const pSolar_activity);

#endif // NEQUICK_G_JRC_IONO_F2_LAYER_COEFFICIENTS_H
