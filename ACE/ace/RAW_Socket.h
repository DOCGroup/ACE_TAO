// -*- C++ -*-

//=============================================================================
/**
 *  @file RAW_Socket.h
 *
 *  @author Smith.Achang
 */
//=============================================================================

#ifndef ACE_RAW_SOCKET_H
#define ACE_RAW_SOCKET_H

#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "ace/SOCK.h"
#include "ace/Copy_Disabled.h"

#include "ace/Time_Value.h"
#include "ace/os_include/netinet/os_in.h"
#include "ace/Addr.h"
#include "ace/INET_Addr.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_RAW_Socket
 *
 * @brief An RAW socket implemention class that forms the basis for some usage of user-space network protocol stack 
 * e.g. using RAW + protocol[UDP] socket to avoid creating multiple UDP dgram sockets with only bound port variance
 */
class ACE_Export ACE_RAW_SOCKET : public ACE_SOCK, private ACE_Copy_Disabled
{
  public:
  /// Default constructor.
  ACE_RAW_SOCKET ();

  /// Constructor that bind a local address and fiter UDP protocol.
  ACE_RAW_SOCKET (ACE_Addr const & local,
                  int protocol   = IPPROTO_UDP,
                  int reuse_addr = 1);

  /**
   * @name Data transfer routines.
   *
   * Data transfer routines.
   */
  //@{

  /**
   * Wait up to @a timeout amount of time to send a datagram to
   * @a buf.  The ACE_Time_Value indicates how long to blocking
   * trying to receive.  If @a timeout == 0, the caller will block
   * until action is possible, else will wait until the relative time
   * specified in *@a timeout elapses).  If <send> times out a -1 is
   * returned with @c errno == ETIME.  If it succeeds the number of
   * bytes sent is returned.
   */
  ssize_t send (const void *buf,
                size_t n,
                const ACE_Addr &addr,
                int flags = 0,
                const ACE_Time_Value *timeout = NULL) const;


  /**
   * Wait up to @a timeout amount of time to receive a datagram into
   * @a buf.  The ACE_Time_Value indicates how long to blocking
   * trying to receive.  If @a timeout == 0, the caller will block
   * until action is possible, else will wait until the relative time
   * specified in *@a timeout elapses).  If <recv> times out a -1 is
   * returned with @c errno == ETIME.  If it succeeds the number of
   * bytes received is returned.
   */
  ssize_t recv (void *buf,
                size_t n,
                ACE_Addr &addr,
                int flags = 0,
                const ACE_Time_Value *timeout = NULL) const;

#if defined (ACE_HAS_MSG)

  /// Send an <iovec> of size @a n to the datagram socket (uses
  /// <sendmsg(3)>).
  ssize_t send (const iovec iov[],
                int n,
                const ACE_Addr &addr,
                int flags = 0) const;

  /// Recv an <iovec> of size @a n to the datagram socket (uses
  /// <recvmsg(3)>).  The IP destination address will be placed in @a
  /// *to_addr if it is not null and set_option has been called with
  /// 1) level IPPROTO_IP, option ACE_RECVPKTINFO, and value 1 for
  /// IPV4 addresses or 2) IPPROTO_IPV6, option ACE_RECVPKTINFO6, and
  /// value 1 for IPV6 addresses.
  ssize_t recv (iovec iov[],
                int n,
                ACE_Addr &addr,
                int flags = 0,
                ACE_INET_Addr *to_addr = 0) const;

  #endif

  //@}

  /// Wrapper around the BSD-style @c socket system call (no QoS).
  int open (ACE_Addr const & local,
            int protocol = IPPROTO_UDP,
            int reuse_addr = 1);

  /// Dump the state of object.
  void dump () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_RAW_SOCKET_H */
