/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    string.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_STRING_H
# define ACE_OS_INCLUDE_STRING_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Matthew Stevens 7-10-95 Fix GNU GCC 2.7 for memchr() problem.
# if defined (ACE_HAS_GNU_CSTRING_H)
// Define this file to keep /usr/include/memory.h from being included.
#   include /**/ <cstring>
# else
#   if !defined (ACE_LACKS_MEMORY_H)
#     include /**/ <memory.h>
#   endif /* !ACE_LACKS_MEMORY_H */
# endif /* ACE_HAS_GNU_CSTRING_H */

// why was this included here?
//# include "ace/ace_stdlib.h"

# if !defined (ACE_LACKS_STRING_H)
#   include /**/ <string.h>
# endif /* !ACE_LACKS_STRING_H */

// IRIX5 defines bzero() in this odd file...
#   if defined (ACE_HAS_BSTRING)
#     include /**/ <bstring.h>
#   endif /* ACE_HAS_BSTRING */

// We need strings.h on some platforms (qnx-neutrino, for example)
// to get the declaration for strcasecmp.  And bzero() for AIX.
# if defined (ACE_HAS_STRINGS)
#   include /**/ <strings.h>
# endif /* ACE_HAS_STRINGS */

# if defined (ACE_LACKS_STRTOK_R_PROTOTYPE) && !defined (_POSIX_SOURCE)
extern "C" char *strtok_r (char *s, const char *delim, char **save_ptr);
# endif  /* ACE_LACKS_STRTOK_R_PROTOTYPE */

#endif /* ACE_OS_INCLUDE_STRING_H */

