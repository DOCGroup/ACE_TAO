// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_arpa_inet.h
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

#ifndef ACE_OS_NS_ARPA_INET_H
# define ACE_OS_NS_ARPA_INET_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/arpa/os_inet.h"

namespace ACE_OS {

  unsigned long inet_addr (const char *name);

  int inet_aton (const char *strptr,
                 struct in_addr *addr);

  char *inet_ntoa (const struct in_addr addr);

  const char *inet_ntop (int family,
                         const void *addrptr,
                         char *strptr,
                         size_t len);

  int inet_pton (int family,
                 const char *strptr,
                 void *addrptr);

}; /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_arpa_inet.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_ARPA_INET_H */
