// -*- C++ -*-

// ===================================================================
/**
 *  @file   Connector_Impl.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_CONNECTOR_IMPL_H
#define TAO_CONNECTOR_IMPL_H

#include /**/ "ace/pre.h"
#include "ace/Strategies_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Thread_Manager;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

/**
 * @class TAO_Connect_Creation_Strategy
 *
 * @brief Creation strategy helper
 *
 * Creates the TAO_*_Connection_Handler object for the TAO_*_Connector
 * objects. This template class can now be used by all the Connector
 * objects instead of having to duplicate code. This class can be used
 * to set any required properties on the connection handlers at
 * creation time.
 *
 */

template <class SVC_HANDLER>
class TAO_Connect_Creation_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
{
public:

  /// Constructor.
  TAO_Connect_Creation_Strategy (ACE_Thread_Manager * = 0,
                                 TAO_ORB_Core* orb_core = 0);

  /// Makes TAO_*_Client_Connection_Handlers
  virtual int make_svc_handler (SVC_HANDLER *&sh);


private:

  /// Pointer to the ORB_Core on which we are activated
  TAO_ORB_Core * const orb_core_;
};



/**
 * @class TAO_Connect_Concurrency_Strategy
 *
 * @brief Concurrency strategy helper
 *
 * Activates the Svc_Handler, and then if the correct wait strategy is
 * in use registers the handler with the reactor.
 *
 */

template <class SVC_HANDLER>
class TAO_Connect_Concurrency_Strategy :
  public ACE_Concurrency_Strategy<SVC_HANDLER>
{
public:

  /// Constructor.
  TAO_Connect_Concurrency_Strategy (TAO_ORB_Core *orb_core);


  /// Activates the Svc_Handler, and if the right wait strategy is in
  /// use, registers the handle with the reactor.
  int activate_svc_handler (SVC_HANDLER *svc_handler, void *arg);

private:

  /// Pointer to the ORB Core.
  TAO_ORB_Core * const orb_core_;
};

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Connector_Impl.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Connector_Impl.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /*TAO_CONNECTOR_IMPL_H*/
