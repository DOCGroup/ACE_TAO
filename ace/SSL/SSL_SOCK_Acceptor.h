// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_SSL
//
// = FILENAME
//    SSL_SOCK_Acceptor.h
//
// = AUTHOR
//    John Heitmann and Chris Zimman
//    Ossama Othman <ossama@ece.uci.edu>
//
// ============================================================================

#ifndef ACE_SSL_SOCK_ACCEPTOR_H
#define ACE_SSL_SOCK_ACCEPTOR_H

#include "SSL_SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_SSL)

class ACE_SSL_Export ACE_SSL_SOCK_Acceptor : public ACE_SSL_SOCK
{
  // = TITLE
  //     Defines a factory that creates new <ACE_SSL_SOCK_Stream>s
  //     passively.
  //
  // = DESCRIPTION
  //     The <ACE_SSL_SOCK_Acceptor> has its own <ACE_SOCK_Acceptor>
  //     which handles virtually all of the socket acceptance. This
  //     class is a wrapper which only adds the SSL acceptance.
public:
  // = Initialization and termination methods.
  ACE_SSL_SOCK_Acceptor (void);
  // Default constructor.

  ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
                     int reuse_addr = 0,
                     int protocol_family = PF_INET,
                     int backlog = ACE_DEFAULT_BACKLOG,
                     int protocol = 0);
  // Initiate a passive mode ssl/BSD-style acceptor socket.
  // <local_sap> is the address that we-re going to listen for
  // connections on.

  ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
                     ACE_Protocol_Info *protocolinfo,
                     ACE_SOCK_GROUP g,
                     u_long flags,
                     int reuse_addr,
                     int protocol_family,
                     int backlog = ACE_DEFAULT_BACKLOG,
                     int protocol = 0);
  // Initialize a passive-mode QoS-enabled acceptor socket.  Returns 0
  // on success and -1 on failure.

  int open (const ACE_Addr &local_sap,
            int reuse_addr = 0,
            int protocol_family = PF_INET,
            int backlog = ACE_DEFAULT_BACKLOG,
            int protocol = 0);
  // Initiate a passive mode SSL/BSD-style acceptor socket.
  // <local_sap> is the address that we-re going to listen for
  // connections on.

  int close (void);

  ~ACE_SSL_SOCK_Acceptor (void);
  // Default dtor.

  // = Passive connection <accept> methods.
  int accept (ACE_SSL_SOCK_Stream &new_stream,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new <ACE_SSL_SOCK_Stream> connection.  A <timeout> of 0
  // means block forever, a <timeout> of {0, 0} means poll.  <restart>
  // == 1 means "restart if interrupted," i.e., if errno == EINTR.

  int accept (ACE_SSL_SOCK_Stream &new_stream,
	      ACE_Accept_QoS_Params qos_params,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new <ACE_SSL_SOCK_Stream> connection using the RVSP QoS
  // information in <qos_params>.  A <timeout> of 0 means block
  // forever, a <timeout> of {0, 0} means poll.  <restart> == 1 means
  // "restart if interrupted," i.e., if errno == EINTR.

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_SSL_SOCK_Stream PEER_STREAM;

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  int shared_accept_start (ACE_Time_Value *timeout,
			   int restart,
			   int &in_blocking_mode) const;
  // Perform operations that must occur before <ACE_OS::accept> is
  // called.

  int shared_accept_finish (ACE_SSL_SOCK_Stream &new_stream,
			    int in_blocking_mode,
			    int reset_new_handle) const;
  // Perform operations that must occur after <ACE_OS::accept> is
  // called.

  int ssl_accept (ACE_SSL_SOCK_Stream &new_stream) const;
  // Complete SSL passive connection establishment.

private:
  ACE_SOCK_Acceptor acceptor_;
  // The BSD-socket workhorse
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Acceptor.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_SSL_SOCK_ACCEPTOR_H */
#endif /* ACE_HAS_SSL */
