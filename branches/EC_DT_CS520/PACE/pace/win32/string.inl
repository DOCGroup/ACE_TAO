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

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_memchr (const void *s, int c, pace_size_t n)
{
  return memchr (s, c, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_memcmp (const void *s1, const void *s2, pace_size_t n)
{
  return memcmp (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_memcpy (void * s1, const void * s2, pace_size_t n)
{
  return memcpy (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_memmove (void *s1, const void *s2, pace_size_t n)
{
  return memmove (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_memset (void *s, int c, pace_size_t n)
{
  return memset (s, c, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strcat (char * s1, const char * s2)
{
  return strcat (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strncat (char * s1, const char * s2, size_t n)
{
  return strncat (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strchr (const char * s, int c)
{
  return strchr (s, c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strrchr (const char * s, int c)
{
  return strrchr (s, c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_strcmp (const char * s1, const char * s2)
{
  return strcmp (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_strncmp (const char * s1, const char * s2, size_t n)
{
  return strncmp (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strcpy (char * s1, const char * s2)
{
  return strcpy (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strncpy (char * s1, const char * s2, size_t n)
{
  return strncpy (s1, s2, n);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
size_t
pace_strcspn (const char * s1, const char * s2)
{
  return strcspn (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
size_t
pace_strspn (const char * s1, const char * s2)
{
  return strspn (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
size_t
pace_strlen (const char * s)
{
  return strlen (s);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strpbrk (const char * s1, const char * s2)
{
  return strpbrk (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strstr (const char * s1, const char * s2)
{
  return strstr (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_strtok (char * s1, const char * s2)
{
  return strtok (s1, s2);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
char *
pace_strtok_r (char * s, const char * sep, char ** lasts)
{
  return strtok_r (s, sep, lasts);
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */


