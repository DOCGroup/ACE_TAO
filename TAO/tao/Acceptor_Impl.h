// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Acceptor_Impl.h
//
// = AUTHOR
//   Carlos O'Ryan <coryan@cs.wustl.edu>
//   Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ACCEPTOR_IMPL_H
#include "ace/pre.h"
#define TAO_ACCEPTOR_IMPL_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class SVC_HANDLER>
class TAO_Creation_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Creates a Svc_Handler and set the ORB_Core pointer on it.
public:
  TAO_Creation_Strategy (TAO_ORB_Core *orb_core,
                         CORBA::Boolean flag = 0);
  // Constructor.

  int make_svc_handler (SVC_HANDLER *&sh);
  // Create a SVC_HANDLER  and set the ORB_Core pointer on it.

protected:
  TAO_ORB_Core *orb_core_;
  // Pointer to the ORB Core.
  
  CORBA::Boolean lite_flag_;
  // Should we use the Lite version for any protocol?
};

template <class SVC_HANDLER>
class TAO_Concurrency_Strategy : public ACE_Concurrency_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Activates the Svc_Handler, and then if specified by the
  //     TAO_Server_Strategy_Factory, it activates the Svc_Handler to
  //     run in its own thread.
public:
  TAO_Concurrency_Strategy (TAO_ORB_Core *orb_core);
  // Constructor.

  int activate_svc_handler (SVC_HANDLER *svc_handler,
                            void *arg);
  // Activates the Svc_Handler, and then if specified by the
  // TAO_Server_Strategy_Factory, it activates the Svc_Handler to run
  // in its own thread.

protected:
  TAO_ORB_Core *orb_core_;
  // Pointer to the ORB Core.
};

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class TAO_Accept_Strategy : public ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
{
public:

  TAO_Accept_Strategy (TAO_ORB_Core *orb_core);
  // Constructor.

  int open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
            int restart = 0);
  // Initialize the <peer_acceptor_> with <local_addr>.  If the
  // process runs out of handles, purge some "old" connections.

  int accept_svc_handler (SVC_HANDLER *svc_handler);
  // Delegates to the <accept> method of the PEER_ACCEPTOR. If the
  // process runs out of handles, purge some "old" connections.

protected:
#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  int out_of_sockets_handler (void);
  // Handler which deals with purging "old" connections.
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

  typedef ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> ACCEPT_STRATEGY_BASE;
  // Base class.

  TAO_ORB_Core *orb_core_;
  // Pointer to the ORB Core.
};

#if defined(__ACE_INLINE__)
#include "tao/Acceptor_Impl.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Acceptor_Impl.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Acceptor_Impl.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_ACCEPTOR_IMPL_H */
