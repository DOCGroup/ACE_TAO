/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_errno.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_ERRNO_H
#define ACE_ACE_ERRNO_H
#include "ace/pre.h"

#include "ace/config-all.h"

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


#if defined (ACE_PSOS)
// Some versions of pSOS do not define error numbers, but newer
// versions do. So, include errno.h and then see which ones are not
// yet defined.
#   if !defined (EPERM)
#     define EPERM        1        /* Not super-user                        */
#   endif /* EPERM */
#   if !defined (ENOENT)
#     define ENOENT       2        /* No such file or directory             */
#   endif /* ENOENT */
#   if !defined (ESRCH)
#     define ESRCH        3        /* No such process                       */
#   endif /* ESRCH */
#   if ! defined (EINTR)
#     define EINTR        4        /* interrupted system call               */
#   endif /* EINTR */
#   if !defined (EBADF)
#     define EBADF        9        /* Bad file number                       */
#   endif /* EBADF */
#   if !defined (EAGAIN)
#     define EAGAIN       11       /* Resource temporarily unavailable      */
#   endif /* EAGAIN */
#   if !defined (EWOULDBLOCK)
#     define EWOULDBLOCK  EAGAIN   /* Blocking resource request would block */
#   endif /* EWOULDBLOCK */
#   if !defined (ENOMEM)
#     define ENOMEM       12       /* Not enough core                       */
#   endif /* ENOMEM */
#   if !defined (EACCESS)
#     define EACCESS      13       /* Permission denied                     */
#   endif /* EACCESS */
#   if !defined (EFAULT)
#     define EFAULT       14       /* Bad access                            */
#   endif /* EFAULT */
#   if !defined (EEXIST)
#     define EEXIST       17       /* File exists                           */
#   endif /* EEXIST */
#   if !defined (ENOSPC)
#     define ENOSPC       28       /* No space left on device               */
#   endif /* ENOSPC */
#   if !defined (EPIPE)
#     define EPIPE        32       /* Broken pipe                           */
#   endif /* EPIPE */
#   if !defined (ETIME)
#     define ETIME        62       /* timer expired                         */
#   endif /* ETIME */
#   if !defined (ENAMETOOLONG)
#     define ENAMETOOLONG 78       /* path name is too long                 */
#   endif /* ENAMETOOLONG */
#   if !defined (ENOSYS)
#     define ENOSYS       89       /* Unsupported file system operation     */
#   endif /* ENOSYS */
#   if !defined (EADDRINUSE)
#     define EADDRINUSE   125      /* Address already in use                */
#   endif /* EADDRINUSE */
#   if !defined (ENETUNREACH)
#     define ENETUNREACH  128      /* Network is unreachable                */
#   endif /* ENETUNREACH */
#   if !defined (EISCONN)
#     define EISCONN      133      /* Socket is already connected           */
#   endif /* EISCONN */
#   if !defined (ESHUTDOWN)
#     define ESHUTDOWN    143      /* Can't send after socket shutdown      */
#   endif /* ESHUTDOWN */
#   if !defined (ECONNREFUSED)
#     define ECONNREFUSED 146      /* Connection refused                    */
#   endif /* ECONNREFUSED */
#   if !defined (EINPROGRESS)
#     define EINPROGRESS  150      /* operation now in progress             */
#   endif /* EINPROGRESS */
#   if !defined (ERRMAX)
#     define ERRMAX       151      /* Last error number                     */
#   endif /* ERRMAX */
#endif /* ACE_PSOS */

#endif /* ACE_ERRNO_H */
