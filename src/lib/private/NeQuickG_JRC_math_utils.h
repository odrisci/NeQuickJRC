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
#ifndef NEQUICK_G_JRC_MATH_UTILS_H
#define NEQUICK_G_JRC_MATH_UTILS_H

#include <math.h>
#include <stddef.h>

/** Check if the number is odd
 * @param[in] _i operand
 * @return true if odd
 */
#define IS_ODD(_i) ((_i % 2) == 1)

/** A clipped exponential function always returns valid output.
 *  See F.2.12.2.
 *
 * @param[in] power Power for exponential function
 * @return Clipped exponential value
 */
extern double_t NeQuickG_exp(double_t power);

/** Allows smooth joining of functions f1 and f2
 * (i.e. continuous first derivatives) at origin.
 * Alpha determines width of transition region.
 * Calculates value of joined functions at x.
 * This function calculates:<br><CENTER>
 *         (func1 * e<SUP>alpha*X</SUP>) + func2<br>
 *         ---------------------------------------------<br>
 *          e<SUP>alpha*X</SUP> + 1<br></CENTER>
 * See F.2.12.1.
 *
 * @param[in] func1 Input term for NeqJoin computation
 * @param[in] func2 Input term for NeqJoin computation
 * @param[in] alpha Input term for NeqJoin computation
 * @param[in] x Input term for NeqJoin computation
 */
extern double_t NeQuickG_func_join(
  double_t func1,
  double_t func2,
  double_t alpha,
  double_t x);

/** Calculates array of powers
 *
 * @param[in] grade grade of the array to calculate
 * @param[in] base the base a<SUB>0</SUB>
 * @param[in] zero_threshold treshold to consider a term as 0.0
 * @param[out] pOut output array, must be of size grade
 */
extern void NeQuickG_get_power_array(
  size_t grade,
  double_t base,
  double_t zero_threshold,
  double_t* const pOut);

/** Calculates the square of a number. See F.2.12.3.
 *
 * @param[in] value Input value
 * @return Square of the input value = value*value
 */
extern double_t NeQuickG_square(double_t value);

/** Get the cosinus from the sinus
 *
 * @param[in] sin sinus
 * @return cosinus
 */
extern double_t get_cos_from_sin(const double_t sin);

/** Get the sinus from the cosinus
 *
 * @param[in] cos cosinus
 * @return sinus
 */
extern double_t get_sin_from_cos(const double_t cos);

#endif // !NEQUICK_G_JRC_MATH_UTILS_H
