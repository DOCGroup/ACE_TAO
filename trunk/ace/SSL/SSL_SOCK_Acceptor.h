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
#include "ace/OS_QoS.h"

/**
 * @class ACE_SSL_SOCK_Acceptor
 *
 * @brief Defines a factory that creates new ACE_SSL_SOCK_Stream>s
 * passively.
 *
 * The ACE_SSL_SOCK_Acceptor has its own ACE_SOCK_Acceptor
 * which handles virtually all of the socket acceptance.  This
 * class is a wrapper which only adds the SSL acceptance.
 * @par
 * Since SSL is record-oriented, some additional steps must be taken
 * to make the ACE_SSL_SOCK_Acceptor interact properly with the
 * Reactor (if one is used) when performing non-blocking accept()
 * calls.  In particular, the ACE_SSL_SOCK_Acceptor registers an event
 * handler with the Reactor set in the constructor or in the
 * ACE_SSL_SOCK_Acceptor::reactor() method.  If no Reactor is
 * explicitly set, the singleton Reactor instance will be used.
 *
 * @note The user must currently ensure that only one thread services
 *       a given SSL session at any given time since some underlying
 *       SSL implementations, such as OpenSSL, are not entirely
 *       thread-safe or reentrant.
 */
class ACE_SSL_Export ACE_SSL_SOCK_Acceptor : public ACE_SSL_SOCK
{
public:

  /// Default constructor.
  ACE_SSL_SOCK_Acceptor (void);

  /// Default dtor.
  ~ACE_SSL_SOCK_Acceptor (void);

  /**
   * Initiate a passive mode SSL/BSD-style acceptor socket.
   * @param local_sap  The address that we're going to listen for
   *                   connections on.
   */
  ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
			 int reuse_addr = 0,
			 int protocol_family = PF_INET,
			 int backlog = ACE_DEFAULT_BACKLOG,
			 int protocol = 0);

  /// Initiate a passive-mode QoS-enabled acceptor socket.
  ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
			 ACE_Protocol_Info *protocolinfo,
			 ACE_SOCK_GROUP g,
			 u_long flags,
			 int reuse_addr,
			 int protocol_family,
			 int backlog = ACE_DEFAULT_BACKLOG,
			 int protocol = 0);

  /**
   * Initiate a passive mode SSL/BSD-style acceptor socket.
   * @param local_sap  The address that we're going to listen for
   *                   connections on.
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
   *
   * These are the canonical methods exposed by the Acceptor pattern.
   */
  //@{
  /**
   * Accept a new ACE_SSL_SOCK_Stream connection.  A timeout of 0
   * means block forever, a timeout of {0, 0} means poll.  restart
   * == 1 means "restart if interrupted," i.e., if errno == EINTR.
   */
  int accept (ACE_SSL_SOCK_Stream &new_stream,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;

  /**
   * Accept a new ACE_SSL_SOCK_Stream connection using the RVSP QoS
   * information in qos_params.  A timeout of 0 means block
   * forever, a timeout of {0, 0} means poll.  restart == 1 means
   * "restart if interrupted," i.e., if errno == EINTR.
   */
  int accept (ACE_SSL_SOCK_Stream &new_stream,
	      ACE_Accept_QoS_Params qos_params,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0) const;
  //@}

  /// Meta-type info
  //@{
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_SSL_SOCK_Stream PEER_STREAM;
  //@}

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:

  /// Complete SSL passive connection establishment.
  int ssl_accept (ACE_SSL_SOCK_Stream &new_stream,
                  ACE_Time_Value *timeout) const;

private:

  /// The BSD-socket workhorse
  ACE_SOCK_Acceptor acceptor_;

};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Acceptor.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"

#endif /* ACE_SSL_SOCK_ACCEPTOR_H */

