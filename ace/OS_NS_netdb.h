// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_netdb.h
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

#ifndef ACE_OS_NS_NETDB_H
# define ACE_OS_NS_NETDB_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_netdb.h"

namespace ACE_OS {

  struct hostent *gethostbyaddr (const char *addr,
                                 int length,
                                 int type);

  struct hostent *gethostbyaddr_r (const char *addr,
                                   int length,
                                   int type,
                                   struct hostent *result,
                                   ACE_HOSTENT_DATA buffer,
                                   int *h_errnop);

  struct hostent *gethostbyname (const char *name);

  struct hostent *gethostbyname_r (const char *name,
                                   struct hostent *result,
                                   ACE_HOSTENT_DATA buffer,
                                   int *h_errnop);


  struct hostent *getipnodebyaddr (const void *src, size_t len,
                                   int family);

  struct hostent *getipnodebyname (const char *name, int family,
                                   int flags = 0);

  /**
   * Get the first adapter found on the machine.
   * @todo: this is really useless except for UUID, move there? dhinton
   */
  struct macaddr_node_t {
    unsigned char node[6];
  };

  int getmacaddress (struct macaddr_node_t *node);

  struct protoent *getprotobyname (const char *name);

  struct protoent *getprotobyname_r (const char *name,
                                     struct protoent *result,
                                     ACE_PROTOENT_DATA buffer);

  struct protoent *getprotobynumber (int proto);

  struct protoent *getprotobynumber_r (int proto,
                                       struct protoent *result,
                                       ACE_PROTOENT_DATA buffer);

  struct servent *getservbyname (const char *svc,
                                 const char *proto);

  struct servent *getservbyname_r (const char *svc,
                                   const char *proto,
                                   struct servent *result,
                                   ACE_SERVENT_DATA buf);

# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0) && defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS)
  int netdb_acquire (void);
  int netdb_release (void);
# endif /* defined (ACE_MT_SAFE) && ACE_LACKS_NETDB_REENTRANT_FUNCTIONS */

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_netdb.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_NETDB_H */
