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
  /* Emulation not yet finished! Not yet working! */

#if defined (__BORLANDC__)
  return utime (path, (const struct utimbuf*)times);  
#else /* __BORLANDC__ */
  /* Not quite the perfect answer... win32 doesn't take const args. */
  return _utime ((char*)path, (struct _utimbuf*)times);  
#endif /* __BORLANDC__ */
}
#endif /* PACE_HAS_POSIX_FS_UOF */
