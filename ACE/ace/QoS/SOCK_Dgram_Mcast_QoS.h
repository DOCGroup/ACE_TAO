/**
 *  @file    SOCK_Dgram_Mcast_QoS.h
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */

#ifndef ACE_SOCK_DGRAM_MCAST_QOS_H
#define ACE_SOCK_DGRAM_MCAST_QOS_H
#include /**/ "ace/pre.h"

#include "ace/SOCK_Dgram_Mcast.h"
#include "QoS_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_SOCK_Dgram_Mcast_QoS
 *
 * @brief Defines the member functions for the ACE QoS enabled socket
 * wrapper for UDP/IP multicast.
 */
class ACE_QoS_Export ACE_SOCK_Dgram_Mcast_QoS : public ACE_SOCK_Dgram_Mcast
{
public:
  /// Uses the same default options as ACE_SOCK_Dgram_Mcast.
  ACE_SOCK_Dgram_Mcast_QoS (options opts = DEFOPTS);

  // Note that there is no public <open> method.  Therefore, this
  // class cannot be used unless you <subscribe> to a multicast group.
  // If you just want to send (and not listen) to a multicast group,
  // use <ACE_SOCK_Dgram> or <ACE_SOCK_CODgram> instead.

  ~ACE_SOCK_Dgram_Mcast_QoS ();

  ACE_SOCK_Dgram_Mcast_QoS (ACE_SOCK_Dgram_Mcast_QoS const &) = delete;
  ACE_SOCK_Dgram_Mcast_QoS &operator= (ACE_SOCK_Dgram_Mcast_QoS const &) = delete;
  ACE_SOCK_Dgram_Mcast_QoS (ACE_SOCK_Dgram_Mcast_QoS &&) = delete;
  ACE_SOCK_Dgram_Mcast_QoS &operator= (ACE_SOCK_Dgram_Mcast_QoS &&) = delete;

  /// @name Multicast group management
  /// @{

  /**
   * This is a QoS-enabled method for joining a multicast group, which
   * passes @a qos_params via @c ACE_OS::join_leaf(). The network
   * interface device driver is instructed to accept datagrams with
   * @a mcast_addr multicast addresses. If the socket has already been
   * opened, @c subscribe() closes the socket and opens a new socket
   * bound to @a mcast_addr. The session object specifies the QoS
   * session that the socket wants to subscribe to. A socket may
   * subscribe to multiple QoS sessions by calling this method multiple
   * times with different session objects.
   *
   * The @a net_if interface is hardware specific, e.g., use "netstat
   * -i" to find the interface name, such as "le0". If @a net_if is
   * @c nullptr, @c subscribe() uses the default multicast
   * interface.  Returns: -1 if the call fails.
   *
   * @note some platforms, such as pSoS, support only number, not
   * names, for network interfaces. For these platforms, give the numbers
   * in alphanumeric form and @c subscribe() converts them via
   * @c ACE_OS::atoi().
   */
  int subscribe (ACE_INET_Addr const &mcast_addr,
                 ACE_QoS_Params const &qos_params,
                 int reuse_addr = 1,
                 ACE_TCHAR const *net_if = nullptr,
                 int protocol_family = PF_INET,
                 int protocol = 0,
                 ACE_Protocol_Info *protocolinfo = nullptr,
                 ACE_SOCK_GROUP g = 0,
                 u_long flags = 0,
                 ACE_QoS_Session *qos_session = nullptr);

  /// @}

  /// @name Data transfer
  /// @{

  /// Send @a buffer_count buffers from @a buffers to @a addr using overlapped
  /// I/O (uses @c WSASendTo()). Returns 0 on success.
  ssize_t send (iovec const buffers[],
                int buffer_count,
                size_t &number_of_bytes_sent,
                int flags,
                ACE_Addr const &addr,
                ACE_OVERLAPPED *overlapped,
                ACE_OVERLAPPED_COMPLETION_FUNC func) const;

  /// Send @a n bytes from @a buf to the datagram socket (uses @c WSASendTo()).
  ssize_t send (void const *buf,
                size_t n,
                ACE_Addr const &addr,
                int flags,
                ACE_OVERLAPPED *overlapped,
                ACE_OVERLAPPED_COMPLETION_FUNC func) const;

  /// @}

  /// Returns the QoS manager for this socket.
  ACE_QoS_Manager qos_manager ();

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  int open (ACE_INET_Addr const &addr,
            ACE_QoS_Params const &qos_params,
            int protocol_family = PF_INET,
            int protocol = 0,
            ACE_Protocol_Info *protocolinfo = nullptr,
            ACE_SOCK_GROUP g = 0,
            u_long flags = 0,
            int reuse_addr = 0);

private:
  /// Subscribe to the multicast interface using QoS-enabled semantics.
  int subscribe_ifs (ACE_INET_Addr const &mcast_addr,
                     ACE_QoS_Params const &qos_params,
                     ACE_TCHAR const *net_if,
                     int protocol_family,
                     int protocol,
                     int reuse_addr,
                     ACE_Protocol_Info *protocolinfo);

  /// Manages the QoS sessions that this socket subscribes to.
  ACE_QoS_Manager qos_manager_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "SOCK_Dgram_Mcast_QoS.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_SOCK_DGRAM_MCAST_QOS_H */
