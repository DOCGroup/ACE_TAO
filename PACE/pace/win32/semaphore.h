/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/semaphore.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SEMAPHORE_H_WIN32
#define PACE_SEMAPHORE_H_WIN32

#include <windows.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_SEM_T
#define PACE_SEM_T
  /* Temporary fix until we emulate these funcs.
     typedef mqd_t pace_mqd_t;
   */
  typedef HANDLE pace_sem_t;
#endif /* PACE_SEM_T */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SEMAPHORE_H_WIN32 */
