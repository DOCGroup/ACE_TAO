/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_wait.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_WAIT_H
# define ACE_ACE_SYS_WAIT_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_SYS_WAIT_H)
#   include <sys/wait.h>
# endif /* ACE_LACKS_SYS_WAIT_H */


  // Wrapping around wait status <wstat> macros for platforms that
  // lack them.

  // Evaluates to a non-zero value if status was returned for a child
  // process that terminated normally.  0 means status wasn't
  // returned.
# if !defined (WIFEXITED)
#   define WIFEXITED(stat) 1
# endif /* WIFEXITED */

  // If the value of WIFEXITED(stat) is non-zero, this macro evaluates
  // to the exit code that the child process exit(3C), or the value
  // that the child process returned from main.  Peaceful exit code is
  // 0.
# if !defined (WEXITSTATUS)
#   define WEXITSTATUS(stat) stat
# endif /* WEXITSTATUS */

  // Evaluates to a non-zero value if status was returned for a child
  // process that terminated due to the receipt of a signal.  0 means
  // status wasnt returned.
# if !defined (WIFSIGNALED)
#   define WIFSIGNALED(stat) 0
# endif /* WIFSIGNALED */

  // If the value of  WIFSIGNALED(stat)  is non-zero,  this macro
  // evaluates to the number of the signal that  caused  the
  // termination of the child process.
# if !defined (WTERMSIG)
#   define WTERMSIG(stat) 0
# endif /* WTERMSIG */

# if !defined (WIFSTOPPED)
#   define WIFSTOPPED(stat) 0
# endif /* WIFSTOPPED */

# if !defined (WSTOPSIG)
#   define WSTOPSIG(stat) 0
# endif /* WSTOPSIG */

# if !defined (WIFCONTINUED)
#   define WIFCONTINUED(stat) 0
# endif /* WIFCONTINUED */

# if !defined (WCOREDUMP)
#   define WCOREDUMP(stat) 0
# endif /* WCOREDUMP */
# if defined (ACE_HAS_IDTYPE_T) && !defined (ACE_IDTYPE_T_TYPE)
//  typedef idtype_t ACE_idtype_t;
#   define ACE_IDTYPE_T_TYPE idtype_t   
# else
//  typedef int ACE_idtype_t;
#   define ACE_IDTYPE_T_TYPE int
# endif /* ACE_HAS_IDTYPE_T */

# if defined (ACE_HAS_STHREADS) || defined (DIGITAL_UNIX)
#   if defined (ACE_LACKS_PRI_T)
//    typedef int pri_t;
#     if !defined (ACE_PRI_T_TYPE)
#       define ACE_PRI_T_TYPE int
#     endif /* !ACE_PRI_T_TYPE */
#   endif /* ACE_LACKS_PRI_T */
#   if !defined (ACE_ID_T_TYPE)
//  typedef id_t ACE_id_t;
#     define ACE_ID_T_TYPE id_t
#   endif /* !ACE_ID_T_TYPE */
#   if !defined (ACE_SELF)
#     define ACE_SELF P_MYID
#   endif /* ACE_SELF */
#   if !defined (ACE_PRI_T_TYPE)
#     define ACE_PRI_T_TYPE pri_t
//  typedef pri_t ACE_pri_t;
#   endif /* !ACE_PRI_T_TYPE */
#   else  /* ! ACE_HAS_STHREADS && ! DIGITAL_UNIX */
#     if !defined (ACE_ID_T_TYPE)
#       define ACE_ID_T_TYPE long
//  typedef long ACE_id_t;
#     endif /* !ACE_ID_T_TYPE */
#     if !defined (ACE_SELF)
#       define ACE_SELF (-1)
#     endif /* !ACE_SELF */
#     if !defined (ACE_PRI_T_TYPE)
#       define ACE_PRI_T_TYPE short
//  typedef short ACE_pri_t;
#     endif /* !ACE_PRI_T_TYPE */
#   endif /* ! ACE_HAS_STHREADS && ! DIGITAL_UNIX */


typedef ACE_IDTYPE_T_TYPE ACE_idtype_t;
typedef ACE_ID_T_TYPE ACE_id_t;
typedef ACE_PRI_T_TYPE ACE_pri_t;

#endif /* ACE_ACE_SYS_WAIT_H */
