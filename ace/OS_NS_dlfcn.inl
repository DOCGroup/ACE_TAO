// -*- C++ -*-
// $Id$

#include "ace/OS_NS_macros.h"
#include "ace/OS_NS_errno.h"

#if defined (ACE_USES_ASM_SYMBOL_IN_DLSYM)
#  include "ace/OS_Memory.h"
#  include "ace/OS_NS_string.h"
#endif /* ACE_USES_ASM_SYMBOL_IN_DLSYM */

ACE_INLINE int
ACE_OS::dlclose (ACE_SHLIB_HANDLE handle)
{
  ACE_OS_TRACE ("ACE_OS::dlclose");
#if defined (ACE_LACKS_DLCLOSE)
  ACE_UNUSED_ARG (handle);
  return 0;
#elif defined (ACE_HAS_SVR4_DYNAMIC_LINKING)

# if !defined (ACE_HAS_AUTOMATIC_INIT_FINI)
  // SunOS4 does not automatically call _fini()!
  void *ptr;

  ACE_OSCALL (::dlsym (handle, ACE_LIB_TEXT ("_fini")), void *, 0, ptr);

  if (ptr != 0)
    (*((int (*)(void)) ptr)) (); // Call _fini hook explicitly.
# endif /* ACE_HAS_AUTOMATIC_INIT_FINI */
#if defined (_M_UNIX)
  ACE_OSCALL_RETURN (::_dlclose (handle), int, -1);
#else /* _MUNIX */
    ACE_OSCALL_RETURN (::dlclose (handle), int, -1);
#endif /* _M_UNIX */
#elif defined (ACE_WIN32)
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::FreeLibrary (handle), ace_result_), int, -1);
#elif defined (__hpux)
  // HP-UX 10.x and 32-bit 11.00 do not pay attention to the ref count
  // when unloading a dynamic lib.  So, if the ref count is more than
  // 1, do not unload the lib.  This will cause a library loaded more
  // than once to not be unloaded until the process runs down, but
  // that's life.  It's better than unloading a library that's in use.
  // So far as I know, there's no way to decrement the refcnt that the
  // kernel is looking at - the shl_descriptor is a copy of what the
  // kernel has, not the actual struct.  On 64-bit HP-UX using dlopen,
  // this problem has been fixed.
  struct shl_descriptor  desc;
  if (shl_gethandle_r (handle, &desc) == -1)
    return -1;
  if (desc.ref_count > 1)
    return 0;
# if defined(__GNUC__) || __cplusplus >= 199707L
  ACE_OSCALL_RETURN (::shl_unload (handle), int, -1);
# else
  ACE_OSCALL_RETURN (::cxxshl_unload (handle), int, -1);
# endif  /* aC++ vs. Hp C++ */
#else
  ACE_UNUSED_ARG (handle);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SVR4_DYNAMIC_LINKING */
}

ACE_INLINE ACE_TCHAR *
ACE_OS::dlerror (void)
{
  ACE_OS_TRACE ("ACE_OS::dlerror");
# if defined (ACE_HAS_SVR4_DYNAMIC_LINKING)
#if defined(_M_UNIX)
  ACE_OSCALL_RETURN ((char *)::_dlerror (), char *, 0);
#else /* _M_UNIX */
  ACE_OSCALL_RETURN ((char *)::dlerror (), char *, 0);
#endif /* _M_UNIX */
# elif defined (__hpux) || defined (VXWORKS)
  ACE_OSCALL_RETURN (::strerror(errno), char *, 0);
# elif defined (ACE_WIN32)
  static ACE_TCHAR buf[128];
#   if defined (ACE_HAS_PHARLAP)
  ACE_OS::sprintf (buf, "error code %d", GetLastError());
#   else
  ACE_TEXT_FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM,
                          0,
                          ::GetLastError (),
                          0,
                          buf,
                          sizeof buf / sizeof buf[0],
                          0);
#   endif /* ACE_HAS_PHARLAP */
  return buf;
# else
  ACE_NOTSUP_RETURN (0);
# endif /* ACE_HAS_SVR4_DYNAMIC_LINKING */
}

# if defined (ACE_HAS_CHARPTR_DL)
typedef ACE_TCHAR * ACE_DL_TYPE;
# else
typedef const ACE_TCHAR * ACE_DL_TYPE;
# endif /* ACE_HAS_CHARPTR_DL */

ACE_INLINE ACE_SHLIB_HANDLE
ACE_OS::dlopen (const ACE_TCHAR *fname,
                int mode)
{
  ACE_OS_TRACE ("ACE_OS::dlopen");

  // Get the correct OS type.
  ACE_DL_TYPE filename = ACE_const_cast (ACE_DL_TYPE, fname);

# if defined (ACE_HAS_SVR4_DYNAMIC_LINKING)
  void *handle;
#   if defined (ACE_HAS_SGIDLADD)
  ACE_OSCALL (::sgidladd (filename, mode), void *, 0, handle);
#   elif defined (_M_UNIX)
  ACE_OSCALL (::_dlopen (filename, mode), void *, 0, handle);
#   else
  ACE_OSCALL (::dlopen (filename, mode), void *, 0, handle);
#   endif /* ACE_HAS_SGIDLADD */
#   if !defined (ACE_HAS_AUTOMATIC_INIT_FINI)
  if (handle != 0)
    {
      void *ptr;
      // Some systems (e.g., SunOS4) do not automatically call _init(), so
      // we'll have to call it manually.

      ACE_OSCALL (::dlsym (handle, ACE_LIB_TEXT ("_init")), void *, 0, ptr);

      if (ptr != 0 && (*((int (*)(void)) ptr)) () == -1) // Call _init hook explicitly.
        {
          // Close down the handle to prevent leaks.
          ::dlclose (handle);
          return 0;
        }
    }
#   endif /* ACE_HAS_AUTOMATIC_INIT_FINI */
  return handle;
# elif defined (ACE_WIN32)
  ACE_UNUSED_ARG (mode);

  ACE_WIN32CALL_RETURN (ACE_TEXT_LoadLibrary (filename), ACE_SHLIB_HANDLE, 0);
# elif defined (__hpux)

#   if defined(__GNUC__) || __cplusplus >= 199707L
  ACE_OSCALL_RETURN (::shl_load(filename, mode, 0L), ACE_SHLIB_HANDLE, 0);
#   else
  ACE_OSCALL_RETURN (::cxxshl_load(filename, mode, 0L), ACE_SHLIB_HANDLE, 0);
#   endif  /* aC++ vs. Hp C++ */
# elif defined (VXWORKS)
  MODULE* handle;
  // Open readonly
  ACE_HANDLE filehandle = ACE_OS::open (filename,
                                        O_RDONLY,
                                        ACE_DEFAULT_FILE_PERMS);

  if (filehandle != ACE_INVALID_HANDLE)
    {
      ACE_OS::last_error(0);
      ACE_OSCALL ( ::loadModule (filehandle, mode ), MODULE *, 0, handle);
      int loaderror = ACE_OS::last_error();
      ACE_OS::close (filehandle);

      if ( (loaderror != 0) && (handle != 0) )
        {
          // ouch something went wrong most likely unresolved externals
          ::unldByModuleId ( handle, 0 );
          handle = 0;
        }
    }
  else
    {
      // couldn't open file
      handle = 0;
    }
  return handle;
# else
  ACE_UNUSED_ARG (filename);
  ACE_UNUSED_ARG (mode);
  ACE_NOTSUP_RETURN (0);
# endif /* ACE_HAS_SVR4_DYNAMIC_LINKING */
}

ACE_INLINE void *
ACE_OS::dlsym (ACE_SHLIB_HANDLE handle,
               const ACE_TCHAR *sname)
{
  ACE_OS_TRACE ("ACE_OS::dlsym");

#if defined (ACE_HAS_DLSYM_SEGFAULT_ON_INVALID_HANDLE)
  // Check if the handle is valid before making any calls using it.
  if (handle == ACE_SHLIB_INVALID_HANDLE)
    return 0;
#endif /* ACE_HAS_DLSYM_SEGFAULT_ON_INVALID_HANDLE */

  // Get the correct OS type.
#if defined (ACE_HAS_WINCE)
  const wchar_t *symbolname = sname;
#elif defined (ACE_HAS_CHARPTR_DL)
  char *symbolname = ACE_const_cast (char *, sname);
#elif !defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
  const char *symbolname = sname;
#endif /* ACE_HAS_CHARPTR_DL */

# if defined (ACE_HAS_SVR4_DYNAMIC_LINKING)

#   if defined (ACE_LACKS_POSIX_PROTOTYPES)
  ACE_OSCALL_RETURN (::dlsym (handle, symbolname), void *, 0);
#   elif defined (ACE_USES_ASM_SYMBOL_IN_DLSYM)
  int l = ACE_OS::strlen (symbolname) + 2;
  char *asm_symbolname = 0;
  ACE_NEW_RETURN (asm_symbolname, char[l], 0);
  ACE_OS::strcpy (asm_symbolname, "_") ;
  ACE_OS::strcpy (asm_symbolname + 1, symbolname) ;
  void *ace_result;
  ACE_OSCALL (::dlsym (handle, asm_symbolname), void *, 0, ace_result);
  delete [] asm_symbolname;
  return ace_result;
#   elif defined (_M_UNIX)
  ACE_OSCALL_RETURN (::_dlsym (handle, symbolname), void *, 0);
#   else
  ACE_OSCALL_RETURN (::dlsym (handle, symbolname), void *, 0);
#   endif /* ACE_LACKS_POSIX_PROTOTYPES */

# elif defined (ACE_WIN32) && defined (ACE_USES_WCHAR) && !defined (ACE_HAS_WINCE)

  ACE_WIN32CALL_RETURN (::GetProcAddress (handle, ACE_TEXT_ALWAYS_CHAR (sname)), void *, 0);

# elif defined (ACE_WIN32)

  ACE_WIN32CALL_RETURN (::GetProcAddress (handle, symbolname), void *, 0);

# elif defined (__hpux)

  void *value;
  int status;
  shl_t _handle = handle;
  ACE_OSCALL (::shl_findsym(&_handle, symbolname, TYPE_UNDEFINED, &value), int, -1, status);
  return status == 0 ? value : 0;

# elif defined (VXWORKS)

  // For now we use the VxWorks global symbol table
  // which resolves the most recently loaded symbols .. which resolve mostly what we want..
  ACE_UNUSED_ARG (handle);
  SYM_TYPE symtype;
  void *value = 0;
  STATUS status;
  ACE_OSCALL (::symFindByName(sysSymTbl, symbolname, (char **)&value, &symtype), int, -1, status);

  return status == OK ? value : 0;

# else

  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (symbolname);
  ACE_NOTSUP_RETURN (0);

# endif /* ACE_HAS_SVR4_DYNAMIC_LINKING */
}
