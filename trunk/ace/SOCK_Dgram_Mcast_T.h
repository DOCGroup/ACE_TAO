/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SOCK_Dgram_Mcast_T.h
 *
 *  $Id$
 *
 *  @author Irfan Pyrali <irfan@cs.wustl.edu>
 *  @author Tim Harrison <harrison@cs.wustl.edu>
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author and Bill Fulton <bill_a_fulton@raytheon.com>
 */
//=============================================================================

#ifndef ACE_SOCK_DGRAM_MCAST_T_H
#define ACE_SOCK_DGRAM_MCAST_T_H
#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/Containers_T.h"
#include "ace/Synch_T.h"

/**
 * @class ACE_SOCK_Dgram_Mcast_Ex
 *
 * @brief Defines the ACE socket wrapper for UDP/IP multicast.
 *
 * Supports multiple simultaneous subscriptions, unsubscription from one or
 * all subscriptions, and independent send/recv address and interface
 * specifications.  Template parameters and/or ctor arguments determine
 * per-instance optional functionality.
 * Note that multicast semantics and implementation details are _very_
 * environment-specific; this class is just a wrapper around the underlying
 * implementation and does not try to normalize the concept of multicast
 * communications.
 *
 * Usage Notes:
 * - Send and Recv addresses and network interfaces, but not port#, are
 *   independent.  While this instance is open, 1 send interface (and a default
 *   send address) is in effect and 0, 1, or multiple recv addresses/interfaces
 *   are in effect.
 * - The first <open>/<subscribe> invocation defines the network interface
 *   and default address used for all sends by this instance, defines the
 *   port# and optionally the multicast address bound to the underlying
 *   socket, and defines the (one) port# that is used for all subscribes
 *   (subsequent subscribes must pass the same port# or '0').
 * - The default loopback state is not explicitly set; the environment will
 *   determine the default state.  Note that some environments (e.g. some Win32)
 *   do not allow the default to be changed, and that the semantics of
 *   loopback control are environment dependent (e.g. sender vs receiver
 *   control).
 * - In general, due to multicast design and implementation quirks/bugs, it is
 *   difficult to tell which address a received message was sent to or which
 *   interface it was received on (even if only one subscription is active).
 *   However; there are filtering options that can be applied, to narrow it
 *   down considerably.
 *
 * Interface specification notes (for <subscribe> and <unsubscribe>):
 * - If net_if == NULL, the null_iface_opt option determines whether only the
 *   system "default" interface or all interfaces is affected.  Specifying
 *   "all" interfaces is supported only for environments for which
 *   <ACE_Sock_Connect::get_ip_interfaces> is properly implemented.
 * - Multiple subscriptions for the same address but different interfaces is
 *   normally supported, but re-subscription to an address/interface that is
 *   already subscribed is normally not allowed.
 * - The <net_if> interface specification syntax is environment-specific.
 *   UNIX systems will normally use device specifications such as "le0" or
 *   "elxl1", while other systems will use the IP address of the interface.
 *   Some platforms, such as pSoS, support only cardinal numbers as network
 *   interface specifications; for these platforms, just give these numbers in
 *   alphanumeric form and <subscribe> will convert them into numbers via
 *   <ACE_OS::atoi>.
 */
template <class ACE_SDM_LOCK>
class ACE_SOCK_Dgram_Mcast_Ex : public ACE_SOCK_Dgram
{
public:

  /**
   * @brief Option parameters.
   *
   * These control per-instance optional functionality.  They are set via
   * optional constructor arguments.
   * Note: Certain option values are not valid for all environments (see
   * comments in source file for environment-specific restrictions).  Default
   * values are always valid values for the compilation environment.
   */
  enum  options
  {
  // Define whether a specific (multicast) address (in addition to the port#)
  // is bound to the socket.
  // Notes:
  // - Effect of doing this is stack/environment dependent, but in most
  //   environments can be used to filter out unwanted unicast, broadcast, and
  //   (other) multicast messages sent to the same port#.
  // - Some IP stacks (e.g. some Win32) do not support binding multicast
  //   addresses.  Using this option will always cause an <open> error.
  // - It not strictly possible for user code to do this level of filtering
  //   w/out the bind; some environments support ways to determine which address
  //   a message was sent _to_, but this class interface does not support access
  //   to that info.
  // - The address (and port#) passed to <open> (or the first <subscribe>, if
  //   <open> is not explicitly invoked) is the one that is bound.
  //
    /// Disable address bind. (Bind port only.)
    OPT_BINDADDR_NO    = 0,
    /// Enable address bind. (Bind port and address.)
    OPT_BINDADDR_YES   = 1,
    /// Default value for BINDADDR option. (Environment-dependent.)
#ifdef linux
      // (Bring forward ad-hoc legacy hack: enable if 'linux' macro is defined)
      DEFOPT_BINDADDR  = OPT_BINDADDR_YES,
#elif  defined (ACE_WIN32)
      // At least some Win32 OS's can not bind mcast addr, so disable it.
      DEFOPT_BINDADDR  = OPT_BINDADDR_NO,
#else
      // General-purpose default behavior is 'disabled', since effect is
      // environment-specific and side-effects might be surprising.
      DEFOPT_BINDADDR  = OPT_BINDADDR_NO,
#endif /* linux */
  //
  /// Define the interpretation of 0 as a recv interface specification.
  // If the interface part of a multicast address specification is 0, it
  // will be interpreted to mean either "the default interface" or "all
  // interfaces", depending on the setting of this option.
  // Notes:
  // - The 'nulliface_all' option can not be used in environments which do
  //   not fully support the <ACE_Sock_Connect::get_ip_interfaces> method
  //   (e.g. non-Windows).
  //   If it is, using 0 for iface will _always_ fail.
  // - The default behavior in most IP stacks is to use the 'default' interface,
  //   where 'default' has rather ad-hoc semantics.
  // - This applies only to receives, not sends (which always use only one
  //   interface; 0 means use the "system default" interface).
  // Supported values:
    /// If (net_if==0), use default interface.
    OPT_NULLIFACE_ONE  = 0,
    /// If (net_if==0), use all mcast interfaces.
    OPT_NULLIFACE_ALL  = 2,
    /// Default value for NULLIFACE option. (Environment-dependent.)
#ifdef ACE_WIN32
      // This is the (ad-hoc) legacy behavior for Win32/WinSock.
      // Notice: Older version of WinSock/MSVC may not get all multicast-capable
      // interfaces (e.g. PPP interfaces).
      DEFOPT_NULLIFACE = OPT_NULLIFACE_ALL,
#else
      // General-purpose default behavior (as per legacy behavior).
      DEFOPT_NULLIFACE = OPT_NULLIFACE_ONE,
#endif /* ACE_WIN32 */
  /// Control explicit unsubscribe events by the dtor.
  // If enabled, the destructor does an explicit <unsubscribe> for every active
  // subscription.
  // Notes:
  // - This is not normally necessary or useful (an implicit <unsubscribe> is
  //   done by most IP stacks when the socket is closed by the dtor); but some
  //   environments may either require this or this may reduce latency of the
  //   "leave" event on the network.  Also; if the underlying socket is
  //   duplicated (e.g. by fork()), and the duplicate instance/fd is not
  //   intended to be used, this may be a way to "force" a "leave" event when
  //   the parent's instance is destoyed (e.g. on exit).
  // - The same effect can be acheived by invoking <unsubscribe>(void).
  // Supported values:
    /// Dtor does not do explicit <unsubscribe>.
    OPT_DTORUNSUB_NO   = 0,
    /// Dtor does explicit <unsubscribe>.
    OPT_DTORUNSUB_YES  = 4,
    /// Default value for DTORUNSUB option.
    DEFOPT_DTORUNSUB   = OPT_DTORUNSUB_NO
  };

  // = Initialization routines.

  /// Ctor - Create an unitialized instance and define per-instance optional
  /// functionality.
  /**
   * You must invoke <open> or <subscribe>, to create/bind a socket and define
   * operational parameters, before performing any I/O with this instance.
   */
  ACE_SOCK_Dgram_Mcast_Ex (options bind_addr_opt = DEFOPT_BINDADDR,
                           options null_iface_opt = DEFOPT_NULLIFACE,
                           options dtor_unsub_opt = DEFOPT_DTORUNSUB);

  /// Dtor - Release all resources and implicitly or explicitly unsubscribe
  /// from all currently subscribed groups.
  /**
   * The dtor_unsub_opt_ option defines whether an explicit <unsusbcribe> is
   * done by the destructor.  If not, most systems will automatically
   * unsubscribe upon the close of the socket.
   */
  ~ACE_SOCK_Dgram_Mcast_Ex (void);

  /// Explicitly open/bind the socket and define the network interface
  /// and default multicast address used for sending messages.
  /**
   * This method is optional; if not explicitly invoked, it is invoked by
   * the first <subscribe>, using the subscribed address/port# and network
   * interface paramters.
   * The <mcast_addr> parameter defines the default send address/port# and
   * also the port# and, if the OPT_BINDADDR_YES option is used,
   * the multicast address that is bound to this socket.
   * If the <send_net_if> parameter != NULL, it defines the network interface
   * used for all sends by this instance, otherwise the system "default"
   * interface is used. (The <send_net_if> parameter is ignored if this
   * feature is not supported by the envriornment.)
   * The port# in <mcast_addr> may be 0, in which case a system-assigned
   * (ephemeral) port# is used for sending and receiving.
   * If <reuse_addr> != 0, the SO_REUSEADDR option and, if it is supported,
   * the SO_REUSEPORT option are enabled.
   *
   * Returns: -1 if the call fails.  Failure can occur due to problems with
   * the address, port#, and/or interface parameters or during system open()
   * or socket option processing.
   */
  int open (const ACE_INET_Addr &mcast_addr,        // Bound & sendto address.
            const ACE_TCHAR *send_net_if = 0,    // Net interface for sends.
            int reuse_addr = 1);                    // Reuse addr/port sock opt.

  // = Multicast group subscribe/unsubscribe routines.

  /// Join a multicast group on a given interface (or all interfaces, if
  /// supported).
  /**
   * The given group is joined on the specified interface.  If option
   * OPT_NULLIFACE_ALL is used and <net_if> is NULL, the group is joined on
   * all multicast capable interfaces (IFF supported).  Multiple subscriptions
   * to various address and interface combinations are supported and tracked.
   * If this is the first invocation of <subscribe>, and <open> was not
   * previously invoked, <open> will be invoked using <mcast_addr> for binding
   * the socket and <net_if> as the interface for <send>.
   *
   * Returns: -1 if the call fails.  Failure can occur due to problems with
   * the address, port#, and/or interface parameters or during the subscription
   * attempt.  Once bind() has been invoked (by the first <open> or
   * <subscribe>), returns errno of ENXIO if the port# is not 0 and does not
   * match the bound port#, or if OPT_BINDADDR_YES option is used
   * and the address does not match the bound address.  Returns errno of
   * ENODEV if the addr/port#/interface parameters appeared valid, but no
   * subscription(s) succeeded.  An error is unconditionally returned if
   * option OPT_NULLIFACE_ALL is used, <net_if> is NULL, and 
   * <ACE_Sock_Connect::get_ip_interfaces> is not implemented in this
   * environment.
   *
   * Note that the optional <reuse_addr> parameter does not apply to 
   * subscriptions; it is only used if <open> is implicitly invoked (see above).
   *
   * NOTICE - Deprecated paramters: optional paramaters <protocol_family> and
   * <protocol> have no effect, and may be removed in a future release (by
   * adding a new method signature). New code should not explicitly specify
   * these default parameters.
   */
  int subscribe (const ACE_INET_Addr &mcast_addr,
                 int reuse_addr = 1,               // (see above)
                 const ACE_TCHAR *net_if = 0,
                 int protocol_family = PF_INET,    // (deprecated - not used)
                 int protocol = 0);                // (deprecated - not used)

  /// Leave a multicast group on a given interface (or all interfaces, if
  /// supported).
  /**
   * The specified group/interface combination is unsubscribed.  If option
   * OPT_NULLIFACE_ALL is used and <net_if> is NULL, the group is unsubscribed
   * from all interfaces (IFF supported).
   *
   * Returns: -1 if the unsubscribe failed. Most environments will return -1
   * if there was no active subscription for this address/interface combination.
   * An error is unconditionally returned if option OPT_NULLIFACE_ALL is used,
   * <net_if> is NULL, and <ACE_Sock_Connect::get_ip_interfaces> is not
   * implemented in this environment (_even if_ the <subscribe> specifies a
   * non- NULL <net_if>).
   *
   * NOTICE - Deprecated paramters: optional paramaters <protocol_family> and
   * <protocol> have no effect, and may be removed in a future release (by
   * adding a new method signature). New code should not explicitly specify
   * these default parameters.
   */
  int unsubscribe (const ACE_INET_Addr &mcast_addr,
                   const ACE_TCHAR *net_if = 0,
                   int protocol_family = PF_INET,  // (deprecated - not used)
                   int protocol = 0);              // (deprecated - not used)

  /// Unsubscribe all current subscriptions.
  /**
   * Unsubscribe all active group/interface subscriptions (if any).
   *
   * Returns -1 if any unsubscribe failed, 0 if there are no errors or no
   * current subscriptions.
   */
  int unsubscribe (void);

  // = Data transfer routines.

  /// Send <n> bytes in <buf>, using the multicast address and network interface
  /// defined by the first <open> or <subscribe>.
  ssize_t send (const void *buf,
                size_t n,
                int flags = 0) const;

  /// Send <n> <iovecs>, using the multicast address and network interface
  /// defined by the first <open> or <subscribe>.
  ssize_t send (const iovec[],
                size_t n,
                int flags = 0) const;

  // = Options.

  /// Set a socket option.
  /**
   * Set an ip option that takes a char as input, such as <IP_MULTICAST_LOOP>
   * or <IP_MULTICAST_TTL>.  This is just a more concise, nice interface to a
   * subset of possible <ACE_SOCK::set_option> calls.
   * Returns 0 on success, -1 on failure.
   */
  int set_option (int option,
                  char optval);

  /// Dump the state of an object.
  /**
   * Logs the setting of all options, the bound address, the send address and
   * interface, and the list of current subscriptions.
   */
  void dump (void) ACE_CONST_WHEN_MUTABLE;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  /// Subscribe to a multicast address on one or more network interface(s).
  /// (No QoS support.)
  int subscribe_ifs (const ACE_INET_Addr &mcast_addr,
                     const ACE_TCHAR *net_if,
                     int protocol_family,
                     int protocol,
                     int reuse_addr);

  /// Do subscription processing w/out updating the subscription list.
  //  (Layered method for <subscribe> processing).
  int subscribe_2 (const ACE_INET_Addr &mcast_addr,
                   int reuse_addr = 1,
                   const ACE_TCHAR *net_if = 0,
                   int protocol_family = PF_INET,
                   int protocol = 0);

  /// Unsubscribe from a multicast address on one or more network interface(s).
  int unsubscribe_ifs (const ACE_INET_Addr &mcast_addr,
                       const ACE_TCHAR *net_if = 0,
                       int protocol_family = PF_INET,
                       int protocol = 0);

  /// Do unsubscription processing w/out udpating subscription list.
  //  (Layered method for <unsubscribe> processing).
  int unsubscribe_2 (const ACE_INET_Addr &mcast_addr,
                     const ACE_TCHAR *net_if = 0,
                     int protocol_family = PF_INET,
                     int protocol = 0);

  /// Not publically visible - hidden base class method.
  int open (const ACE_Addr &mcast_addr,
            int protocol_family = PF_INET,
            int protocol = 0,
            int reuse_addr = 0);

  /// Not publically visible - unimplemented.
  int open (const ACE_Addr &mcast_addr,
            const ACE_QoS_Params &qos_params,
            int protocol_family = PF_INET,
            int protocol = 0,
            ACE_Protocol_Info *protocolinfo = 0,
            ACE_SOCK_GROUP g = 0,
            u_long flags = 0,
            int reuse_addr = 0);

  // = Disable public use of <ACE_SOCK_Dgram::send>s
  // This forces <ACE_SOCK_Dgram_Mcast_Ex::send>s inline.
  /// Not publically accessible - internal use only.
  ssize_t send (const void *buf,
                size_t n,
                const ACE_Addr &addr,
                int flags = 0) const;
  /// Not publically accessible - internal use only.
  ssize_t send (const iovec iov[],
                size_t n,
                const ACE_Addr &addr,
                int flags = 0) const;

protected:
  /// Create a multicast addr/if pair, in format useful for system calls.
  /// If mreq param is NULL, just verify the passed addr/interface specs.
  int make_multicast_ifaddr (ip_mreq *mreq,     // Put result here, if != NULL.
                             const ACE_INET_Addr &mcast_addr,
                             const ACE_TCHAR *net_if);

  /// Create a multicast addr/if pair.  This method factors out common
  /// code called by <make_multicast_address> and <subscribe>.
  int make_multicast_ifaddr_i (ip_mreq* mreq,   // Put result here, if != NULL.
                               const ACE_INET_Addr &mcast_addr,
                               const ACE_TCHAR *net_if = ACE_LIB_TEXT ("le0"));

  /// Empty the dynamic subscription list.  Optionally unsubscribe from all
  /// groups in the list.
  int clear_subs_list (int do_unsubscribe);

private:

  /// Per-instance option: Bind address as well as port#.
  int bind_addr_opt_;

  /// Per-instance option: Interpretation of 0 interface specification.
  int null_iface_opt_;

  /// Per-instance option: Dtor does explicit <unsubscribe>.
  int dtor_unsub_opt_;

  /// Bound multicast address (or INADDR_ANY) and port#.
  ACE_INET_Addr  bound_addr_;

  /// Multicast address to which local <send> methods send datagrams.
  ACE_INET_Addr  send_addr_;
  /// Network interface to which all <send> methods send multicast datagrams.
  ACE_TCHAR *send_net_if_;

  typedef ACE_DLList<ip_mreq>  subscription_list_t;
  typedef ACE_DLList_Iterator<ip_mreq>  subscription_list_iter_t;
  /// List of currently subscribed addr/iface pairs (and assc. types).
  ACE_MUTABLE subscription_list_t  subscription_list_;
  /// Lock used to protect subscription list.
  ACE_MUTABLE ACE_SDM_LOCK subscription_list_lock_;
      // (Lock type does not need to support recursive locking.)
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Dgram_Mcast_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/SOCK_Dgram_Mcast_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("SOCK_Dgram_Mcast_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_SOCK_DGRAM_MCAST_T_H */
