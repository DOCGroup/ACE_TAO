// -*- C++ -*-
// $Id$

#include "ace/Object_Manager_Base.h"
#include "ace/OS_NS_string.h"
#include "ace/Global_Macros.h"

// Doesn't need a macro since it *never* returns!

ACE_INLINE void
ACE_OS::_exit (int status)
{
  ACE_OS_TRACE ("ACE_OS::_exit");
#if defined (VXWORKS)
  ::exit (status);
#elif defined (ACE_PSOSIM)
  ::u_exit (status);
#elif defined (ACE_PSOS)
# if defined (ACE_PSOS_LACKS_PREPC)  /* pSoS TM does not support exit. */
  ACE_UNUSED_ARG (status);
  return;
# else
  ::exit (status);
# endif /* defined (ACE_PSOS_LACKS_PREPC) */
#elif !defined (ACE_HAS_WINCE)
  ::_exit (status);
#else
  ::TerminateProcess (::GetCurrentProcess (),
                      status);
#endif /* VXWORKS */
}

ACE_INLINE void
ACE_OS::abort (void)
{
#if !defined (ACE_HAS_WINCE)
  ::abort ();
#else
  // @@ CE doesn't support abort?
  exit (1);
#endif /* !ACE_HAS_WINCE */
}

ACE_INLINE int
ACE_OS::atexit (ACE_EXIT_HOOK func)
{
  return ACE_OS_Object_Manager::instance ()->at_exit (func);
}

ACE_INLINE int
ACE_OS::atoi (const char *s)
{
  ACE_OSCALL_RETURN (::atoi (s), int, -1);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS::atoi (const wchar_t *s)
{
#if defined (ACE_WIN32)
  ACE_OSCALL_RETURN (::_wtoi (s), int, -1);
#else /* ACE_WIN32 */
  return ACE_OS::atoi (ACE_Wide_To_Ascii(s).char_rep());
#endif /* ACE_WIN32 */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE void *
ACE_OS::atop (const char *s)
{
  ACE_TRACE ("ACE_OS::atop");
  // It would be nice to make use of Basic_Types.h here, but that
  // file relies on OS.h. Fortunately, most platforms have int
  // the same as pointer size (IA32, IA64), with Win64 being the
  // exception.
#if defined (ACE_WIN64)
  __int64 ip = ::_atoi64 (s);
#else
  int ip = ::atoi (s);
#endif /* ACE_WIN64 */
  void *p = ACE_reinterpret_cast (void *, ip);
  return p;
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE void *
ACE_OS::atop (const wchar_t *s)
{
#  if defined (ACE_WIN64)
  __int64 ip = ::_wtoi64 (s);
#  else
  int ip = ACE_OS::atoi (s);
#  endif /* ACE_WIN64 */
  void *p = ACE_reinterpret_cast (void *, ip);
  return p;
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE void *
ACE_OS::bsearch (const void *key,
                 const void *base,
                 size_t nel,
                 size_t size,
                 ACE_COMPARE_FUNC compar)
{
#if !defined (ACE_LACKS_BSEARCH)
  return ::bsearch (key, base, nel, size, compar);
#else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (base);
  ACE_UNUSED_ARG (nel);
  ACE_UNUSED_ARG (size);
  ACE_UNUSED_ARG (compar);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_LACKS_BSEARCH */
}

ACE_INLINE char *
ACE_OS::getenv (const char *symbol)
{
  ACE_OS_TRACE ("ACE_OS::getenv");
#if defined (ACE_LACKS_ENV)
  ACE_UNUSED_ARG (symbol);
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_PSOS)
  ACE_UNUSED_ARG (symbol);
  ACE_NOTSUP_RETURN (0);
#else /* ACE_PSOS */
  ACE_OSCALL_RETURN (::getenv (symbol), char *, 0);
#endif /* ACE_LACKS_ENV */
}

#if defined (ACE_HAS_WCHAR) && defined (ACE_WIN32)
ACE_INLINE wchar_t *
ACE_OS::getenv (const wchar_t *symbol)
{
#if defined (ACE_LACKS_ENV)
  ACE_UNUSED_ARG (symbol);
  ACE_NOTSUP_RETURN (0);
#else
  ACE_OSCALL_RETURN (::_wgetenv (symbol), wchar_t *, 0);
#endif /* ACE_LACKS_ENV */
}
#endif /* ACE_HAS_WCHAR && ACE_WIN32 */

ACE_INLINE char *
ACE_OS::itoa (int value, char *string, int radix)
{
#if !defined (ACE_HAS_ITOA)
  return ACE_OS::itoa_emulation (value, string, radix);
#elif defined (ACE_ITOA_EQUIVALENT)
  return ACE_ITOA_EQUIVALENT (value, string, radix);
#else /* !ACE_HAS_ITOA */
  return ::itoa (value, string, radix);
#endif /* !ACE_HAS_ITOA */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS::itoa (int value, wchar_t *string, int radix)
{
#if defined (ACE_LACKS_ITOW)
  return ACE_OS::itow_emulation (value, string, radix);
#else /* ACE_LACKS_ITOW */
  return ::_itow (value, string, radix);
#endif /* ACE_LACKS_ITOW */
}
#endif /* ACE_HAS_WCHAR */

#if !defined (ACE_LACKS_MKSTEMP)
ACE_INLINE ACE_HANDLE
ACE_OS::mkstemp (ACE_TCHAR *s)
{
  return ::mkstemp (s);
}
#endif /* !ACE_LACKS_MKSTEMP */

#if !defined (ACE_LACKS_MKTEMP)
ACE_INLINE ACE_TCHAR *
ACE_OS::mktemp (ACE_TCHAR *s)
{
# if defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
  return ::_wmktemp (s);
# elif defined (ACE_WIN32)
  return ::_mktemp (s);
# else /* ACE_WIN32 */
  return ::mktemp (s);
# endif /* ACE_WIN32 */
}
#endif /* !ACE_LACKS_MKTEMP */

#if defined(INTEGRITY)
extern "C" {
  int putenv(char *string);
}
#endif

ACE_INLINE int
ACE_OS::putenv (const ACE_TCHAR *string)
{
  ACE_OS_TRACE ("ACE_OS::putenv");
#if defined (ACE_HAS_WINCE) || defined (ACE_PSOS)
  // WinCE and pSOS don't have the concept of environment variables.
  ACE_UNUSED_ARG (string);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_LACKS_ENV)
  ACE_UNUSED_ARG (string);
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
  ACE_OSCALL_RETURN (::_wputenv (string), int, -1);
#else /* ! ACE_HAS_WINCE && ! ACE_PSOS */
  // VxWorks declares ::putenv with a non-const arg.
  ACE_OSCALL_RETURN (::putenv ((char *) string), int, -1);
#endif /* ACE_HAS_WINCE */
}

ACE_INLINE void
ACE_OS::qsort (void *base,
               size_t nel,
               size_t width,
               ACE_COMPARE_FUNC compar)
{
#if !defined (ACE_LACKS_QSORT)
  ::qsort (base, nel, width, compar);
#else
  ACE_UNUSED_ARG (base);
  ACE_UNUSED_ARG (nel);
  ACE_UNUSED_ARG (width);
  ACE_UNUSED_ARG (compar);
#endif /* !ACE_LACKS_QSORT */
}

ACE_INLINE int
ACE_OS::rand (void)
{
  ACE_OS_TRACE ("ACE_OS::rand");
  ACE_OSCALL_RETURN (::rand (), int, -1);
}

#if !defined (ACE_WIN32)

ACE_INLINE int
ACE_OS::rand_r (ACE_RANDR_TYPE &seed)
{
  ACE_OS_TRACE ("ACE_OS::rand_r");
# if defined (ACE_HAS_REENTRANT_FUNCTIONS) && \
    !defined (ACE_LACKS_RAND_REENTRANT_FUNCTIONS)
#   if defined (DIGITAL_UNIX)
  ACE_OSCALL_RETURN (::_Prand_r (&seed), int, -1);
#   elif defined (HPUX_10)
  // rand() is thread-safe on HP-UX 10.  rand_r's signature is not consistent
  // with latest POSIX and will change in a future HP-UX release so that it
  // is consistent.  At that point, this #elif section can be changed or
  // removed, and just call rand_r.
  ACE_UNUSED_ARG (seed);
  ACE_OSCALL_RETURN (::rand(), int, -1);
#   elif defined (ACE_HAS_BROKEN_RANDR)
  ACE_OSCALL_RETURN (::rand_r (seed), int, -1);
#   else
  ACE_OSCALL_RETURN (::rand_r (&seed), int, -1);
#   endif /* DIGITAL_UNIX */
# else
  ACE_UNUSED_ARG (seed);
  ACE_OSCALL_RETURN (::rand (), int, -1);
# endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}

#else /* ACE_WIN32 */

ACE_INLINE int
ACE_OS::rand_r (ACE_RANDR_TYPE& seed)
{
  ACE_OS_TRACE ("ACE_OS::rand_r");

  long new_seed = (long)(seed);
  if (new_seed == 0)
    new_seed = 0x12345987;
  long temp = new_seed / 127773;
  new_seed = 16807 * (new_seed - temp * 127773) - 2836 * temp;
  if (new_seed < 0)
    new_seed += 2147483647;
  (seed) = (unsigned int)new_seed;
  return (int)(new_seed & RAND_MAX);
}

#endif /* !ACE_WIN32 */

ACE_INLINE ACE_EXIT_HOOK
ACE_OS::set_exit_hook (ACE_EXIT_HOOK exit_hook)
{
  ACE_EXIT_HOOK old_hook = exit_hook_;
  exit_hook_ = exit_hook;
  return old_hook;
}

ACE_INLINE void
ACE_OS::srand (u_int seed)
{
  ACE_OS_TRACE ("ACE_OS::srand");
  ::srand (seed);
}

// Return a dynamically allocated duplicate of <str>, substituting the
// environment variable if <str[0] == '$'>.  Note that the pointer is
// allocated with <ACE_OS::malloc> and must be freed by
// <ACE_OS::free>.

ACE_INLINE ACE_TCHAR *
ACE_OS::strenvdup (const ACE_TCHAR *str)
{
#if defined (ACE_HAS_WINCE)
  // WinCE doesn't have environment variables so we just skip it.
  return ACE_OS::strdup (str);
#elif defined (ACE_LACKS_ENV)
  ACE_UNUSED_ARG (str);
  ACE_NOTSUP_RETURN (0);
#else
  ACE_TCHAR *temp = 0;

  if (str[0] == ACE_LIB_TEXT ('$')
      && (temp = ACE_OS::getenv (&str[1])) != 0)
    return ACE_OS::strdup (temp);
  else
    return ACE_OS::strdup (str);
#endif /* ACE_HAS_WINCE */
}

#if !defined (ACE_LACKS_STRTOD)
ACE_INLINE double
ACE_OS::strtod (const char *s, char **endptr)
{
  return ::strtod (s, endptr);
}
#endif /* !ACE_LACKS_STRTOD */

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOD)
ACE_INLINE double
ACE_OS::strtod (const wchar_t *s, wchar_t **endptr)
{
  return ::wcstod (s, endptr);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOD */

ACE_INLINE long
ACE_OS::strtol (const char *s, char **ptr, int base)
{
#if defined (ACE_LACKS_STRTOL)
  return ACE_OS::strtol_emulation (s, ptr, base);
#else  /* ACE_LACKS_STRTOL */
  return ::strtol (s, ptr, base);
#endif /* ACE_LACKS_STRTOL */
}

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOL)
ACE_INLINE long
ACE_OS::strtol (const wchar_t *s, wchar_t **ptr, int base)
{
  return ::wcstol (s, ptr, base);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOL */

ACE_INLINE unsigned long
ACE_OS::strtoul (const char *s, char **ptr, int base)
{
#if defined (ACE_LACKS_STRTOUL)
  return ACE_OS::strtoul_emulation (s, ptr, base);
#else /* ACE_LACKS_STRTOUL */
  return ::strtoul (s, ptr, base);
#endif /* ACE_LACKS_STRTOUL */
}

#if defined (ACE_HAS_WCHAR) && !defined (ACE_LACKS_WCSTOUL)
ACE_INLINE unsigned long
ACE_OS::strtoul (const wchar_t *s, wchar_t **ptr, int base)
{
  return ::wcstoul (s, ptr, base);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_WCSTOUL */

ACE_INLINE int
ACE_OS::system (const ACE_TCHAR *s)
{
  // ACE_OS_TRACE ("ACE_OS::system");
#if defined (CHORUS) || defined (ACE_HAS_WINCE) || defined(ACE_PSOS)
  ACE_UNUSED_ARG (s);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
  ACE_OSCALL_RETURN (::_wsystem (s), int, -1);
#else
  ACE_OSCALL_RETURN (::system (s), int, -1);
#endif /* !CHORUS */
}

