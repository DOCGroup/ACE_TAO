// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_Errno.h
 *
 *  $Id$
 *
 *  @author (Originally in OS.h)Doug Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_OS_ERRNO_H
#define ACE_OS_ERRNO_H
#include "ace/pre.h"

#include "ace/config.h"
#include "OS_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_H_ERRNO)
void herror (const char *str);
#endif /* ACE_HAS_H_ERRNO */

#if !defined (ACE_HAS_WINCE)
# include /**/ <errno.h>
#endif /* ACE_HAS_WINCE */

#if !defined (ACE_WIN32) && !defined (ACE_PSOS) && defined (ACE_LACKS_T_ERRNO)
extern int t_errno;
#endif /* ACE_WIN32 && !ACE_PSOS && ACE_LACKS_T_ERRNO */

#if !defined (ENOSYS)
# define ENOSYS EFAULT /* Operation not supported or unknown error. */
#endif /* !ENOSYS */

#if !defined (ENOTSUP)
# define ENOTSUP ENOSYS  /* Operation not supported. */
#endif /* !ENOTSUP */

#if defined (ACE_HAS_WINCE_BROKEN_ERRNO)
/**
 * @class ACE_CE_Errno
 *
 * Some versions of CE don't support <errno> and some versions'
 * implementations are busted.  So we implement our own.
 * Our implementation takes up one Tls key, however, it does not
 * allocate memory fromt the heap so there's no problem with cleanin
 * up the errno when a thread exit.
 */
class ACE_OS_Export ACE_CE_Errno
{
public:
  ACE_CE_Errno () {}
  static void init ();
  static void fini ();
  static ACE_CE_Errno *instance ();

  operator int (void) const;
  int operator= (int);

private:
  static ACE_CE_Errno *instance_;
  static DWORD errno_key_;
};

# define errno (* (ACE_CE_Errno::instance ()))
#endif /* ACE_HAS_WINCE_BROKEN_ERRNO */

#if defined (ACE_HAS_WINCE_BROKEN_ERRNO)
#  define ACE_ERRNO_TYPE ACE_CE_Errno
#else
#  define ACE_ERRNO_TYPE int
#endif /* ACE_HAS_WINCE */

/**
 * @class ACE_Errno_Guard
 *
 * @brief Provides a wrapper to improve performance when thread-specific
 * errno must be saved and restored in a block of code.
 *
 * The typical use-case for this is the following:
 * int error = errno;
 * call_some_function_that_might_change_errno ();
 * errno = error;
 * This can be replaced with
 * {
 * ACE_Errno_Guard guard (errno);
 * call_some_function_that_might_change_errno ();
 * }
 * This implementation is more elegant and more efficient since it
 * avoids an unnecessary second access to thread-specific storage
 * by caching a pointer to the value of errno in TSS.
 */
class ACE_OS_Export ACE_Errno_Guard
{
public:
  ///  Stash the value of <error> into <error_> and initialize the
  ///  <errno_ptr_> to the address of <errno_ref>.
  ACE_Errno_Guard (ACE_ERRNO_TYPE &errno_ref,
                   int error);

  ///  Stash the value of <errno> into <error_> and initialize the
  ///  <errno_ptr_> to the address of <errno_ref>.
  ACE_Errno_Guard (ACE_ERRNO_TYPE &errno_ref);

  /// Reset the value of <errno> to <error>.
  ~ACE_Errno_Guard (void);

#if defined (ACE_HAS_WINCE_BROKEN_ERRNO)
  /// Assign <errno_ref> to <error_>.
  int operator= (const ACE_ERRNO_TYPE &errno_ref);
#endif /* ACE_HAS_WINCE_BROKEN_ERRNO */

  /// Assign <error> to <error_>.
  int operator= (int error);

  /// Compare <error> with <error_> for equality.
  int operator== (int error);

  /// Compare <error> with <error_> for inequality.
  int operator!= (int error);

private:
#if defined (ACE_MT_SAFE)
  ACE_ERRNO_TYPE *errno_ptr_;
#endif /* ACE_MT_SAFE */
  int error_;
};

#if defined (ACE_HAS_INLINED_OSCALLS)
# if defined (ACE_INLINE)
#  undef ACE_INLINE
# endif /* ACE_INLINE */
# define ACE_INLINE inline
# include "OS_Errno.inl"
#endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/post.h"
#endif /* ACE_OS_ERRNO_H */
