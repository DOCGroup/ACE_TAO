// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   ace
//
// = FILENAME
//   OS_String.h
//
// = AUTHOR
//   (Originally in OS.h)
//   Doug Schmidt <schmidt@cs.wustl.edu>, Jesper S. M|ller
//   <stophph@diku.dk>, and a cast of thousands...
//
// ============================================================================

#ifndef ACE_OS_STRING_H
#define ACE_OS_STRING_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_Export.h"
#include /**/ <stddef.h>

class ACE_OS_Export ACE_OS_String
  // = TITLE
  //     This class is a wrapper for string operations
  //     Mainly includes the stuff found in string.h and ctype.h
  //
  // = DESCRIPTION
{
public:
  static char *strcat (char *s, const char *t);
  static char *strncat (char *s, const char *t, size_t len);
#if defined (ACE_HAS_WCHAR)
  static wchar_t *strcat (wchar_t *s, const wchar_t *t);
  static wchar_t *strncat (wchar_t *s, const wchar_t *t, size_t len);
#endif /* ACE_HAS_WCHAR */

  static char *strchr (char *s, int c);
  static const char *strchr (const char *s, int c);
  static char *strrchr (char *s, int c);
  static const char *strrchr (const char *s, int c);
  static char *strnchr (char *s, int c, size_t len);
  static const char *strnchr (const char *s, int c, size_t len);
#if defined (ACE_HAS_WCHAR)
  static wchar_t *strchr (wchar_t *s, wint_t c);
  static const wchar_t *strchr (const wchar_t *s, wint_t c);
  static wchar_t *strrchr (wchar_t *s, wint_t c);
  static const wchar_t *strrchr (const wchar_t *s, wint_t c);
  static wchar_t *strnchr (wchar_t *s, wint_t c, size_t len);
  static const wchar_t *strnchr (const wchar_t *s, wint_t c, size_t len);
#endif /* ACE_HAS_WCHAR */

  static int strcmp (const char *s, const char *t);
  static int strncmp (const char *s, const char *t, size_t len);
  static int strcasecmp (const char *s, const char *t);
  static int strncasecmp (const char *s, const char *t, size_t len);
#if defined (ACE_HAS_WCHAR)
  static int strcmp (const wchar_t *s, const wchar_t *t);
  static int strncmp (const wchar_t *s, const wchar_t *t, size_t len);
  static int strcasecmp (const wchar_t *s, const wchar_t *t);
  static int strncasecmp (const wchar_t *s, const wchar_t *t, size_t len);
#endif /* ACE_HAS_WCHAR */

  static char *strcpy (char *s, const char *t);
  static char *strecpy (char *des, const char *src);
  // Copies <src> to <des>, returning a pointer to the end of the
  // copied region, rather than the beginning, as <strcpy> does.
  static char *strncpy (char *s, const char *t, size_t len);
#if defined (ACE_HAS_WCHAR)
  static wchar_t *strcpy (wchar_t *s, const wchar_t *t);
  static wchar_t *strecpy (wchar_t *s, const wchar_t *t);
  static wchar_t *strncpy (wchar_t *s, const wchar_t *t, size_t len);
#endif /* ACE_HAS_WCHAR */

  static char *strpbrk (char *s1, const char *s2);
  static const char *strpbrk (const char *s1, const char *s2);
#if defined (ACE_HAS_WCHAR)
  static wchar_t *strpbrk (wchar_t *s1, const wchar_t *s2);
  static const wchar_t *strpbrk (const wchar_t *s1, const wchar_t *s2);
#endif /* ACE_HAS_WCHAR */

  static size_t strcspn (const char *s, const char *reject);

  static size_t strspn (const char *s1, const char *s2);
#if defined (ACE_HAS_WCHAR)
  static size_t strspn (const wchar_t *s1, const wchar_t *s2);
#endif /* ACE_HAS_WCHAR */

  static char *strstr (char *s, const char *t);
  static const char *strstr (const char *s, const char *t);
  static char *strnstr (char *s, const char *t, size_t len);
  static const char *strnstr (const char *s, const char *t, size_t len);
#if defined (ACE_HAS_WCHAR)
  static wchar_t *strstr (wchar_t *s, const wchar_t *t);
  static const wchar_t *strstr (const wchar_t *s, const wchar_t *t);
  static wchar_t *strnstr (wchar_t *s, const wchar_t *t, size_t len);
  static const wchar_t *strnstr (const wchar_t *s, 
                                 const wchar_t *t, 
                                 size_t len);
#endif /* ACE_HAS_WCHAR */

  static char *strdup (const char *s); // Uses malloc
#if defined (ACE_HAS_WCHAR)
  static wchar_t *strdup (const wchar_t *s);
#endif /* ACE_HAS_WCHAR */

  static size_t strlen (const char *s);
#if defined (ACE_HAS_WCHAR)
  static size_t strlen (const wchar_t *s);
#endif /* ACE_HAS_WCHAR */

  static char *strtok (char *s, const char *tokens);
  static char *strtok_r (char *s, const char *tokens, char **lasts);
#if defined (ACE_HAS_WCHAR)
  static wchar_t *strtok (wchar_t *s, const wchar_t *tokens);
#endif /* ACE_HAS_WCHAR */

  static long strtol (const char *s, char **ptr, int base);
  static unsigned long strtoul (const char *s, char **ptr, int base);
  static double strtod (const char *s, char **endptr);
#if defined (ACE_HAS_WCHAR)
  static long strtol (const wchar_t *s, wchar_t **ptr, int base);
  static unsigned long strtoul (const wchar_t *s, wchar_t **ptr, int base);
  static double strtod (const wchar_t *s, wchar_t **endptr);
#endif /* ACE_HAS_WCHAR */

  static int to_lower (int c);
#if defined (ACE_HAS_WCHAR)
  static wint_t to_lower (wint_t c);
#endif /* ACE_HAS_WCHAR */

  // = A set of wrappers for memory copying operations.
  static int memcmp (const void *t, const void *s, size_t len);
  static const void *memchr (const void *s, int c, size_t len);
  static void *memcpy (void *t, const void *s, size_t len);
  static void *memmove (void *t, const void *s, size_t len);
  static void *memset (void *s, int c, size_t len);

  static int ace_isspace (const ACE_TCHAR s);
  static int ace_isprint (const ACE_TCHAR s);

private:
  // = These are emulation or platform specific versions of methods.
  static const void *memchr_emulation (const void *s, int c, size_t len);
  static char *strchr_emulation (char *s, int c);
  static const char *strchr_emulation (const char *s, int c);
  static char *strtok_r_emulation (char *s, const char *tokens, char **lasts);
  static char *strrchr_emulation (char *s, int c);
  static const char *strrchr_emulation (const char *s, int c);
  static int strcasecmp_emulation (const char *s, const char *t);

#if defined (ACE_HAS_WCHAR)
  static wchar_t *strchr_emulation (wchar_t *s, wint_t c);
  static const wchar_t *strchr_emulation (const wchar_t *s, wint_t c);
  static wchar_t *strrchr_emulation (wchar_t *s, wint_t c);
  static const wchar_t *strrchr_emulation (const wchar_t *s, wint_t c);
  static int strcasecmp_emulation (const wchar_t *s, const wchar_t *t);
#endif /* ACE_HAS_WCHAR */
};

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_String.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/post.h"
#endif /* ACE_OS_STRING_H */
