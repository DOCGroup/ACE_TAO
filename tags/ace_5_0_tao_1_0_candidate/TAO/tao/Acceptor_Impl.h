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
  TAO_Creation_Strategy (TAO_ORB_Core *orb_core);
  // Constructor.

  int make_svc_handler (SVC_HANDLER *&sh);
  // Create a SVC_HANDLER  and set the ORB_Core pointer on it.

protected:
  TAO_ORB_Core *orb_core_;
  // Pointer to the ORB Core.
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

#if defined(__ACE_INLINE__)
#include "tao/Acceptor_Impl.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Acceptor_Impl.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Acceptor_Impl.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ACCEPTOR_IMPL_H */
