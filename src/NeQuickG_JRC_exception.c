/**
 * NeQuickG test program exceptions
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_main
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_exception.h"

#include <stdlib.h>

int16_t g_NeQuick_exception_type;
int16_t g_NeQuick_exception_reason;
bool g_NeQuick_exception_last_exception_caught;
NeQuick_exception_jumpBuffer_t *g_NeQuick_exception_pJumpBuffer;

void NeQuick_exception_throw(
  int16_t exceptionType, int16_t exceptionReason) {
  if (g_NeQuick_exception_pJumpBuffer == NULL) {
    exit(NEQUICK_FATAL_EXCEPTION);
  }
  g_NeQuick_exception_type = exceptionType;
  g_NeQuick_exception_reason = exceptionReason;
  longjmp(g_NeQuick_exception_pJumpBuffer->jumpBuffer, true);
}
