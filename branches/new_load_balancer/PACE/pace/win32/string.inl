/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/string.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <string.h>

PACE_INLINE
char *
pace_strcat (char * s1, const char * s2)
{
  return strcat (s1, s2);
}

PACE_INLINE
char *
pace_strncat (char * s1, const char * s2, size_t n)
{
  return strncat (s1, s2, n);
}

PACE_INLINE
const char *
pace_strchr (const char * s, int c)
{
  return strchr (s, c);
}

PACE_INLINE
const char *
pace_strrchr (const char * s, int c)
{
  return strrchr (s, c);
}

PACE_INLINE
int
pace_strcmp (const char * s1, const char * s2)
{
  return strcmp (s1, s2);
}

PACE_INLINE
int
pace_strncmp (const char * s1, const char * s2, size_t n)
{
  return strncmp (s1, s2, n);
}

PACE_INLINE
char *
pace_strcpy (char * s1, const char * s2)
{
  return strcpy (s1, s2);
}

PACE_INLINE
char *
pace_strncpy (char * s1, const char * s2, size_t n)
{
  return strncpy (s1, s2, n);
}

PACE_INLINE
size_t
pace_strcspn (const char * s1, const char * s2)
{
  return strcspn (s1, s2);
}

PACE_INLINE
size_t
pace_strspn (const char * s1, const char * s2)
{
  return strspn (s1, s2);
}

PACE_INLINE
size_t
pace_strlen (const char * s)
{
  return strlen (s);
}

PACE_INLINE
const char *
pace_strpbrk (const char * s1, const char * s2)
{
  return strpbrk (s1, s2);
}

PACE_INLINE
const char *
pace_strstr (const char * s1, const char * s2)
{
  return strstr (s1, s2);
}

PACE_INLINE
char *
pace_strtok (char * s1, const char * s2)
{
  return strtok (s1, s2);
}

PACE_INLINE
char *
pace_strtok_r (char * s, const char * sep, char ** lasts)
{
# if defined (PACE_HAS_REENTRANT)
  return strtok_r (s, sep, lasts);
# else  /* ! PACE_HAS_REENTRANT */
  PACE_UNUSED_ARG (s);
  PACE_UNUSED_ARG (sep);
  PACE_UNUSED_ARG (lasts);
  PACE_ERRNO_NO_SUPPORT ();
  return 0;
# endif /* ! PACE_HAS_REENTRANT */
}
