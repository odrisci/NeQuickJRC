/** NeQuickG solar auxiliary routines
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_solar.h"

#include "NeQuickG_JRC_math_utils.h"
#include "NeQuickG_JRC_time.h"

#define NEQUICK_G_JRC_SOLAR_MEAN_ANOMALY_DEGREE_PER_DAY (0.9856)
#define NEQUICK_G_JRC_SOLAR_MEAN_ANOMALY_OFFSET_DEGREE (-3.289)

#define NEQUICK_G_JRC_SOLAR_ECLIPTIC_ZERO_COEFF_DEGREE \
  (NEQUICKG_JRC_DEGREE_TO_RAD(282.634))
#define NEQUICK_G_JRC_SOLAR_ECLIPTIC_FIRST_COEFF_DEGREE \
  (NEQUICKG_JRC_DEGREE_TO_RAD(1.916))
#define NEQUICK_G_JRC_SOLAR_ECLIPTIC_SECOND_COEFF_DEGREE \
  (NEQUICKG_JRC_DEGREE_TO_RAD(0.02))

#define NEQUICK_G_JRC_SOLAR_SIN_EARTH_AXIAL_TILT (0.39782)

/** Zenith angle of sun at day night transition (degress). */
#define NEQUICK_G_JRC_SOLAR_ZENITH_ANGLE_AT_DAY_NIGHT_TRANSITION_DEGREE \
  (86.23292796211615)

#define NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST1 (90.0)
#define NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST2 (0.24)
#define NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST3 (20.0)
#define NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST4 (0.2)
#define NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST5 (12.0)

static double_t get_mean_anomaly(
  double_t time_days) {
  return NEQUICKG_JRC_DEGREE_TO_RAD(
           NEQUICK_G_JRC_SOLAR_MEAN_ANOMALY_DEGREE_PER_DAY * time_days +
            NEQUICK_G_JRC_SOLAR_MEAN_ANOMALY_OFFSET_DEGREE);
}

/** The ecliptic longitude of the Sun is:
 *  L + 1.916sin(g) + 0.020sin(2g) where
 *   L is the mean longitude of the Sun
 *   L = 280.460 + 0.9856*n
 * and g is the mean anomaly of the Sun
 *   g = 357.528 + 0.9856*n
 *
 *   where n is the number of days (positive or negative)
 *    since Greenwich noon, Terrestrial Time,
 *    on 1 January 2000 (J2000.0).
 *
 */
static double_t get_ecliptic_longitude(
  double_t time_days) {

  double_t mean_anomaly_rad = get_mean_anomaly(time_days);

  double_t ecliptic_longitude_rad =
   (mean_anomaly_rad +
    NEQUICK_G_JRC_SOLAR_ECLIPTIC_ZERO_COEFF_DEGREE);

  ecliptic_longitude_rad +=
    NEQUICK_G_JRC_SOLAR_ECLIPTIC_FIRST_COEFF_DEGREE *
       sin(mean_anomaly_rad);

  ecliptic_longitude_rad +=
    NEQUICK_G_JRC_SOLAR_ECLIPTIC_SECOND_COEFF_DEGREE *
       sin(2 * mean_anomaly_rad);

  return ecliptic_longitude_rad;
}

angle_restricted_t
solar_get_declination(const NeQuickG_time_t * const pTime) {

  double_t day_of_year = NeQuickG_time_get_day_of_year(pTime);

  double_t ecliptic_longitude_rad = get_ecliptic_longitude(day_of_year);

  angle_restricted_t solar_declination;
  {
    solar_declination.sin = NEQUICK_G_JRC_SOLAR_SIN_EARTH_AXIAL_TILT *
                              sin(ecliptic_longitude_rad);
    solar_declination.cos = get_cos_from_sin(solar_declination.sin);
  }
  return solar_declination;
}

/** The solar hour angle is an expression of time,
 *  expressed in angular measurement,
 *  from solar noon.
 *  At solar noon the hour angle is 0 degree,
 *  with the time before solar noon expressed as negative degrees,
 *  and the local time after solar noon expressed as positive degrees.
 */
static double_t get_cosinus_hour_angle(
  const NeQuickG_time_t * const pTime,
  const position_t* const pPosition) {

  double_t local_time =
    NeQuickG_time_get_local(pTime, pPosition->longitude.degree);

  return cos((NEQUICK_G_JRC_PI *
     (NEQUICK_G_JRC_TIME_HALF_DAY_IN_HOURS - local_time)) /
      NEQUICK_G_JRC_TIME_HALF_DAY_IN_HOURS);
}

/** Compute solar zenith angle chi [deg] for the given location.
 * The solar zenith angle is the angle between the zenith and the centre
 * of the Sun's disc
 * cosinus_angle = sin(local_latitude)*sin(declination_sun) +
 *                 cos(local_latitude)*cos(declination_sun)*cos(hour_angle)
 */
static angle_t get_zenith_angle(
  const position_t* const pPosition,
  const NeQuickG_time_t* const pTime,
  const angle_restricted_t* const pSolar_declination) {

  angle_t zenith;
  zenith.cos =
    (pPosition->latitude.sin * pSolar_declination->sin) +
      (pPosition->latitude.cos * pSolar_declination->cos *
        get_cosinus_hour_angle(pTime, pPosition));

  angle_set_fom_cos(&zenith);
  return zenith;
}

double_t
  solar_get_effective_zenith_angle(
  const position_t* const pPosition,
  const NeQuickG_time_t * const pTime,
  const angle_restricted_t* const pSolar_declination) {

  angle_t zenith_angle =
    get_zenith_angle(pPosition, pTime, pSolar_declination);

  double_t func1 =
    NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST1 -
    NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST2 *
    NeQuickG_exp(NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST3 -
                (NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST4 *
                 zenith_angle.degree));

  return NeQuickG_func_join(
    func1,
    zenith_angle.degree,
    NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST5,
    zenith_angle.degree -
      NEQUICK_G_JRC_SOLAR_ZENITH_ANGLE_AT_DAY_NIGHT_TRANSITION_DEGREE);
}

double_t solar_get_longitude(
  const NeQuickG_time_t* const pTime) {
  return (
    NEQUICKG_JRC_DEGREE_TO_RAD(
      NEQUICK_G_JRC_TIME_DEGREE_PER_HOUR*pTime->utc -
      NEQUICK_G_JRC_LONG_MAX_VALUE_DEGREE));
}

#undef NEQUICK_G_JRC_SOLAR_MEAN_ANOMALY_DEGREE_PER_DAY
#undef NEQUICK_G_JRC_SOLAR_MEAN_ANOMALY_OFFSET_DEGREE
#undef NEQUICK_G_JRC_SOLAR_ECLIPTIC_ZERO_COEFF_DEGREE
#undef NEQUICK_G_JRC_SOLAR_ECLIPTIC_FIRST_COEFF_DEGREE
#undef NEQUICK_G_JRC_SOLAR_ECLIPTIC_SECOND_COEFF_DEGREE
#undef NEQUICK_G_JRC_SOLAT_SIN_EARTH_AXIAL_TILT
#undef NEQUICK_G_JRC_SOLAR_ZENITH_ANGLE_AT_DAY_NIGHT_TRANSITION_DEGREE
#undef NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST1
#undef NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST2
#undef NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST3
#undef NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST4
#undef NEQUICK_G_JRC_SOLAR_EFFECTIVE_ZENITH_CONST5
