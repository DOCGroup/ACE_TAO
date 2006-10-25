// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_netdb.h
 *
 *  definitions for network database operations
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_NETDB_H
#define ACE_OS_INCLUDE_OS_NETDB_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/netinet/os_in.h"
#include "ace/os_include/os_limits.h"

#if !defined (ACE_LACKS_NETDB_H)
#  if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#    define queue _Queue_
#  endif /* ACE_HAS_STL_QUEUE_CONFLICT */
   extern "C" {
#  include /**/ <netdb.h>
   }
#  if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#    undef queue
#  endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#endif /* !ACE_LACKS_NETDB_H */

#if defined (ACE_VXWORKS) && (ACE_VXWORKS < 0x620)
#  include /**/ <hostLib.h>
#endif /* ACE_VXWORKS */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_LACKS_HOSTENT)
struct  hostent {
        char    *h_name;        /* official name of host */
        char    **h_aliases;    /* alias list */
        int     h_addrtype;     /* host address type */
        int     h_length;       /* length of address */
        char    **h_addr_list;  /* list of addresses from name server */
#define h_addr  h_addr_list[0]  /* address, for backward compatibility */
};
#endif /* ACE_LACKS_HOSTENT */

#if defined (ACE_LACKS_PROTOENT)
struct  protoent {
        char    *p_name;        /* official protocol name */
        char    **p_aliases;    /* alias list */
        int     p_proto;        /* protocol # */
};
#endif /* ACE_LACKS_PROTOENT */

#if defined (ACE_HAS_STRUCT_NETDB_DATA)
   typedef char ACE_HOSTENT_DATA[sizeof(struct hostent_data)];
   typedef char ACE_SERVENT_DATA[sizeof(struct servent_data)];
   typedef char ACE_PROTOENT_DATA[sizeof(struct protoent_data)];
#else
#  if !defined ACE_HOSTENT_DATA_SIZE
#    define ACE_HOSTENT_DATA_SIZE (4*1024)
#  endif /*ACE_HOSTENT_DATA_SIZE */
#  if !defined ACE_SERVENT_DATA_SIZE
#    define ACE_SERVENT_DATA_SIZE (4*1024)
#  endif /*ACE_SERVENT_DATA_SIZE */
#  if !defined ACE_PROTOENT_DATA_SIZE
#    define ACE_PROTOENT_DATA_SIZE (2*1024)
#  endif /*ACE_PROTOENT_DATA_SIZE */
   typedef char ACE_HOSTENT_DATA[ACE_HOSTENT_DATA_SIZE];
   typedef char ACE_SERVENT_DATA[ACE_SERVENT_DATA_SIZE];
   typedef char ACE_PROTOENT_DATA[ACE_PROTOENT_DATA_SIZE];
#endif /* ACE_HAS_STRUCT_NETDB_DATA */

# if !defined(MAXHOSTNAMELEN)
#   define MAXHOSTNAMELEN  HOST_NAME_MAX
# endif /* MAXHOSTNAMELEN */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_NETDB_H */
