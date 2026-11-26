/**
 *  @file   OS_NS_netdb.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */

#ifndef ACE_OS_NS_NETDB_H
#define ACE_OS_NS_NETDB_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_netdb.h"
#include "ace/os_include/sys/os_socket.h"
#include /**/ "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_OS
{
  ACE_NAMESPACE_INLINE_FUNCTION
  struct hostent *gethostbyaddr (char const *addr,
                                 int length,
                                 int type);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct hostent *gethostbyaddr_r (char const *addr,
                                   int length,
                                   int type,
                                   struct hostent *result,
                                   ACE_HOSTENT_DATA buffer,
                                   int *h_errnop);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct hostent *gethostbyname (char const *name);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct hostent *gethostbyname_r (char const *name,
                                   struct hostent *result,
                                   ACE_HOSTENT_DATA buffer,
                                   int *h_errnop);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct hostent *getipnodebyaddr (void const *src, size_t len,
                                   int family);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct hostent *getipnodebyname (char const *name, int family,
                                   int flags = 0);

  /**
   * Get the first adapter found on the machine.
   * @todo: this is really useless except for UUID, move there? dhinton
   */
  struct macaddr_node_t {
    unsigned char node[6];
  };

  extern ACE_Export
  int getmacaddress (struct macaddr_node_t *node);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct protoent *getprotobyname (char const *name);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct protoent *getprotobyname_r (char const *name,
                                     struct protoent *result,
                                     ACE_PROTOENT_DATA buffer);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct protoent *getprotobynumber (int proto);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct protoent *getprotobynumber_r (int proto,
                                       struct protoent *result,
                                       ACE_PROTOENT_DATA buffer);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct servent *getservbyname (char const *svc,
                                 char const *proto);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct servent *getservbyname_r (char const *svc,
                                   char const *proto,
                                   struct servent *result,
                                   ACE_SERVENT_DATA buf);

  ACE_NAMESPACE_INLINE_FUNCTION
  int getaddrinfo (char const *name, char const *service,
                   const addrinfo *hints, addrinfo **result);

  ACE_NAMESPACE_INLINE_FUNCTION
  void freeaddrinfo (addrinfo *result);

  ACE_NAMESPACE_INLINE_FUNCTION
  ACE_TCHAR const *gai_strerror (int errcode);

  ACE_NAMESPACE_INLINE_FUNCTION
  int getnameinfo (const sockaddr *addr, ACE_SOCKET_LEN addr_len,
                   char *host, ACE_SOCKET_LEN host_len,
                   char *service, ACE_SOCKET_LEN service_len,
                   unsigned int flags);

#ifdef ACE_LACKS_GETADDRINFO
  extern ACE_Export
  int getaddrinfo_emulation (char const *name, addrinfo **result);

  extern ACE_Export
  void freeaddrinfo_emulation (addrinfo *result);
#endif

#ifdef ACE_LACKS_GETNAMEINFO
  extern ACE_Export
  int getnameinfo_emulation (const sockaddr *addr, ACE_SOCKET_LEN addr_len,
                             char *host, ACE_SOCKET_LEN host_len);
#endif

# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0) && defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS)
  extern ACE_Export
  int netdb_acquire ();

  extern ACE_Export
  int netdb_release ();
# endif /* defined (ACE_MT_SAFE) && ACE_LACKS_NETDB_REENTRANT_FUNCTIONS */

} /* namespace ACE_OS */

ACE_END_VERSIONED_NAMESPACE_DECL

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_netdb.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_NETDB_H */
