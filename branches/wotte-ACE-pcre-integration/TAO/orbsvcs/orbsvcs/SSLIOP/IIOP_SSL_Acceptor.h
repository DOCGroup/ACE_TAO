// -*- C++ -*-

//=============================================================================
/**
 *  @file    IIOP_SSL_Acceptor.h
 *
 *  $Id$
 *
 *  SSL-aware IIOP specific acceptor processing
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_SSL_ACCEPTOR_H
#define TAO_IIOP_SSL_ACCEPTOR_H

#include /**/ "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IIOP_SSL_Connection_Handler.h"
#include "SSLIOP_Connection_Handler.h"
#include "tao/IIOP_Acceptor.h"

// TAO IIOP_SSL_Acceptor concrete call defination

/**
 * @class TAO_IIOP_SSL_Acceptor
 *
 * @brief An SSL aware IIOP acceptor.
 *
 * The class differs from the non-SSL aware TAO_IIOP_Acceptor class
 * since it uses a different a set of Acceptor strategies.  In
 * particular, it overrides the open_i() method to use the Acceptor
 * strategies tailored to use the SSL aware server connection handler.
 * See the TAO_IIOP_SSL_Connection_Handler notes for details.
 */
class TAO_SSLIOP_Export TAO_IIOP_SSL_Acceptor
  : public TAO_IIOP_Acceptor
{
public:

  typedef ACE_Strategy_Acceptor<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR> TAO_IIOP_SSL_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler> TAO_IIOP_SSL_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler> TAO_IIOP_SSL_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR> TAO_IIOP_SSL_ACCEPT_STRATEGY;

  /// Constructor.
  TAO_IIOP_SSL_Acceptor (CORBA::Boolean flag = 0);

  /// Destructor.
  ~TAO_IIOP_SSL_Acceptor (void);

  virtual int close (void);

protected:

  /// Implement the common part of the open*() methods.  This method is
  /// virtual to allow a derived class implementation to be invoked
  /// instead.
  virtual int open_i (const ACE_INET_Addr &addr,
                      ACE_Reactor *reactor);

private:

  /// The concrete acceptor, as a pointer to it's base class.
  TAO_IIOP_SSL_BASE_ACCEPTOR base_acceptor_;

  /**
   * @name Acceptor Strategies
   *
   * Server side creation, concurrency and accept strategies.
   */
  //@{
  TAO_IIOP_SSL_CREATION_STRATEGY *creation_strategy_;
  TAO_IIOP_SSL_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_IIOP_SSL_ACCEPT_STRATEGY *accept_strategy_;
  //@}

  /// State that will be passed to each SSLIOP connection handler upon
  /// creation.
  TAO_SSLIOP_Connection_Handler_State handler_state_;

};

#include /**/ "ace/post.h"

#endif  /* TAO_IIOP_SSL_ACCEPTOR_H */
