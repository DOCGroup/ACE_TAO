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
#include "ace/INET_Addr.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_RAW_Socket
 *
 * @brief An RAW Socket implemention class.
 * 
 * An RAW Socket can be used for some user-space network protocol stack.
 * - setting the protocol para to be IPPROTO_UDP or other e.g. IPPROTO_SCTP will filter all relative packages with the destination is its bound address.
 * - setting the protocol para to be IPPROTO_RAW will make it as a send only socket for any customized dgram formed from IP header.
 *   Notice the source address if provided can be different from its bound address.
 * 
 * @note If you really want to receive all IP packets, use a packet(7) socket with the ETH_P_IP protocol. 
 *       For "Single Responsibility Principle" the behavior has notable difference, so the feature is not implemented here.
 */
class ACE_Export ACE_RAW_SOCKET : public ACE_SOCK, private ACE_Copy_Disabled
{
public:
  /// Default constructor.
  ACE_RAW_SOCKET ();
  
  /// Constructor that bind a local address and fiter UDP protocol.
  /// @param local      local IP address to bind
  /// @param protocol   IPPROTO_UDP as default value. often used as a user-space UDP stack
  /// @param reuse_addr Maybe meaningless for RAW Socket
  ACE_RAW_SOCKET (ACE_INET_Addr const & local,
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
  /// <sendmsg(3)>) with Scatter-Gather I/O.
  ssize_t send (const iovec iov[],
                int n,
                const ACE_INET_Addr &addr,
                int flags = 0) const;

  /// Recv an <iovec> of size @a n to the datagram socket (uses
  /// <recvmsg(3)>) with Scatter-Gather I/O.  The IP destination address will be placed in @a
  /// *to_addr if it is not null and set_option has been called with
  /// 1) level IPPROTO_IP, option ACE_RECVPKTINFO, and value 1 for
  /// IPV4 addresses or 2) IPPROTO_IPV6, option ACE_RECVPKTINFO6, and
  /// value 1 for IPV6 addresses.
  ssize_t recv (iovec iov[],
                int n,
                ACE_INET_Addr &addr,
                int flags = 0,
                ACE_INET_Addr *to_addr = 0) const;

  #endif

  //@}

  /**
   * Wrapper around the BSD-style @c socket system call (no QoS).
   * @param local      local IP address to bind
   * @param protocol   filter the protocol based on IP network layer. Using IPPROTO_UDP as default value for often used as a user-space UDP stack
   * @param reuse_addr Maybe meaningless for RAW Socket
   * 
   * @attention can be re-opened after closed
   */
 
  int open (ACE_INET_Addr const & local,
            int protocol   = IPPROTO_UDP,
            int reuse_addr = 1);

  /// Dump the state of object.
  void dump () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_RAW_SOCKET_H */
