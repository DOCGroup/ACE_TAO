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
  //   Defines a factory that creates new <ACE_Stream>s passively. 
  //
  // = DESCRIPTION
  //   The <ACE_SOCK_Acceptor> has its own "passive-mode" socket.
  //   This serves as a factory to create so-called "data-mode"
  //   sockets, which are what the <ACE_SOCK_Stream> encapsulates.
  //   Therefore, by inheriting from <ACE_SOCK>, <ACE_SOCK_Acceptor>
  //   gets its very own socket.
public:
  // = Initialization and termination methods.
  ACE_SOCK_Acceptor (void);
  // Default constructor.

  ACE_SOCK_Acceptor (const ACE_Addr &local_sap,
                     int reuse_addr = 0,
                     int protocol_family = PF_INET,
                     int backlog = ACE_DEFAULT_BACKLOG,
                     int protocol = 0);
    // Initiate a passive mode socket.

  int open (const ACE_Addr &local_sap,
            int reuse_addr = 0,
            int protocol_family = PF_INET,
            int backlog = ACE_DEFAULT_BACKLOG,
            int protocol = 0);
  // Initiate a passive mode socket.

  ~ACE_SOCK_Acceptor (void);
  // Default dtor.

  // = Passive connection <accept> methods.
  int accept (ACE_SOCK_Stream &new_stream,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new <ACE_SOCK_Stream> connection.  A <timeout> of 0
  // means block forever, a <timeout> of {0, 0} means poll.  <restart>
  // == 1 means "restart if interrupted," i.e., if errno == EINTR.

  int accept (ACE_SOCK_Stream &new_stream,
	      ACE_Accept_QoS_Params qos_params,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new <ACE_SOCK_Stream> connection using the RVSP QoS
  // information in <qos_params>.  A <timeout> of 0 means block
  // forever, a <timeout> of {0, 0} means poll.  <restart> == 1 means
  // "restart if interrupted," i.e., if errno == EINTR.

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_SOCK_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  int shared_accept_start (ACE_Time_Value *timeout,
			   int restart,
			   int &in_blocking_mode) const;
  // Perform operations that must occur before <ACE_OS::accept> is
  // called.

  int shared_accept_finish (ACE_SOCK_Stream new_stream,
			    int in_blocking_mode,
			    int reset_new_handle) const;
  // Perform operations that must occur after <ACE_OS::accept> is
  // called.

private:
  int get_remote_addr (ACE_Addr &) const;
  // Do not allow this function to percolate up to this interface...
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Acceptor.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_SOCK_ACCEPTOR_H */
