/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SOCK_Acceptor.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SOCK_ACCEPTOR_H
#define ACE_SOCK_ACCEPTOR_H

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"

class ACE_Export ACE_SOCK_Acceptor : public ACE_SOCK
{
  // = TITLE
  //     Defines the format and interface for an <ACE_Stream>
  //     acceptor.
public:
  // = Initialization methods.
  ACE_SOCK_Acceptor (void);
  // Default constructor.

  ACE_SOCK_Acceptor (const ACE_Addr &local_sap,
                     int reuse_addr = 0,
                     int protocol_family = PF_INET,
                     int backlog = 5,
                     int protocol = 0);
    // Initiate a passive mode socket.

  ~ACE_SOCK_Acceptor (void);
  // Default dtor.

  int open (const ACE_Addr &local_sap,
            int reuse_addr = 0,
            int protocol_family = PF_INET,
            int backlog = 5,
            int protocol = 0);
  // Initiate a passive mode socket.

  // = Passive connection acceptance method.
  int accept (ACE_SOCK_Stream &new_stream,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new data transfer connection.  A <timeout> of 0 means
  // block forever, a <timeout> of {0, 0} means poll.  <restart> == 1
  // means "restart if interrupted."

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_SOCK_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_HANDLE shared_accept (ACE_Addr *remote_addr,
                            ACE_Time_Value *,
                            int restart,
                            int reset_new_handle) const;
  // Shared by both the ACE_LSOCK_Acceptor and ACE_SOCK_Acceptor.

private:
  int get_remote_addr (ACE_Addr &) const;
  // Do not allow this function to percolate up to this interface...
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Acceptor.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_SOCK_ACCEPTOR_H */
