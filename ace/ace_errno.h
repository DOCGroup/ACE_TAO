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
# define ACE_ACE_ERRNO_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_ERRNO_H)
#   include /**/ <errno.h>
# endif /* ACE_HAS_WINCE */

// This only seems to be defined in config-aix-4.x.h, and is never used.
# if defined (ACE_HAS_H_ERRNO)
void herror (const char *str);
# endif /* ACE_HAS_H_ERRNO */

# if defined (ACE_LACKS_T_ERRNO)
extern int t_errno;
# endif /* ACE_LACKS_T_ERRNO */

# if ! (defined(__BORLANDC__) && __BORLANDC__ >= 0x0530) \
     && !defined(__MINGW32__)
#   if defined (__FreeBSD__) || defined(__QNX__)
extern const int sys_nerr;
#   elif defined (__CYGWIN32__) || defined (ACE_WIN32) 
#     define sys_nerr _sys_nerr
#   else
extern int sys_nerr;
#   endif /* !__FreeBSD__ && !__QNX__ */
# endif /* ! (defined(__BORLANDC__) && __BORLANDC__ >= 0x0530) */

# if !defined (ENOSYS)
#   define ENOSYS EFAULT /* Operation not supported or unknown error. */
# endif /* !ENOSYS */

# if !defined (ENOTSUP)
#   define ENOTSUP ENOSYS  /* Operation not supported. */
# endif /* !ENOTSUP */

# if !defined (ESUCCESS)
#   define ESUCCESS 0
# endif /* !ESUCCESS */

# if !defined (EIDRM)
#   define EIDRM 0
# endif /* !EIDRM */

# if !defined (ENFILE)
#   define ENFILE EMFILE /* No more socket descriptors are available. */
# endif /* !ENOSYS */

# if !defined (ECOMM)
    // Not the same, but ECONNABORTED is provided on NT.
#   define ECOMM ECONNABORTED
# endif /* ECOMM */

# if !defined (WNOHANG)
#   define WNOHANG 0100
# endif /* !WNOHANG */

# if !defined (EDEADLK)
#   define EDEADLK 1000 /* Some large number.... */
# endif /* !EDEADLK */

# if !defined (ETIMEDOUT) && defined (ETIME)
#   define ETIMEDOUT ETIME
# endif /* ETIMEDOUT */

# if !defined (ETIME) && defined (ETIMEDOUT)
#   define ETIME ETIMEDOUT
# endif /* ETIMED */

# if !defined (EBUSY)
#   define EBUSY ETIME
# endif /* EBUSY */

#endif /* ACE_ERRNO_H */
