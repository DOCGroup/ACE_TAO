/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    unistd.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_UNISTD_H
# define ACE_ACE_UNISTD_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/os_include/sys/types.h"
// got to move the llseek stuff out of basic_types.h...
# include "ace/Basic_Types.h"

# if !defined (ACE_LACKS_UNISTD_H)
#   include /**/ <unistd.h>
# endif /* ACE_LACKS_UNISTD_H */

# if defined (ACE_LACKS_GETPGID_PROTOTYPE) && \
    !defined (_XOPEN_SOURCE) && !defined (_XOPEN_SOURCE_EXTENDED)
extern "C" pid_t getpgid (pid_t pid);
# endif  /* ACE_LACKS_GETPGID_PROTOTYPE &&
           !_XOPEN_SOURCE && !_XOPEN_SOURCE_EXTENDED */


# if !defined (_LARGEFILE64_SOURCE)
#   if defined (ACE_LACKS_LSEEK64_PROTOTYPE) && \
     defined (ACE_LACKS_LLSEEK_PROTOTYPE)
#     error Define either ACE_LACKS_LSEEK64_PROTOTYPE or ACE_LACKS_LLSEEK_PROTOTYPE, not both!
#   elif defined (ACE_LACKS_LSEEK64_PROTOTYPE)
extern "C" ACE_LOFF_T lseek64 (int fd, ACE_LOFF_T offset, int whence);
#   elif defined (ACE_LACKS_LLSEEK_PROTOTYPE)
extern "C" ACE_LOFF_T llseek (int fd, ACE_LOFF_T offset, int whence);
#   endif
# endif  /* _LARGEFILE64_SOURCE */

# if defined (ACE_LACKS_PREAD_PROTOTYPE) && (_XOPEN_SOURCE - 0) < 500
// _XOPEN_SOURCE == 500    Single Unix conformance
// It seems that _XOPEN_SOURCE == 500 means that the prototypes are
// already defined in the system headers.
extern "C" ssize_t pread (int fd,
                          void *buf,
                          size_t nbytes,
                          off_t offset);

extern "C" ssize_t pwrite (int fd,
                           const void *buf,
                           size_t n,
                           off_t offset);
# endif  /* ACE_LACKS_PREAD_PROTOTYPE && (_XOPEN_SOURCE - 0) < 500 */


# if defined (ACE_LACKS_UALARM_PROTOTYPE)
extern "C" u_int ualarm (u_int usecs, u_int interval);
# endif /* ACE_LACKS_UALARM_PROTOTYPE */

# if defined (m88k) && !defined (RUSAGE_SELF)
#   define RUSAGE_SELF 1
# endif  /*  m88k */

# if defined (ACE_LACKS_SYSCALL)
extern "C" int syscall (int, ACE_HANDLE, struct rusage *);
# endif /* ACE_LACKS_SYSCALL */

# if !defined (R_OK)
#   define R_OK    04      /* Test for Read permission. */
# endif /* R_OK */

# if !defined (W_OK)
#   define W_OK    02      /* Test for Write permission. */
# endif /* W_OK */

# if !defined (X_OK)
#   define X_OK    01      /* Test for eXecute permission. */
# endif /* X_OK */

# if !defined (F_OK)
#   define F_OK    0       /* Test for existence of File. */
# endif /* F_OK */


# if !defined (_SC_TIMER_MAX)
#   define _SC_TIMER_MAX 44
# endif /* _SC_TIMER_MAX */


// Default number of <ACE_Event_Handler>s supported by
// <ACE_Timer_Heap>.
# if !defined (ACE_DEFAULT_TIMERS)
#   define ACE_DEFAULT_TIMERS _SC_TIMER_MAX
# endif /* ACE_DEFAULT_TIMERS */

#endif /* ACE_OS_INCLUDE_UNISTD_H */
