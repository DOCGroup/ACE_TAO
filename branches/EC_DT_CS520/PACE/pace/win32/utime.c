/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/utime.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/utime.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/utime.inl"
#endif /* ! PACE_HAS_INLINE */

#if (PACE_HAS_POSIX_FS_UOF)
int
win32_utime (const char * path, const pace_utimbuf * times)
{
  PACE_UNUSED_ARG (path);
  PACE_UNUSED_ARG (times);

  PACE_ERRNO_NO_SUPPORT_RETURN (-1);

#if 0
  /* Not yet working */
     char * path2 = (char*) malloc (sizeof (path));
     pace_strcpy (path2, path);

     pace_utimbuf * times2 = (pace_utimbuf*) malloc (sizeof (pace_utimbuf);
     memcpy (times2, times, sizeof times);
     return _utime (path2, times2);
#endif
}
#endif /* PACE_HAS_POSIX_FS_UOF */
