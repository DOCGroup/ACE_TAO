// -*- C++ -*-
// $Id$

#include "ace/config-all.h"           /* Need ACE_TRACE */
#include "ace/Object_Manager_Base.h"
#include "ace/OS_NS_string.h"
#include "ace/Global_Macros.h"
#include "ace/os_include/os_errno.h"
#include "ace/os_include/os_search.h"
#include "ace/os_include/os_limits.h"
//#include "ace/TSS_T.h"

#if defined(ACE_WCHAR_IN_STD_NAMESPACE)
# define ACE_WCHAR_STD_NAMESPACE std
#else
# define ACE_WCHAR_STD_NAMESPACE ACE_STD_NAMESPACE
#endif /* ACE_WCHAR_IN_STD_NAMESPACE */

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

ACE_INLINE int
ACE_OS::atoi (const wchar_t *s)
{
#if defined (ACE_WIN32)
  ACE_OSCALL_RETURN (::_wtoi (s), int, -1);
#else /* ACE_WIN32 */
  return ACE_OS::atoi (ACE_TEXT_TO_CHAR_IN (s));
#endif /* ACE_WIN32 */
}

#if defined (atop)
#  undef atop
#endif /* atop */

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
  void *p = reinterpret_cast<void *> (ip);
  return p;
}

ACE_INLINE void *
ACE_OS::atop (const wchar_t *s)
{
#  if defined (ACE_WIN64)
  __int64 ip = ::_wtoi64 (s);
#  else
  int ip = ACE_OS::atoi (s);
#  endif /* ACE_WIN64 */
  void *p = reinterpret_cast<void *> (ip);
  return p;
}

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

ACE_INLINE wchar_t *
ACE_OS::getenv (const wchar_t *symbol)
{
#if defined (ACE_LACKS_ENV)
  ACE_UNUSED_ARG (symbol);
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_WIN32)
  ACE_OSCALL_RETURN (::_wgetenv (symbol), wchar_t *, 0);
#else
// Including ACE_TSS_T.h causes error!
/*
  wchar_t* init = 0;
  static ACE_TSS< wchar_t* > wvalue (&init);
  ACE::String_Conversion::Allocator_malloc<wchar_t>().free(*wvalue);
  *wvalue.ts_object() = ACE_TEXT_TO_MALLOC_WCHAR_OUT (
                ACE_OS::getenv (ACE_TEXT_TO_CHAR_IN (symbol)));
  return *wvalue;
*/
  static wchar_t* wvalue = 0;
  ACE::String_Conversion::Allocator_malloc<wchar_t>().free(wvalue);
  wvalue = ACE_TEXT_TO_MALLOC_WCHAR_OUT (
                ACE_OS::getenv (ACE_TEXT_TO_CHAR_IN (symbol)));
  return wvalue;
#endif /* ACE_LACKS_ENV */
}

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

ACE_INLINE wchar_t *
ACE_OS::itoa (int value, wchar_t *string, int radix)
{
#if defined (ACE_LACKS_ITOW)
  return ACE_OS::itow_emulation (value, string, radix);
#else /* ACE_LACKS_ITOW */
  return ::_itow (value, string, radix);
#endif /* ACE_LACKS_ITOW */
}

ACE_INLINE ACE_HANDLE
ACE_OS::mkstemp (char *s)
{
#if !defined (ACE_LACKS_MKSTEMP)
  return ::mkstemp (s);
#else
  s[0] = 0;
  return ACE_OS::mkstemp_emulation (ACE_TEXT_TO_TCHAR_INOUT (s) );
#endif  /* !ACE_LACKS_MKSTEMP */
}

ACE_INLINE ACE_HANDLE
ACE_OS::mkstemp (wchar_t *s)
{
  s[0] = 0;
#  if !defined (ACE_LACKS_MKSTEMP)
  ACE::String_Conversion::Convert_InOut<char, wchar_t> convert( s, MAX_PATH );
  return ::mkstemp (convert.c_str());
#  else
  ACE::String_Conversion::Convert_InOut<ACE_TCHAR, ACE_ANTI_TCHAR> convert( s, MAX_PATH );
  return ACE_OS::mkstemp_emulation (convert.c_str());
#  endif  /* !ACE_LACKS_MKSTEMP */
}


ACE_INLINE char *
ACE_OS::mktemp (char *s)
{
# if defined (ACE_WIN32)
  return ::_mktemp (s);
# elif !defined (ACE_LACKS_MKTEMP)
  return ::mktemp (s);
# else
  ACE_UNUSED_ARG( s );
  ACE_NOTSUP_RETURN (0);
# endif
}

ACE_INLINE wchar_t *
ACE_OS::mktemp (wchar_t *s)
{
#if defined (ACE_LACKS_MKTEMP)
  ACE_UNUSED_ARG( s );
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_WIN32)
  return ::_wmktemp (s);
#else
  // For narrow-char filesystems, we must convert the wide-char input to
  // a narrow-char string for mktemp(), then convert the name back to
  // wide-char for the caller.
  s[0] = 0;
  ACE::String_Conversion::Convert_InOut<char, wchar_t> convert( s, MAX_PATH );
  if (::mktemp (convert.c_str()) == 0)
    return 0;
  return s;
#endif
}

#if defined(INTEGRITY)
extern "C" {
  int putenv(char *string);
}
#endif

ACE_INLINE int
ACE_OS::putenv (const char *string)
{
  ACE_OS_TRACE ("ACE_OS::putenv");
#if defined (ACE_HAS_WINCE) || defined (ACE_PSOS)
  // WinCE and pSOS don't have the concept of environment variables.
  ACE_UNUSED_ARG (string);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_LACKS_ENV)
  ACE_UNUSED_ARG (string);
  ACE_NOTSUP_RETURN (0);
#else /* ! ACE_HAS_WINCE && ! ACE_PSOS */
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::putenv (const_cast <char *> (string)), int, -1);
#endif /* ACE_HAS_WINCE */
}

#if defined (ACE_WIN32)
ACE_INLINE int
ACE_OS::putenv (const wchar_t *string)
{
  ACE_OS_TRACE ("ACE_OS::putenv");
#if defined (ACE_HAS_WINCE)
  // WinCE doesn't have the concept of environment variables.
  ACE_UNUSED_ARG (string);
  ACE_NOTSUP_RETURN (-1);
#else
  ACE_OSCALL_RETURN (::_wputenv (string), int, -1);
#endif /* ACE_HAS_WINCE */
}
#endif /* ACE_WIN32 */

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

#if !defined (ACE_HAS_WINCE)
#  if !defined (ACE_LACKS_REALPATH)
ACE_INLINE char *
ACE_OS::realpath (const char *file_name,
                  char *resolved_name)
{
#    if defined (ACE_WIN32)
  return ::_fullpath (resolved_name, file_name, PATH_MAX);
#    else /* ACE_WIN32 */
  return ::realpath (file_name, resolved_name);
#    endif /* ! ACE_WIN32 */
}
#  endif /* !ACE_LACKS_REALPATH */

ACE_INLINE wchar_t *
ACE_OS::realpath (const wchar_t *file_name,
                  wchar_t *resolved_name)
{
#    if defined (ACE_WIN32)
  return ::_wfullpath (resolved_name, file_name, PATH_MAX);
#    else /* ACE_WIN32 */
  char n_resolved[PATH_MAX];
  if (0 != ACE_OS::realpath ( ACE_TEXT_TO_CHAR_IN (file_name), n_resolved))
    {
      ACE_OS::string_copy (resolved_name, n_resolved, PATH_MAX);
      return resolved_name;
    }
  return 0;
#    endif /* ! ACE_WIN32 */
}
#endif /* ACE_HAS_WINCE */

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
  if (str[0] == ACE_LIB_TEXT ('$'))
    {
#  if defined (ACE_WIN32)
      // Always use the ACE_TCHAR for Windows.
      ACE_TCHAR *temp = 0;
      if ((temp = ACE_OS::getenv (&str[1])) != 0)
        return ACE_OS::strdup (temp);
#  else
      // Use char * for environment on non-Windows.
      char *temp = 0;
      if ((temp = ACE_OS::getenv (ACE_TEXT_TO_CHAR_IN(&str[1])) != 0)
        return ACE_OS::strdup (ACE_TEXT_TO_TCHAR_IN (temp));
#  endif /* ACE_WIN32 */
      return ACE_OS::strdup (str);
    }
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

ACE_INLINE double
ACE_OS::strtod (const wchar_t *s, wchar_t **endptr)
{
  #if !defined (ACE_LACKS_WCSTOD)
    return ACE_WCHAR_STD_NAMESPACE::wcstod (s, endptr);
  #else
    ACE_UNUSED_ARG( s );
    ACE_NOTSUP_RETURN (0);
  #endif
}

ACE_INLINE long
ACE_OS::strtol (const char *s, char **ptr, int base)
{
#if defined (ACE_LACKS_STRTOL)
  return ACE_OS::strtol_emulation (s, ptr, base);
#else  /* ACE_LACKS_STRTOL */
  return ::strtol (s, ptr, base);
#endif /* ACE_LACKS_STRTOL */
}

ACE_INLINE long
ACE_OS::strtol (const wchar_t *s, wchar_t **ptr, int base)
{
#if !defined (ACE_LACKS_WCSTOL)
  return ACE_WCHAR_STD_NAMESPACE::wcstol (s, ptr, base);
#else
    ACE_UNUSED_ARG( s );
    ACE_UNUSED_ARG( ptr );
    ACE_UNUSED_ARG( base );
    ACE_NOTSUP_RETURN (0);
#endif
}

ACE_INLINE unsigned long
ACE_OS::strtoul (const char *s, char **ptr, int base)
{
#if defined (ACE_LACKS_STRTOUL)
  return ACE_OS::strtoul_emulation (s, ptr, base);
#else /* ACE_LACKS_STRTOUL */
  return ::strtoul (s, ptr, base);
#endif /* ACE_LACKS_STRTOUL */
}

ACE_INLINE unsigned long
ACE_OS::strtoul (const wchar_t *s, wchar_t **ptr, int base)
{
#if !defined (ACE_LACKS_WCSTOUL)
  return ACE_WCHAR_STD_NAMESPACE::wcstoul (s, ptr, base);
#else
  ACE_UNUSED_ARG( s );
  ACE_UNUSED_ARG( ptr );
  ACE_UNUSED_ARG( base );
  ACE_NOTSUP_RETURN (0);
#endif
}

ACE_INLINE int
ACE_OS::system (const ACE_TCHAR *s)
{
  // ACE_OS_TRACE ("ACE_OS::system");
#if defined (CHORUS) || defined (ACE_HAS_WINCE) || defined(ACE_PSOS)
  ACE_UNUSED_ARG (s);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
  ACE_OSCALL_RETURN (::_wsystem (s), int, -1);
#elif defined(ACE_TANDEM_T1248_PTHREADS)
  ACE_OSCALL_RETURN (::spt_system (s), int, -1);
#else
  ACE_OSCALL_RETURN (::system (ACE_TEXT_TO_CHAR_IN (s)), int, -1);
#endif /* !CHORUS */
}
