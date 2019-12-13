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
#ifndef NEQUICK_EXCEPTION_H
#define NEQUICK_EXCEPTION_H

#include <setjmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "NeQuickG_JRC_error.h"
#define NEQUICK_USAGE_EXCEPTION (-7)
#define NEQUICK_FATAL_EXCEPTION (-8)

/** Error code: input file cannot be opened. */
#define NEQUICK_INPUT_DATA_FILE_REASON (12)
#define NEQUICK_INPUT_DATA_BAD_COEFF (13)
#define NEQUICK_INPUT_DATA_BAD_DATA (14)
#define NEQUICK_USAGE_BAD_FORMAT (15)
#define NEQUICK_USAGE_BAD_COUNT (16)

typedef struct exceptionJumpBuffer_s {
  jmp_buf jumpBuffer;
  struct exceptionJumpBuffer_s *pPrev;
} NeQuick_exception_jumpBuffer_t;

/** Exception type for last exception, set in #NeQuick_exception_throw. */
extern int16_t g_NeQuick_exception_type;

/** Exception reason for last exception, set in #NeQuick_exception_throw. */
extern int16_t g_NeQuick_exception_reason;

/** true if the last exception was caught, false otherwise */
extern bool g_NeQuick_exception_last_exception_caught;

/** Set to NULL if no exception handler is active. */
extern NeQuick_exception_jumpBuffer_t *g_NeQuick_exception_pJumpBuffer;

//lint -emacro(789, NEQUICK_TRY) Assign address of auto to static. This is safe as the exception handling will unwind all.
#define NEQUICK_TRY \
{ \
  NeQuick_exception_jumpBuffer_t _pExceptionJumpBuffer; \
  g_NeQuick_exception_type = (int16_t)NEQUICK_OK; \
  g_NeQuick_exception_last_exception_caught = (setjmp(_pExceptionJumpBuffer.jumpBuffer) == 0); \
  if ((int16_t)NEQUICK_OK == g_NeQuick_exception_type) { \
      _pExceptionJumpBuffer.pPrev = g_NeQuick_exception_pJumpBuffer; \
      g_NeQuick_exception_pJumpBuffer = &_pExceptionJumpBuffer;

#define NEQUICK_CATCH(x) \
    } else if ((int16_t)(x) == g_NeQuick_exception_type) { \
      g_NeQuick_exception_pJumpBuffer = _pExceptionJumpBuffer.pPrev; \
      g_NeQuick_exception_last_exception_caught = true;

#define NEQUICK_CATCH_ALL_EXCEPTIONS \
    } else { \
      g_NeQuick_exception_pJumpBuffer = _pExceptionJumpBuffer.pPrev; \
      g_NeQuick_exception_last_exception_caught = true;

//lint -emacro(717, NEQUICK_END) while (0) intentional to force ';'
#define NEQUICK_END \
  } \
  g_NeQuick_exception_pJumpBuffer = _pExceptionJumpBuffer.pPrev; \
  if (!g_NeQuick_exception_last_exception_caught) { \
    NeQuick_exception_throw(g_NeQuick_exception_type, g_NeQuick_exception_reason); \
  } \
} do { } while (false)

#define NEQUICK_RETHROW() (NeQuick_exception_throw(g_NeQuick_exception_type, g_NeQuick_exception_reason))

#define NEQUICK_EXCEPTION_THROW(exceptionType, exceptionReason, ...) \
  fprintf(stderr, "Nequick exception. File = %s, line = %i\n", __FILE__, __LINE__); \
  fprintf(stderr,__VA_ARGS__); \
  fprintf(stderr, "\n"); \
  NeQuick_exception_throw(exceptionType, exceptionReason);

extern void NeQuick_exception_throw(int16_t exceptionType, int16_t exceptionReason)
#if defined(__GNUC__) || defined(__GNUG__)
__attribute__((noreturn))
#endif
;

#endif // NEQUICK_EXCEPTION_H

