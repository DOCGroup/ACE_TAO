// -*- C++ -*-

//=============================================================================
/**
 *  @file   Acceptor_Impl.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu>
 *  @author Ossama Othman <othman@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACCEPTOR_IMPL_H
#define TAO_ACCEPTOR_IMPL_H

#include /**/ "ace/pre.h"

#include "ace/Strategies_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_ORB_Core;

/**
 * @class TAO_Creation_Strategy
 *
 * @brief Creates a Svc_Handler and set the ORB_Core pointer on it.
 */
template <class SVC_HANDLER>
class TAO_Creation_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
{
public:
  /**
   * Constructor.
   */
  TAO_Creation_Strategy (TAO_ORB_Core *orb_core);

  /// Create a SVC_HANDLER  and set the ORB_Core pointer on it.
  int make_svc_handler (SVC_HANDLER *&sh);

protected:
  /// Pointer to the ORB Core.
  TAO_ORB_Core *orb_core_;
};

/**
 * @class TAO_Concurrency_Strategy
 *
 * @brief Activates the Svc_Handler, and then if specified by the
 * TAO_Server_Strategy_Factory, it activates the Svc_Handler to
 * run in its own thread.
 */
template <class SVC_HANDLER>
class TAO_Concurrency_Strategy : public ACE_Concurrency_Strategy<SVC_HANDLER>
{
public:
  /// Constructor.
  TAO_Concurrency_Strategy (TAO_ORB_Core *orb_core);

  /**
   * Activates the Svc_Handler, and then if specified by the
   * TAO_Server_Strategy_Factory, it activates the Svc_Handler to run
   * in its own thread.
   */
  int activate_svc_handler (SVC_HANDLER *svc_handler, void *arg);

protected:
  /// Pointer to the ORB Core.
  TAO_ORB_Core *orb_core_;
};

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class TAO_Accept_Strategy : public ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
{
public:

  /// Constructor.
  TAO_Accept_Strategy (TAO_ORB_Core *orb_core);

  /// Initialize the <peer_acceptor_> with @a local_addr.  If the
  /// process runs out of handles, purge some "old" connections.
  int open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
            bool restart = false);

  /// Delegates to the <accept> method of the PEER_ACCEPTOR. If the
  /// process runs out of handles, purge some "old" connections.
  int accept_svc_handler (SVC_HANDLER *svc_handler);

protected:
  /// Base class.
  typedef ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> ACCEPT_STRATEGY_BASE;

  /// Pointer to the ORB Core.
  TAO_ORB_Core *orb_core_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Acceptor_Impl.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Acceptor_Impl.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_ACCEPTOR_IMPL_H */
