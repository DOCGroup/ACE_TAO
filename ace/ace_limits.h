/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_limits.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_LIMITS_H
# define ACE_ACE_LIMITS_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_LIMITS_H)
#   include /**/ <limits.h>
# endif /* ACE_LACKS_LIMITS_H */

# if !defined (L_cuserid)
#   if !defined (ACE_L_CUSERID)
#     define ACE_L_CUSERID  9  
      // 8 character user ID + NULL
#   endif /* !ACE_L_CUSERID */
#   define L_cuserid ACE_L_CUSERID
# endif /* L_cuserid */

// need to fix this
# if !defined ACE_MAX_USERID
#   define ACE_MAX_USERID L_cuserid
# endif /* !ACE_MAX_USERID */

# if !defined (MAXNAMELEN)
#   if !defined (ACE_MAXNAMELEN)
#     if defined (FILENAME_MAX)
#       define ACE_MAXNAMELEN FILENAME_MAX
#     else
#       define ACE_MAXNAMELEN 256
#     endif /* FILENAME_MAX */
#   endif /* ACE_MAXNAMELEN */
#   define MAXNAMELEN ACE_MAXNAMELEN
# endif /* MAXNAMELEN */

# if !defined (PIPE_BUF)
#   define PIPE_BUF 5120
# endif /* PIPE_BUF */

#endif /* ACE_ACE_LIMITS_H */
