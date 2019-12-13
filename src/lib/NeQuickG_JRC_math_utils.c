/** NeQuickG mathematical auxiliary routines
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_math_utils.h"

#include <math.h>

/** Maximum exponent */
#define NEQUICKG_EXP_MAX_ABS (80.0)
/** Maximum value for exponential ie. exp( > 80) */
#define NEQUICKG_EXP_MAX_VALUE (5.5406e34)
/** Minimum value for exponential ie. exp( < -80) */
#define NEQUICKG_EXP_MIN_VALUE  (1.8049e-35)

double_t NeQuickG_exp(double_t power) {

  double_t result;

  if (power > NEQUICKG_EXP_MAX_ABS) {
    result = NEQUICKG_EXP_MAX_VALUE;
  } else if (power < -NEQUICKG_EXP_MAX_ABS) {
    result = NEQUICKG_EXP_MIN_VALUE;
  } else {
    result = exp(power);
  }
  return result;
}

double_t NeQuickG_func_join(
  double_t func1,
  double_t func2,
  double_t alpha,
  double_t x) {

  double_t temp = NeQuickG_exp(alpha * x);
  return(((func1 * temp) + func2) /
          (temp + 1.0));
}

void NeQuickG_get_power_array(
  size_t grade,
  double_t first_term,
  double_t negligible_value,
  double_t* const p) {
  p[0] = first_term;
  for (size_t i = 1; i < grade; i++) {
    double_t term = p[i - 1] * p[0];
    if (fabs(term) <= negligible_value) {
      term = 0.0;
    }
    p[i] = term;
  }
}

double_t NeQuickG_square(double_t val) {
  return (val * val);
}

double_t get_cos_from_sin(const double_t sin_rad) {
  return sqrt(1.0 - sin_rad * sin_rad);
}

double_t get_sin_from_cos(const double_t cos_rad) {
  return sqrt(1.0 - cos_rad * cos_rad);
}

#undef NEQUICKG_EXP_MIN_VALUE
#undef NEQUICKG_EXP_MAX_VALUE
#undef NEQUICKG_EXP_MAX_ABS
