// -*- C++ -*-

//=============================================================================
/**
 *  @file   IIOP_SSL_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_SSL_CONNECTION_HANDLER_H
#define TAO_IIOP_SSL_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOPC.h"
#include "tao/IIOP_Connection_Handler.h"

#include "SSLIOP_Current.h"

class TAO_SSLIOP_Current_Impl;

/**
 * @class TAO_IIOP_SSL_Connection_Handler
 *
 * @brief
 * IIOP connection handler designed to be "SSL aware," i.e. it is
 * aware of the existence of the SSLIOP connection handler.   It makes
 * sure that SSL session state from a previous connection is not
 * associated with the non-SSL connection handled by this handler.
 *
 * This class is just a place holder to create the
 * TAO_IIOP_SSL_Transport  which does the work of clearing the TSS SSL
 * state
 */
class TAO_SSLIOP_Export TAO_IIOP_SSL_Connection_Handler
  : public TAO_IIOP_Connection_Handler
{
public:

  /// Constructor. <arg> parameter is used by the Acceptor to pass the
  /// protocol configuration properties for this connection.
  TAO_IIOP_SSL_Connection_Handler (ACE_Thread_Manager* t = 0);
  TAO_IIOP_SSL_Connection_Handler (TAO_ORB_Core *orb_core,
                                   CORBA::Boolean flag,
                                   void *arg);

  /// Destructor.
  ~TAO_IIOP_SSL_Connection_Handler (void);

protected:

};

// ****************************************************************

/**
 * @class TAO_Null_SSL_State_Guard
 *
 * @brief
 * This class sets up null TSS SSL state upon instantiation, and
 * restores the previous TSS SSL state when that instance goes out of
 * scope.
 *
 * This guard is used to make TSS SSL state configuration and
 * deconfiguration during an upcall exception safe.  Exceptions are
 * not supposed to be propagated up to the scope this guard is used
 * in, so this guard may be unnecessary.  However, since proper TSS
 * SSL state configuration/deconfiguration is critical to proper
 * security support, this guard is used to ensure that
 * configuration/deconfiguration is exception safe.
 */
class TAO_Null_SSL_State_Guard
{
public:

  /// Constructor that sets up the null TSS SSL state.
  TAO_Null_SSL_State_Guard (TAO_SSLIOP_Current_ptr current,
                            int &result);

  /// Destructor that restores the previous TSS SSL state.
  ~TAO_Null_SSL_State_Guard (void);

private:

  /// The SSLIOP::Current implementation that was previously
  /// associated with the current thread and invocation.
  /**
   * It is stored here until the invocation completes, after which it
   * placed back into TSS.
   */
  TAO_SSLIOP_Current_Impl *previous_current_impl_;

  /// Reference to the SSLIOP::Current object.
  TAO_SSLIOP_Current_ptr current_;

  /// Flag that specifies whether or not setup of the SSLIOP::Current
  /// object completed for the current thread and invocation.
  CORBA::Boolean setup_done_;

};


#if defined (__ACE_INLINE__)
#include "IIOP_SSL_Connection_Handler.inl"
#endif /* __ACE_INLINE__ */


#include /**/ "ace/post.h"

#endif  /* TAO_IIOP_SSL_CONNECTION_HANDLER_H */
