/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_resource.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_RESOURCE_H
# define ACE_ACE_SYS_RESOURCE_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_SYS_RESOURCE_H)
#   include /**/ <sys/resource.h>
# endif /* !ACE_LACKS_SYS_RESOURCE_H */

# if defined (ACE_HAS_BROKEN_SETRLIMIT)
typedef struct rlimit ACE_SETRLIMIT_TYPE;
# else
typedef const struct rlimit ACE_SETRLIMIT_TYPE;
# endif /* ACE_HAS_BROKEN_SETRLIMIT */

# if defined (ACE_HAS_PRUSAGE_T)
    typedef prusage_t ACE_Rusage;
# elif defined (ACE_HAS_GETRUSAGE)
    typedef rusage ACE_Rusage;
# else
    typedef int ACE_Rusage;
# endif /* ACE_HAS_PRUSAGE_T */

# if !defined (ACE_HAS_GETRUSAGE_PROTO)
extern "C" int getrusage (int who, struct rusage *rusage);
# endif /* ! ACE_HAS_GETRUSAGE_PROTO */

// There must be a better way to do this...
# if !defined (RLIMIT_NOFILE)
#   if defined (linux) || defined (AIX) || defined (SCO)
#     if defined (RLIMIT_OFILE)
#       define RLIMIT_NOFILE RLIMIT_OFILE
#     else
#       define RLIMIT_NOFILE 200
#     endif /* RLIMIT_OFILE */
#   endif /* defined (linux) || defined (AIX) || defined (SCO) */
# endif /* RLIMIT_NOFILE */

#endif /* ACE_ACE_SYS_RESOURCE_H */
