// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_pwd.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_PWD_H
# define ACE_OS_NS_PWD_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_pwd.h"

# if defined (ACE_HAS_BROKEN_R_ROUTINES)
#   undef getpwnam_r
# endif /* ACE_HAS_BROKEN_R_ROUTINES */

namespace ACE_OS {


  //@{ @name A set of wrappers for password routines.

  void endpwent (void);

  struct passwd *getpwent (void);

  struct passwd *getpwnam (const char *user);

  struct passwd *getpwnam_r (const char *name,
                             struct passwd *pwent,
                             char *buffer,
                             int buflen);
  void setpwent (void);
  //@}

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_pwd.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_PWD_H */
