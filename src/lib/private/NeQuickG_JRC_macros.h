/** NeQuickG utility macros
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef IF_SYSTEM_MACROS_H
#define IF_SYSTEM_MACROS_H

#ifdef FTR_UNIT_TEST
#include <errno.h>
#endif
#include <math.h>
#ifdef FTR_UNIT_TEST
#include <stdio.h>
#endif

#if defined(__GNUC__) || defined(__GNUG__)
#ifndef max
/** Returns the largest of a and b. If both are equivalent, a is returned.
 * @param[in] a Value to compare.
 * @param[in] b Value to compare.
 * @return The largest of the values passed as arguments.
 */
#define max(a, b) ((a) >= (b) ? (a) : (b))
#endif
#ifndef min
/** Returns the smallest  of a and b. If both are equivalent, a is returned.
 * @param[in] a Value to compare.
 * @param[in] b Value to compare.
 * @return The smallest  of the values passed as arguments.
 */
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#endif

#ifdef FTR_UNIT_TEST
/** Print error message to stderr optionally preceding it with the custom message specified in _text.
 * @param[in] _text C string containing a custom message to be printed before the error message itself.
 * @return none
 */
#define LOG_ERROR(_text) (perror( _text))
#endif

/** Threshold based comparison of doubles.
 * Uses fabs to compute the difference between the two numbers and compares the difference to a threshold value.
 * @param[in] d1 first double
 * @param[in] d2 second double
 * @param[in] threshold threshold value (&epsilon;)
 * @return true if operands are equal
 */
#define THRESHOLD_COMPARE(d1, d2, threshold) \
  (fabs(d1-d2) < threshold)

/** Checks if the double is zero using a Threshold based comparison approach
 * @param[in] d double
 * @param[in] threshold threshold value (&epsilon;)
 * @return true if value is close to zero
 */
#define THRESHOLD_COMPARE_TO_ZERO(d, threshold) \
  (fabs(d) < threshold)
#endif  // !IF_SYSTEM_MACROS_H
