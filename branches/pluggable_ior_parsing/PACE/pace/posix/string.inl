/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/string.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <string.h>

PACE_INLINE
void *
pace_memccpy (void * s1, const void * s2, int c, size_t n)
{
  return memccpy (s1, s2, c, n);
}

PACE_INLINE
const void *
pace_memchr (const void * s, int c, size_t n)
{
  return memchr (s, c, n);
}

PACE_INLINE
int
pace_memcmp (const void * s1, const void * s2, size_t n)
{
  return memcmp (s1, s2, n);
}

PACE_INLINE
void *
pace_memcpy (void * s1, const void * s2, size_t n)
{
  return memcpy (s1, s2, n);
}

PACE_INLINE
void *
pace_memmove (void * s1, const void * s2, size_t n)
{
  return memmove (s1, s2, n);
}

PACE_INLINE
void *
pace_memset (void * s, int c, size_t n)
{
  return memset (s, c, n);
}

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
char *
pace_strdup (const char * s1)
{
  return strdup (s1);
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
# else  /* !PACE_HAS_REENTRANT */
  PACE_SET_ERRNO (PACE_ERRNO_LACKS_POSIX_C_SOURCE);
  return 0;
# endif /* !PACE_HAS_REENTRANT */
}


