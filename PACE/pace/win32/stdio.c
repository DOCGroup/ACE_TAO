/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/stdio.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/stdio.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/stdio.inl"
#endif /* ! PACE_HAS_INLINE */


#if (PACE_HAS_POSIX_FM_UOF)
int
pace_win32_fseek (FILE * stream, long offset, int whence)
{
# if SEEK_SET != FILE_BEGIN \
  || SEEK_CUR != FILE_CURRENT \
  || SEEK_END != FILE_END
  //#error Windows NT is evil AND rude!
  switch (whence)
    {
    case SEEK_SET:
      whence = FILE_BEGIN;
      break;
    case SEEK_CUR:
      whence = FILE_CURRENT;
      break;
    case SEEK_END:
      whence = FILE_END;
      break;
    default:
      errno = EINVAL;
      return -1; // rather safe than sorry
    }
#   endif  /* SEEK_SET != FILE_BEGIN
           || SEEK_CUR != FILE_CURRENT
           || SEEK_END != FILE_END */
  PACE_OSCALL_RETURN (fseek (stream, offset, whence), int, -1);
}
#endif /* PACE_HAS_POSIX_FM_UOF */
