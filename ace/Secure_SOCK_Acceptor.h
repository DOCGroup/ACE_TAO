/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Secure_SOCK_Acceptor.h
//
// = AUTHOR
//    John Heitmann
//
// ============================================================================

#define ACE_HAS_OPENSSL
#if defined (ACE_HAS_OPENSSL)

#ifndef ACE_SECURE_SOCK_ACCEPTOR_H
#define ACE_SECURE_SOCK_ACCEPTOR_H

#include "Secure_SOCK_Stream.h"
#include <ace/SOCK_Acceptor.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Secure_SOCK_Acceptor
{
  // = TITLE
  //   Defines a factory that creates new <ACE_Secure_SOCK_Stream>s passively.
  //
  // = DESCRIPTION
  //   The <ACE_Secure_SOCK_Acceptor> has its own <ACE_SOCK_Acceptor> which
  //   handles virtually all of the socket acceptance. This class is a wrapper
  //   which only adds the ssl acceptance.
public:
  // = Initialization and termination methods.
  ACE_Secure_SOCK_Acceptor (void);
  // Default constructor.

  ACE_Secure_SOCK_Acceptor (const ACE_Addr &local_sap,
                     int reuse_addr = 0,
                     int protocol_family = PF_INET,
                     int backlog = ACE_DEFAULT_BACKLOG,
                     int protocol = 0);
  // Initiate a passive mode ssl/BSD-style acceptor socket.
  // <local_sap> is the address that we-re going to listen for
  // connections on. jwh1: can reuse be used w/ ssl?

  ACE_Secure_SOCK_Acceptor (const ACE_Addr &local_sap,
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
  // Initiate a passive mode ssl/BSD-style acceptor socket.
  // <local_sap> is the address that we-re going to listen for
  // connections on. jwh1: can reuse be used w/ ssl?

  ~ACE_Secure_SOCK_Acceptor (void);
  // Default dtor.

  // = Passive connection <accept> methods.
  int accept (ACE_Secure_SOCK_Stream &new_stream,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new <ACE_Secure_SOCK_Stream> connection.  A <timeout> of 0
  // means block forever, a <timeout> of {0, 0} means poll.  <restart>
  // == 1 means "restart if interrupted," i.e., if errno == EINTR.

  int accept (ACE_Secure_SOCK_Stream &new_stream,
	      ACE_Accept_QoS_Params qos_params,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  // Accept a new <ACE_Secure_SOCK_Stream> connection using the RVSP QoS
  // information in <qos_params>.  A <timeout> of 0 means block
  // forever, a <timeout> of {0, 0} means poll.  <restart> == 1 means
  // "restart if interrupted," i.e., if errno == EINTR.


  int get_local_addr (ACE_Addr &) const;
  // Gets the address which is being listened on.

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_Secure_SOCK_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  int shared_ssl_init_one (ACE_Secure_SOCK_Stream& new_stream) const;
  // SSL init method which gets called before the socket acceptance.

  int shared_ssl_init_two (ACE_Secure_SOCK_Stream& new_stream) const;
  // SSL init method which gets calles after the socket acceptance,
  // SSL_accept is called here.

public:
  ACE_SOCK_Acceptor acceptor_;
  // The BSD-socket workhorse
};

#endif /* ACE_SECURE_SOCK_ACCEPTOR_H */
#endif /* defined (ACE_HAS_OPENSSL) */
