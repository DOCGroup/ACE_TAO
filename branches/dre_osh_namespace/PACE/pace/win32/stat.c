/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/stat.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/sys/stat.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/stat.inl"
#endif /* ! PACE_HAS_INLINE */

#include <windows.h>
#include <direct.h>

#if (PACE_HAS_POSIX_FS_UOF)
int
pace_win32_mkdir (const char * path, pace_mode_t mode)
{
  PACE_UNUSED_ARG (mode);
# if defined (__IBMCPP__) && (__IBMCPP__ >= 400)
  PACE_OSCALL_RETURN (_mkdir ((char *) path), int, -1);
# elif defined (PACE_WINCE)
  PACE_WIN32CALL_RETURN
    (PACE_ADAPT_RETVAL
     (CreateDirectory (path, NULL), pace_result_), int, -1);
# else
  PACE_OSCALL_RETURN (_mkdir (path), int, -1);
# endif /* PACE_WIN32 */
}
#endif /* PACE_HAS_POSIX_FS_UOF */
