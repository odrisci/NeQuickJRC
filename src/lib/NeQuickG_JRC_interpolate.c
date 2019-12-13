/** NeQuickG Third Order Interpolation function
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_interpolate.h"

#include <stddef.h>
#include <stdint.h>

/** 2.5.7.1. Threshold for interpolation */
#define NEQUICK_G_JRC_INTERPOL_EPSILON (5.0e-11)

/** Index for interpolation point 0 */
#define NEQUICK_G_JRC_INTERPOL_POINT_0_INDEX (0)
/** Index for interpolation point 1 */
#define NEQUICK_G_JRC_INTERPOL_POINT_1_INDEX (1)
/** Index for interpolation point 2 */
#define NEQUICK_G_JRC_INTERPOL_POINT_2_INDEX (2)
/** Index for interpolation point 3 */
#define NEQUICK_G_JRC_INTERPOL_POINT_3_INDEX (3)

/** First Constant used in interpolation_third_order */
#define NEQUICK_G_JRC_INTERPOL_FIRST_CONST (9.0)
/** Second Constant used in interpolation_third_order */
#define NEQUICK_G_JRC_INTERPOL_SECOND_CONST (16.0)

double_t interpolation_third_order(
  const double_t interpol_points[NEQUICK_G_JRC_INTERPOLATE_POINT_COUNT],
  double_t offset) {
  double_t result = 0.0;

  if (fabs(offset) < NEQUICK_G_JRC_INTERPOL_EPSILON) {
    return interpol_points[NEQUICK_G_JRC_INTERPOL_POINT_1_INDEX];
  }

  double_t sum_1_2 = interpol_points[NEQUICK_G_JRC_INTERPOL_POINT_2_INDEX] +
                   interpol_points[NEQUICK_G_JRC_INTERPOL_POINT_1_INDEX];
  double_t grad_2_1 = interpol_points[NEQUICK_G_JRC_INTERPOL_POINT_2_INDEX] -
                    interpol_points[NEQUICK_G_JRC_INTERPOL_POINT_1_INDEX];

  double_t sum_3_0 = interpol_points[NEQUICK_G_JRC_INTERPOL_POINT_3_INDEX] +
                   interpol_points[NEQUICK_G_JRC_INTERPOL_POINT_0_INDEX];
  double_t grad_3_0 = (interpol_points[NEQUICK_G_JRC_INTERPOL_POINT_3_INDEX] -
                     interpol_points[NEQUICK_G_JRC_INTERPOL_POINT_0_INDEX])/
                     (double_t)((double_t)NEQUICK_G_JRC_INTERPOLATE_POINT_COUNT - 1.0);

  double_t coefficients[NEQUICK_G_JRC_INTERPOLATE_POINT_COUNT];
  {
     size_t i = 0;
     coefficients[i++] =
      (NEQUICK_G_JRC_INTERPOL_FIRST_CONST*sum_1_2) - sum_3_0;
     coefficients[i++] =
      (NEQUICK_G_JRC_INTERPOL_FIRST_CONST*grad_2_1) - grad_3_0;
     coefficients[i++] = (sum_3_0 - sum_1_2);
     coefficients[i] = (grad_3_0 - grad_2_1);
  }
  double_t delta = (2.0 * offset) - 1.0;
  for (int_fast8_t i = NEQUICK_G_JRC_INTERPOLATE_POINT_COUNT - 1;
       i >= 0x00 ; i--) {
    result = (result * delta) + coefficients[i];
  }
  return (result / NEQUICK_G_JRC_INTERPOL_SECOND_CONST);
}

#undef NEQUICK_G_JRC_INTERPOL_EPSILON
#undef NEQUICK_G_JRC_INTERPOL_POINT_0_INDEX
#undef NEQUICK_G_JRC_INTERPOL_POINT_1_INDEX
#undef NEQUICK_G_JRC_INTERPOL_POINT_2_INDEX
#undef NEQUICK_G_JRC_INTERPOL_POINT_3_INDEX
#undef NEQUICK_G_JRC_INTERPOL_FIRST_CONST
#undef NEQUICK_G_JRC_INTERPOL_SECOND_CONST
