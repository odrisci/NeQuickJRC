/** NeQuickG Time constants and routines
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_time.h"

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_error.h"
#include "NeQuickG_JRC_macros.h"

/** Average number of days in a month.*/
#define NEQUICK_G_JRC_TIME_AVG_MONTH_IN_DAYS (30.5)
/** Typical mid-month point in days. */
#define NEQUICK_G_JRC_TIME_MID_MONTH_POINT_IN_DAYS (15.0)
/** Number of hours in a day */
#define NEQUICK_G_JRC_TIME_DAY_IN_HOURS (24.0)

#define NEQUICK_G_JRC_TIME_CONSTANT_TO_COMPUTE_DAY (18.0)

#define NEQUICK_G_JRC_TIME_UTC_EPSILON (1.0e-20)

void NeQuickG_time_init(
  NeQuickG_time_t * const pTime) {
  pTime->month = 0;
  pTime->utc = 0x00;
}

int32_t NeQuickG_time_set(
  NeQuickG_time_t * const pTime,
  const uint_fast8_t month,
  const double_t utc) {

  if ((month < NEQUICK_G_JRC_MONTH_JANUARY) ||
      (month > NEQUICK_G_JRC_MONTH_DECEMBER)) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_INPUT_DATA,
      NEQUICK_ERROR_CODE_BAD_MONTH,
      "Invalid month %u", month);
  }

  if ((utc < 0.0) ||
      (utc > NEQUICK_G_JRC_TIME_DAY_IN_HOURS)) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_INPUT_DATA,
      NEQUICK_ERROR_CODE_BAD_UTC,
       "Invalid UTC %lf, must be between 0 and 24", utc);
  }

  pTime->month = month;
  pTime->utc = utc;
  return NEQUICK_OK;
}

bool NeQuickG_time_utc_is_equal(
  double_t op1,
  double_t op2) {
  return THRESHOLD_COMPARE(op1, op2, NEQUICK_G_JRC_TIME_UTC_EPSILON);
}

double_t NeQuickG_time_get_day_of_year(
  const NeQuickG_time_t * const pTime) {

  // compute day of year at the middle of the month.
  double_t day_of_year_at_middle_of_month =
    NEQUICK_G_JRC_TIME_AVG_MONTH_IN_DAYS*(double_t)pTime->month -
    NEQUICK_G_JRC_TIME_MID_MONTH_POINT_IN_DAYS;

  // compute time [days].
  double_t time_days = day_of_year_at_middle_of_month;
  time_days += (NEQUICK_G_JRC_TIME_CONSTANT_TO_COMPUTE_DAY - pTime->utc)
               / NEQUICK_G_JRC_TIME_DAY_IN_HOURS;
  return time_days;
}

double_t NeQuickG_time_get_local(
  const NeQuickG_time_t * const pTime,
  double_t longitude_degree) {
  double_t local_time = pTime->utc +
    (longitude_degree / NEQUICK_G_JRC_TIME_DEGREE_PER_HOUR);

  if (local_time < 0.0) {
    local_time += NEQUICK_G_JRC_TIME_DAY_IN_HOURS;
  } else if (local_time >= NEQUICK_G_JRC_TIME_DAY_IN_HOURS) {
    local_time -= NEQUICK_G_JRC_TIME_DAY_IN_HOURS;
  }
  return local_time;
}

#undef NEQUICK_G_JRC_TIME_UTC_EPSILON

#undef NEQUICK_G_JRC_TIME_CONSTANT_TO_COMPUTE_DAY
#undef NEQUICK_G_JRC_TIME_DAY_IN_HOURS
#undef NEQUICK_G_JRC_TIME_MID_MONTH_POINT_IN_DAYS
#undef NEQUICK_G_JRC_TIME_AVG_MONTH_IN_DAYS
