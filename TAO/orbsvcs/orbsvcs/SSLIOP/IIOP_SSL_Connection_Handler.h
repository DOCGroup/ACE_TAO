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


#ifndef TAO_IIOP_SSL_CONNECT_H
#define TAO_IIOP_SSL_CONNECT_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOPC.h"
#include "tao/IIOP_Connection_Handler.h"


/**
 * @class TAO_IIOP_SSL_Connection_Handler
 *
 * @brief
 * IIOP connection handler designed to be "SSL aware," i.e. it is
 * aware of the existence of the SSLIOP connection handler.   It makes
 * sure that SSL session state from a previous connection is not
 * associated with the non-SSL connection handled by this handler.
 *
 * This connection handler is essentially the same as the
 * standard IIOP client connection handler it is derived from.
 * However, this class overrides the handle_input() method to
 * invalidate the current TSS SSL state during a standard IIOP
 * (insecure) upcall.  This prevents SSL session state from a previous
 * SSL connection from being associated with non-SSL connections
 * processed by this connection handler.  In particular, this is very
 * important for closing a security hole in nested upcalls.  For
 * example, an SSLIOP request is made.  During that secure upcall, an
 * insecure nested upcall is made.  A naive implementation would
 * associate the TSS SSL state from the secure upcall with the
 * insecure upcall.  This implementation closes that security hole.
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

  /// Overridden method that invalidates the TSS SSL state for the
  /// current upcall, and restores the previous state once the upcall
  /// is done.
  virtual int handle_input_i (ACE_HANDLE = ACE_INVALID_HANDLE,
                              ACE_Time_Value *max_wait_time = 0);

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
class TAO_SSLIOP_Export TAO_Null_SSL_State_Guard
{
public:

  /// Constructor that sets up the null TSS SSL state.
  TAO_Null_SSL_State_Guard (TAO_ORB_Core *orb_core,
                            int &result);

  /// Destructor that restores the previous TSS SSL state.
  ~TAO_Null_SSL_State_Guard (void);

private:

  /// Reference to the SSLIOP::Current object.
  SSLIOP::Current_var current_;

};

#include "ace/post.h"

#endif  /* TAO_IIOP_SSL_CONNECT_H */
