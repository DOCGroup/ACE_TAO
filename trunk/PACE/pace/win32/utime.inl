/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/utime.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/string.h"

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_utime (const char * path,
            const pace_utimbuf * times)
{
  /* Not yet working
     char * path2 = (char*) malloc (sizeof (path));
     pace_strcpy (path2, path);

     pace_utimbuf * times2 = (pace_utimbuf*) malloc (sizeof (pace_utimbuf);
     memcpy (times2, times, sizeof times);
     return _utime (path2, times2);
   */

  /* Not quite the perfect answer... win32 doesn't take const args. */
  return _utime ((char*)path, (struct _utimbuf*)times);  
}
#endif /* PACE_HAS_POSIX_FS_UOF */
