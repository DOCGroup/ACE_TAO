// -*- C++ -*-

//=============================================================================
/**
 *  @file   RAW_Socket.h
 *
 *  @author Smith.Achang <changyunlei@126.com>
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
 * - Setting the protocol parameter to be IPPROTO_UDP will filter all UDP protocol packages with the destination is its bound address.
 *   It can reduce the total num of socket needed with only port difference.
 *
 * - Setting the protocol parameter to be IPPROTO_SCTP will filter all SCTP protocol packages with the destination is its bound address.
 *   It can form the basis of a user-space SCTP protocol stack in more general way.
 *
 * - Setting the protocol parameter to be IPPROTO_RAW will make it as a send only socket for any customized datagram formed from the IP header to be sent.
 *   Notice the source address can be different from its bound address if provided in the customized package or keep them default to zero.
 *   Notice the IPPROTO_RAW RAW socket does not support fragment function when the passed package exceeds the MTU, so it need a upper layer fragment before called
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
  /// @param protocol   IPPROTO_UDP as default value because often used as a user-space UDP stack
  ACE_RAW_SOCKET (ACE_INET_Addr const & local, int protocol = IPPROTO_UDP);

  /**
   * @name Data transfer routines.
   *
   * Data transfer routines.
   */
  //@{

  /**
   * Wait up to @a timeout amount of time to send a datagram to
   * @a buf.  The ACE_Time_Value indicates how long to blocking
   * trying to receive.  If @a timeout == NULL, the caller will block
   * until action is possible, else will wait until the relative time
   * specified in *@a timeout elapses).  If <send> times out a -1 is
   * returned with @c errno == ETIME.  If it succeeds the number of
   * bytes sent is returned.
   */
  ssize_t send (const void *buf,
                size_t n,
                const ACE_INET_Addr &addr,
                int flags = 0,
                const ACE_Time_Value *timeout = nullptr) const;

  /**
   * Wait up to @a timeout amount of time to receive a datagram into
   * @a buf.  The ACE_Time_Value indicates how long to blocking
   * trying to receive.  If @a timeout == NULL, the caller will block
   * until action is possible, else will wait until the relative time
   * specified in *@a timeout elapses).  If <recv> times out a -1 is
   * returned with @c errno == ETIME.  If it succeeds the number of
   * bytes received is returned.
   * The IP destination address will be placed in @a *to_addr if it is not null
   */
  ssize_t recv (void *buf,
                size_t n,
                ACE_INET_Addr &addr,
                int flags = 0,
                const ACE_Time_Value *timeout = nullptr,
                ACE_INET_Addr *to_addr = nullptr) const;

  /**
   * Send an <iovec> of size @a n to the datagram socket (uses<sendmsg(3)>)
   * The IP destination address will be placed in @a *to_addr if it is not null
   * @return if iovec call is unsupported by platforms, the method will return -1 immediately
  */
  ssize_t send (const iovec iov[],
                int n,
                const ACE_INET_Addr &addr,
                int flags = 0,
                const ACE_Time_Value *timeout = nullptr) const;

  /**
   * Recv an <iovec> of size @a n to the datagram socket (uses <recvmsg(3)>).
   * The IP destination address will be placed in @a *to_addr if it is not null.
   * @return if iovec call is unsupported by platforms, the method will return -1 immediately
   */
  ssize_t recv (iovec iov[],
                int n,
                ACE_INET_Addr &addr,
                int flags = 0,
                const ACE_Time_Value *timeout = nullptr,
                ACE_INET_Addr *to_addr = nullptr) const;

  //@}

  /**
   * Wrapper around the BSD-style @c socket system call (no QoS).
   * @param local      local IP address to bind
   * @param protocol   filter the protocol based on IP network layer. Using IPPROTO_UDP as default value for often used as a user-space UDP stack
   *
   * @attention can be re-opened after closed
   */
  int open (ACE_INET_Addr const & local, int protocol = IPPROTO_UDP);

  /// Dump the state of object.
  void dump () const;

  /// Return @c true if the protocol is IPPROTO_RAW.
  bool is_send_only () const;

  /// Return the protocol field value.
  int protocol () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  int protocol_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/RAW_Socket.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACE_RAW_SOCKET_H */
