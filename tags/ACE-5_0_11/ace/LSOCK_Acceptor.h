/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    LSOCK_Aceeptor.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_LOCAL_SOCK_ACCEPTOR_H
#define ACE_LOCAL_SOCK_ACCEPTOR_H

#include "ace/SOCK_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/UNIX_Addr.h"
#include "ace/LSOCK_Stream.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

// Forward decl.
class ACE_Reactor;

class ACE_Export ACE_LSOCK_Acceptor : public ACE_SOCK_Acceptor
{
  // = TITLE
  //     Defines the format and interface for the acceptor side of the
  //     local ACE_SOCK ACE_Stream.
public:
  // = Initialization methods.
  ACE_LSOCK_Acceptor (void);
  // Default constructor.

  ACE_LSOCK_Acceptor (const ACE_Addr &local_sap,
                      int reuse_addr = 0,
                      int protocol_family = PF_UNIX,
                      int backlog = ACE_DEFAULT_BACKLOG,
                      int protocol = 0);
  // Initiate a passive mode socket.

  int open (const ACE_Addr &local_sap,
            int reuse_addr = 0,
            int protocol_family = PF_UNIX,
            int backlog = ACE_DEFAULT_BACKLOG,
            int protocol = 0);
  // Initiate a passive mode socket.

  int accept (ACE_LSOCK_Stream &new_ipc_sap,
              ACE_Addr * = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new data transfer connection.

  int remove (void);
  // Close down the ACE_LSOCK and remove the rendezvous point from the
  // file system.

  int get_local_addr (ACE_Addr &) const;
  // Return the local endpoint address.

  // = Meta-type info
  typedef ACE_UNIX_Addr PEER_ADDR;
  typedef ACE_LSOCK_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_UNIX_Addr local_addr_;
  // Address of our rendezvous point.
};

#if defined (__ACE_INLINE__)
#include "ace/LSOCK_Acceptor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif /* ACE_LOCAL_SOCK_ACCEPTOR_H */
