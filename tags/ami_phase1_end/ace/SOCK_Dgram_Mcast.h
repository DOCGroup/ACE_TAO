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
//    Irfan Pyrali (ip1@cs.wustl.edu)
//    Tim Harrison (harrison@cs.wustl.edu)
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
  //     Defines the member functions for the ACE SOCK wrapper
  //     multicast abstraction.
public:
  // = Initialization routine.
  ACE_SOCK_Dgram_Mcast (void);
  // Note that there is no open ().  This cannot be used unless you
  // subscribe to the multicast group.  If you just want to send (and
  // not listen) to the multicast group, use ACE_SOCK_Dgram or
  // ACE_SOCK_CODgram.

  ~ACE_SOCK_Dgram_Mcast (void);
  // Default dtor.

  // = Multicast group management routines.

  int subscribe (const ACE_INET_Addr &mcast_addr,
                 int reuse_addr = 1,
#if defined (ACE_PSOS)
                 // pSOS supports numbers, not names for network interfaces
                 long net_if = 0,
#else
                 const ASYS_TCHAR *net_if = 0,
#endif /* defined (ACE_PSOS) */
                 int protocol_family = PF_INET,
                 int protocol = 0);
  // Join a multicast group by telling the network interface device
  // driver to accept datagrams with ACE_INET_Addr &mcast_addr
  // multicast addresses.
  //
  // If you have called open already, subscribe closes the socket and
  // opens a new socket bound to the mcast_addr.
  //
  // Interface is hardware specific. use netstat -i to find whether
  // your interface is, say, le0 or something else.  If net_if == 0,
  // subscribe uses the default mcast interface.
  // Returns: -1 on error, else 0.

  int unsubscribe (const ACE_INET_Addr &mcast_addr,
#if defined (ACE_PSOS)
                   // pSOS supports numbers, not names for network interfaces
                   long net_if = 0,
#else
                   const ASYS_TCHAR *net_if = 0,
#endif /* defined (ACE_PSOS) */
                   int protocol_family = PF_INET,
                   int protocol = 0);
  // Leave a multicast group.
  //
  // Interface is hardware specific. use netstat -i to find whether
  // your interface is, say, le0 or something else.  If net_if == 0,
  // subscribe uses the default mcast interface.
  // Returns: -1 on error, else 0.

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
  int set_option (int option, char optval);
  // Set an ip option that takes a char as input.
  // e.g. IP_MULTICAST_LOOP.  This is just a nice interface to a
  // subset of possible setsockopt/ACE_SOCK::set_option calls Returns
  // 0 on success, -1 on failure.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_HANDLE open (const ACE_Addr &local,
                   int protocol_family = PF_INET,
                   int protocol = 0);
  // disable public use of ACE_SOCK_Dgram::open ()

  // = Disable public use of ACE_SOCK_Dgram::sends and force
  // ACE_SOCK_Dgram_Mcast::sends inline
  ssize_t send (const void *buf,
                size_t n,
                const ACE_Addr &addr,
                int flags = 0) const;
  ssize_t send (const iovec iov[],
                size_t n,
                const ACE_Addr &addr,
                int flags = 0) const;

  int make_multicast_address (const ACE_INET_Addr &mcast_addr,
#if defined (ACE_PSOS)
                              // pSOS supports numbers, not
                              // names for network interfaces
                              long net_if = 0
#else
                              const ASYS_TCHAR *net_if = ASYS_TEXT ("le0")
#endif /* defined (ACE_PSOS) */
                             );
  // Initialize the <multicast_addres_ field>

  int make_multicast_address_i (const ACE_INET_Addr &mcast_addr,
                                ip_mreq& multicast_address,
#if defined (ACE_PSOS)
                                // pSOS supports numbers, not
                                // names for network interfaces
                                long net_if = 0
#else
                                const ASYS_TCHAR *net_if = ASYS_TEXT ("le0")
#endif /* defined (ACE_PSOS) */
                                );
  // Initialize a multicast address.

  ACE_INET_Addr mcast_addr_;
  // Multicast group address.

  ip_mreq multicast_address_;
  // IP address.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Dgram_Mcast.i"
#endif

#endif /* ACE_SOCK_DGRAM_MCAST_H */
