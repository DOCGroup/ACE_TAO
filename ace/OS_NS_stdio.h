// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_stdio.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_STDIO_H
# define ACE_OS_NS_STDIO_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_stdio.h"
#include "ace/os_include/os_fcntl.h"
#include "ace/OS_Export.h"

/**
 * On some platforms clearerr is a macro. Defining ACE_OS::clearerr()
 * becomes really hard, as there is no way to save the macro
 * definition using the pre-processor.
 */
# if !defined (ACE_LACKS_CLEARERR)
#   if defined (clearerr)
#     define __ace_clearerr_hack
inline void __ace_clearerr(FILE *stream)
{
  clearerr(stream);
}
#     undef clearerr
#   endif /* defined (clearerr) */
# endif /* !ACE_LACKS_CLEARERR */

/// Helper for the ACE_OS::cuserid() function
/**
 * On some platforms cuserid is a macro.  Defining ACE_OS::cuserid()
 * becomes really hard, as there is no way to save the macro
 * definition using the pre-processor.
 * This inline function achieves the same effect, without namespace
 * pollution or performance penalties.
 *
 * @todo We maybe should move a lot of the code in ACE_OS::cuserid here so
 *       it is treated the same as the above ace_difftime and ace_timezone.
 *       But since there is a good deal more code in ACE_OS::cuserid, we
 *       probably need to move some of it off into some sort of emulation
 *       function.
 */
#if !defined (ACE_LACKS_CUSERID) && !defined(ACE_HAS_ALT_CUSERID) \
    && !defined(ACE_WIN32) && !defined (VXWORKS)
inline char *ace_cuserid(char *user)
{
  return cuserid(user);
}
#endif /* !ACE_LACKS_CUSERID && !ACE_HAS_ALT_CUSERID && ... */

# if defined (ACE_LACKS_FILELOCKS)
#   if ! defined (VXWORKS) && ! defined (ACE_PSOS) && ! defined (__rtems__) && !defined (INTEGRITY)
// VxWorks defines struct flock in sys/fcntlcom.h.  But it doesn't
// appear to support flock ().  RTEMS defines struct flock but
// currently does not support locking.
struct flock
{
  short l_type;
  short l_whence;
  off_t l_start;
  off_t l_len;          /* len == 0 means until end of file */
  long  l_sysid;
  pid_t l_pid;
  long  l_pad[4];               /* reserve area */
};
#   endif /* ! VXWORKS */
# endif /* ACE_LACKS_FILELOCKS */

namespace ACE_OS {

  /**
   * @class ace_flock_t
   *
   * @brief OS file locking structure.
   */
  class ACE_OS_Export ace_flock_t
  {
  public:
  /// Dump state of the object.
    void dump (void) const;

# if defined (ACE_WIN32)
    ACE_OVERLAPPED overlapped_;
# else
    struct flock lock_;
# endif /* ACE_WIN32 */

    /// Name of this filelock.
    const ACE_TCHAR *lockname_;

    /// Handle to the underlying file.
    ACE_HANDLE handle_;

# if defined (CHORUS)
    /// This is the mutex that's stored in shared memory.  It can only
    /// be destroyed by the actor that initialized it.
    ACE_mutex_t *process_lock_;
# endif /* CHORUS */
  };

# if defined (ACE_USES_WCHAR)
  // If fp points to the Unicode format file, the file pointer will be moved right next
  // to the Unicode header (2 types).  Otherwise, file pointer will be at the beginning.
  void checkUnicodeFormat (FILE* fp);
# endif  // ACE_USES_WCHAR

# if !defined (ACE_LACKS_CLEARERR)
  void clearerr (FILE* fp);
# endif /* !ACE_LACKS_CLEARERR */

  //@{ @name Wrappers to obtain the current user id
  // Legacy as per SUSV3
# if !defined (ACE_LACKS_CUSERID)
#   if defined(cuserid)
#     undef cuserid
#   endif /* cuserid */

  char *cuserid (char *user,
                 size_t maxlen = ACE_MAX_USERID);

#   if defined (ACE_HAS_WCHAR)
  wchar_t *cuserid (wchar_t *user,
                    size_t maxlen = ACE_MAX_USERID);
#   endif /* ACE_HAS_WCHAR */
# endif /* ACE_LACKS_CUSERID */
  //@}

  int fclose (FILE *fp);

# if defined (fdopen)
#   undef fdopen
# endif /* fdopen */

  FILE *fdopen (ACE_HANDLE handle, const ACE_TCHAR *mode);

  int fflush (FILE *fp);

  int fgetc (FILE* fp);

  int fgetpos (FILE* fp, fpos_t* pos);

  ACE_TCHAR *fgets (ACE_TCHAR *buf, int size, FILE *fp);

  //@{ @name A set of wrappers for file locks.

# if defined (ACE_WIN32)
  void flock_adjust_params (ace_flock_t *lock,
                            short whence,
                            off_t &start,
                            off_t &len);
# endif /* ACE_WIN32 */

  int flock_init (ace_flock_t *lock,
                  int flags = 0,
                  const ACE_TCHAR *name = 0,
                  mode_t perms = 0);

  int flock_destroy (ace_flock_t *lock,
                     int unlink_file = 1);

  int flock_rdlock (ace_flock_t *lock,
                    short whence = 0,
                    off_t start = 0,
                    off_t len = 0);

  int flock_tryrdlock (ace_flock_t *lock,
                       short whence = 0,
                       off_t start = 0,
                       off_t len = 0);

  int flock_trywrlock (ace_flock_t *lock,
                       short whence = 0,
                       off_t start = 0,
                       off_t len = 0);

  int flock_unlock (ace_flock_t *lock,
                    short whence = 0,
                    off_t start = 0,
                    off_t len = 0);

  int flock_wrlock (ace_flock_t *lock,
                    short whence = 0,
                    off_t start = 0,
                    off_t len = 0);

  //@}

  FILE *fopen (const ACE_TCHAR *filename, const ACE_TCHAR *mode);

# if defined (ACE_WIN32)

  /// Translate fopen's mode char to open's mode.  This helper function
  /// is here to avoid maintaining several pieces of identical code.
  void fopen_mode_to_open_mode_converter (ACE_TCHAR x, int &hmode);

  OSVERSIONINFO win32_versioninfo_;

  HINSTANCE win32_resource_module_;

# endif /* ACE_WIN32 */

  int fprintf (FILE *fp, const char *format, ...);

# if defined (ACE_HAS_WCHAR)
  int fprintf (FILE *fp, const wchar_t *format, ...);
# endif /* ACE_HAS_WCHAR */

  int fputs (const ACE_TCHAR *s,
             FILE *stream);

  size_t fread (void *ptr,
                size_t size,
                size_t nelems,
                FILE *fp);

  FILE *freopen (const ACE_TCHAR *filename,
                 const ACE_TCHAR *mode,
                 FILE* stream);

  int fseek (FILE *fp,
             long offset,
             int ptrname);

  int  fsetpos (FILE* fp, fpos_t* pos);

  long ftell (FILE* fp);

  size_t fwrite (const void *ptr,
                 size_t size,
                 size_t nitems,
                 FILE *fp);

  // The old gets () which directly maps to the evil, unprotected
  // gets () has been deprecated.  If you really need gets (),
  // consider the following one.

  // A better gets ().
  //   If n == 0, input is swallowed, but NULL is returned.
  //   Otherwise, reads up to n-1 bytes (not including the newline),
  //              then swallows rest up to newline
  //              then swallows newline
  char *gets (char *str, int n = 0);

  void perror (const ACE_TCHAR *s);

  int printf (const char *format, ...);

  int puts (const ACE_TCHAR *s);

  int rename (const ACE_TCHAR *old_name,
              const ACE_TCHAR *new_name,
              int flags = -1);

  void rewind (FILE *fp);

  int snprintf (char *buf, size_t maxlen, const char *format, ...);

# if defined (ACE_HAS_WCHAR)
  int snprintf (wchar_t *buf, size_t maxlen, const wchar_t *format,...);
# endif /* ACE_HAS_WCHAR */

  int sprintf (char *buf, const char *format, ...);

# if defined (ACE_HAS_WCHAR)
  int sprintf (wchar_t *buf, const wchar_t *format, ...);
# endif /* ACE_HAS_WCHAR */

  ACE_TCHAR *tempnam (const ACE_TCHAR *dir = 0,
                      const ACE_TCHAR *pfx = 0);

  int vsprintf (char *buffer, const char *format, va_list argptr);

# if defined (ACE_HAS_WCHAR)
  int vsprintf (wchar_t *buffer, const wchar_t *format, va_list argptr);
# endif /* ACE_HAS_WCHAR */

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_stdio.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_STDIO_H */
