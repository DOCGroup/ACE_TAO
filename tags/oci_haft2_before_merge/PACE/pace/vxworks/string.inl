/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/string.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#include <string.h>
#include "pace/string.h"

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_memchr (const void *s, int c, pace_size_t n)
{
  PACE_TRACE("pace_memchr");

  return (void*) memchr (s, c, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_memcmp (const void *s1, const void *s2, pace_size_t n)
{
  PACE_TRACE("pace_memcmp");

  return memcmp (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_memcpy (void * s1, const void * s2, pace_size_t n)
{
  PACE_TRACE("pace_memcpy");

  return memcpy (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_memmove (void *s1, const void *s2, pace_size_t n)
{
  PACE_TRACE("pace_memmove");

  return memmove (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_BROKEN_INLINE
void *
pace_memset (void *s, int c, pace_size_t n)
{
  PACE_TRACE("pace_memset");

  return memset (s, c, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */


#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strcat (char * s1, const char * s2)
{
  PACE_TRACE("pace_strcat");

  return strcat (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strncat (char * s1, const char * s2, size_t n)
{
  PACE_TRACE("pace_strncat");

  return strncat (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strchr (const char * s, int c)
{
  PACE_TRACE("pace_strchr");

  return (char*) strchr (s, c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strrchr (const char * s, int c)
{
  PACE_TRACE("pace_strrchr");

  return (char*) strrchr (s, c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_BROKEN_INLINE
int
pace_strcmp (const char * s1, const char * s2)
{
  PACE_TRACE("pace_strcmp");

  return strcmp (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_BROKEN_INLINE
int
pace_strncmp (const char * s1, const char * s2, size_t n)
{
  PACE_TRACE("pace_strncmp");

  return strncmp (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_BROKEN_INLINE
char *
pace_strcpy (char * s1, const char * s2)
{
  PACE_TRACE("pace_strcpy");

  return strcpy (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strncpy (char * s1, const char * s2, size_t n)
{
  PACE_TRACE("pace_strncpy");

  return strncpy (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_BROKEN_INLINE
size_t
pace_strcspn (const char * s1, const char * s2)
{
  PACE_TRACE("pace_strcspn");

  return strcspn (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
size_t
pace_strspn (const char * s1, const char * s2)
{
  PACE_TRACE("pace_strspn");

  return strspn (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
size_t
pace_strlen (const char * s)
{
  PACE_TRACE("pace_strlen");

  return strlen (s);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_BROKEN_INLINE
char *
pace_strpbrk (const char * s1, const char * s2)
{
  PACE_TRACE("pace_strpbrk");

  return (char*) strpbrk (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strstr (const char * s1, const char * s2)
{
  PACE_TRACE("pace_strstr");

  return (char*) strstr (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strtok (char * s1, const char * s2)
{
  PACE_TRACE("pace_strtok");

  return strtok (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
char *
pace_strtok_r (char * s, const char * sep, char ** lasts)
{
  PACE_TRACE("pace_strtok_r");

  return strtok_r (s, sep, lasts);
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */
