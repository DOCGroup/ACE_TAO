// $Id$
//=============================================================================
/**
 *  @file    SOCK_Netlink.h
 *
 *  SOCK_Netlink.cpp,v 5.4.2 2004/10/10  Exp
 * 
 *  @author Raz Ben Yehuda <raziebe@013.net.il>
 */
//=============================================================================

#ifndef ACE_SOCK_Netlink_H
#define ACE_SOCK_Netlink_H
#include /* */ "ace/pre.h"

#include /* */ "ace/SOCK.h"
#include /* */ "ace/Netlink_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"

/**
 * @class ACE_SOCK_Netlink
 *
 * @brief Defines the member functions for the ACE_SOCK Netlink 
 * abstraction.
 
 * Netlink sockets are used in Linux as a communication facilty of kernel to user 
 * and user to kernel.
 * This code was created so one could use ACE reactor 
 * as a gateway to a linux kernel.
 *
 */
class ACE_Export ACE_SOCK_Netlink : public ACE_SOCK
{
public:
  // = Initialization and termination methods.
  /// Default constructor.
  ACE_SOCK_Netlink(void);
  ~ACE_SOCK_Netlink(void);
  ACE_SOCK_Netlink (ACE_Netlink_Addr &local,
                  int protocol_family,
                  int protocol);
                  
  int open (ACE_Netlink_Addr &local,
           int protocol_family,
           int protocol);

  ssize_t recv (void *buf,
               size_t n,
               int flags) const;

  ssize_t send (void *buf,
                size_t n,
                int flags) const;

  /// Recv an <iovec> of size <n> to the datagram socket (uses
  /// <recvmsg(3)>).
  ssize_t recv (iovec iov[],
                int n,
                ACE_Addr &addr,
                int flags = 0) const;

  /// Send an <iovec> of size <n> to the datagram socket (uses
  /// <sendmsg(3)>).
  ssize_t send (const iovec iov[],
                int n,
                const ACE_Addr &addr,
                int flags = 0) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

#if defined (__ACE_INLINE__)
#include "ace/SOCK_Netlink.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_SOCK_Netlink_H */



