/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SOCK_Dgram_Mcast.h
 *
 *  $Id$
 *
 *  @author Irfan Pyrali <irfan@cs.wustl.edu>
 *  @author Tim Harrison <harrison@cs.wustl.edu>
 *  @author and Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_SOCK_DGRAM_MCAST_H
#define ACE_SOCK_DGRAM_MCAST_H
#include "ace/pre.h"

#include "ace/Sockets/SOCK_Dgram.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/IPC/INET_Addr.h"

/**
 * @class ACE_SOCK_Dgram_Mcast
 *
 * @brief Defines the member functions for the ACE socket wrapper
 * for UDP/IP multicast.
 */
class ACE_Export ACE_SOCK_Dgram_Mcast : public ACE_SOCK_Dgram
{
public:
  // = Initialization routine.
  /**
   * Note that there is no public <open> method.  Therefore, this
   * class cannot be used unless you <subscribe> to a multicast group.
   * If you just want to send (and not listen) to a multicast group,
   * use <ACE_SOCK_Dgram> or <ACE_SOCK_CODgram> instead.
   */
  ACE_SOCK_Dgram_Mcast (void);

  /// Default dtor.
  ~ACE_SOCK_Dgram_Mcast (void);

  // = Multicast group management routines.

  /**
   * This is a BSD-style method (i.e., no QoS) for joining a multicast
   * group.  The network interface device driver is instructed to
   * accept datagrams with <mcast_addr> multicast addresses.  If the
   * socket has already been opened, <subscribe> closes the socket and
   * opens a new socket bound to the <mcast_addr>.
   *
   * The <net_if> interface is hardware specific, e.g., use "netstat
   * -i" to find whether your interface is, such as "le0" or something
   * else.  If net_if == 0, <subscribe> uses the default mcast
   * interface.  Returns: -1 if the call fails.
   *
   * Note that some platforms, such as pSoS, support only number, not
   * names, for network interfaces.  For these platforms, just give
   * these numbers in alphanumeric form and <subscribe> will convert
   * them into numbers via <ACE_OS::atoi>.
   */
  int subscribe (const ACE_INET_Addr &mcast_addr,
                 int reuse_addr = 1,
                 const ACE_TCHAR *net_if = 0,
                 int protocol_family = PF_INET,
                 int protocol = 0);

  /**
   * Leave a multicast group identified by <mcast_addr>.  The <net_if>
   * interface is hardware specific.  Use something like "netstat -i"
   * to find whether your interface is, such as "le0" or something
   * else.  If <net_if> == 0, <subscribe> uses the default mcast
   * interface.  Returns: -1 if the call fails.
   *
   * Note that some platforms, such as pSoS, support only number, not
   * names, for network interfaces.  For these platforms, just give
   * these numbers in alphanumeric form and <subscribe> will convert
   * them into numbers via <ACE_OS::atoi>.
   */
  int unsubscribe (const ACE_INET_Addr &mcast_addr,
                   const ACE_TCHAR *net_if = 0,
                   int protocol_family = PF_INET,
                   int protocol = 0);

  /// Unsubscribe from a multicast group.  Returns -1 if the call
  /// fails.
  int unsubscribe (void);

  // = Data transfer routines.
  /// Send <n> bytes in <buf>.
  ssize_t send (const void *buf,
                size_t n,
                int flags = 0) const;

  /// Send <n> <iovecs>.
  ssize_t send (const iovec iov[],
                size_t n,
                int flags = 0) const;

  // = Options.
  /**
   * Set an ip option that takes a char as input, such as
   * <IP_MULTICAST_LOOP> or <IP_MULTICAST_TTL>.  This is just a more
   * concise nice interface to a subset of possible
   * <ACE_SOCK::set_option> calls.  Returns 0 on success, -1 on
   * failure.
   */
  int set_option (int option,
		  char optval);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  // = Disable public <open> method to ensure class used properly.

  /// Not publically visible.
  int open (const ACE_Addr &mcast_addr,
            int protocol_family = PF_INET,
            int protocol = 0,
            int reuse_addr = 0);

  /// Not publically visible.
  int open (const ACE_Addr &mcast_addr,
            const ACE_QoS_Params &qos_params,
            int protocol_family = PF_INET,
            int protocol = 0,
            ACE_Protocol_Info *protocolinfo = 0,
            ACE_SOCK_GROUP g = 0,
            u_long flags = 0,
            int reuse_addr = 0);

  /// Subscribe to the multicast interface using BSD-style semantics
  /// (no QoS).
  int subscribe_ifs (const ACE_INET_Addr &mcast_addr,
                     const ACE_TCHAR *net_if,
                     int protocol_family,
                     int protocol,
                     int reuse_addr);

  /// Unsubscribe to multicast interfaces subscribed to previously by
  /// <subscribe_ifs>.
  int unsubscribe_ifs (const ACE_INET_Addr &mcast_addr,
                       const ACE_TCHAR *net_if = 0,
                       int protocol_family = PF_INET,
                       int protocol = 0);

  // = Disable public use of <ACE_SOCK_Dgram::send>s

  // This forces <ACE_SOCK_Dgram_Mcast::send>s inline.
  ssize_t send (const void *buf,
                size_t n,
                const ACE_Addr &addr,
                int flags = 0) const;
  ssize_t send (const iovec iov[],
                size_t n,
                const ACE_Addr &addr,
                int flags = 0) const;

protected:
  /// Initialize the <multicast_address_> IP address.
  int make_multicast_address (const ACE_INET_Addr &mcast_addr,
                              const ACE_TCHAR *net_if = ACE_LIB_TEXT ("le0"));

  /// Initialize a multicast address.  This method factors out common
  /// code called by <make_multicast_address> and <subscribe>.
  int make_multicast_address_i (const ACE_INET_Addr &mcast_addr,
                                ip_mreq& multicast_address,
                                const ACE_TCHAR *net_if = ACE_LIB_TEXT ("le0"));

  /// A copy of the address that we use to <send> multicasts.
  ACE_INET_Addr mcast_addr_;

  /// IP address of the interface upon which we're receiving
  /// multicasts.
  ip_mreq mcast_request_if_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Sockets/SOCK_Dgram_Mcast.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif /* ACE_SOCK_DGRAM_MCAST_H */
