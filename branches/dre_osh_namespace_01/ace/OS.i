// -*- C++ -*-
// $Id$

#if !defined (ACE_HAS_INLINED_OSCALLS)
# undef ACE_INLINE
# define ACE_INLINE
#endif /* ACE_HAS_INLINED_OSCALLS */

#if defined (ACE_HAS_XLI)
# include /**/ <xliuser.h>
#endif /* ACE_HAS_XLI */

#if !defined (ACE_HAS_CPLUSPLUS_HEADERS)
# include /**/ <libc.h>
# include /**/ <osfcn.h>
#endif /* ACE_HAS_CPLUSPLUS_HEADERS */

#if defined (ACE_HAS_SYSENT_H)
# include /**/ <sysent.h>
#endif /* ACE_HAS_SYSENT_H_*/

#if defined (ACE_USES_STD_NAMESPACE_FOR_STDC_LIB) && \
            (ACE_USES_STD_NAMESPACE_FOR_STDC_LIB != 0)
using std::bsearch;
using std::qsort;
# if defined (ACE_WIN32)
using std::_tzset;
# else
using std::tzset;
# endif
using std::ctime;
using std::localtime;
using std::gmtime;
using std::asctime;
using std::strftime;
#endif /* ACE_USES_STD_NAMESPACE_FOR_STDC_LIB */

#if !defined (ACE_LACKS_MALLOC_H)
# include /**/ <malloc.h>
#endif /* ACE_LACKS_MALLOC_H */

#if !defined (ACE_WIN32)

// Matthew Stevens 7-10-95 Fix GNU GCC 2.7 for memchr() problem.
# if defined (ACE_HAS_GNU_CSTRING_H)
// Define this file to keep /usr/include/memory.h from being included.
#   include /**/ <cstring>
# else
#   if defined (ACE_LACKS_MEMORY_H)
#     if !defined (ACE_PSOS_DIAB_MIPS)
#       include "ace/os_include/os_string.h"
#     endif /* ACE_PSOS_DIAB_MIPS */
#   else
#     include /**/ <memory.h>
#   endif /* VXWORKS */
# endif /* ACE_HAS_GNU_CSTRING_H */

// The following are #defines and #includes that must be visible for
// ACE to compile it's OS wrapper class implementation correctly.  We
// put them inside of here to reduce compiler overhead if we're not
// inlining...

# if defined (ACE_HAS_REGEX)
#   include /**/ <regexpr.h>
# endif /* ACE_HAS_REGEX */

# if defined (ACE_HAS_SYSINFO)
#   include /**/ <sys/systeminfo.h>
# endif /* ACE_HAS_SYS_INFO */

# if defined (ACE_HAS_SYSCALL_H)
#   include /**/ <sys/syscall.h>
# endif /* ACE_HAS_SYSCALL_H */

# if defined (UNIXWARE)  /* See strcasecmp, below */
#   include /**/ <ctype.h>
# endif /* UNIXWARE */

# if defined (ACE_HAS_GETIFADDRS)
#   include /**/ <ifaddrs.h>
# endif /* ACE_HAS_GETIFADDRS */


# if defined (DIGITAL_UNIX)
extern "C" {
  extern char *_Pctime_r (const time_t *, char *);
  extern struct tm *_Plocaltime_r (const time_t *, struct tm *);
  extern struct tm *_Pgmtime_r (const time_t *, struct tm *);
  extern char *_Pasctime_r (const struct tm *, char *);
  extern int _Prand_r (unsigned int *seedptr);
  extern int _Pgetpwnam_r (const char *, struct passwd *,
                           char *, size_t, struct passwd **);
}
# endif /* DIGITAL_UNIX */

// VAC++ doesn't correctly grok the ::getpwnam_r - the function is redefined
// in pwd.h, and that redefinition is used here
# if defined (_AIX) && defined (__IBMCPP__) && (__IBMCPP__ >= 400)
extern "C" {
  extern int _posix_getpwnam_r(const char *, struct passwd *, char *,
                               int, struct passwd **);
           }
#endif /* AIX and VAC++ 4 */

#else /* ACE_WIN32 */

ACE_INLINE LPSECURITY_ATTRIBUTES
ACE_OS::default_win32_security_attributes (LPSECURITY_ATTRIBUTES sa)
{
#if defined (ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES)
  if (sa == 0)
    {
      // @@ This is a good place to use pthread_once.
      static SECURITY_ATTRIBUTES default_sa;
      static SECURITY_DESCRIPTOR sd;
      InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
      SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);
      default_sa.nLength = sizeof(SECURITY_ATTRIBUTES);
      default_sa.lpSecurityDescriptor = &sd;
      default_sa.bInheritHandle       = TRUE;
      sa = &default_sa;
    }
  return sa;
#else /* !ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES */
  return sa;
#endif /* ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES */
}

#endif /* WIN32 */

#if defined (ACE_HAS_SHM_OPEN) && defined(INTEGRITY)
#include "ace/os_include/sys/os_mman.h"
#endif

// ****************************************************************

#if defined (ACE_WIN32)
ACE_INLINE const OSVERSIONINFO &
ACE_OS::get_win32_versioninfo ()
{
  return ACE_OS::win32_versioninfo_;
}

ACE_INLINE HINSTANCE
ACE_OS::get_win32_resource_module ()
{
  return ACE_OS::win32_resource_module_;
}

ACE_INLINE void
ACE_OS::set_win32_resource_module (HINSTANCE instance)
{
  ACE_OS::win32_resource_module_ = instance;
}
#endif /* ACE_WIN32 */
