/* $Id$ -*- C -*-

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/locale.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#include <locale.h>

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_lconv *
pace_localeconv (void)
{
  PACE_TRACE("pace_localeconv");

  return localeconv ();
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
char *
pace_setlocale (int category, const char * locale)
{
  PACE_TRACE("pace_setlocale");

  return setlocale (category, locale);
}
#endif /* PACE_HAS_POSIX_MP_UOF */
