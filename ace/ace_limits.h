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

# include /**/ <limits.h>

# if !defined (ACE_WIN32)
#   define ACE_MAX_USERID L_cuserid
# endif /*!ACE_WIN32*/

# if !defined (MAXNAMELEN)
#   if defined (FILENAME_MAX)
#     define MAXNAMELEN FILENAME_MAX
#   else
#     define MAXNAMELEN 256
#   endif /* FILENAME_MAX */
# endif /* MAXNAMELEN */

#endif /* ACE_ACE_LIMITS_H */
