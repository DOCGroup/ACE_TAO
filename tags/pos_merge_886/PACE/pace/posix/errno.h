/* $Id$

* ============================================================================
*
* = LIBRARY
*    pace
*
* = FILENAME
*    pace/posix/errno.h
*
* = AUTHOR
*    John Heitmann
*
* =========================================================================== *\
/

#ifndef PACE_ERRNO_H_POSIX
#define PACE_ERRNO_H_POSIX

#include <errno.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_E2BIG E2BIG
#define PACE_EACCES EACCES
#define PACE_EAGAIN EAGAIN
#define PACE_EBADF EBADF
#define PACE_EBADMSG
#define PACE_EBUSY EBUSY
#define PACE_ECANCELED ECANCELED
#define PACE_ECHILD ECHILD
#define PACE_EDEADLK EDEADLK
#define PACE_EDOM EDOM
#define PACE_EEXIST EEXIST
#define PACE_EFAULT EFAULT
#define PACE_EFBIG EFBIG
#define PACE_EINPROGRESS EINPROGRESS
#define PACE_EINTR EINTR
#define PACE_EINVAL EINVAL
#define PACE_EIO EIO
#define PACE_EISDIR EISDIR
#define PACE_EMFILE EMFILE
#define PACE_EMLINK EMLINK
#define PACE_EMSGSIZE EMSGSIZE
#define PACE_EENAMETOOLONG ENAMETOOLONG
#define PACE_ENFILE ENFILE
#define PACE_ENODEV ENODEV
#define PACE_ENOENT ENOENT
#define PACE_ENOEXEC ENOEXEC
#define PACE_ENOLCK ENOLCK
#define PACE_ENOMEM ENOMEM
#define PACE_ENOSPC ENOSPC
#define PACE_ENOSYS ENOSYS
#define PACE_ENOTDIR ENOTDIR
#define PACE_ENOTEMPTY ENOTEMPTY
#define PACE_ENOTSUP ENOTSUP
#define PACE_ENOTTY ENOTTY
#define PACE_ENXIO ENXIO
#define PACE_EPERM EPERM
#define PACE_EPIPE EPIPE
#define PACE_ERANGE ERANGE
#define PACE_EROFS EROFS
#define PACE_ESPIPE ESPIPE
#define PACE_ESRCH ESRCH
#define PACE_ETIMEDOUT ETIMEDOUT
#define PACE_EXDEV EXDEV

#define pace_errno errno

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_ERRNO_H_POSIX */
