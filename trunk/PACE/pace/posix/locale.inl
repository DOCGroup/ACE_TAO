/* $Id$ -*- C -*-

 * ===========================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    locale.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ===========================================================================
 */


PACE_INLINE
char *
pace_setlocale (int category,
                const char * locale)
{
  return setlocale (category,
                    locale);
}
