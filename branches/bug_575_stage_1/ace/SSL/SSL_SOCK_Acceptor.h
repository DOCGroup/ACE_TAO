// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSL_SOCK_Acceptor.h
 *
 *  $Id$
 *
 *  @author John Heitmann
 *  @author Chris Zimman
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef ACE_SSL_SOCK_ACCEPTOR_H
#define ACE_SSL_SOCK_ACCEPTOR_H

#include "ace/pre.h"

#include "SSL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSL_SOCK_Stream.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor.h"

/**
 * @class ACE_SSL_SOCK_Acceptor
 *
 * @brief Defines a factory that creates new <ACE_SSL_SOCK_Stream>s
 * passively.
 *
 * The ACE_SSL_SOCK_Acceptor has its own ACE_SOCK_Acceptor
 * which handles virtually all of the socket acceptance. This
 * class is a wrapper which only adds the SSL acceptance.
 */
class ACE_SSL_Export ACE_SSL_SOCK_Acceptor : public ACE_SSL_SOCK
{
public:

  /// Default constructor.
  ACE_SSL_SOCK_Acceptor (ACE_Reactor *reactor =
                           ACE_Reactor::instance ());

  /// Default dtor.
  ~ACE_SSL_SOCK_Acceptor (void);

  /**
   * Initiate a passive mode SSL/BSD-style acceptor socket.
   * "local_sap" is the address that we're going to listen for
   * connections on.
   */
  ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
			 int reuse_addr = 0,
			 int protocol_family = PF_INET,
			 int backlog = ACE_DEFAULT_BACKLOG,
			 int protocol = 0,
                         ACE_Reactor *reactor =
                           ACE_Reactor::instance ()
);

  /// Initialize a passive-mode QoS-enabled acceptor socket.  Returns 0
  /// on success and -1 on failure.
  ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
			 ACE_Protocol_Info *protocolinfo,
			 ACE_SOCK_GROUP g,
			 u_long flags,
			 int reuse_addr,
			 int protocol_family,
			 int backlog = ACE_DEFAULT_BACKLOG,
			 int protocol = 0,
                         ACE_Reactor *reactor =
                           ACE_Reactor::instance ());

  /**
   * Initiate a passive mode SSL/BSD-style acceptor socket.
   * "local_sap" is the address that we're going to listen for
   * connections on.
   */
  int open (const ACE_Addr &local_sap,
            int reuse_addr = 0,
            int protocol_family = PF_INET,
            int backlog = ACE_DEFAULT_BACKLOG,
            int protocol = 0);

  /// Close the listening socket.
  int close (void);

  /**
   * @name Passive Connection "accept" Methods
   */
  //@{
  /**
   * Accept a new <ACE_SSL_SOCK_Stream> connection.  A <timeout> of 0
   * means block forever, a <timeout> of {0, 0} means poll.  <restart>
   * == 1 means "restart if interrupted," i.e., if errno == EINTR.
   */
  int accept (ACE_SSL_SOCK_Stream &new_stream,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;

  /**
   * Accept a new <ACE_SSL_SOCK_Stream> connection using the RVSP QoS
   * information in <qos_params>.  A <timeout> of 0 means block
   * forever, a <timeout> of {0, 0} means poll.  <restart> == 1 means
   * "restart if interrupted," i.e., if errno == EINTR.
   */
  int accept (ACE_SSL_SOCK_Stream &new_stream,
	      ACE_Accept_QoS_Params qos_params,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  //@}

  /// Set the Reactor used when completing the SSL passive
  /// connection.
  void reactor (ACE_Reactor *r);

  /// Return the Reactor used when completing the SSL passive
  /// connection.
  ACE_Reactor *reactor (void) const;

  /// Meta-type info
  //@{
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_SSL_SOCK_Stream PEER_STREAM;
  //@}

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:

  /// Perform operations that must occur before <ACE_OS::accept> is
  /// called.
  int shared_accept_start (ACE_Time_Value *timeout,
			   int restart,
			   int &in_blocking_mode) const;

  /// Perform operations that must occur after <ACE_OS::accept> is
  /// called.
  int shared_accept_finish (ACE_SSL_SOCK_Stream &new_stream,
			    int in_blocking_mode,
			    int reset_new_handle) const;

  /// Complete blocking SSL passive connection establishment.
  int ssl_accept (ACE_SSL_SOCK_Stream &new_stream) const;

  /// Complete non-blocking SSL passive connection establishment.
  int ssl_accept (ACE_SSL_SOCK_Stream &new_stream,
                  ACE_Time_Value *timeout) const;

private:

  /// The BSD-socket workhorse
  ACE_SOCK_Acceptor acceptor_;

  /// Pointer to the Reactor responsible for dispatching the event
  /// handler responsible for completing the SSL passive connection.
  ACE_Reactor *reactor_;

};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Acceptor.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"

#endif /* ACE_SSL_SOCK_ACCEPTOR_H */

