/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/sched.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SCHED_H_WIN32
#define PACE_SCHED_H_WIN32

  /* These definitions are used in other files.
   * Separate the declarations from the definitions to avoid circular
   * inclusion. ie: some other funcs just need the typedefs and structs. */

#include "pace/win32/schedtypes.h"
#include "pace/time.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SCHED_H_WIN32 */
