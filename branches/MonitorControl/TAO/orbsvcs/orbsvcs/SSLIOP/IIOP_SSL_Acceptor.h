// -*- C++ -*-

//=============================================================================
/**
 *  @file    IIOP_SSL_Acceptor.h
 *
 *  $Id$
 *
 *  SSL-aware IIOP specific acceptor processing
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_SSL_ACCEPTOR_H
#define TAO_IIOP_SSL_ACCEPTOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/IIOP_SSL_Connection_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IIOP_Acceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @class IIOP_SSL_Acceptor
   *
   * @brief An SSL aware IIOP acceptor.
   *
   * The class differs from the non-SSL aware TAO_IIOP_Acceptor class
   * since it uses a different a set of Acceptor strategies.  In
   * particular, it overrides the open_i() method to use the Acceptor
   * strategies tailored to use the SSL aware server connection handler.
   * See the TAO_IIOP_SSL_Connection_Handler notes for details.
   */
  class IIOP_SSL_Acceptor : public TAO_IIOP_Acceptor
  {
  public:

    typedef ACE_Strategy_Acceptor<IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR> TAO_IIOP_SSL_BASE_ACCEPTOR;
    typedef TAO_Creation_Strategy<IIOP_SSL_Connection_Handler> TAO_IIOP_SSL_CREATION_STRATEGY;
    typedef TAO_Concurrency_Strategy<IIOP_SSL_Connection_Handler> TAO_IIOP_SSL_CONCURRENCY_STRATEGY;
    typedef TAO_Accept_Strategy<IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR> TAO_IIOP_SSL_ACCEPT_STRATEGY;

    /// Constructor.
    IIOP_SSL_Acceptor (void);

    /// Destructor.
    ~IIOP_SSL_Acceptor (void);

    virtual int close (void);

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

  };

} // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_IIOP_SSL_ACCEPTOR_H */
