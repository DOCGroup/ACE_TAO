// -*- C++ -*-

//=============================================================================
/**
 *  @file   IIOP_SSL_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_SSL_CONNECTION_HANDLER_H
#define TAO_IIOP_SSL_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_Current.h"

#include "orbsvcs/SSLIOPC.h"
#include "tao/IIOP_Connection_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @class IIOP_SSL_Connection_Handler
   *
   * @brief
   * IIOP connection handler designed to be "SSL aware," i.e. it is
   * aware of the existence of the SSLIOP connection handler.   It
   * makes sure that SSL session state from a previous connection is
   * not associated with the non-SSL connection handled by this
   * handler.
   *
   * This class is just a place holder to create the
   * TAO_IIOP_SSL_Transport  which does the work of clearing the TSS
   * SSL state.
   */
  class IIOP_SSL_Connection_Handler : public TAO_IIOP_Connection_Handler
  {
  public:

    /// Constructor.
    IIOP_SSL_Connection_Handler (ACE_Thread_Manager* t = 0);
    IIOP_SSL_Connection_Handler (TAO_ORB_Core *orb_core);

    /// Destructor.
    ~IIOP_SSL_Connection_Handler (void);

  };

  // ****************************************************************

  /**
   * @class Null_SSL_State_Guard
   *
   * @brief
   * This class sets up null TSS SSL state upon instantiation, and
   * restores the previous TSS SSL state when that instance goes out
   * of scope.
   *
   * This guard is used to make TSS SSL state configuration and
   * deconfiguration during an upcall exception safe.  Exceptions are
   * not supposed to be propagated up to the scope this guard is used
   * in, so this guard may be unnecessary.  However, since proper TSS
   * SSL state configuration/deconfiguration is critical to proper
   * security support, this guard is used to ensure that
   * configuration/deconfiguration is exception safe.
   */
  class Null_SSL_State_Guard
  {
  public:

    /// Constructor that sets up the null TSS SSL state.
    Null_SSL_State_Guard (TAO::SSLIOP::Current_ptr current,
                          int &result);

    /// Destructor that restores the previous TSS SSL state.
    ~Null_SSL_State_Guard (void);

  private:

    /// The SSLIOP::Current implementation that was previously
    /// associated with the current thread and invocation.
    /**
     * It is stored here until the invocation completes, after which
     * it placed back into TSS.
     */
    TAO::SSLIOP::Current_Impl *previous_current_impl_;

    /// Reference to the SSLIOP::Current object.
    TAO::SSLIOP::Current_ptr current_;

    /// Flag that specifies whether or not setup of the SSLIOP::Current
    /// object completed for the current thread and invocation.
    bool setup_done_;

  };

} // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/SSLIOP/IIOP_SSL_Connection_Handler.inl"
#endif /* __ACE_INLINE__ */


#include /**/ "ace/post.h"

#endif  /* TAO_IIOP_SSL_CONNECTION_HANDLER_H */
