/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SOCK_Dgram_Mcast.h
//
// = AUTHORS
//    Irfan Pyrali <irfan@cs.wustl.edu>,
//    Tim Harrison <harrison@cs.wustl.edu>, and
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_SOCK_DGRAM_MCAST_H
#define ACE_SOCK_DGRAM_MCAST_H

#include "ace/SOCK_Dgram.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"

class ACE_Export ACE_SOCK_Dgram_Mcast : public ACE_SOCK_Dgram
{
  // = TITLE
  //     Defines the member functions for the ACE socket wrapper
  //     for UDP/IP multicast.
public:
  // = Initialization routine.
  ACE_SOCK_Dgram_Mcast (void);
  // Note that there is no public <open> method.  Therefore, this
  // class cannot be used unless you <subscribe> to a multicast group.
  // If you just want to send (and not listen) to a multicast group,
  // use <ACE_SOCK_Dgram> or <ACE_SOCK_CODgram> instead.

  ~ACE_SOCK_Dgram_Mcast (void);
  // Default dtor.

  // = Multicast group management routines.

  int subscribe (const ACE_INET_Addr &mcast_addr,
                 int reuse_addr = 1,
                 const ASYS_TCHAR *net_if = 0,
                 int protocol_family = PF_INET,
                 int protocol = 0);
  // This is a BSD-style method for joining a multicast group (i.e.,
  // no QoS).  The network interface device driver is instructed to
  // accept datagrams with <mcast_addr> multicast addresses.  If the
  // socket has already been opened, <subscribe> closes the socket and
  // opens a new socket bound to the <mcast_addr>.
  //
  // The <net_if> interface is hardware specific, e.g., use "netstat
  // -i" to find whether your interface is, such as "le0" or something
  // else.  If net_if == 0, <subscribe> uses the default mcast
  // interface.  Returns: -1 on error, else 0.
  // 
  // Note that some platforms, such as pSoS, support only number, not
  // names, for network interfaces.  For these platforms, just give
  // these numbers in alphanumeric form and <subscribe> will convert
  // them into numbers via <ACE_OS::atoi>.

  int subscribe (const ACE_INET_Addr &mcast_addr,
		 ACE_Connect_QoS_Params qos_params,
                 int reuse_addr = 1,
                 const ASYS_TCHAR *net_if = 0,
                 int protocol_family = PF_INET,
                 int protocol = 0);
  // This is a QoS-enabled method for joining a multicast group, which
  // passes <qos_params> via <ACE_OS::join_leaf>.  The network
  // interface device driver is instructed to accept datagrams with
  // <mcast_addr> multicast addresses.  If the socket has already been
  // opened, <subscribe> closes the socket and opens a new socket
  // bound to the <mcast_addr>.
  //
  // The <net_if> interface is hardware specific, e.g., use "netstat
  // -i" to find whether your interface is, such as "le0" or something
  // else.  If net_if == 0, <subscribe> uses the default mcast
  // interface.  Returns: -1 on error, else 0.
  // 
  // Note that some platforms, such as pSoS, support only number, not
  // names, for network interfaces.  For these platforms, just give
  // these numbers in alphanumeric form and <subscribe> will convert
  // them into numbers via <ACE_OS::atoi>.

  int unsubscribe (const ACE_INET_Addr &mcast_addr,
                   const ASYS_TCHAR *net_if = 0,
                   int protocol_family = PF_INET,
                   int protocol = 0);
  // Leave a multicast group identified by <mcast_addr>.  The <net_if>
  // interface is hardware specific.  Use something like "netstat -i"
  // to find whether your interface is, such as "le0" or something
  // else.  If <net_if> == 0, <subscribe> uses the default mcast
  // interface.  Returns: -1 on error, else 0.
  // 
  // Note that some platforms, such as pSoS, support only number, not
  // names, for network interfaces.  For these platforms, just give
  // these numbers in alphanumeric form and <subscribe> will convert
  // them into numbers via <ACE_OS::atoi>.

  int unsubscribe (void);
  // Unsubscribe from a multicast group.  Returns 0 on success, -1 on
  // failure.

  // = Data transfer routines.
  ssize_t send (const void *buf,
                size_t n,
                int flags = 0) const;
  // Send <n> bytes in <buf>.

  ssize_t send (const iovec iov[],
                size_t n,
                int flags = 0) const;
  // Send <n> <iovecs>.

  // = Options.
  int set_option (int option, 
		  char optval);
  // Set an ip option that takes a char as input, such as
  // <IP_MULTICAST_LOOP>.  This is just a more concise nice interface
  // to a subset of possible <ACE_SOCK::set_option> calls.  Returns 0
  // on success, -1 on failure.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_HANDLE open (const ACE_Addr &local,
                   int protocol_family = PF_INET,
                   int protocol = 0);
  // Disable public use of <ACE_SOCK_Dgram::open> to ensure the class
  // is used properly.

  int subscribe_i (const ACE_INET_Addr &mcast_addr,
		   int reuse_addr = 1,
		   const ASYS_TCHAR *net_if = 0,
		   int protocol_family = PF_INET,
		   int protocol = 0);
  // Implementation method of <subscribe>.

  int unsubscribe_i (const ACE_INET_Addr &mcast_addr,
		     const ASYS_TCHAR *net_if = 0,
		     int protocol_family = PF_INET,
		     int protocol = 0);
  // Implementation method of <unsubscribe>.

  // = Disable public use of <ACE_SOCK_Dgram::send>s and force
  // <ACE_SOCK_Dgram_Mcast::send>s inline.
  ssize_t send (const void *buf,
                size_t n,
                const ACE_Addr &addr,
                int flags = 0) const;
  ssize_t send (const iovec iov[],
                size_t n,
                const ACE_Addr &addr,
                int flags = 0) const;

  int make_multicast_address (const ACE_INET_Addr &mcast_addr,
                              const ASYS_TCHAR *net_if = ASYS_TEXT ("le0"));
  // Initialize the <multicast_address_> IP address.

  int make_multicast_address_i (const ACE_INET_Addr &mcast_addr,
                                ip_mreq& multicast_address,
                                const ASYS_TCHAR *net_if = ASYS_TEXT ("le0"));
  // Initialize a multicast address.  This method factors out common
  // code called by <make_multicast_address> and <subscribe>.

  ACE_INET_Addr mcast_addr_;
  // A copy of the address that we use to <send> multicasts.

  ip_mreq mcast_request_if_;
  // IP address of the interface upon which we're receiving
  // multicasts.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Dgram_Mcast.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_SOCK_DGRAM_MCAST_H */
