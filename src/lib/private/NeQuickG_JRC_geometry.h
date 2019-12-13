/** NeQuickG auxiliary geometric defines and routines.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_GEOMETRY_H
#define NEQUICK_G_JRC_GEOMETRY_H

#include <math.h>

/** Value of mathematical constant &pi;*2 */
#define NEQUICK_G_JRC_TWO_PI (6.283185307179586476925287)

/** Value of mathematical constant &pi; */
#define NEQUICK_G_JRC_PI (NEQUICK_G_JRC_TWO_PI / 2.0)

/** Circle degrees. */
#define NEQUICK_G_JRC_CIRCLE_DEGREES (360.0)

/** Degrees to radians */
#define NEQUICK_G_JRC_DEGREE_TO_RADIAN (NEQUICK_G_JRC_PI / 180.0)

/** Radians to degrees. */
#define NEQUICK_G_JRC_RADIAN_TO_DEGREE (180.0 / NEQUICK_G_JRC_PI)

/** Degrees to radians
 * @param _degree degrees
 */
#define NEQUICKG_JRC_DEGREE_TO_RAD(_degree) \
  ((_degree) * NEQUICK_G_JRC_DEGREE_TO_RADIAN)

/** Radians to degrees
 * @param _rad radians
 */
#define NEQUICKG_JRC_RAD_TO_DEGREE(_rad) \
  ((_rad) * NEQUICK_G_JRC_RADIAN_TO_DEGREE)

/** angle with precomputed values. */
typedef struct NeQuickG_angle_st {
  /** value in degrees */
  double_t degree;
  /** value in radians */
  double_t rad;
  /** sinus of the angle */
  double_t sin;
  /** cosinus of the angle */
  double_t cos;
} angle_t;

/** angle with limited precomputed values. */
typedef struct NeQuickG_angle_restricted_st {
  /** sinus of the angle */
  double_t sin;
  /** cosinus of the angle */
  double_t cos;
} angle_restricted_t;

/* Calculate angle value in radians, the sinus and the cosinus given the angle in degrees
 * @param[out] pAngle angle context
 * @param[in] angle_degree angle value in degrees
 */
extern void angle_set(angle_t* const pAngle, double_t angle_degree);

/* Calculate angle value in degrees, the sinus and the cosinus given the angle in radians
 * @param[out] pAngle angle context
 * @param[in] angle_rad angle value in radians
 */
extern void angle_set_rad(angle_t* const pAngle, double_t angle_rad);

/* Calculate angle value in degrees, radians and the cosinus given the sinus of the angle.
 * @param[in, out] pAngle angle context
 */
extern void angle_set_fom_sin(angle_t* const pAngle);

/* Calculate angle value in degrees, radians and the sinus given the cosinus of the angle.
 * @param[in, out] pAngle angle context
 */
extern void angle_set_fom_cos(angle_t* const pAngle);

/* Calculate the sinus and cosinus of the angle
 * @param[out] pAngle angle context
 * @param[in] angle_rad angle value in radians
 */
extern void angle_restricted_set(angle_restricted_t* const pAngle, double_t angle_rad);

#endif //NEQUICK_G_JRC_GEOMETRY_H
